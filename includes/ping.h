#ifndef FT_PING_H
#define FT_PING_H

#include <arpa/inet.h>
#include <errno.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/ip_icmp.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define PACKET_SIZE 64
#define MAX_WAIT_TIME 1

typedef struct s_config {
  bool verbose;
  char hostname[256];
  char ip_address[INET_ADDRSTRLEN];
  struct sockaddr_in addr;
  int sockfd;
} t_config;

typedef struct s_packet {
  struct icmp header;
  char data[PACKET_SIZE - sizeof(struct icmp)];
} t_packet;

typedef struct s_stats {
  int packets_sent;
  int packets_received;
  double sum_rtt;
  double sum_rtt_squared;
  double min_rtt;
  double max_rtt;
  double avg_rtt;
  double stddev_rtt;
} t_stats;

unsigned int icmp_checksum(void *data, int len);
int get_addr(const char *hostname, t_config *config);
int create_socket();
int send_ping_packet(t_config *config, int seq_num);
int receive_ping_packet(t_config *config, int seq_num, t_stats *stats);
double calculate_rtt(struct timeval *tv_send, struct timeval *tv_recv);
void update_stats(t_stats *stats, double rtt);
void print_stats(t_stats *stats, const char *hostname);

bool is_help(int argc, char **argv);
bool is_verbose(int argc, char **argv);
bool is_valid_args(int argc, char **argv);
char *get_hostname(int argc, char **argv);

void print_help(void);
void print_verbose_icmp_error(struct icmp *icmp_hdr, struct ip *ip_hdr,
                              ssize_t bytes_received, int ip_hdr_len,
                              struct sockaddr_in *recv_addr, char *buffer);

#endif
