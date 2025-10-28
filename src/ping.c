#include "../includes/ping.h"
#include <string.h>

void print_help(void) {
  printf("Usage: ./ft_ping [OPTION...] HOST ...\n");
  printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
  printf("  -v, --verbose              verbose output\n");
  printf("  -?, --help                 give this help list\n");
  printf("\nReport bugs to <bug-inetutils@gnu.org>.\n");
}
