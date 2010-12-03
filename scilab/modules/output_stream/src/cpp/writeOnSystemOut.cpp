/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - Calixte DENIZET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "writeOnSystemOut.hxx"
#include "output_stream.hxx"
extern "C"
{
#include "jni.h"
#include "getScilabJavaVM.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_output_stream;
/*--------------------------------------------------------------------------*/
int writeOnSystemOut(char* buffer)
{
  JavaVM *vm = getScilabJavaVM();
  if (vm == NULL)
  {
        JavaVM **vmBuf = new JavaVM*[1];
        jsize size = 0;
        JNI_GetCreatedJavaVMs(vmBuf, 1, &size);
        if (size)
        {
              vm = *vmBuf;
        }
        delete(vmBuf);
  }

  if (vm == NULL)
  { /* Java not yet or badly initialized */
        printf("%s", buffer);
  }
  else
  {
        output_stream::writeOnSystemOut(vm, buffer);
  }

   return 0;
}
