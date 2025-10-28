#include "../includes/ping.h"
#include <netinet/ip.h>
#include <string.h>
#include <sys/time.h>

unsigned int icmp_checksum(void *data, int len) {
  unsigned short *buf = data;
  unsigned int sum = 0;
  unsigned short result;

  for (sum = 0; len > 1; len -= 2)
    sum += *buf++;
  if (len == 1)
    sum += *(unsigned char *)buf;
  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);
  result = ~sum;
  return result;
}

int send_ping_packet(t_config *config, int seq_num) {
  t_packet packet;
  struct timeval tv;

  memset(&packet, 0, sizeof(t_packet));

  packet.header.icmp_type = ICMP_ECHO;
  packet.header.icmp_code = 0;
  packet.header.icmp_id = htons(getpid());
  packet.header.icmp_seq = htons(seq_num);
  packet.header.icmp_cksum = 0;

  gettimeofday(&tv, NULL);
  memcpy(packet.data, &tv, sizeof(struct timeval));

  packet.header.icmp_cksum = icmp_checksum(&packet, sizeof(t_packet));

  if (sendto(config->sockfd, &packet, sizeof(t_packet), 0,
             (struct sockaddr *)&config->addr,
             sizeof(struct sockaddr_in)) <= 0) {
    perror("sendto");
    return -1;
  }

  return 0;
}

int receive_ping_packet(t_config *config, int seq_num, t_stats *stats) {
  char buffer[1024];
  struct sockaddr_in recv_addr;
  socklen_t addr_len = sizeof(recv_addr);
  struct timeval tv_now;
  ssize_t bytes_received;

  bytes_received = recvfrom(config->sockfd, buffer, sizeof(buffer), 0,
                            (struct sockaddr *)&recv_addr, &addr_len);

  if (bytes_received < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      printf("Request timeout for icmp_seq %d\n", seq_num);
      return -1;
    }
    if (errno == EINTR) {
      return -1;
    }
    perror("recvfrom");
    return -1;
  }

  gettimeofday(&tv_now, NULL);

  struct ip *ip_hdr = (struct ip *)buffer;
  int ip_hdr_len = ip_hdr->ip_hl * 4;
  struct icmp *icmp_hdr = (struct icmp *)(buffer + ip_hdr_len);

  if (icmp_hdr->icmp_type != ICMP_ECHOREPLY) {
    return -1;
  }

  if (ntohs(icmp_hdr->icmp_id) != getpid()) {
    return -1;
  }

  struct timeval tv_send;
  char *data_ptr = (char *)icmp_hdr + sizeof(struct icmp);
  memcpy(&tv_send, data_ptr, sizeof(struct timeval));

  double rtt = calculate_rtt(&tv_send, &tv_now);

  update_stats(stats, rtt);

  printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
         bytes_received - ip_hdr_len, inet_ntoa(recv_addr.sin_addr),
         ntohs(icmp_hdr->icmp_seq), ip_hdr->ip_ttl, rtt);
  return 0;
}
