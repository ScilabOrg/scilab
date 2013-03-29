;
; Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
; Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
;
; This file must be used under the terms of the CeCILL.
; This source file is licensed as described in the file COPYING, which
; you should have received as part of this distribution.  The terms
; are also available at
; http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
;
;-------------------------------------------------------------------------------
; Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
;
;-------------------------------------------------------------------------------


#define COMPN_SCILAB 'scilab'
Name: {#COMPN_SCILAB}; Description: Scilab {#ScilabVersion}; Types: full custom cli; Flags: fixed;

#define COMPN_JVM_MODULE 'JVM_MODULE'
#define COMPN_SCINOTES 'JVM_MODULE\scinotes'
#define COMPN_GRAPHICS 'JVM_MODULE\graphics'
#define COMPN_XCOS 'JVM_MODULE\graphics\xcos'
#define COMPN_JRE 'JVM_MODULE\JRE_1_6'
Name: {#COMPN_JVM_MODULE}; Description: {cm:DescriptionModuleJVM}; Types: full;
Name: {#COMPN_SCINOTES}; Description:{cm:DescriptionSciNotes}; Types: full;
Name: {#COMPN_GRAPHICS}; Description: {cm:DescriptionGraphics}; Types: full;
Name: {#COMPN_XCOS}; Description: {cm:DescriptionXcos}; Types: full;
#ifndef SCILAB_WITHOUT_JRE
Name: {#COMPN_JRE}; Description: {cm:DescriptionJVM} ({#javaUsed}); Types: full;
#endif

#define COMPN_CPU_OPTIMIZATION 'CPU_OPTIMIZATION'
#define COMPN_REF_CPU_LIBRARY 'CPU_OPTIMIZATION\CPU_REF'
#define COMPN_ATLAS_CPU_LIBRARY 'CPU_OPTIMIZATION\ATLAS'
#define COMPN_MKL_CPU_LIBRARY 'CPU_OPTIMIZATION\MKL'
Name: {#COMPN_CPU_OPTIMIZATION}; Description:{cm:DescriptionCPUOptimization}; Types: full custom cli; Flags: fixed;
Name: {#COMPN_MKL_CPU_LIBRARY}; Description:{cm:DescriptionMKLLibrary}; Flags: exclusive
#ifndef SCILAB_X64
Name: {#COMPN_ATLAS_CPU_LIBRARY}; Description:{cm:DescriptionATLASLibrary}; Flags: exclusive
#endif
Name: {#COMPN_REF_CPU_LIBRARY}; Description:{cm:DescriptionRefBlasLibrary}; Flags: exclusive

#define COMPN_FFTW 'FFTW'
#define COMPN_FFTW_REF_LIBRARY 'FFTW\REF_LIBRARY'
#define COMPN_FFTW_MKL_LIBRARY 'FFTW\MKL_LIBRARY'
Name: {#COMPN_FFTW}; Description: {cm:DescriptionFftwModule}; Types: default full custom cli; Flags: fixed;
Name: {#COMPN_FFTW_MKL_LIBRARY};Description: {cm:DescriptionFftwMKLDownload}; Types: custom cli; Flags: exclusive
Name: {#COMPN_FFTW_REF_LIBRARY};Description: {cm:DescriptionRefFftwLibrary}; Types: default full custom cli; Flags: exclusive

#define COMPN_DEVTOOLS 'Development_tools'
#define COMPN_MODULES_MANAGER 'Development_tools\modules_manager'
#define COMPN_TOOLBOX_SKELETON 'Development_tools\toolbox_skeleton'
#define COMPN_TESTS 'Development_tools\tests'
Name: {#COMPN_DEVTOOLS}; Description: {cm:DescriptionDevTools}; Types: full;
Name: {#COMPN_MODULES_MANAGER}; Description: {cm:DescriptionModulesManager}; Types: full;
Name: {#COMPN_TOOLBOX_SKELETON}; Description: {cm:DescriptionTlbxSkeleton}; Types: full;
Name: {#COMPN_TESTS}; Description: {cm:DescriptionTests}; Types: full; ExtraDiskSpaceRequired: {#TESTS_SIZE}

#define COMPN_TCLSCI 'tclsci'
Name: {#COMPN_TCLSCI}; Description: {cm:DescriptionTclTK}; Types: full;

#define COMPN_CHM 'chm_scilab'
Name: {#COMPN_CHM}; Description: {cm:DescriptionCHM}; Types: full; ExtraDiskSpaceRequired: {#CHM_SIZE}
;-------------------------------------------------------------------------------
