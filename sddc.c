#include "credentials.h"
#include <b64/cencode.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_RESPONSE_SIZE 4096
#define __SEQUENCE_ "Current IP Address: "
#define __DDNS_URL_ "dynupdate.no-ip.com"
#define __DDNS_URI_ "/nic/update?hostname=" __HOSTNAME_ "&myip="
#define __GET_IP_URL "checkip.dyndns.com"
#define __PORT_ 80
#define __AUTH_ __USERNAME_ ":" __PASSWORD_

char *find_sequence(char *ptr);
void send_http_request(const char *host, const char *resource,
                       const uint8_t port, char *response, const char *auth,
                       const char *user_agent);

int main() {
  char response[MAX_RESPONSE_SIZE];
  send_http_request(__GET_IP_URL, "/", __PORT_, response, "", "");

  char *ip_addr = find_sequence(response);

  printf("ip address: %s\n", ip_addr);

  uint8_t resource_length = strlen(__DDNS_URI_) + strlen(ip_addr);
  char resource[resource_length];
  strcat(resource, __DDNS_URI_);
  strcat(resource, ip_addr);

  base64_encodestate state;
  base64_init_encodestate(&state);

  char encoded_auth[256] = "";
  int encoded_length =
      base64_encode_block(__AUTH_, strlen(__AUTH_), encoded_auth, &state);
  encoded_length +=
      base64_encode_blockend(encoded_auth + encoded_length, &state);

  printf("encoded: %s\n", encoded_auth);
  printf("host: %s\n", __DDNS_URL_);
  printf("uri: %s\n", resource);

  char *user_agent = "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                     "AppleWebKit/537.36 (KHTML, like Gecko) "
                     "Chrome/58.0.3029.110 Safari/537.36";

  // send_http_request(__DDNS_URL_, resource, __PORT_, response, encoded_auth,
  // user_agent);

  send_http_request(__DDNS_URL_, resource, __PORT_, response, encoded_auth,
                    user_agent);

  return 0;
}

char *find_sequence(char *ptr) {
  uint8_t i = 0;
  char *ret;
  while (i < 20) {
    if (__SEQUENCE_[i] == *ptr)
      ++i;
    ++ptr;
  }
  ret = ptr;
  while (*ptr != '<')
    ++ptr;
  *ptr = '\0';
  return ret;
}

void send_http_request(const char *host, const char *resource,
                       const uint8_t port, char *response, const char *auth,
                       const char *user_agent) {
  // Create socket
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Error opening socket");
    exit(1);
  }

  struct hostent *server = gethostbyname(host);
  if (server == NULL) {
    fprintf(stderr, "Error: no such host\n");
    exit(1);
  }

  struct sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
  server_addr.sin_port = htons(port);

  if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Error connecting");
    exit(1);
  }

  char request[1000];
  sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n", resource, host);

  if (strcmp(auth, "")) {
    strcat(request, "Authorization: Basic ");
    strcat(request, auth);
    strcat(request, "\r\n");
  }

  if (strcmp(user_agent, "")) {
    strcat(request, "User-Agent: ");
    strcat(request, user_agent);
    strcat(request, "\r\n");
  }

  strcat(request, "\r\n");

  if (send(sockfd, request, strlen(request), 0) < 0) {
    perror("Error sending request");
    exit(1);
  }

  ssize_t bytes_received = recv(sockfd, response, MAX_RESPONSE_SIZE - 1, 0);
  if (bytes_received < 0) {
    perror("Error receiving response");
    exit(1);
  } else if (bytes_received == 0) {
    fprintf(stderr, "No data received from server\n");
    exit(1);
  }
  response[bytes_received] = '\0';

  printf("%s\n", response);

  close(sockfd);
}
