
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
const topic *channel;
struct scv_req;

short *unsubscribe_1_svc(void *t, struct svc_req *req){
  char* address = inet_ntoa(req->rq_xprt->xp_raddr.sin_addr);
  for(unsigned short i = 0; i < subCounter; i++){
    if(0 == strcmp(subscribed[i], address)){
      subscribed[i] = subscribed[subCounter-1];
      subCounter--;
      printf("Subscriber-List:\n");
      for(unsigned short i = 0; i < subCounter; i++){
        printf("%s\n", subscribed[i]);
      }
      return 0;
    }
  }
  return CANNOT_UNREGISTER;
}

short *subscribe_1_svc(void *t, struct svc_req *req){
  char* address = inet_ntoa(req->rq_xprt->xp_raddr.sin_addr);
  subscribed[subCounter] = address;
  subCounter++;
  printf("Subscriber-List:\n");
  for(unsigned short i = 0; i < subCounter; i++){
    printf("%s\n", subscribed[i]);
  }
  return 0;
}

short *set_channel_1_svc(topic *tp, struct svc_req *req){
  printf("Setting channel\n");
  channel = tp;
  printf("channel set to %s\n", *channel);
  return 0;
}

short *publish_1_svc(message *message, struct svc_req *req){
  printf("sending message\n");
  for(unsigned short i = 0; i < subCounter; i++){
    CLIENT *cl;
    cl = clnt_create(subscribed[i], PUBSUBCLTPROG, PUBSUBCLTVERS, "tcp");
    char tmp[TOPLEN + MESLEN + 2] = "";
    strcat(tmp, *channel);
    strcat(tmp, ": ");
    strcat(tmp, *message);
    printf("%s\n", tmp);
    postmessage *m = *tmp;
    printf("send message: %s\n", *m);
    deliver_1(m, cl);
    printf("sent\n");
  }
  printf("sending completed.\n");
  return 0;
}
