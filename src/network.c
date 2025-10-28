#include "../includes/ping.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_addr(const char *hostname, t_config *config) {
  struct addrinfo hints, *res;
  struct sockaddr_in *addr_in;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_RAW;
  hints.ai_protocol = IPPROTO_ICMP;

  if (getaddrinfo(hostname, NULL, &hints, &res) != 0)
    return -1;

  addr_in = (struct sockaddr_in *)res->ai_addr;

  strncpy(config->hostname, hostname, sizeof(config->hostname) - 1);
  config->hostname[sizeof(config->hostname) - 1] = '\0';

  inet_ntop(AF_INET, &addr_in->sin_addr, config->ip_address, INET_ADDRSTRLEN);

  memcpy(&config->addr, addr_in, sizeof(struct sockaddr_in));

  freeaddrinfo(res);

  return 0;
}

int create_socket() {
  int sockfd;

  sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  if (sockfd == -1) {
    perror("socket");
    return -1;
  }

  struct timeval timeout;
  timeout.tv_sec = MAX_WAIT_TIME;
  timeout.tv_usec = 0;

  if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) ==
      -1) {
    return -1;
  }

  // Configure TTL (Time To Live)
  // int ttl = 1;
  // if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(ttl)) == -1) {
  //   perror("setsockopt TTL");
  //   return -1;
  // }

  return sockfd;
}
