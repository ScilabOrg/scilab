;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; core module
;--------------------------------------------------------------------------------------------------------------
;
#define CORE "core"
;
Source: bin\{#CORE}_f.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#CORE}_f.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#CORE}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#CORE}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}

;
Source: modules\{#CORE}\jar\org.scilab.modules.{#CORE}.jar;DestDir: {app}\modules\{#CORE}\jar; Components: {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#CORE}\license.txt; DestDir: {app}\modules\{#CORE}; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\sci_gateway\{#CORE}_gateway.xml; DestDir: {app}\modules\{#CORE}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\etc\{#CORE}.quit; DestDir: {app}\modules\{#CORE}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\etc\{#CORE}.start; DestDir: {app}\modules\{#CORE}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#CORE}\includes\*.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\BOOL.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\core_math.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\doublecomplex.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\freeArrayOfString.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\getcommandlineargs.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\getos.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\getmemory.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\machine.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\mode_exec.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\MALLOC.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\PATH_MAX.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\SCIHOME.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\scisparse.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\sciquit.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\scilabDefaults.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\scilabmode.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\setgetSCIpath.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\stack1.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\stack2.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\stack3.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\stack-c.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\stack-optional.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\sci_types.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\stack-def.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\stack.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\stackinfo.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\stackTypeVariable.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\version.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\warningmode.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\win_mem_alloc.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\getversion.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\Thread_Wrapper.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\includes\backtrace_print.h; DestDir: {app}\modules\{#CORE}\includes; Components: {#COMPN_SCILAB}

;
Source: modules\{#CORE}\macros\buildmacros.sce; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\buildmacros.bat; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\cleanmacros.bat; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\lib; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\names; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\*.bin; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#CORE}\macros\*.sci; DestDir: {app}\modules\{#CORE}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\xml\*.dtd; DestDir: {app}\modules\{#CORE}\xml; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\demos\*.*; DestDir: {app}\modules\{#CORE}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
; Source: modules\{#CORE}\examples\*.*; DestDir: {app}\modules\{#CORE}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#CORE}\tests\*.*; DestDir: {app}\modules\{#CORE}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
