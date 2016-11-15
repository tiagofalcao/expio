#include "expio.h"

#include <netdb.h>
#include <string.h>
#include <unistd.h>
#define FQDN_SIZE 256

char hostname[FQDN_SIZE] = "\0";

const char *expio_fqdn_get() {
  if (hostname[0])
    return hostname;

  gethostname(hostname, FQDN_SIZE - 1);
  if (hostname[0])
    return hostname;

  struct addrinfo hints, *info, *p;
  int gai_result;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; /*either IPV4 or IPV6*/
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_CANONNAME;

  if ((gai_result = getaddrinfo(hostname, "http", &hints, &info)) != 0) {
    EXPIO_LOG_ERR("getaddrinfo: %s\n", gai_strerror(gai_result));
    return NULL;
  }

  for (p = info; p != NULL; p = p->ai_next) {
    if (p->ai_flags & AI_CANONNAME) {
      strncpy(hostname, p->ai_canonname, FQDN_SIZE - 1);
      hostname[FQDN_SIZE - 1] = '\0';
      break;
    }
  }

  freeaddrinfo(info);
  return hostname;
}
