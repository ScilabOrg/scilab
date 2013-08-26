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
; matio module
;--------------------------------------------------------------------------------------------------------------
;
#define MATIO "matio"
;
Source: bin\{#MATIO}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
Source: bin\lib{#MATIO}.dll; DestDir: {app}\bin; Components: {#COMPN_SCILAB}
;
Source: modules\{#MATIO}\license.txt; DestDir: {app}\modules\{#MATIO}; Components: {#COMPN_SCILAB}
;
Source: modules\{#MATIO}\etc\{#MATIO}.quit; DestDir: {app}\modules\{#MATIO}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\etc\{#MATIO}.start; DestDir: {app}\modules\{#MATIO}\etc; Components: {#COMPN_SCILAB}
;
;Source: modules\{#MATIO}\includes\*.h; DestDir: {app}\modules\{#MATIO}\includes; Components: {#COMPN_SCILAB}
;
Source: modules\{#MATIO}\macros\lib; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\names; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\*.bin; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\*.sci; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\buildmacros.sce; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\buildmacros.bat; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
Source: modules\{#MATIO}\macros\cleanmacros.bat; DestDir: {app}\modules\{#MATIO}\macros; Components: {#COMPN_SCILAB}
;
Source: modules\{#MATIO}\sci_gateway\{#MATIO}_gateway.xml; DestDir: {app}\modules\{#MATIO}\sci_gateway; Components: {#COMPN_SCILAB}
;
;Source: modules\{#MATIO}\demos\*.*; DestDir: {app}\modules\{#MATIO}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#MATIO}\tests\*.*; DestDir: {app}\modules\{#MATIO}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
