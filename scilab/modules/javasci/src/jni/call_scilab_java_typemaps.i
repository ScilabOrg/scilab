/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */


//////////////////////////////////////////////



//////////////////////////////////////////////

%define JAVASCI_ARRAYS_IMPL(CTYPE, JNITYPE, JAVATYPE, JAVAPRIMITIVETYPE)

%typemap(jni) (CTYPE *) "jobjectArray"
%typemap(jtype) (CTYPE *) "JAVAPRIMITIVETYPE[][]"
%typemap(jstype) (CTYPE *) "JAVAPRIMITIVETYPE[][]"
%typemap(javain) CTYPE *OUTVALUE "$javainput"
%typemap(javaout) (CTYPE *) {
    return $jnicall;
}


%typemap(javain) CTYPE[ANY], CTYPE[] "$javainput"

//////////////////////////

//%include "arrays_java.i"

/* Transform the input datatype CTYPE[] to JAVAPRIMITIVETYPE[][] to facilitate the
matching in Java */
%typemap(jni) CTYPE[] "jobjectArray"
%typemap(jtype) CTYPE[] "JAVAPRIMITIVETYPE[][]"
%typemap(jstype) CTYPE[] "JAVAPRIMITIVETYPE[][]"


%typemap(argout) (CTYPE variable[], int nbRow, int nbCol) {
// Specific target because it was freeing the wrong argument
    free($1);
}


%typemap(in) (CTYPE variable[], int nbRow, int nbCol) {
// Convert the CTYPE[][] => CTYPE *
      int i = 0, j = 0;
      $2 = (*jenv)->GetArrayLength(jenv, $input);
      $3 = 0;
      $1 = NULL;
      

      for (; i < $2; i++)
      {
          jboolean isCopy = JNI_FALSE;
          ##JNITYPE##* element = NULL;
          ##JNITYPE##Array oneDim = (##JNITYPE##Array)(*jenv)->GetObjectArrayElement(jenv, $input, i);
          if ($3 == 0)
	  {
              /* First time we are here, init + create the array where we store the data */
              $3 = (*jenv)->GetArrayLength(jenv, oneDim);
              $1 = (CTYPE*)malloc(sizeof(CTYPE) * $2 * $3);
          }
	  isCopy = JNI_FALSE;
	  element = (##JNITYPE##*)(*jenv)->GetPrimitiveArrayCritical(jenv, oneDim, &isCopy);

          for (j = 0; j < $3; j++)
	  {
              $1[j * $2 + i] = element[j];
          }
	  (*jenv)->ReleasePrimitiveArrayCritical(jenv, oneDim, element, JNI_ABORT);
	  (*jenv)->DeleteLocalRef(jenv, oneDim);
      }
}

%typemap(in) (CTYPE imag[], int nbRowI, int nbColI) = (CTYPE variable[], int nbRow, int nbCol);
%typemap(argout) (CTYPE imag[], int nbRowI, int nbColI) = (CTYPE variable[], int nbRow, int nbCol);
%enddef

// See SWIG documentation for the full list:
// http://www.swig.org/Doc1.3/Java.html#default_primitive_type_mappings

// Scilab: int8
JAVASCI_ARRAYS_IMPL(byte, jbyte, Byte, byte)     /* signed char[] */
JAVASCI_ARRAYS_IMPL(unsigned char, jshort, Short, jshort) /* unsigned char */

// Scilab: int16

JAVASCI_ARRAYS_IMPL(short, jshort, Short, short)         /* short[] */
JAVASCI_ARRAYS_IMPL(unsigned short, jchar, Char, short)   /* unsigned short[] */

// Scilab: int32
JAVASCI_ARRAYS_IMPL(int, jint, Int, int)                 /* int[] */
JAVASCI_ARRAYS_IMPL(unsigned int, jint, Int, int)     /* unsigned int[] */

#ifdef __SCILAB_INT64__
// Scilab: int64
JAVASCI_ARRAYS_IMPL(long, jint, int, long)               /* long[] */
JAVASCI_ARRAYS_IMPL(unsigned long, jlong, Long, long)   /* unsigned long[] */
#endif

//JAVASCI_ARRAYS_IMPL(float, jfloat, Float, float, )         /* float[] */
// Scilab: double
JAVASCI_ARRAYS_IMPL(double, jdouble, Double, double)     /* double[] */
// Scilab: boolean
JAVASCI_ARRAYS_IMPL(BOOL, jboolean, Boolean, boolean)     /* double[] */
