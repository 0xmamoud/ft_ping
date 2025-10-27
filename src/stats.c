#include "../includes/ping.h"

double calculate_rtt(struct timeval *tv_send, struct timeval *tv_recv) {
  return (tv_recv->tv_sec - tv_send->tv_sec) * 1000.0 +
         (tv_recv->tv_usec - tv_send->tv_usec) / 1000.0;
}

void update_stats(t_stats *stats, double rtt) {
  stats->packets_received++;
  stats->sum_rtt += rtt;

  if (stats->packets_received == 1) {
    stats->min_rtt = rtt;
    stats->max_rtt = rtt;
  } else {
    if (rtt < stats->min_rtt)
      stats->min_rtt = rtt;
    if (rtt > stats->max_rtt)
      stats->max_rtt = rtt;
  }

  stats->avg_rtt = stats->sum_rtt / stats->packets_received;
}

void print_stats(t_stats *stats, const char *hostname) {
  double packet_loss = 0.0;

  if (stats->packets_sent > 0) {
    packet_loss = ((stats->packets_sent - stats->packets_received) * 100.0) /
                  stats->packets_sent;
  }

  printf("--- %s ping statistics ---\n", hostname);
  printf("%d packets transmitted, %d packets received, %.0f%% packet loss\n",
         stats->packets_sent, stats->packets_received, packet_loss);

  if (stats->packets_received > 0) {
    printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
           stats->min_rtt, stats->avg_rtt, stats->max_rtt, 0.000);
  }
}
