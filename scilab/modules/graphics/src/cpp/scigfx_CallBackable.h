/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*----------------------------------------------------------------------------*/
/* file: scigfx_CallBackable.h                                                */
/* desc : declare the class CallBackable                                      */
/*  //TODO                                                                    */
/*----------------------------------------------------------------------------*/


#ifndef _SCIGFX_CALLBACKABLE_H_
#define _SCIGFX_CALLBACKABLE_H_

  // Include nécessaire au std::string & strcpy
/*----------------------------------------------------------------------------*/
#include <string>
#include <cstring>

  // Include nécessaire pour sciPointObj
/*----------------------------------------------------------------------------*/
#include "ObjectStructure.h"

    // C function about CallBackable
/* -------------------------------------------------------------------------- */
extern "C"
{
  void SetCallback(sciPointObj * pObj, char *code, int len, int mevent);
  void GetCallback(sciPointObj * pObj, char *code);
  void SetCallbackMouseEvent(sciPointObj * pObj, int mevent);
  void ResetCallback(sciPointObj * pObj);
  int GetCallbackMouseEvent(sciPointObj * pObj);
  int GetCallbackSize(sciPointObj * pObj);
}
/* -------------------------------------------------------------------------- */

class CallBackable
{
protected:
  std::string callback;
  int mouseEvent;
public:
  CallBackable();
  void setCallback(char *code, int len, int mevent);
  void getCallback(char *code);
  int  getCallbackSize();

  void setCallbackMouseEvent(int mevent);
  int  getCallbackMouseEvent();
  void resetCallback();
};


#endif //_SCIGFX_CALLBACKABLE_H_
