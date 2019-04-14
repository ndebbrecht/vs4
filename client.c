#include <stdio.h>
#include "pub_sub_deliv.h"
#include "pub_sub.h"

int main(int argc, char *argv[]){
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
		printf("server not found");
		exit(1);
	}

	short *ch = set_channel_1(&topic, cl);
	printf("topic set to %s\ncode: %i", topic, *ch);
	void *ptr = NULL;
	printf("subscribing...\n");
	short *code = subscribe_1(ptr, cl);
	printf("subscribed\ncode: %i", *code);

	printf("Bitte Nachrichten eingeben und mit Enter senden: \n");
	char *msg;
	if(fgets(msg, MESLEN, stdin)){
		publish_1(&msg, cl);
	}

	unsubscribe_1(NULL, cl);
}
