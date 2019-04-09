
#include <stdio.h>
#include <rpc/rpc.h>
#include "pub_sub.h"

CLIENT subscribed[512];
unsigned short subCounter = 0;
topic *channel;

int main(int argc, char const *argv[]) {

  return 0;
}

short *unsubscribe_1_svc(topic *tp, svc_req *req){
  for(unsigned short i = 0; i <= subCounter; i++){
    if(subscribed[i] == req->rq_xprt->xp_raddr.sin_addr.s_addr){
      subscribed[i] = subscribed[subCounter];
      subCounter--;
      return 0;
    }
  }
}

short *subscribe_1_svc(topic *tp, svc_req *req){
  subscribed[subCounter] = req->rq_xprt->xp_raddr.sin_addr.s_addr;
  subCounter++;
  return 0;
}

short *set_channel_1_svc(topic *tp, svc_req *req){
  channel = tp;
}

short *publish_1_svc(message *message, svc_req *req){
  for(unsigned short i = 0; i <= subCounter; i++){
    deliver_1(message, subscribed[i]);
  }
}
