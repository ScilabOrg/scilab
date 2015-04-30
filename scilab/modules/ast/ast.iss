;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Pierre MARECHAL
; Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;--------------------------------------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;--------------------------------------------------------------------------------------------------------------
; AST module
;--------------------------------------------------------------------------------------------------------------

#define AST "ast"
#define AST_TOOLS "ast-tools"
;
Source: bin\{#AST}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\{#AST}.lib; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\includes\analysis\*.*; DestDir: {app}\modules\{#AST}\includes\analysis; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\includes\ast\*.*; DestDir: {app}\modules\{#AST}\includes\ast; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\includes\exps\*.*; DestDir: {app}\modules\{#AST}\includes\exps; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\includes\operations\*.*; DestDir: {app}\modules\{#AST}\includes\operation; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\includes\parse\*.*; DestDir: {app}\modules\{#AST}\includes\parse; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\includes\symbol\*.*; DestDir: {app}\modules\{#AST}\includes\symbol; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\includes\system_env\*.*; DestDir: {app}\modules\{#AST}\includes\system_env; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\includes\types\*.*; DestDir: {app}\modules\{#AST}\includes\types; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\etc\{#AST}.quit; DestDir: {app}\modules\{#AST}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\etc\{#AST}.start; DestDir: {app}\modules\{#AST}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\macros\buildmacros.sce; DestDir: {app}\modules\{#AST}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\buildmacros.bat; DestDir: {app}\modules\{#AST}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\cleanmacros.bat; DestDir: {app}\modules\{#AST}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\lib; DestDir: {app}\modules\{#AST}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\*.sci; DestDir: {app}\modules\{#AST}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#AST}\macros\*.bin; DestDir: {app}\modules\{#AST}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#AST}\tests\*.*; DestDir: {app}\modules\{#AST}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
