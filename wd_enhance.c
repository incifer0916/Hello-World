#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define IP_MAC_WIFIDOG_HASH_SIZE 0x100 * 0x10
#define IP_MAC_WIFIDOG_HASH_KEY IP_MAC_WIFIDOG_HASH_SIZE - 1
#define WIFIDOG_IP_HASH 0xFFFF

#define HASH(ip, mac) (((ip & 0xFFFF)<< 8) + (mac[5] & 0xFF)) & IP_MAC_WIFIDOG_HASH_KEY

#define printk  printf
#define kfree   free

#define NIPQUAD(addr) \
  ((unsigned char *)&addr)[0], \
  ((unsigned char *)&addr)[1], \
  ((unsigned char *)&addr)[2], \
  ((unsigned char *)&addr)[3]


typedef struct ip_mac_wifidog_entry {
  struct ip_mac_wifidog_entry *next;
  unsigned int ip;
  unsigned char mac[6];
} ip_mac_wifidog_entry_t;


struct wifidog_head {
  ip_mac_wifidog_entry_t *start;
//   spinlock_t WIFIDOGLock;
};

struct wifidog_ssl_head {
  ip_mac_wifidog_entry_t *start;
//   spinlock_t WIFIDOGLock_ssl;
};

struct wifidog_head nk_ip_mac_wifidog_entry_list[IP_MAC_WIFIDOG_HASH_SIZE];
struct wifidog_ssl_head nk_ip_mac_wifidog_entry_list_ssl[IP_MAC_WIFIDOG_HASH_SIZE];

ip_mac_wifidog_entry_t *del_wifidog_list_ssl, **wifidog_list_ssl;
ip_mac_wifidog_entry_t *del_wifidog_list, **wifidog_list;

unsigned int wd_count=0;
unsigned int wd_ssl_count=0;

void wifidog_add ( const unsigned int ip, const unsigned char *mac )
{
    ip_mac_wifidog_entry_t *tmp,*tmp_entry;
    unsigned int idx=0;
    
    printf("%s: ip:%03u.%03u.%03u.%03u, %02X:%02X:%02X:%02X:%02X:%02X\n", __func__, NIPQUAD(ip), mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    
    tmp_entry = (ip_mac_wifidog_entry_t *)malloc(sizeof(ip_mac_wifidog_entry_t));
    if(!tmp_entry)
    {
      printk("kmalloc ip_mac_wifidog_entry_t fail SIOCADDWIFIDOG\n");
      return;
    }
//     copy_from_user(tmp_entry,(ip_mac_wifidog_entry_t *)arg,sizeof(ip_mac_wifidog_entry_t));
    tmp_entry->ip = ip;
    strncpy(tmp_entry->mac, mac, 6);
//     idx = jhash_2words((tmp_entry->ip & WIFIDOG_IP_HASH),tmp_entry->mac[5],0)%IP_MAC_WIFIDOG_HASH_SIZE;

//     spin_lock_bh(&(nk_ip_mac_wifidog_entry_list[idx].WIFIDOGLock));
    for(tmp=nk_ip_mac_wifidog_entry_list[idx].start; tmp;tmp=tmp->next)
    {
      if(tmp->ip == tmp_entry->ip &&
        tmp->mac[5]==tmp_entry->mac[5] &&
        tmp->mac[4]==tmp_entry->mac[4] &&
        tmp->mac[3]==tmp_entry->mac[3] &&
        tmp->mac[2]==tmp_entry->mac[2]    )
      {
        kfree(tmp_entry);
        goto wifidog_entry_exist;
      }
    }

    tmp_entry->next = nk_ip_mac_wifidog_entry_list[idx].start;
    nk_ip_mac_wifidog_entry_list[idx].start = tmp_entry;

  wifidog_entry_exist:
  return;
//     spin_unlock_bh(&(nk_ip_mac_wifidog_entry_list[idx].WIFIDOGLock));

//     break;
}
void wifidog_del ( unsigned int ip, unsigned char *mac )
{
}
void wifidog_clr ( void )
{
}
void wifidog_prt ( void )
{
    //Show all WIFIDOG list
    ip_mac_wifidog_entry_t *tmp_entry;
    unsigned int i;

    wd_count=0;

    for(i=0; i<IP_MAC_WIFIDOG_HASH_SIZE; i++)
    {
//       spin_lock_bh(&(nk_ip_mac_wifidog_entry_list[i].WIFIDOGLock));
      tmp_entry=nk_ip_mac_wifidog_entry_list[i].start;
      while(tmp_entry)
      {
        wd_count++;
//         printk("WIFIDOG LIST HASH[%d] IP: %u.%u.%u.%u MAC[%02X] \n",i, NIPQUAD(tmp_entry->ip),tmp_entry->mac[5]);
        printf("%s: ip:%03u.%03u.%03u.%03u, %02X:%02X:%02X:%02X:%02X:%02X\n", __func__,
              NIPQUAD(tmp_entry->ip), 
              tmp_entry->mac[0], tmp_entry->mac[1], tmp_entry->mac[2], tmp_entry->mac[3], tmp_entry->mac[4], tmp_entry->mac[5]);

        tmp_entry = tmp_entry->next;
      }
//       spin_unlock_bh(&(nk_ip_mac_wifidog_entry_list[i].WIFIDOGLock));
    }

    printk("ALL wifidog list ssl COUNT=%lu\n",wd_ssl_count);
    printk("ALL wifidog list Count=%lu\n",wd_count);
//     break;
}



int main ( int argc, char **argv )
{
  unsigned int i,j;
  volatile unsigned int idx;
  unsigned int ip;
  unsigned char mac[6] = {0x08, 0x00, 0x27, 0x78, 0x52, 0xCD};
  unsigned int times = 0xFFFFFFFF;
  clock_t sc,ec;

  #if 0
  sc = clock();
  for(i=0; i<times; i++)
  {
    ip = i & 0xFFFFFFFF;
    mac[5] = i & 0xFF;
    idx = HASH(ip, mac);
  }
  ec = clock();
  printf("%u times cost %.3f sec\n", times, ((float)(ec-sc)/CLOCKS_PER_SEC));
  #endif

  for(i=0; i<=250;i++)
  {
    for(j=0;j<=3;j++)
    ip = 0x0A0A0000+i;
    ip = htonl(ip);
    printf("%s: ip(0x%08X):%03u.%03u.%03u.%03u, %02X:%02X:%02X:%02X:%02X:%02X\n", __func__, ip,
              NIPQUAD(ip),
              mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    wifidog_add(ip,mac);
  }

//   wifidog_prt();


}