
#include <stdio.h>
#include <rpc/rpc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "return_codes.h"
#include "pub_sub.h"
#include "pub_sub_deliv.h"

char *subscribed[512];
unsigned short subCounter = 0;
topic channel;
struct scv_req;
char top[TOPLEN];

short *unsubscribe_1_svc(void *t, struct svc_req *req){
  char *tmp = inet_ntoa(req->rq_xprt->xp_raddr.sin_addr);
  char address[16] = *tmp;
  for(unsigned short i = 0; i < subCounter; i++){
    if(0 == strcmp(subscribed[i], address)){
      strcpy(subscribed[i], subscribed[subCounter-1]);
      subCounter--;
      printf("Subscriber-List:\n");
      for(unsigned short i = 0; i < subCounter; i++){
        printf("%s\n", subscribed[i]);
      }
      return 0;
    }
  }
}

short *subscribe_1_svc(void *t, struct svc_req *req){
  char *tmp = inet_ntoa(req->rq_xprt->xp_raddr.sin_addr);
  char address[16] = *tmp;
  printf("New Subscriber: %s\n", address);
  strcpy(subscribed[subCounter], address);
  subCounter++;
  printf("Subscriber-List:\n");
  for(unsigned short i = 0; i < subCounter; i++){
    printf("%i: %s\n",i , subscribed[i]);
  }
  return 0;
}

short *set_channel_1_svc(topic *tp, struct svc_req *req){
  channel = *tp;
  strcpy(top, channel);
  printf("Channel: %s\n", top);
  return 0;
}

short *publish_1_svc(message *message, struct svc_req *req){
  for(unsigned short i = 0; i < subCounter; i++){
    CLIENT *cl;
    cl = clnt_create(subscribed[i], PUBSUBCLTPROG, PUBSUBCLTVERS, "tcp");
    char tmp[TOPLEN + MESLEN + 2] = "";
    strcat(tmp, top);
    strcat(tmp, ": ");
    strcat(tmp, *message);
    postmessage m = tmp;
    printf("%s ==> %s\n", m, subscribed[i]);
    deliver_1(&m, cl);
  }
  printf("sending completed.\n");
  return 0;
}
