
#include <stdio.h>
#include <rpc/rpc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "pub_sub.h"
#include "pub_sub_deliv.h"

char *subscribed[512];
unsigned short subCounter = 0;
topic *channel;
struct scv_req;

/*int main(int argc, char const *argv[]) {

  return 0;
}*/

short *unsubscribe_1_svc(void *t, struct svc_req *req){
  for(unsigned short i = 0; i <= subCounter; i++){
    char* address = inet_ntoa(req->rq_xprt->xp_raddr.sin_addr);
    if(strcmp(subscribed[i], address)){
      subscribed[i] = subscribed[subCounter];
      subCounter--;
      return 0;
    }
  }
}

short *subscribe_1_svc(void *t, struct svc_req *req){
  char* address = inet_ntoa(req->rq_xprt->xp_raddr.sin_addr);
  subscribed[subCounter] = address;
  subCounter++;
  return 0;
}

short *set_channel_1_svc(topic *tp, struct svc_req *req){
  channel = tp;
}

short *publish_1_svc(message *message, struct svc_req *req){
  for(unsigned short i = 0; i <= subCounter; i++){
    CLIENT *cl;
    cl = clnt_create(subscribed[i], PUBSUBCLTPROG, PUBSUBCLTVERS, "tcp");
    postmessage *m = message;
    deliver_1(m, cl);
  }
}
