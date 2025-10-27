#include "../includes/ping.h"

int loop = 1;

void signal_handler(int signum) {
  (void)signum; // Éviter warning unused parameter
  loop = 0;
}

int main(int argc, char *argv[]) {
  t_config config = {0};
  t_stats stats = {0};
  int seq_num = 0;

  if (argc < 2) {
    printf("Ping: missing operand\n");
    printf("Try 'ping -?' for more information\n");
    return 1;
  }

  if (get_addr(argv[1], &config) != 0) {
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
