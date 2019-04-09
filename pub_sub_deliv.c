
#include <stdio.h>
#include <rpc/rpc.h>
#include "pub_sub_deliv.h"

int main(int argc, char const *argv[]) {
  
  return 0;
}

 void *deliver_1_svc(postmessage *message, struct svc_req *req){
   printf("%s\n", *message);
 }
