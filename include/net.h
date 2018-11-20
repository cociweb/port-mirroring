/*
 * Copyright (c) 2015 Mike Maraya <mike[dot]maraya[at]gmail[dot]com>
 * All rights reserved.
 *
 * This file is subject to the terms and conditions defined in
 * https://github.com/mmaraya/port-mirroring/blob/master/LICENSE,
 * which is part of this software package.
 *
 */

#ifndef PORT_MIRRORING_NETWORK_UTIL_H_
#define PORT_MIRRORING_NETWORK_UTIL_H_

#include <arpa/inet.h>
#include <errno.h>
#include <linux/rtnetlink.h>
#include <linux/netlink.h>
#include <net/if.h>
#include <netinet/in.h>
#include "pcap.h"
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <unistd.h>
#include "config.h"

#define BUFSIZE         8192
#define ETH_ALEN        6                   /* Octets in one ethernet addr  */
#define ETH_P_ARP       0x0806              /* Address Resolution packet    */
#define ARPOP_REQUEST   1                   /* ARP request                  */
#define ARPOP_REPLY     2                   /* ARP reply                    */
#define ARP_WAIT_TIME   500                 /* ARP response wait time (ms)  */
#define ARP_ETH_PADDING 18                  /* ARP ethernet padding         */
#define SNAP_LEN        65535               /* pcap snapshot length         */
#define ETH_P_802_3     0x0001              /* Dummy type for 802.3 frames  */
#define ETH_P_IP        0x0800              /* Internet Protocol packet     */

// TaZMen Sniffer Protol (TZSP) Header
typedef struct
{
    unsigned char   ver;
    unsigned char   type;
    unsigned short  proto;
    unsigned char   tagend;
} TZSP_HEAD;

// Ethernet header
typedef struct
{
    unsigned char h_dest[ETH_ALEN];   /* destination ethernet address   */
    unsigned char h_source[ETH_ALEN]; /* source ethernet address        */
    unsigned short h_proto;           /* packet type ID field           */
} ETHHDR;

// Internet Protocol Header
typedef struct
{
    unsigned char   h_lenver;
    unsigned char   tos;
    unsigned short  total_len;
    unsigned short  ident;
    unsigned short  frag_and_flags;
    unsigned char   ttl;
    unsigned char   proto;
    unsigned short  checksum;
    unsigned int    sourceIP;
    unsigned int    destIP;
} IP_HEADER;

// User Datagram Protocl Header
typedef struct
{
    unsigned short uh_sport;
    unsigned short uh_dport;
    unsigned short uh_len;
    unsigned short uh_sum;
} UDP_HEADER;

// Address Resolution Protocol Header
typedef struct
{
    unsigned short ar_hrd;              /* format of hardware address   */
    unsigned short ar_pro;              /* format of protocol address   */
    unsigned char ar_hln;               /* length of hardware address   */
    unsigned char ar_pln;               /* length of protocol address   */
    unsigned short ar_op;               /* ARP opcode (command)         */
    unsigned char ar_sha[ETH_ALEN];     /* sender hardware address      */
    unsigned int ar_sip;                /* sender IP address            */
    unsigned char ar_tha[ETH_ALEN];     /* target hardware address      */
    unsigned int ar_tip;                /* target IP address            */
} ARPHDR;

// ARP Packet Header
typedef struct
{
    ETHHDR ethhdr;
    ARPHDR arphdr;
} ARPPACKET;

int getInterfaceMac(const char *device, char *mac);
int getInterfaceIP(const char *device, unsigned int *ip);
int nlmsg_ok(const struct nlmsghdr *nlh, ssize_t len);
int getSenderInterface(unsigned int targetIP, char *device, char *mac);
int getRemoteARP(struct pm_cfg cfg, unsigned int targetIP, const char *device, char *mac);
int readNlSock(int sockFd, char *bufPtr, uint32_t seqNum, uint32_t pId);

#endif  // PORT_MIRRORING_NETWORK_UTIL_H_
