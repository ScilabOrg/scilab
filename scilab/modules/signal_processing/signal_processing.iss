;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010 - Allan CORNET
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
; signal_processing module
;--------------------------------------------------------------------------------------------------------------
;
#define SIGPROC "signal_processing"
;
Source: bin\{#SIGPROC}_f.dll; DestDir: {app}\bin; Components:  {#COMPN_SIGPROC}
Source: bin\{#SIGPROC}.dll; DestDir: {app}\bin; Components:  {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\VERSION.xml; DestDir: {app}\modules\{#SIGPROC}; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\license.txt; DestDir: {app}\modules\signal_processing; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\sci_gateway\{#SIGPROC}_gateway.xml; DestDir: {app}\modules\{#SIGPROC}\sci_gateway; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\etc\{#SIGPROC}.quit; DestDir: {app}\modules\{#SIGPROC}\etc; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\etc\{#SIGPROC}.start; DestDir: {app}\modules\{#SIGPROC}\etc; Components: {#COMPN_SIGPROC}
;
;Source: modules\{#SIGPROC}\macros\lib; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\*.sci; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\buildmacros.sce; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\buildmacros.bat; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
Source: modules\{#SIGPROC}\macros\cleanmacros.bat; DestDir: {app}\modules\{#SIGPROC}\macros; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\demos\*.*; DestDir: {app}\modules\{#SIGPROC}\demos; Flags: recursesubdirs; Components: {#COMPN_SIGPROC}
;
Source: modules\{#SIGPROC}\tests\*.*; DestDir: {app}\modules\{#SIGPROC}\tests; Flags: recursesubdirs; Components: {#COMPN_SIGPROC} and {#COMPN_TESTS}
;--------------------------------------------------------------------------------------------------------------
