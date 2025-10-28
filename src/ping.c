#include "../includes/ping.h"
#include <netinet/ip.h>
#include <string.h>

void print_help(void) {
  printf("Usage: ./ft_ping [OPTION...] HOST ...\n");
  printf("Send ICMP ECHO_REQUEST packets to network hosts.\n\n");
  printf("  -v, --verbose              verbose output\n");
  printf("  -?, --help                 give this help list\n");
  printf("\nReport bugs to <bug-inetutils@gnu.org>.\n");
}

void print_verbose_icmp_error(struct icmp *icmp_hdr, struct ip *ip_hdr,
                              ssize_t bytes_received, int ip_hdr_len,
                              struct sockaddr_in *recv_addr, char *buffer) {

  switch (icmp_hdr->icmp_type) {
  case ICMP_TIMXCEED: // ICMP_TIME_EXCEEDED (11)
    printf("%ld bytes from %s: Time to live exceeded\n",
           bytes_received - ip_hdr_len, inet_ntoa(recv_addr->sin_addr));
    break;
  case ICMP_UNREACH: // ICMP_DEST_UNREACH (3)
    printf("%ld bytes from %s: Destination unreachable\n",
           bytes_received - ip_hdr_len, inet_ntoa(recv_addr->sin_addr));
    break;
  default:
    printf("%ld bytes from %s: ICMP type %d\n", bytes_received - ip_hdr_len,
           inet_ntoa(recv_addr->sin_addr), icmp_hdr->icmp_type);
    break;
  }

  // dump IP header
  printf("IP Hdr Dump:\n ");
  unsigned char *ip_bytes = (unsigned char *)ip_hdr;
  for (int i = 0; i < ip_hdr_len; i++) {
    printf("%02x", ip_bytes[i]);
    if (i % 2 == 1)
      printf(" ");
  }
  printf("\n");

  // dump IP header details
  struct in_addr src, dst;
  src.s_addr = ip_hdr->ip_src.s_addr;
  dst.s_addr = ip_hdr->ip_dst.s_addr;

  printf("Vr HL TOS  Len   ID Flg  off TTL Pro  cks      Src    Dst    Data\n");
  printf(" %d  %d  %02x %04x %04x   %d %04x  %02x  %02x %04x %s  %s\n",
         ip_hdr->ip_v, ip_hdr->ip_hl, ip_hdr->ip_tos, ntohs(ip_hdr->ip_len),
         ntohs(ip_hdr->ip_id), (ntohs(ip_hdr->ip_off) & 0x4000) >> 14,
         ntohs(ip_hdr->ip_off) & 0x1fff, ip_hdr->ip_ttl, ip_hdr->ip_p,
         ntohs(ip_hdr->ip_sum), inet_ntoa(src), inet_ntoa(dst));

  struct icmp *orig_icmp = (struct icmp *)(buffer + ip_hdr_len + 8);
  printf("ICMP: type %d, code %d, size %ld, id 0x%04x, seq 0x%04x\n",
         orig_icmp->icmp_type, orig_icmp->icmp_code,
         bytes_received - ip_hdr_len - 8, ntohs(orig_icmp->icmp_id),
         ntohs(orig_icmp->icmp_seq));
}
