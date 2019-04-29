all: pub_sub_client pub_sub_server client

pub_sub_client: pub_sub_deliv.c pub_sub_deliv_xdr.c pub_sub_deliv_clnt.c pub_sub_deliv.h
	gcc -o pub_sub_deliv pub_sub_xdr.c pub_sub_deliv_xdr.c pub_sub_deliv_svc.c pub_sub_deliv.c pub_sub_clnt.c

pub_sub_server: pub_sub.c pub_sub_svc.c pub_sub_xdr.c pub_sub.h sha_256.h sha_256.c
	gcc -o pub_sub pub_sub_xdr.c pub_sub_deliv_xdr.c pub_sub_deliv_clnt.c pub_sub.c pub_sub_svc.c sha_256.c -lssl -lcrypto 

client: client.c
	gcc -o client pub_sub_xdr.c pub_sub_clnt.c client.c
clean:
	rm pub_sub_deliv pub_sub
