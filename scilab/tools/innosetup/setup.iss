;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
;
;-------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;-------------------------------------------------------------------------------

; data to modify with version
#ifdef SCILAB_X64
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
#endif
; Detect if scilab runs
AppMutex={#ScilabBaseDirectory}
;
SourceDir={#BinariesSourcePath}
#ifndef SCILAB_WITHOUT_JRE
OutputBaseFilename={#ScilabBaseFilename}
#else
OutputBaseFilename={#ScilabBaseFilename}-nojre
#endif
AppName={#ScilabName}
AppVerName={#ScilabName}
DefaultDirName={code:DefDirRoot}\{#ScilabBaseDirectory}
DefaultGroupName={#ScilabName}
SetupIconFile=tools\innosetup\scilab.ico
;InfoAfterfile=README_Windows.txt
LicenseFile=COPYING
ChangesAssociations=yes
WindowVisible=false
AppPublisher=Scilab Enterprises
AppPublisherURL=http://www.scilab.org
AppSupportURL=http://bugzilla.scilab.org/
AppUpdatesURL=http://www.scilab.org/products/scilab/download
WizardImageStretch=no
WizardImageBackColor=clBlack
WizardImageFile=tools\innosetup\ScilabLogo.bmp
WizardSmallImageFile=tools\innosetup\ScilabLogoSmall.bmp
BackColor=clGray
BackColor2=clBlack
BackColorDirection=lefttoright
AppCopyright=Scilab Enterprises - Copyright � {#CurrentYear}
UninstallDisplayIcon={app}\bin\wscilex.exe
Compression=lzma/ultra64
InternalCompressLevel=ultra64
SolidCompression=true
VersionInfoVersion={#ScilabVersion}
VersionInfoCompany=Scilab Enterprises
; minimun right to install Scilab
PrivilegesRequired=none
;-------------------------------------------------------------------------------
;
