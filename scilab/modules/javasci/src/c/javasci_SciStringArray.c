/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ENPC
 * Copyright (C) INRIA
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "CallScilab.h"
#include "javasci_SciStringArray.h"
/*****************************************************************************/
JNIEXPORT jstring JNICALL Java_javasci_SciStringArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg);

/*****************************************************************************/

/*! public native String GetElement(int indr, int indc); */
JNIEXPORT jstring JNICALL Java_javasci_SciStringArray_GetElement(JNIEnv *env , jobject obj_this,jint indrarg, jint indcarg)
/*****************************************************************************/
{
  const char *cname; 
  int indx, indy, nlr;
  char *tmpStr=MALLOC(sizeof(char)*bsiz);
  
  /* get the class */
  jclass class_Mine = (*env)->GetObjectClass(env, obj_this);
  
  /* get the fields i.e x,m,n,name  */
  jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");
  
   /* get the field value */
  jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

  jstring StrReturn;
  
  cname = (*env)->GetStringUTFChars(env, jname, NULL);

  indx= indrarg;
  indy = indcarg;

  nlr = bsiz;

  if (!C2F(creadchains)((char *)cname, &indx, &indy, &nlr, tmpStr, (unsigned long)strlen(cname), (unsigned long)strlen(tmpStr)) )
  {
	  fprintf(stderr,"Error in Java_javasci_SciStringArray_GetElement routine.\n");
  }

  (*env)->ReleaseStringUTFChars(env, jname , cname);

  StrReturn=(*env)->NewStringUTF(env, tmpStr);

  FREE(tmpStr);

  return StrReturn;
}
/*****************************************************************************/
/*! private native void SendString(String str,int indx, int indy); */
JNIEXPORT void JNICALL Java_javasci_SciStringArray_SendString(JNIEnv *env , jobject obj_this,jstring strarg,jint indxarg, jint indyarg)
/*****************************************************************************/
{
	const char *cname;
	const char *cstr;
	char Job[bsiz];
	int lencstr=0;

	/* get the class */
	jclass class_Mine = (*env)->GetObjectClass(env, obj_this);

	/* get the fields i.e x,name  */
	jfieldID id_name =  (*env)->GetFieldID(env, class_Mine, "name","Ljava/lang/String;");

	/* get the field value */
	jstring jname = (jstring) (*env)->GetObjectField(env, obj_this, id_name);

	cname = (*env)->GetStringUTFChars(env, jname, NULL);
	cstr = (*env)->GetStringUTFChars(env, strarg, NULL);

	lencstr=(int)strlen(cstr);

	if (!C2F(cwritechain)("TMP_SendString",&lencstr,(char*)cstr,(int)strlen("TMP_SendString"),lencstr) )
	{
		fprintf(stderr,"Error in Java_javasci_SciStringArray_SendString routine (1).\n");
	}
	else
	{
		sprintf(Job,"%s(%d,%d)=TMP_SendString;",cname,indyarg+1,indxarg+1);
		if (SendScilabJob(Job))
		{
			fprintf(stderr,"Error in Java_javasci_SciStringArray_SendString routine (2).\n");
		}
		else
		{
			sprintf(Job,"clear TMP_SendString;");
			if (SendScilabJob(Job))
			{
				fprintf(stderr,"Error in Java_javasci_SciStringArray_SendString routine (3).\n");
			}
		}
	}

	(*env)->ReleaseStringUTFChars(env, jname , cname);
	(*env)->ReleaseStringUTFChars(env, strarg , cstr);
}
/*****************************************************************************/
