all: pub_sub_client pub_sub_server

rls_client: pub_sub_deliv.c pub_sub_deliv_xdr.c pub_sub_deliv_clnt.c pub_sub_deliv.h
	gcc -o pub_sub_deliv pub_sub_deliv_xdr.c pub_sub_deliv_clnt.c pub_sub_deliv.c

rls_server: pub_sub.c pub_sub_svc.c pub_sub_xdr.c pub_sub.h
	gcc -o pub_sub pub_sub_xdr.c pub_sub_svc_proc.c pub_sub_svc.c

clean:
	rm pub_sub_deliv pub_sub
