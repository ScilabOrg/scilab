/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* getScilabVariable.i */
/**
 * Windows: swig -java -package org.scilab.modules.types -outdir ../java/org/scilab/modules/types/ getScilabVariable.i
 * Other: Use the option --enable-build-swig to the configure
*/

%module GetScilabVariable

%{
#include "GetScilabVariable.h"
%}

#ifdef SWIGJAVA

/* JavaDoc for GetScilabVariableJNI class */
%pragma(java) jniclassclassmodifiers=%{

/* It is generated code. Disable checkstyle */
//CHECKSTYLE:OFF
 /**
   * Connector for Javasci v2.
   * This class should <b>not</b> be used directly
   * @see org.scilab.modules.javasci.Scilab
   * @author DIGITEO - Sylvestre LEDRU
   */
class%}

/* Constructor for GetScilabVariableJNI class */
%pragma(java) jniclasscode="
  /**
    * Constructor
    */
  protected GetScilabVariableJNI() {
    throw new UnsupportedOperationException();
  }";

/* static load of library */
%pragma(java) jniclasscode=%{
  static {
   if (System.getProperty("os.name").toLowerCase().indexOf("win") >= 0) {
    try {
        System.loadLibrary("scitypes");
    } catch (SecurityException e) {
        System.err.println("A security manager exists and does not allow the loading of the specified dynamic library :");
        e.printStackTrace(System.err);
    } catch (UnsatisfiedLinkError e)    {
        System.err.println("The native library commons does not exist or cannot be found.");
        e.printStackTrace(System.err);
    }
   }
  }
%}

/* JavaDoc for  class */
%pragma(java) moduleclassmodifiers="
 /**
   * Connector for Javasci v2.
   * This class should <b>not</b> be used directly
   * @see org.scilab.modules.javasci.Scilab
   * @author DIGITEO - Sylvestre LEDRU
   */
public class";

/* Constructor for Call_Scilab class */
%pragma(java) modulecode="
 /**
   * Constructor
   */
 protected GetScilabVariable() {
    throw new UnsupportedOperationException();
 }";

#endif

void getScilabVariable(const char * variableName, int, int);
