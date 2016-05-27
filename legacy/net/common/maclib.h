#ifndef MACLIB_H
#define MACLIB_H

#include <net/if.h>

const char * getMacForIP(unsigned int ipNetworkOrder, struct ifreq *out, unsigned int *outNetmaskNetworkOrder);

#endif
