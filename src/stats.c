#include "../includes/ping.h"

double calculate_rtt(struct timeval *tv_send, struct timeval *tv_recv) {
  return (tv_recv->tv_sec - tv_send->tv_sec) * 1000.0 +
         (tv_recv->tv_usec - tv_send->tv_usec) / 1000.0;
}

double calculate_stddev(t_stats *stats) {
  if (stats->packets_received <= 0)
    return 0.0;

  return stats->stddev_rtt;
}

void update_stats(t_stats *stats, double rtt) {
  stats->packets_received++;
  stats->sum_rtt += rtt;
  stats->sum_rtt_squared += rtt * rtt;

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

  // sqrt((sum(x²)/n) - (avg)²)
  double variance = 0.0;
  if (stats->packets_received > 0) {
    double mean_of_squares = stats->sum_rtt_squared / stats->packets_received;
    double square_of_mean = stats->avg_rtt * stats->avg_rtt;
    variance = mean_of_squares - square_of_mean;
  }
  stats->stddev_rtt = sqrt(variance);
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
           stats->min_rtt, stats->avg_rtt, stats->max_rtt, stats->stddev_rtt);
  }
}
