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

	set_channel_1(&topic, cl);
	printf("topic set to %s\n", topic);
	subscribe_1(NULL, cl);

	printf("Eingabe: \n");
	char *msg;
	scanf("%s\n",msg);
	publish_1(&msg, cl);

	unsubscribe_1(NULL, cl);
}
