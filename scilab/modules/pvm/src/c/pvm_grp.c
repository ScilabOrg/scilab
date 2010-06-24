/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997-1999 - INRIA - Eric FLEURY
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/***
   NAME
     pvm_grp
   PURPOSE
     
   NOTES
     
   HISTORY
     fleury - Nov 25, 1997: Created.
     $Id: pvm_grp.c,v 1.5 2005/01/07 20:49:25 cornet Exp $
***/

#include <stdio.h>
#include <string.h>

#include "sci_tools.h"
#include "stack2.h"
#include "sci_pvm.h"
#include "localization.h"


void C2F(scipvmbcast)(char *group, int *l, 
		      int *pack, int *n, 
		      double *buff,
		      int *msgtag, int *res)
{
  int info, bufid;
  double *ptr_double;
  int *ptr_int;
  int i;
  
  /*  (void) fprintf(stderr, "BCAST: %s,%d:%d:%d\n", group, *n, pack[0], pack[1]);*/

  bufid = pvm_initsend(PvmDataDefault);
  if (bufid < 0) {
    (void) fprintf(stderr, "Error pvm_bcast: -init- %d\n", bufid);
    *res = bufid;
    return;
  }

  /* Pack the size of the packing vector */
  info = pvm_pkint(n, 1, 1);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_bcast: -pack- %d\n", info);
    pvm_freebuf(bufid);
    *res = info;
    return;
  }
  /* Pack the packing vector */
  info = pvm_pkint(pack, *n, 1);
  if (info < 0) {
    (void) fprintf(stderr, "Error pvm_bcast: -pack- %d\n", info);
    pvm_freebuf(bufid);
    *res = info;
    return;
  }
  
  /* Pack the msg using the packing vector info */
  ptr_double = buff;
  ptr_int = (int*) buff;
  for (i = 0; i < *n; i+=2) {
    if (pack[i] > 0) {		/* have to pack some int */
      info = pvm_pkint(ptr_int, pack[i], 1);
      if (info < 0) {
	(void) fprintf(stderr, "Error pvm_bcast: -pack- %d\n", info);
	pvm_freebuf(bufid);
	*res = info;
	return;
      }
      ptr_int += pack[i] + (pack[i] % 2);
      ptr_double += ((pack[i]-1)/2 + 1);
    }
    if (pack[i+1] > 0) {	/* have to pack some double */
      info = pvm_pkdouble(ptr_double, pack[i+1], 1);
      if (info < 0) {
	(void) fprintf(stderr, "Error pvm_bcast: -pack- %d\n", info);
	pvm_freebuf(bufid);
	*res = info;
	return;
      }
      ptr_int += (pack[i+1]*2);
      ptr_double += pack[i+1];
    }
  }
  *res = pvm_bcast(group, *msgtag);
} /* scipvmbcast */


void C2F(scipvmreduce)(char *func, int *l1,
		       double *buff, int *m, int *n, int *msgtag, 
		       char *group, int *l2, int *rootginst, int *res)
{
  int type, datatype;
  int size;
  void (*op)(int *, void *, void *, int *, int *);
  /* Check the type of the buff */
  type = TYPE(buff);
  size = *m * *n;
  switch (type)    
    {
    case TYPE_DOUBLE :
      datatype = PVM_DOUBLE;
      break;
    case TYPE_COMPLEX :
      datatype = PVM_DCPLX;
      SciToF77(buff, size, size);
      break;
    default :
      (void) fprintf(stderr, "%s", _("Error pvm_reduce: Not scalar type\n"));
      *res = PvmBadMsg;
      return;
    }
  /* Check the reduction operation */
  if (!strcmp(func, "Max")) {
    op = PvmMax;
  }
  else if (!strcmp(func, "Min")) {
    op = PvmMin;
  } 
  else if (!strcmp(func, "Sum")) {
    op = PvmSum;
  } 
  else if (!strcmp(func, "Pro")) {
    op = PvmProduct;
  } 
  else {
    (void) fprintf(stderr, _("Error pvm_reduce: Unknown reduction operation %s\n"), func);
    *res = PvmBadMsg;
    return;
  }
  *res = pvm_reduce(op, buff, size, datatype, *msgtag, group, *rootginst);
  if (type == TYPE_COMPLEX)
    F77ToSci(buff, size, size);
  return;
} 

