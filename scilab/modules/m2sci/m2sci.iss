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
; m2sci module
;--------------------------------------------------------------------------------------------------------------
;
#define M2SCI "m2sci"
;
Source: modules\{#M2SCI}\license.txt; DestDir: {app}\modules\{#M2SCI}; Components: {#COMPN_SCILAB}
;
Source: modules\{#M2SCI}\etc\{#M2SCI}.quit; DestDir: {app}\modules\{#M2SCI}\etc; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\etc\{#M2SCI}.start; DestDir: {app}\modules\{#M2SCI}\etc; Components: {#COMPN_SCILAB}
;
Source: modules\{#M2SCI}\macros\lib; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\names; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\*.bin; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\*.sci; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
Source: modules\{#M2SCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#M2SCI}\macros; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
;Source: modules\{#M2SCI}\demos\*.*; DestDir: {app}\modules\{#M2SCI}\demos; Flags: recursesubdirs; Components: {#COMPN_SCILAB}
;
Source: modules\{#M2SCI}\tests\*.*; DestDir: {app}\modules\{#M2SCI}\tests; Flags: recursesubdirs; Components: {#COMPN_SCILAB} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
