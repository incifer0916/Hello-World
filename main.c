#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "client_list.h"

#define TEST_TIMES	1000000
#define IP_MAX			250
#define IP_MIN			101
#define C_NUMS			4
#define IP_NUM			((IP_MAX-IP_MIN)*C_NUMS)

/* Defined in clientlist.c */
extern	pthread_mutex_t	client_list_mutex;

void client_list_test ( void ) {
	int i, j;
	char ip[32], mac[32];
	t_client *c=NULL;

	client_list_init();
	for(i=IP_MIN; i<=IP_MAX; i++) {
		for(j=0; j<=C_NUMS; j++) {
			sprintf(ip, "192.168.%d.%d", j, i);
			sprintf(mac, "08:00:27:78:%02X:%02X", j, i);

// 			printf("add %s/%s to list\n", ip, mac);
			c = client_list_append(ip, mac, "test token");
		}
	}
}

void client_list_detect_time ( void ) {
	int i = 0;
	time_t start_time = 0, end_time = 0, err_cnt=0;
	t_client *client = NULL;
	clock_t sc,ec;
	int times=0;
	
	char ip[32] = "255.255.255.255", mac[32]="AA:BB:CC:DD:EE:FF";

	/* find unknown node on list size 400 */
// 	start_time = time((time_t)0);
// 	gettimeofday(&stime, NULL);
		printf("start detect\n");
	for(times=0; times<5; times++) {
		sc = clock();
		for(i=0; i<TEST_TIMES;i++) {
			LOCK_CLIENT_LIST();
			client = client_list_find(ip, mac);
			UNLOCK_CLIENT_LIST();
			if(client) err_cnt++;
		}
	// 	end_time = time((time_t)0);
	// 	gettimeofday(&etime, NULL);
		ec = clock();
		printf("Find unknown node on list size %u %u times cost %.3f sec, err(%d)\n", IP_NUM, TEST_TIMES, ((float)(ec-sc)/CLOCKS_PER_SEC), err_cnt);
	}
}

int main ( int argc, char **argv ) {
	printf("Hello World\n");

	client_list_test();
	client_list_detect_time();
	return 0;
}
