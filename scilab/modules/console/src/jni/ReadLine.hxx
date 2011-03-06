#ifndef __ORG_SCILAB_MODULES_GUI_BRIDGE_READLINE__
#define __ORG_SCILAB_MODULES_GUI_BRIDGE_READLINE__
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <jni.h>

#include "GiwsException.hxx"

namespace org_scilab_modules_gui_bridge {
  class ReadLine {

  public:
    static char * readLine(JavaVM * jvm_);
    static const std::string className()
    {
      return "org/scilab/modules/gui/bridge/CallScilabBridge";
    }

  };
}
#endif
