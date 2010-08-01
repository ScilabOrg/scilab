/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "MALLOC.h"
#include "stack-c.h"
#include "msgstore.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/ 
#define MEM_LACK 3
#define MAX_LINES 2
/*--------------------------------------------------------------------------*/ 
static char *msg_buff[MAX_MSG_LINES];
static char funname[nlgh+1];    /* Francois VOGEL August 2004 - Replaced 24 by 25 (nlgh+1) (bug 803)*/
static int where = 0;
static int err_n = 0;
static int msg_line_counter=0;
/*--------------------------------------------------------------------------*/ 
int C2F(errstore)(int *n)
{
  err_n = *n;
  return 0;
}
/*--------------------------------------------------------------------------*/ 
int C2F(linestore)(int *n)
{
  where = *n;
  return 0;
}
/*--------------------------------------------------------------------------*/ 
int C2F(funnamestore)(char *str,int *n,int lenstr)
{
  memset(funname,'\0',nlgh+1);    /* Francois VOGEL August 2004 (bug 803)*/
  strncpy(funname, str, (size_t)*n);
  return 0;
}
/*--------------------------------------------------------------------------*/ 
int C2F(msgstore)(char *str,int *n)
{
  char *line, c;
  int i,count=0;
  if (msg_line_counter >= MAX_MSG_LINES)
  {
    return(MAX_LINES);
  }
  if ( (line = (char *) MALLOC((*n + 1)*sizeof(char))) == (char *)0) 
  {
	  return(MEM_LACK);
  }
  /* do not store\n" */ 
  for ( i= 0 ; i < *n ; i++ ) 
  {
    if ( (c=str[i]) != '\n' && c != '\r')
	{
		line[count++]=c;
	}
  }
  line[count]='\0';
  msg_buff[msg_line_counter++]=line;
  return 0;
}
/*--------------------------------------------------------------------------*/ 
void C2F(freemsgtable)()
{
  int k;
  for (k=0 ; k< msg_line_counter ; k++)
  {
    FREE(msg_buff[k]);
  }
  msg_line_counter=0;
  err_n = 0;
}
/*--------------------------------------------------------------------------*/ 
int C2F(lasterror)(char *fname, unsigned long fname_len)
{
  int k, one=1, l1, zero=0, m1, n1, clear, lr;
  int sz[MAX_MSG_LINES];

  Rhs = Max(0, Rhs);
  CheckRhs(0,1);
  CheckLhs(1,4);
  if (msg_line_counter == 0)
  {
    CreateVar(1,MATRIX_OF_DOUBLE_DATATYPE,&zero,&zero,&l1);
    LhsVar(1)=1;
    if (Lhs >= 2) 
	{
      CreateVar(2,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&l1);
      *stk(l1) = (double)0.0;
      LhsVar(2)=2;
    }
    /* Francois VOGEL August 2004 - Added initialization of missing lhs vars in case
       there is no current 'last error' (bug 955)*/
    if (Lhs >= 3)
    {
      CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&l1);
      *stk(l1) = (double)0.0;
      LhsVar(3)=3;
    }
    if (Lhs >= 4)
    {
      l1=0;
      CreateVar(4,STRING_DATATYPE, &one,&l1 , &lr);
      strcpy(cstk(lr),"");
      LhsVar(4)=4;
    }
  }
  else
  {
    clear = 1;
    if (Rhs==1)
    {
      GetRhsVar(1,MATRIX_OF_BOOLEAN_DATATYPE,&m1,&n1,&l1);
      clear = *istk(l1);
    }
    for (k=0;k<msg_line_counter ; k++)
	{
		sz[k]=(int)strlen(msg_buff[k])-1;
	}
    CreateVarFromPtr(one,MATRIX_OF_STRING_DATATYPE, &msg_line_counter, &one,(void *) msg_buff);
    LhsVar(1) = 1;
    if (Lhs >= 2)
    {
      CreateVar(2,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&l1);
      *stk(l1) = (double)err_n;
      LhsVar(2)=2;
    }
    if (Lhs >= 3)
    {
      CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE,&one,&one,&l1);
      *stk(l1) = (double)where;
      LhsVar(3)=3;
    }
    if (Lhs >= 4)
    {
      l1=(int)strlen(funname);
      CreateVar(4,STRING_DATATYPE, &one,&l1 , &lr);
      strcpy(cstk(lr),funname);
      LhsVar(4)=4;
    }
    if (clear) {
      where=0;
      funname[0]='\0';
      C2F(freemsgtable)();
    }
  }
  C2F(putlhsvar)();
  return(0);
}
/*--------------------------------------------------------------------------*/
int GetLastErrorCode(void)
{
	return err_n;
}
/*--------------------------------------------------------------------------*/ 
