#ifndef NET_H
#define NET_H

struct IPHeader {
  unsigned char  ihl:4,version:4;
  unsigned char  typeOfService;
  unsigned short length;
  unsigned short identification;
  unsigned char  flags;
  unsigned short offset;
  unsigned char  timeToLive;
  unsigned char  protocol;
  unsigned short checksum;
  unsigned int   sourceIP;
  unsigned int   destIP;
};

struct UdpHeader {
  unsigned short sourcePort;
  unsigned short destPort;
  unsigned short length;
  unsigned short checksum;
};

#endif
