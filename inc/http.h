#ifndef __HTTP_H
#define __HTTP_H

#include "credentials.h"
#include <b64/cencode.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define __MAX_RESPONSE_SIZE_ 4096
#define __PORT_ 80

#define __GET_IP_URL "checkip.dyndns.com"
#define __IP_SEQ_ "Current IP Address: "

#define __DDNS_URL_ "dynupdate.no-ip.com"
#define __DDNS_URI_ "/nic/update?hostname=" __HOSTNAME_ "&myip="
#define __AUTH_ __USERNAME_ ":" __PASSWORD_
#define __USER_AGENT_                                                          \
  "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 "  \
  "(KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36"

void send_http_request(const char *host, const char *resource, const char *auth,
                       const char *user_agent, const uint8_t port,
                       char *response);
char *extract_ip(char *ptr);
void get_ip(char *ip_addr);
void update_ddns(const char *ip_addr);

#endif // __HTTP_H
