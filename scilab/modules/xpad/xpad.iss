;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
; xpad module
;--------------------------------------------------------------------------------------------------------------
;
#define XPAD "xpad"
;
Source: bin\sci{#XPAD}.dll; DestDir: {app}\bin; Components: {#COMPN_XPAD}
;
Source: modules\{#XPAD}\jar\org.scilab.modules.{#XPAD}.jar; DestDir: {app}\modules\{#XPAD}\jar; Components: {#COMPN_XPAD}
;
Source: modules\{#XPAD}\VERSION.xml; DestDir: {app}\modules\{#XPAD}; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\readme.txt; DestDir: {app}\modules\{#XPAD}; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\license.txt; DestDir: {app}\modules\{#XPAD}; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\changelog.txt; DestDir: {app}\modules\{#XPAD}; Components: {#COMPN_XPAD}
;
Source: modules\{#XPAD}\sci_gateway\{#XPAD}_gateway.xml; DestDir: {app}\modules\{#XPAD}\sci_gateway; Components: {#COMPN_XPAD}
;
Source: modules\{#XPAD}\etc\{#XPAD}.quit; DestDir: {app}\modules\{#XPAD}\etc; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\etc\{#XPAD}.start; DestDir: {app}\modules\{#XPAD}\etc; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\etc\{#XPAD}Configuration.xml; DestDir: {app}\modules\{#XPAD}\etc; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\etc\keysConfiguration.xml; DestDir: {app}\modules\{#XPAD}\etc; Components: {#COMPN_XPAD}
;
Source: modules\{#XPAD}\macros\lib; DestDir: {app}\modules\{#XPAD}\macros; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\macros\names; DestDir: {app}\modules\{#XPAD}\macros; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\macros\*.bin; DestDir: {app}\modules\{#XPAD}\macros; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\macros\*.sci; DestDir: {app}\modules\{#XPAD}\macros; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\macros\buildmacros.sce; DestDir: {app}\modules\{#XPAD}\macros; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\macros\buildmacros.bat; DestDir: {app}\modules\{#XPAD}\macros; Components: {#COMPN_XPAD}
Source: modules\{#XPAD}\macros\cleanmacros.bat; DestDir: {app}\modules\{#XPAD}\macros; Components: {#COMPN_XPAD}
;
;Source: modules\{#XPAD}\includes\*.h; DestDir: {app}\modules\{#XPAD}\includes; Components: {#COMPN_XPAD}
;
;
;Source: modules\{#XPAD}\demos\*.*; DestDir: {app}\modules\{#XPAD}\demos; Flags: recursesubdirs; Components: {#COMPN_XPAD}
;
Source: modules\{#XPAD}\tests\*.*; DestDir: {app}\modules\{#XPAD}\tests; Flags: recursesubdirs; Components: {#COMPN_XPAD} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
