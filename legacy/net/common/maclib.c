#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "maclib.h"


#ifdef TEST

#define INITIAL_IFREQ_LENGTH 0
#define IFREQ_ITERATION_COUNT 1

#else

#define INITIAL_IFREQ_LENGTH 10
#define IFREQ_ITERATION_COUNT 5

#endif


// Returns NULL on success and Error message on error
const char * getMacForIP(unsigned int ipNetworkOrder, struct ifreq *out, unsigned int *outNetmaskNetworkOrder) {
  int i;

  struct ifconf ifconf;
  struct ifreq ifreqInitialMemory[INITIAL_IFREQ_LENGTH];

  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
  if(sock == -1) return "socket call return -1";

  //
  // Initialize Loop Variables
  //
  unsigned int currentIFReqLength = INITIAL_IFREQ_LENGTH;
  ifconf.ifc_len = sizeof(ifreqInitialMemory);
  ifconf.ifc_req = ifreqInitialMemory;

  while(1) {
    int lastIfcLength = ifconf.ifc_len;
    if(ioctl(sock, SIOCGIFCONF, &ifconf) == -1) return "ioctl returned -1";
    if(ifconf.ifc_len < lastIfcLength) break;

    //
    // Make buffer bigger:)
    //
    currentIFReqLength += IFREQ_ITERATION_COUNT;
    ifconf.ifc_len = sizeof(struct ifreq) * currentIFReqLength;

    if(currentIFReqLength == INITIAL_IFREQ_LENGTH + IFREQ_ITERATION_COUNT) {
      ifconf.ifc_req = (struct ifreq*)malloc(ifconf.ifc_len);      
    } else {
      ifconf.ifc_req = (struct ifreq*)realloc(ifconf.ifc_req, ifconf.ifc_len);
    }
  }

  //
  // Check interfaces for an IP in the correct subnet
  //
  for(i = 0; i + sizeof(struct ifreq) <= ifconf.ifc_len; i += sizeof(struct ifreq)) {
    struct ifreq *ifreq = (struct ifreq*)((char*)(ifconf.ifc_req) + i);
    char *name = ifreq->ifr_name;
    if(name[0] == 'e' && name[1] == 't' && name[2] == 'h') {
      struct sockaddr_in *addr = (struct sockaddr_in*)&(ifreq->ifr_addr);
      if(addr->sin_family != AF_INET) continue;

      unsigned int interfaceIPNetworkOrder = addr->sin_addr.s_addr;
      interfaceIPNetworkOrder = htonl(interfaceIPNetworkOrder);

      // Get Netmask
      if(ioctl(sock, SIOCGIFNETMASK, ifreq) == -1) continue;

      addr = (struct sockaddr_in*)&(ifreq->ifr_addr);
      unsigned int netmaskNetworkOrder = addr->sin_addr.s_addr;
      netmaskNetworkOrder = htonl(netmaskNetworkOrder);
      
      
      if((ipNetworkOrder & netmaskNetworkOrder) != (interfaceIPNetworkOrder & netmaskNetworkOrder)) continue;

      // get mac address
      strcpy(out->ifr_name, name);
      if(ioctl(sock, SIOCGIFHWADDR, out)) {
	return "ioctl for SIOCGIFHWADDR failed";
      }

      *outNetmaskNetworkOrder = netmaskNetworkOrder;
      return NULL;
    }
  }

  return "Could not find any valid eth devices";
}
/*
int main()
{

  struct ifreq f;
  unsigned int netmask;
  const char *error = getMacForIP(0x0F081AC8, &f, &netmask);

  if(error) {
    puts(error);
    return -1;
  }

  printf("if=%s, netmask=%x\n", f.ifr_name, netmask);

  return 0;
}
*/
