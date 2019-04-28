#include <stdio.h>
#include <unistd.h>
#include "pub_sub_deliv.h"
#include "pub_sub.h"
#include "return_codes.h"

int main(int argc, char *argv[]){
  static short * error_no;
	CLIENT *cl;
	char *address;
	address = argv[1];
	//char *topic;
	//topic = argv[2];
  char topic_str[TOPLEN];
  topic T=topic_str;
  char msg[MESLEN];
  char *msg2 = msg;
  void *ptr = NULL;
  void *ptr2 = NULL;
  
  unsigned int input=0;
	printf("Client starten\n");
	if(argc != 2){
		printf("Missing arguments\n");
		exit(1);
	}
	
	cl = clnt_create(address, PUBSUBPROG, PUBSUBVERS, "tcp");
 
	if(cl == NULL){
		printf("server not found\n");
		exit(1);
	}
  /*
	
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

		
		printf("unsubscribing...\n");
		error_no=unsubscribe_1(ptr2, cl);
		printf("%s",PUB_SUB_RET_CODE[*error_no]);
		kill(pid, SIGKILL);
		
		printf("%s",PUB_SUB_RET_CODE[*error_no]);
		printf("finished\nquitting...\n");
	}
 
 
 */
 while(input!=5){
    printf("Press 1 to set a Topic\n"
           "Press 2 to subscribe to a server\n"
           "Press 3 to unsubscribe from a server\n"
           "Press 4 to publish a message\n"
           "Press 5 to exit\n");
           scanf("%u",&input);
           
    switch (input){
      case 1:
        printf("Set a Topic\n");
        scanf("%s",T);
        
        
        error_no=set_channel_1(&T, cl);
        printf("%s\n",PUB_SUB_RET_CODE[*error_no]);
        if(*error_no!=CANNOT_SET_TOPIC){
		      printf("topic set to %s\n", T);
	      }
        break;
      case 2:
        error_no=subscribe_1(ptr, cl);
      	printf("%s\n",PUB_SUB_RET_CODE[*error_no]);
      	int pid = fork();
      
      	if(pid == 0){
      		printf("Starting the Receiver...\n");
      		execl("./pub_sub_deliv", "./pub_sub_deliv", NULL);
      	} 
        else {
      		printf("subscribed\n");
        }
        break;
      case 3:
        printf("unsubscribing...\n");
    		error_no=unsubscribe_1(ptr2, cl);
    		printf("%s\n",PUB_SUB_RET_CODE[*error_no]);
    		if(*error_no==OK){
          kill(pid, SIGKILL);
    		}
        break;
      case 4:
        printf("Enter a message:\n");
        
  		  //if(fgets(msg, MESLEN, stdin)!=NULL){
          //while(msg==NULL){
        fgets(msg, MESLEN, stdin);
        //}
        printf("past fgets");
  			  error_no=publish_1(&msg2, cl);
          printf("%s\n",PUB_SUB_RET_CODE[*error_no]);
  		  
        break;
      case 5:break;
      default:printf("unknown imput - please enter a number from 1 to 5 \n");
    }
 
  }
 
 
 
	return 0;
}
