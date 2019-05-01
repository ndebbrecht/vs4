#include <stdio.h>
#include <unistd.h>
#include "pub_sub_deliv.h"
#include "pub_sub.h"
#include "return_codes.h"
#include "sha_256.h"

int main(int argc, char *argv[]){
	static short * error_no;
	CLIENT *cl;
	char *address;
	address = argv[1];

	char topic_str[TOPLEN];
	topic T=topic_str;
	char msg[MESLEN];
	char *msg2 = msg;
	char input_c[3];

	int input_i=0;
	char *ptr;

	void *ptr2 = NULL;

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
	//Authentifizierung starten
	printf("Username:\n");
	//fflush(stdin);
	char username[USERLEN];
	user user = username;
	fgets(user,USERLEN,stdin);
	user[strlen(user)-1]='\0';
	printf("%s\n", user);
 
	sessionid* sessionid = get_session_1(&user, cl);
  printf("Passwort:\n");
	//fflush(stdin);
  char pwd[PWDLEN];
	char* ppwd = pwd;
	fgets(ppwd,PWDLEN,stdin);
	ppwd[strlen(ppwd)-1]='\0';
	printf("%s\n", ppwd);
	param myParam;
	printf("%i\n",*sessionid);
	myParam.id = *sessionid;
	char *hashtext = hash_user_pwd(username,ppwd);

  char tmp[HASHLEN];
  char * ptmp;
  sprintf(tmp,"%d",*sessionid);
  //strcat(tmp, sessionid);
  strcat(tmp, ";;");
  strcat(tmp,hashtext);
  
  ptmp=hash_sha(tmp);//Äußere hash funktion
  hashtext=ptmp;
	printf("%s\n", hashtext);
	myParam.hash = hashtext;
	error_no = validate_1(&myParam, cl);
	printf("%s\n",PUB_SUB_RET_CODE[*error_no]);
  if(*error_no!=OK){
    return 0;
  }
	while(input_i!=5){
		printf("Press 1 to set a Topic\n"
		"Press 2 to subscribe to a server\n"
		"Press 3 to unsubscribe from a server\n"
		"Press 4 to publish a message\n"
		"Press 5 to exit\n");
		fgets(input_c,3,stdin);//Zahl + Enter

		input_i=atoi(&input_c[0]);

		switch (input_i){
			case 1:
				printf("Set a Topic\n");
				//fflush(stdin);
				fgets(T,TOPLEN,stdin);
				T[strlen(T)-1]='\0';
				error_no=set_channel_1(&T, cl);
				printf("%s\n",PUB_SUB_RET_CODE[*error_no]);
				if(*error_no!=CANNOT_SET_TOPIC){
					printf("topic set to %s\n", T);
				}
				break;
			case 2:
				error_no=subscribe_1(ptr2, cl);
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

				fgets(msg, MESLEN, stdin);
				msg[strlen(msg)-1]='\0';

				error_no=publish_1(&msg2, cl);
				printf("%s\n",PUB_SUB_RET_CODE[*error_no]);

				break;
			case 5:
				error_no = invalidate_1(&myParam.id, cl);
				printf("%s\n",PUB_SUB_RET_CODE[*error_no]);
				exit(0);

				break;
			default:printf("unknown imput - please enter a number from 1 to 5 \n");
		}

	}

	return 0;
}
