#include "../includes/ping.h"
#include <string.h>

int loop = 1;

void signal_handler(int signum) {
  (void)signum;
  loop = 0;
}

int main(int argc, char *argv[]) {
  t_config config = {0};
  t_stats stats = {0};
  int seq_num = 0;

  if (argc < 2 || argc > 4) {
    printf("ft_ping: missing operand\n");
    printf("Try 'ping -?' for more information\n");
    return 1;
  }

  if (!is_valid_args(argc, argv)) {
    printf("ft_ping: invalid option\n");
    printf("Try 'ping -?' for more information\n");
    return 1;
  }

  if (is_help(argc, argv)) {
    print_help();
    return 0;
  }

  if (is_verbose(argc, argv)) {
    config.verbose = true;
  }

  const char *hostname = get_hostname(argc, argv);

  if (get_addr(hostname, &config) != 0) {
    printf("Ping: invalid hostname\n");
    printf("Try 'ping -?' for more information\n");
    return 1;
  }

  config.sockfd = create_socket();
  if (config.sockfd == -1) {
    printf("Ping: failed to create socket\n");
    printf("Try 'ping -?' for more information\n");
    return 1;
  }

  printf("PING %s (%s): %d data bytes\n", config.hostname, config.ip_address,

         56);

  signal(SIGINT, signal_handler);

  while (loop) {
    stats.packets_sent++;

    if (send_ping_packet(&config, seq_num) == 0) {
      receive_ping_packet(&config, seq_num, &stats);
    }

    seq_num++;
    sleep(1);
  }

  print_stats(&stats, config.hostname);
  return 0;
}
