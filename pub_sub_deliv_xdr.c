/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "pub_sub_deliv.h"

bool_t
xdr_postmessage (XDR *xdrs, postmessage *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, objp, POSTMESLEN))
		 return FALSE;
	return TRUE;
}
