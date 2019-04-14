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
		printf("server not found\n");
		exit(1);
	}

	set_channel_1(&topic, cl);
	printf("topic set to %s\n", topic);
	void *ptr = NULL;
	printf("subscribing...\n");
	subscribe_1(ptr, cl);
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
			publish_1(&msg2, cl);
		}

		void *ptr2 = NULL;
		printf("unsubscribing...\n");
		kill(pid, SIGKILL);
		unsubscribe_1(ptr2, cl);
		printf("finished\nquitting...\n");
	}
}
