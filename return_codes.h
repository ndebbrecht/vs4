/*
 * Status-Codes des Publish-Subscriber-Systems.
 * Die Ausgabe der passenden Status-Meldungen kann über
 * das Feld PUB_SUB_RET_CODE erfolgen.
 * Bsp.: printf ("%s\n", PUB_SUB_RET_CODE[*res_code])
 * sofern in short* res_code die Antwort auf die Anfrage
 * des RPC enthalten ist.
 */

#ifndef RETURN_CODES_H
#define	RETURN_CODES_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#define OK                          0
#define CANNOT_REGISTER             1
#define CLIENT_ALREADY_REGISTERED   2
#define CANNOT_UNREGISTER           3
#define CANNOT_SET_TOPIC            4
#define UNKNOWN_ERROR               5
#define VALIDATE_ERROR              6
#define INVALIDATE_ERROR    		    7

static char* PUB_SUB_RET_CODE[] = {
 "Ok.\n",                         /* OK                        */
 "Cannot register client.\n",     /* CANNOT_REGISTER           */
 "Client already registered.\n",  /* CLIENT_ALREADY_REGISTERED */
 "Cannot unregister client.\n",   /* CANNOT_UNREGISTER         */
 "Cannot set topic.\n",           /* CANNOT_SET_TOPIC          */
 "Unknown error.\n",              /* UNKNOWN_ERROR             */
 "Wrong username or password.\n"  /* VALIDATE_ERROR	           */
 "Session not found and cannot be deleted.\n" /* INVALIDATE_ERROR */
 //"\n"/**/
};

typedef short RET_CODE;

#endif /* RETURN_CODES_H */
