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
/* file: scigfx_CallBackable.cpp                                              */
/* desc : implement the methodes of CallBackable                              */
/*  //TODO étoffer                                                            */
/*----------------------------------------------------------------------------*/

#include "scigfx_CallBackable.h"

  // Include nécessaire au CastObject
/*----------------------------------------------------------------------------*/
#include "scigfx_CastObject.h"

    // C function about CallBackable
/* -------------------------------------------------------------------------- */
extern "C"
{
  void SetCallback(sciPointObj * pobj, char *code, int len, int mevent)
  {
    CastObject<CallBackable*>(pobj)->setCallback(code, len, mevent);
  }
  void GetCallback(sciPointObj * pobj, char *code)
  {
    CastObject<CallBackable*>(pobj)->getCallback(code);
  }
  void SetCallbackMouseEvent(sciPointObj * pobj, int mevent)
  {
    CastObject<CallBackable*>(pobj)->setCallbackMouseEvent(mevent);
  }
  void ResetCallback(sciPointObj * pobj)
  {
    CastObject<CallBackable*>(pobj)->resetCallback();
  }
  int GetCallbackMouseEvent(sciPointObj * pobj)
  {
    return CastObject<CallBackable*>(pobj)->getCallbackMouseEvent();
  }
  int GetCallbackSize(sciPointObj * pobj)
  {
    return CastObject<CallBackable*>(pobj)->getCallbackSize();
  }
}
/* -------------------------------------------------------------------------- */



CallBackable::CallBackable():
    mouseEvent(100)
{
}


void CallBackable::setCallback(char *code, int len, int mevent)
{
  callback = code;
  mouseEvent = mevent;
}


void CallBackable::getCallback(char *code)
{
  strcpy(code,callback.c_str());
}


int CallBackable::getCallbackSize()
{
  return callback.size();
}


void CallBackable::setCallbackMouseEvent(int mevent)
{
  mouseEvent = mevent;
}

void CallBackable::resetCallback()
{
  mouseEvent=100;
  callback.clear();
}


int CallBackable::getCallbackMouseEvent()
{
  return mouseEvent;
}
