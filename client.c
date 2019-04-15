#include <stdio.h>
#include <unistd.h>
#include "pub_sub_deliv.h"
#include "pub_sub.h"
#include "return_codes.h"

int main(int argc, char *argv[]){
  static short * error_no;
	
	printf("Client starten\n");
	if(argc != 3){
		printf("Missing arguments");
		exit(1);
	}
	CLIENT *cl;
	char *address;
	address = argv[1];
	char *topic;
	topic = argv[2];
	cl = clnt_create(address, PUBSUBPROG, PUBSUBVERS, "tcp");
	if(cl == NULL){
		printf("server not found\n");
		exit(1);
	}

	error_no=set_channel_1(&topic, cl);
  	printf("%s\n",PUB_SUB_RET_CODE[*error_no]);
  	if(*error_no!=CANNOT_SET_TOPIC){
		printf("topic set to %s\n", topic);
	}
	void *ptr = NULL;
	printf("subscribing...\n");
	error_no=subscribe_1(ptr, cl);
  	printf("%s\n",PUB_SUB_RET_CODE[*error_no]);
	int pid = fork();

	if(pid == 0){
		printf("Starting the Receiver...\n");
		execl("./pub_sub_deliv", "./pub_sub_deliv", NULL);
	} else {
		printf("subscribed\n");

		printf("Bitte Nachrichten eingeben und mit Enter senden: \n");
		char msg[MESLEN];
		if(fgets(msg, MESLEN, stdin)){
			char *msg2 = msg;
      
			error_no=publish_1(&msg2, cl);
      			printf("%s",PUB_SUB_RET_CODE[*error_no]);
		}

		void *ptr2 = NULL;
		printf("unsubscribing...\n");
		error_no=unsubscribe_1(ptr2, cl);
		printf("%s",PUB_SUB_RET_CODE[*error_no]);
		kill(pid, SIGKILL);
		
    		printf("%s",PUB_SUB_RET_CODE[*error_no]);
		printf("finished\nquitting...\n");
	}
	return 0;
}
