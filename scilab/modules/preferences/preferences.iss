;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab

; Copyright (C) DIGITEO - 2011-2012 - Allan CORNET
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
; preferences module
;--------------------------------------------------------------------------------------------------------------
;
#define PREFERENCES "preferences"
;
Source: bin\{#PREFERENCES}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#PREFERENCES}\jar\org.scilab.modules.{#PREFERENCES}.jar; DestDir: {app}\modules\{#PREFERENCES}\jar; Components: {#COMPN_SCILAB}
;
Source: modules\{#PREFERENCES}\license.txt; DestDir: {app}\modules\{#PREFERENCES}; Components: {#COMPN_SCILAB}
;
Source: modules\{#PREFERENCES}\sci_gateway\{#PREFERENCES}_gateway.xml; DestDir: {app}\modules\{#PREFERENCES}\sci_gateway; Components: {#COMPN_SCILAB}
;
Source: modules\{#PREFERENCES}\etc\{#PREFERENCES}.quit; DestDir: {app}\modules\{#PREFERENCES}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#PREFERENCES}\etc\{#PREFERENCES}.start; DestDir: {app}\modules\{#PREFERENCES}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#PREFERENCES}\etc\*.xml; DestDir: {app}\modules\{#PREFERENCES}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#PREFERENCES}\etc\*.xsl; DestDir: {app}\modules\{#PREFERENCES}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#PREFERENCES}\src\xslt\*.xsl; DestDir: {app}\modules\{#PREFERENCES}\src\xslt; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#PREFERENCES}\includes\*.h; DestDir: {app}\modules\{#PREFERENCES}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#PREFERENCES}\macros\buildmacros.sce; DestDir: {app}\modules\{#PREFERENCES}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#PREFERENCES}\macros\buildmacros.bat; DestDir: {app}\modules\{#PREFERENCES}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#PREFERENCES}\macros\cleanmacros.bat; DestDir: {app}\modules\{#PREFERENCES}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#PREFERENCES}\macros\lib; DestDir: {app}\modules\{#PREFERENCES}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#PREFERENCES}\macros\names; DestDir: {app}\modules\{#PREFERENCES}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#PREFERENCES}\macros\*.bin; DestDir: {app}\modules\{#PREFERENCES}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#PREFERENCES}\macros\*.sci; DestDir: {app}\modules\{#PREFERENCES}\macros; Components: {#COMPN_SCILAB}
;
;Source: modules\{#PREFERENCES}\examples\*.*; DestDir: {app}\modules\{#PREFERENCES}\examples; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#PREFERENCES}\demos\*.*; DestDir: {app}\modules\{#PREFERENCES}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#PREFERENCES}\tests\*.*; DestDir: {app}\modules\{#PREFERENCES}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
