/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <jni.h>
#include <string.h>
#include "system_getproperty.h"
#include "getScilabJNIEnv.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
void system_total_memory()
{
    jlong totalMem = 0;
    jlong freeMem = 0;
    JNIEnv *env = (JNIEnv *)getScilabJNIEnv();

    //get class Runtime
    jclass runtime = (*env)->NewGlobalRef(env, (jclass)(*env)->FindClass(env, "java/lang/Runtime"));

    //get pointer on function Runtime::getRuntime
    jmethodID getRuntimeID = (*env)->GetStaticMethodID(env, runtime, "getRuntime", "()Ljava/lang/Runtime;");

    //get pointer on Runtime::gc
    jmethodID gcID = (*env)->GetMethodID(env, runtime, "gc", "()V");

    //get pointer on Runtime::totalMemory
    jmethodID totalMemoryID = (*env)->GetMethodID(env, runtime, "totalMemory", "()J");

    //get pointer on Runtime::freeMemory
    jmethodID freeMemoryID = (*env)->GetMethodID(env, runtime, "freeMemory", "()J");

    //get Runtime pointer
    jobject rt = (*env)->CallStaticObjectMethod(env, runtime, getRuntimeID);

    //call totalMemory function
    (*env)->CallVoidMethod(env, rt, gcID);
    totalMem = (*env)->CallLongMethod(env, rt, totalMemoryID);
    freeMem = (*env)->CallLongMethod(env, rt, freeMemoryID);

    printf("total mem : %d Mo\n", totalMem / (1024 * 1024));
    printf("free  mem : %d Mo\n", freeMem / (1024 * 1024));
}

char * system_getproperty(char *property, char *defaultproperty)
{
    char *retValue = NULL;
    JNIEnv * currentENV = getScilabJNIEnv();

    system_total_memory();
    if (currentENV)
    {
        jclass cls = NULL;
        cls = (*currentENV)->FindClass(currentENV, "java/lang/System");
        if (cls)
        {
            jmethodID mid = NULL;
            mid = (*currentENV)->GetStaticMethodID(currentENV, cls, "getProperty", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
            if (mid)
            {
                const char *strValue = NULL;
                jstring jstrProperty;
                jstring jstrDefaultValue;
                jstring jstrValue;
                jstrProperty = (*currentENV)->NewStringUTF(currentENV, property);
                jstrDefaultValue = (*currentENV)->NewStringUTF(currentENV, defaultproperty);

                jstrValue = (*currentENV)->CallStaticObjectMethod(currentENV, cls, mid, jstrProperty, jstrDefaultValue);

                strValue = (*currentENV)->GetStringUTFChars(currentENV, jstrValue, 0);
                if (strValue)
                {
                    retValue = strdup(strValue);
                }
                (*currentENV)->ReleaseStringUTFChars(currentENV, jstrValue , strValue);
            }
        }
    }
    return retValue;
}
/*--------------------------------------------------------------------------*/
