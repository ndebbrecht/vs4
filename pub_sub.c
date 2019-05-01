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
	while(counter < MAX_HASH_DIGEST_LENGTH){

   printf("strcmp(%s,%s)\n",GLOB_hash_digest[counter].user,*user);
   fflush(stdout);
	  if(strcmp(GLOB_hash_digest[counter].user,*user) == 0){
      printf("GLOB_hash copied into param_st.hash%s\n",GLOB_hash_digest[counter].hash);
      fflush(stdout);
		  param_st[paramCounter-1].hash = GLOB_hash_digest[counter].hash;
      return (&session);
	  }
   	counter++; 
  }
	
	//else
	param_st[counter].hash = NULL;
  printf("get session%s\n", param_st[paramCounter-1].hash);
 	fflush(stdout);

	return (&session);
}

short * validate_1_svc(param * param, struct svc_req * req){
  static short errorno=0;
  char tmp[HASHLEN];
  char * ptmp;
  //strcat(tmp, itoa(param.id));
  sprintf(tmp,"%d",param->id);
  strcat(tmp, ";;");
  printf("param id%i\n", param->id);
  fflush(stdout);
  printf("hash num param%s\n", param->hash);
	fflush(stdout);
  for(unsigned short i = 0; i < paramCounter; i++){
    printf("%i\n", i);
    printf("param_st %s\n", param_st[i].hash);
   	fflush(stdout);
    if(param_st[i].id == param->id){
      //printf("%i\n", param_st[i].id);
      //printf("%s\n", param_st[i].hash);
      strcat(tmp, param_st[i].hash);
      ptmp=hash_sha(tmp);
      if(param_st[i].hash==NULL){
        errorno=VALIDATE_ERROR;//
        return (&errorno);
      }
      else if(strcmp(param->hash, ptmp)){
        errorno=OK;
        return (&errorno);
      }
      else{
        errorno=VALIDATE_ERROR;//wrong hash
        return (&errorno);
      }
    }
    
  }
  errorno=VALIDATE_ERROR;//id not found
  return (&errorno);
}
short * invalidate_1_svc(sessionid * id, struct svc_req * req){
  static short errorno;
  for(unsigned short i = 0; i < paramCounter; i++){
  printf("invalidateparam id:%i\n", param_st->id);
  fflush(stdout);
    if(param_st[i].id == *id){
      //param_st[i] = param_st[paramCounter];
      param_st[i].id = param_st[paramCounter].id;
      param_st[i].hash = param_st[paramCounter].hash;
      paramCounter--;
      errorno = OK;
      return (&errorno);
    }
  }
  errorno = INVALIDATE_ERROR;//cannot find id in the list
  return (&errorno);
}
