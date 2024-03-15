#include "http.h"

uint8_t continuous = 1;
uint16_t delay = 300;

void print_help_page(void);

int main(int argc, char *argv[]) {
  int c = 0;

  while ((c = getopt(argc, argv, "oh")) != -1) {
    switch (c) {
    case 'o':
      continuous = 0;
      delay = 0;
      break;

    case 'h':
      print_help_page();
      exit(0);
      break;

    default:
      break;
    }
  }

  do {

    char ip_addr[15];
    get_ip(ip_addr);

    printf("ip address: %s\n", ip_addr);

    update_ddns(ip_addr);
    sleep(delay);

  } while (continuous);

  return 0;
}

void print_help_page(void) {
  printf("simple dynamic dns client 1.0\n\nusage:\n\tsddc [options]\n");
  printf("options:\n\t-o\t\texecute once\n\t-h\t\tprint help page\n");
}
