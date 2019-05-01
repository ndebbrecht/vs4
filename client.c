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
  char user_pwd_hash[HASHLEN];

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
  strcpy(user_pwd_hash,hashtext);
  printf("user_pwd_hash%s\n",user_pwd_hash);
  printf("hashtext%s\n",hash_user_pwd(username,ppwd));
  char tmp[HASHLEN];
  char * ptmp;
  sprintf(tmp,"%d",*sessionid);
  strcat(tmp, ";;");
  strcat(tmp,hashtext);
  printf("\n\nstring hashed %s\n",tmp);
  
  ptmp=hash_sha(tmp);//Äußere hash funktion
  hashtext=ptmp;
	printf("hash client validate %s\n", ptmp);
	myParam.hash = ptmp;
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
        myParam.arg.topic_or_message=0;//0=topic 
        myParam.arg.argument_u.t=T;
        memset(tmp,'\0',HASHLEN);
        sprintf(tmp,"%d",*sessionid);
        strcat(tmp, ";");
        strcat(tmp,T);
        strcat(tmp, ";");
        strcat(tmp,user_pwd_hash);
        ptmp=hash_sha(tmp);
        myParam.hash=ptmp;
				error_no=set_channel_1(&myParam, cl);
				printf("%s\n",PUB_SUB_RET_CODE[*error_no]);
				if(*error_no==OK){
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
        //read message from console
				fgets(msg, MESLEN, stdin);
				msg[strlen(msg)-1]='\0';
        //prepar myParam for publish_1
        myParam.arg.topic_or_message=1;//1=message 
        myParam.arg.argument_u.t=msg;
        //create string given to hash_sha
        memset(tmp,'\0',HASHLEN);
        sprintf(tmp,"%d",*sessionid);
        strcat(tmp, ";");
        strcat(tmp,msg);
        strcat(tmp, ";");
        //strcpy(user_pwd_hash,"256e8a0f3e81b9bd391dad7c9bc9fda6bc73ffd54d72a77f106f2112180689e3");
        strcat(tmp,user_pwd_hash);
        printf("message die gehashed wird: %s\n",tmp);
        //hash message
        ptmp=hash_sha(tmp);
        myParam.hash=ptmp;
        printf("publish hash%s\n",myParam.hash);
				error_no=publish_1(&myParam, cl);
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
