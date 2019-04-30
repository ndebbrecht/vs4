#include <time.h>
#include <stdio.h>
#include <rpc/rpc.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "return_codes.h"
#include "pub_sub.h"
#include "pub_sub_deliv.h"
#include "sha_256.h"


char subscribed[512][16];
param param_st[16];
unsigned short paramCounter = 0;
unsigned short subCounter = 0;
topic channel;
struct scv_req;
char top[TOPLEN];

void init_hash_digest(void);

short *unsubscribe_1_svc(void *t, struct svc_req *req){
  static short errorno=0;
  char *tmp = inet_ntoa(req->rq_xprt->xp_raddr.sin_addr);
  char address[16];
  strcpy(address, tmp);
  for(unsigned short i = 0; i < subCounter; i++){
    if(0 == strcmp(subscribed[i], address)){
      strcpy(subscribed[i], subscribed[subCounter-1]);
      subCounter--;
      printf("Subscriber-List:\n");
      for(unsigned short i = 0; i < subCounter; i++){
        printf("%s\n", subscribed[i]);
      }
      errorno=OK;
      return (&errorno);
    }
  }
  errorno = CANNOT_UNREGISTER;
  return (&errorno);
}

short *subscribe_1_svc(void *t, struct svc_req *req){
  static short errorno=0;
  char *tmp = inet_ntoa(req->rq_xprt->xp_raddr.sin_addr);
  char address[16];
  strcpy(address, tmp);
  printf("New Subscriber: %s\n", address);
  for(unsigned short i = 0; i < subCounter; i++){
    printf("%i: %s\n",i , subscribed[i]);
    if(strcmp(subscribed[i],tmp)==0){
      errorno=CLIENT_ALREADY_REGISTERED;
      return (&errorno);
    }
  }
  strcpy(subscribed[subCounter], address);
  subCounter++;
  printf("Subscriber-List:\n");

  return (&errorno);
}

short *set_channel_1_svc(topic *tp, struct svc_req *req){
  static short errorno=0;

char *tmp = inet_ntoa(req->rq_xprt->xp_raddr.sin_addr);
	if(strcmp("127.0.0.1",tmp)!=0){
		errorno=CANNOT_SET_TOPIC;
		return (&errorno);
	}
  channel = *tp;
  strcpy(top, channel);
  printf("Channel: %s\n", top);
  errorno=OK;
  return (&errorno);
}

short *publish_1_svc(message *message, struct svc_req *req){
  static short errorno=0;
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

  return (&errorno);
}


sessionid * get_session_1_svc(user * user, struct svc_req * req){
	static sessionid session;
 	session = clock();
	unsigned short counter=0;
	param_st[paramCounter].id = session;
	paramCounter++;
	init_hash_digest();
	while((strcmp(GLOB_hash_digest[counter].user,*user) != 0) && counter < paramCounter){
		counter++;
	}
	if(counter<subCounter){
		param_st[counter].hash = GLOB_hash_digest[counter].hash;
	}
	else{
		param_st[counter].hash = "0";//Gibt es einen Hash H(user;pwd) der 0 ergibt ?
	}

	return (&session);
}

short * validate_1_svc(param * param, struct svc_req * req){
  static short errorno=0;
  printf("%i\n", param->id);
  printf("%s\n", param->hash);
  //printf("%s\n", param->arg);
	for(unsigned short i = 0; i < 16; i++){
    if(param_st[i].id == param->id){
      if(strcmp(param->hash, param_st[i].hash)){
        errorno=OK;
        return (&errorno);
      }
    }
    errorno=VALIDATE_ERROR;
    return (&errorno);
  }

}
short * invalidate_1_svc(sessionid * id, struct svc_req * req){

}
