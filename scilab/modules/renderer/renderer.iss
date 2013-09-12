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
; renderer module
;--------------------------------------------------------------------------------------------------------------
;
#define RENDERER "renderer"
;
Source: bin\jogl_desktop.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\jogl_cg.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\gluegen2-rt.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\nativewindow_awt.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\nativewindow_win32.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
Source: bin\newt.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}

Source: bin\sci{#RENDERER}.dll; DestDir: {app}\bin; Components: {#COMPN_GRAPHICS}
;Source: bin\no{#RENDERER}.dll; DestDir: {app}\bin; DestName: scirenderer.dll; Components: not ({#COMPN_GRAPHICS})
;
Source: thirdparty\gluegen2-rt.jar; DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
Source: thirdparty\jogl2.jar;DestDir: {app}\thirdparty; Components: {#COMPN_GRAPHICS}
Source: modules\scirenderer\jar\scirenderer.jar;DestDir: {app}\modules\scirenderer\jar; Components: {#COMPN_GRAPHICS}
;
Source: thirdparty\fonts\scilabsymbols.ttf;DestDir: {app}\thirdparty\fonts; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#RENDERER}\jar\org.scilab.modules.{#RENDERER}.jar;DestDir: {app}\modules\{#RENDERER}\jar; Components: {#COMPN_GRAPHICS}
;

Source: modules\{#RENDERER}\license.txt; DestDir: {app}\modules\{#RENDERER}; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#RENDERER}\sci_gateway\{#RENDERER}_gateway.xml; DestDir: {app}\modules\{#RENDERER}\sci_gateway; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#RENDERER}\includes\*.h; DestDir: {app}\modules\{#RENDERER}\includes; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#RENDERER}\etc\{#RENDERER}.quit; DestDir: {app}\modules\{#RENDERER}\etc; Components: {#COMPN_GRAPHICS}
Source: modules\{#RENDERER}\etc\{#RENDERER}.start; DestDir: {app}\modules\{#RENDERER}\etc; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#RENDERER}\macros\buildmacros.sce; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#RENDERER}\macros\buildmacros.bat; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
Source: modules\{#RENDERER}\macros\cleanmacros.bat; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#RENDERER}\macros\*.bin; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#RENDERER}\macros\*.sci; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#RENDERER}\macros\lib; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
;Source: modules\{#RENDERER}\macros\names; DestDir: {app}\modules\{#RENDERER}\macros; Components: {#COMPN_GRAPHICS}
;
;Source: modules\{#RENDERER}\demos\*.*; DestDir: {app}\modules\{#RENDERER}\demos; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS}
;
Source: modules\{#RENDERER}\tests\*.*; DestDir: {app}\modules\{#RENDERER}\tests; Flags: recursesubdirs; Components: {#COMPN_GRAPHICS} and {#COMPN_TESTS}
;
;--------------------------------------------------------------------------------------------------------------
