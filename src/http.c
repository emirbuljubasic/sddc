#include "http.h"

void send_http_request(const char *host, const char *resource, const char *auth,
                       const char *user_agent, const uint8_t port,
                       char *response) {

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

  ssize_t bytes_received = recv(sockfd, response, __MAX_RESPONSE_SIZE_ - 1, 0);
  if (bytes_received < 0) {
    perror("Error receiving response");
    exit(1);
  } else if (bytes_received == 0) {
    fprintf(stderr, "No data received from server\n");
    exit(1);
  }
  response[bytes_received] = '\0';

  close(sockfd);
}

char *extract_ip(char *ptr) {
  uint8_t i = 0;
  char *ret;

  while (i < 20) {
    if (__IP_SEQ_[i] == *ptr)
      ++i;
    ++ptr;
  }
  ret = ptr;

  while (*ptr != '<')
    ++ptr;
  *ptr = '\0';

  return ret;
}

void get_ip(char *ip_addr) {
  char response[__MAX_RESPONSE_SIZE_];
  send_http_request(__GET_IP_URL, "/", "", "", __PORT_, response);
  strcpy(ip_addr, extract_ip(response));
}

void update_ddns(const char *ip_addr) {
  char response[__MAX_RESPONSE_SIZE_];
  uint8_t resource_length = strlen(__DDNS_URI_) + strlen(ip_addr);
  char resource[resource_length];
  strcat(resource, __DDNS_URI_);
  strcat(resource, ip_addr);

  char encoded_auth[256] = "";
  base64_encodestate state;
  base64_init_encodestate(&state);

  int encoded_length =
      base64_encode_block(__AUTH_, strlen(__AUTH_), encoded_auth, &state);

  base64_encode_blockend(encoded_auth + encoded_length, &state);

  send_http_request(__DDNS_URL_, resource, encoded_auth, __USER_AGENT_, __PORT_,
                    response);

  printf("response: %s\n", response);
}
