;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) INRIA - Allan CORNET
; Copyright (C) DIGITEO - 2010-2012 - Allan CORNET
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
; javasci module
;--------------------------------------------------------------------------------------------------------------
;
#define JAVASCI "javasci"
;
Source: modules\{#JAVASCI}\license.txt; DestDir: {app}\modules\{#JAVASCI}; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: bin\{#JAVASCI}.dll; DestDir: {app}\bin; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#JAVASCI}\jar\org.scilab.modules.{#JAVASCI}.jar;DestDir: {app}\modules\{#JAVASCI}\jar; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#JAVASCI}\etc\{#JAVASCI}.quit; DestDir: {app}\modules\{#JAVASCI}\etc; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#JAVASCI}\etc\{#JAVASCI}.start; DestDir: {app}\modules\{#JAVASCI}\etc; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#JAVASCI}\macros\buildmacros.sce; DestDir: {app}\modules\{#JAVASCI}\macros; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#JAVASCI}\macros\buildmacros.bat; DestDir: {app}\modules\{#JAVASCI}\macros; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
Source: modules\{#JAVASCI}\macros\cleanmacros.bat; DestDir: {app}\modules\{#JAVASCI}\macros; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;Source: modules\{#JAVASCI}\macros\*.bin; DestDir: {app}\modules\{#JAVASCI}\macros; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;Source: modules\{#JAVASCI}\macros\*.sci; DestDir: {app}\modules\{#JAVASCI}\macros; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;Source: modules\{#JAVASCI}\macros\lib; DestDir: {app}\modules\{#JAVASCI}\macros; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;Source: modules\{#JAVASCI}\macros\names; DestDir: {app}\modules\{#JAVASCI}\macros; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
;Source: modules\{#JAVASCI}\includes\*.h; DestDir: {app}\modules\{#JAVASCI}\includes; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#JAVASCI}\javadoc\*.*; DestDir: {app}\modules\{#JAVASCI}\javadoc; Flags: recursesubdirs; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#JAVASCI}\examples\*.*; DestDir: {app}\modules\{#JAVASCI}\examples; Flags: recursesubdirs; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE}
;
Source: modules\{#JAVASCI}\tests\*.*; DestDir: {app}\modules\{#JAVASCI}\tests; Flags: recursesubdirs; Components:  {#COMPN_SCILAB} and {#COMPN_JVM_MODULE} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
