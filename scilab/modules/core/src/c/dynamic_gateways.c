/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <stdlib.h>
#include "dynamic_gateways.h"
#include "callDynamicGateway.h"
#include "gw_dynamic_generic.h"
#include "dynamic_parallel.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/* special_functions module */
#define SPECIAL_FUNCTIONS_MODULE_NAME "special_functions"
static DynLibHandle hSpecial_functionsLib = NULL;
static PROC_GATEWAY ptr_gw_special_functions = NULL;
static char *dynlibname_special_functions = NULL;
static char *gatewayname_special_functions = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_special_functions(void)
{
    return gw_dynamic_generic(SPECIAL_FUNCTIONS_MODULE_NAME,
                              &dynlibname_special_functions, &gatewayname_special_functions, &hSpecial_functionsLib, &ptr_gw_special_functions);
}

/*--------------------------------------------------------------------------*/
/* pvm module */
#define PVM_MODULE_NAME "pvm"
static DynLibHandle hPvmLib = NULL;
static PROC_GATEWAY ptr_gw_pvm = NULL;
static char *dynlibname_pvm = NULL;
static char *gatewayname_pvm = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_pvm(void)
{
    return gw_dynamic_generic(PVM_MODULE_NAME, &dynlibname_pvm, &gatewayname_pvm, &hPvmLib, &ptr_gw_pvm);
}

/*--------------------------------------------------------------------------*/
/* helptools module */
#define HELPTOOLS_MODULE_NAME "helptools"
static DynLibHandle hHelptoolsLib = NULL;
static PROC_GATEWAY ptr_gw_helptools = NULL;
static char *dynlibname_helptools = NULL;
static char *gatewayname_helptools = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_helptools(void)
{
    return gw_dynamic_generic(HELPTOOLS_MODULE_NAME, &dynlibname_helptools, &gatewayname_helptools, &hHelptoolsLib, &ptr_gw_helptools);
}

/*--------------------------------------------------------------------------*/
/* optimization module */
#define OPTIMIZATION_MODULE_NAME "optimization"
static DynLibHandle hOptimizationLib = NULL;
static PROC_GATEWAY ptr_gw_optimization = NULL;
static char *dynlibname_optimization = NULL;
static char *gatewayname_optimization = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_optimization(void)
{
    return gw_dynamic_generic(OPTIMIZATION_MODULE_NAME, &dynlibname_optimization, &gatewayname_optimization, &hOptimizationLib, &ptr_gw_optimization);
}

/*--------------------------------------------------------------------------*/
/* matio module */
#define MATIO_MODULE_NAME "matio"
static DynLibHandle hMatioLib = NULL;
static PROC_GATEWAY ptr_gw_matio = NULL;
static char *dynlibname_matio = NULL;
static char *gatewayname_matio = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_matio(void)
{
    return gw_dynamic_generic(MATIO_MODULE_NAME, &dynlibname_matio, &gatewayname_matio, &hMatioLib, &ptr_gw_matio);
}

/*--------------------------------------------------------------------------*/
/* UMFPACK module */
#define UMFPACK_MODULE_NAME "umfpack"
static DynLibHandle hUmfpackLib = NULL;
static PROC_GATEWAY ptr_gw_umfpack = NULL;
static char *dynlibname_umfpack = NULL;
static char *gatewayname_umfpack = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_umfpack(void)
{
    return gw_dynamic_generic(UMFPACK_MODULE_NAME, &dynlibname_umfpack, &gatewayname_umfpack, &hUmfpackLib, &ptr_gw_umfpack);
}

/*--------------------------------------------------------------------------*/
/* spreadsheet module */
#define SPREADSHEET_MODULE_NAME "spreadsheet"
static DynLibHandle hSpreadsheetLib = NULL;
static PROC_GATEWAY ptr_gw_spreadsheet = NULL;
static char *dynlibname_spreadsheet = NULL;
static char *gatewayname_spreadsheet = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_spreadsheet(void)
{
    return gw_dynamic_generic(SPREADSHEET_MODULE_NAME, &dynlibname_spreadsheet, &gatewayname_spreadsheet, &hSpreadsheetLib, &ptr_gw_spreadsheet);
}

/*--------------------------------------------------------------------------*/
/* sound module */
#define SOUND_MODULE_NAME "sound"
static DynLibHandle hSoundLib = NULL;
static PROC_GATEWAY ptr_gw_sound = NULL;
static char *dynlibname_sound = NULL;
static char *gatewayname_sound = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_sound(void)
{
    return gw_dynamic_generic(SOUND_MODULE_NAME, &dynlibname_sound, &gatewayname_sound, &hSoundLib, &ptr_gw_sound);
}

/*--------------------------------------------------------------------------*/
/* scicos module */
#define SCICOS_MODULE_NAME "scicos"
static DynLibHandle hScicosLib = NULL;
static PROC_GATEWAY ptr_gw_scicos = NULL;
static char *dynlibname_scicos = NULL;
static char *gatewayname_scicos = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_scicos(void)
{
    return gw_dynamic_generic(SCICOS_MODULE_NAME, &dynlibname_scicos, &gatewayname_scicos, &hScicosLib, &ptr_gw_scicos);
}

/*--------------------------------------------------------------------------*/
/* randlib module */
#define RANDLIB_MODULE_NAME "randlib"
static DynLibHandle hRandlibLib = NULL;
static PROC_GATEWAY ptr_gw_randlib = NULL;
static char *dynlibname_randlib = NULL;
static char *gatewayname_randlib = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_randlib(void)
{
    return gw_dynamic_generic(RANDLIB_MODULE_NAME, &dynlibname_randlib, &gatewayname_randlib, &hRandlibLib, &ptr_gw_randlib);
}

/*--------------------------------------------------------------------------*/
/* arnoldi module */
#define ARNOLDI_MODULE_NAME "arnoldi"
static DynLibHandle hArnoldiLib = NULL;
static PROC_GATEWAY ptr_gw_arnoldi = NULL;
static char *dynlibname_arnoldi = NULL;
static char *gatewayname_arnoldi = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_arnoldi(void)
{
    return gw_dynamic_generic(ARNOLDI_MODULE_NAME, &dynlibname_arnoldi, &gatewayname_arnoldi, &hArnoldiLib, &ptr_gw_arnoldi);
}

/*--------------------------------------------------------------------------*/
/* symbolic module */
#define SYMBOLIC_MODULE_NAME "symbolic"
static DynLibHandle hSymbolicLib = NULL;
static PROC_GATEWAY ptr_gw_symbolic = NULL;
static char *dynlibname_symbolic = NULL;
static char *gatewayname_symbolic = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_symbolic(void)
{
    return gw_dynamic_generic(SYMBOLIC_MODULE_NAME, &dynlibname_symbolic, &gatewayname_symbolic, &hSymbolicLib, &ptr_gw_symbolic);
}

/*--------------------------------------------------------------------------*/
/* interpolation module */
#define INTERPOLATION_MODULE_NAME "interpolation"
static DynLibHandle hInterpolationLib = NULL;
static PROC_GATEWAY ptr_gw_interpolation = NULL;
static char *dynlibname_interpolation = NULL;
static char *gatewayname_interpolation = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_interpolation(void)
{
    return gw_dynamic_generic(INTERPOLATION_MODULE_NAME,
                              &dynlibname_interpolation, &gatewayname_interpolation, &hInterpolationLib, &ptr_gw_interpolation);
}

/*--------------------------------------------------------------------------*/
/* statistics module */
#define STATISTICS_MODULE_NAME "statistics"
static DynLibHandle hStatisticsLib = NULL;
static PROC_GATEWAY ptr_gw_statistics = NULL;
static char *dynlibname_statistics = NULL;
static char *gatewayname_statistics = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_statistics(void)
{
    return gw_dynamic_generic(STATISTICS_MODULE_NAME, &dynlibname_statistics, &gatewayname_statistics, &hStatisticsLib, &ptr_gw_statistics);
}

/*--------------------------------------------------------------------------*/
/* signal_processing module */
#define SIGNAL_PROCESSING_MODULE_NAME "signal_processing"
static DynLibHandle hSignal_processingLib = NULL;
static PROC_GATEWAY ptr_gw_signal_processing = NULL;
static char *dynlibname_signal_processing = NULL;
static char *gatewayname_signal_processing = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_signal_processing(void)
{
    return gw_dynamic_generic(SIGNAL_PROCESSING_MODULE_NAME,
                              &dynlibname_signal_processing, &gatewayname_signal_processing, &hSignal_processingLib, &ptr_gw_signal_processing);
}

/*--------------------------------------------------------------------------*/
/* functions module */
#define FUNCTIONS_MODULE_NAME "functions"
static DynLibHandle hFunctionsLib = NULL;
static PROC_GATEWAY ptr_gw_functions = NULL;
static char *dynlibname_functions = NULL;
static char *gatewayname_functions = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_functions(void)
{
    return gw_dynamic_generic(FUNCTIONS_MODULE_NAME, &dynlibname_functions, &gatewayname_functions, &hFunctionsLib, &ptr_gw_functions);
}

/*--------------------------------------------------------------------------*/
/* xcos module */
#define XCOS_MODULE_NAME "xcos"
static DynLibHandle hXcosLib = NULL;
static PROC_GATEWAY ptr_gw_xcos = NULL;
static char *dynlibname_xcos = NULL;
static char *gatewayname_xcos = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_xcos(void)
{
    return gw_dynamic_generic(XCOS_MODULE_NAME, &dynlibname_xcos, &gatewayname_xcos, &hXcosLib, &ptr_gw_xcos);
}

/*--------------------------------------------------------------------------*/
/* scinotes module */
#define SCINOTES_MODULE_NAME "scinotes"
static DynLibHandle hSciNotesLib = NULL;
static PROC_GATEWAY ptr_gw_scinotes = NULL;
static char *dynlibname_scinotes = NULL;
static char *gatewayname_scinotes = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_scinotes(void)
{
#ifdef _MSC_VER
    if (dynlibname_scinotes == NULL)
    {
        dynlibname_scinotes = buildModuleDynLibraryName(SCINOTES_MODULE_NAME, DYNLIB_NAME_FORMAT_2);
    }
#endif
    return gw_dynamic_generic(SCINOTES_MODULE_NAME, &dynlibname_scinotes, &gatewayname_scinotes, &hSciNotesLib, &ptr_gw_scinotes);
}

/*--------------------------------------------------------------------------*/
/* hdf5 module */
#define HDF5_MODULE_NAME "hdf5"
static DynLibHandle hHdf5Lib = NULL;
static PROC_GATEWAY ptr_gw_hdf5 = NULL;
static char *dynlibname_hdf5 = NULL;
static char *gatewayname_hdf5 = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_hdf5(void)
{
    int r = gw_dynamic_generic(HDF5_MODULE_NAME,
                               &dynlibname_hdf5,
                               &gatewayname_hdf5,
                               &hHdf5Lib,
                               &ptr_gw_hdf5);

    freeDynamicGateway(&dynlibname_hdf5, &gatewayname_hdf5, &hHdf5Lib, &ptr_gw_hdf5);

    return r;
}

/*--------------------------------------------------------------------------*/
/* graphic_exports module */
#define GRAPHIC_EXPORT_MODULE_NAME "graphic_export"
static DynLibHandle hGraphic_exportLib = NULL;
static PROC_GATEWAY ptr_gw_graphic_export = NULL;
static char *dynlibname_graphic_export = NULL;
static char *gatewayname_graphic_export = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_graphic_export(void)
{
#ifdef _MSC_VER
    if (dynlibname_graphic_export == NULL)
    {
        dynlibname_graphic_export = buildModuleDynLibraryName(GRAPHIC_EXPORT_MODULE_NAME, DYNLIB_NAME_FORMAT_2);
    }
#endif
    return gw_dynamic_generic(GRAPHIC_EXPORT_MODULE_NAME,
                              &dynlibname_graphic_export, &gatewayname_graphic_export, &hGraphic_exportLib, &ptr_gw_graphic_export);
}

/*--------------------------------------------------------------------------*/
/* graphic_exports module */
#define ACTION_BINDING_MODULE_NAME "action_binding"
static DynLibHandle hAction_bindingLib = NULL;
static PROC_GATEWAY ptr_gw_action_binding = NULL;
static char *dynlibname_action_binding = NULL;
static char *gatewayname_action_binding = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_action_binding(void)
{
#ifdef _MSC_VER
    if (dynlibname_action_binding == NULL)
    {
        dynlibname_action_binding = buildModuleDynLibraryName(ACTION_BINDING_MODULE_NAME, DYNLIB_NAME_FORMAT_2);
    }
#endif
    return gw_dynamic_generic(ACTION_BINDING_MODULE_NAME,
                              &dynlibname_action_binding, &gatewayname_action_binding, &hAction_bindingLib, &ptr_gw_action_binding);
}

/*--------------------------------------------------------------------------*/
/* ui_data module */
#define UI_DATA_MODULE_NAME "ui_data"
static DynLibHandle hUi_dataLib = NULL;
static PROC_GATEWAY ptr_gw_ui_data = NULL;
static char *dynlibname_ui_data = NULL;
static char *gatewayname_ui_data = NULL;

/*--------------------------------------------------------------------------*/
int gw_dynamic_ui_data(void)
{
#ifdef _MSC_VER
    if (dynlibname_ui_data == NULL)
    {
        dynlibname_ui_data = buildModuleDynLibraryName(UI_DATA_MODULE_NAME, DYNLIB_NAME_FORMAT_2);
    }
#endif

    return gw_dynamic_generic(UI_DATA_MODULE_NAME, &dynlibname_ui_data, &gatewayname_ui_data, &hUi_dataLib, &ptr_gw_ui_data);
}

/*--------------------------------------------------------------------------*/
void freeAllDynamicGateways(void)
{
    freeDynamicGateway(&dynlibname_special_functions, &gatewayname_special_functions, &hSpecial_functionsLib, &ptr_gw_special_functions);

    freeDynamicGateway(&dynlibname_pvm, &gatewayname_pvm, &hPvmLib, &ptr_gw_pvm);

    freeDynamicGateway(&dynlibname_helptools, &gatewayname_helptools, &hHelptoolsLib, &ptr_gw_helptools);

    freeDynamicGateway(&dynlibname_optimization, &gatewayname_optimization, &hOptimizationLib, &ptr_gw_optimization);

    freeDynamicGateway(&dynlibname_matio, &gatewayname_matio, &hMatioLib, &ptr_gw_matio);

    freeDynamicGateway(&dynlibname_umfpack, &gatewayname_umfpack, &hUmfpackLib, &ptr_gw_umfpack);

    freeDynamicGateway(&dynlibname_spreadsheet, &gatewayname_spreadsheet, &hSpreadsheetLib, &ptr_gw_spreadsheet);

    freeDynamicGateway(&dynlibname_sound, &gatewayname_sound, &hSoundLib, &ptr_gw_sound);

    freeDynamicGateway(&dynlibname_scicos, &gatewayname_scicos, &hScicosLib, &ptr_gw_scicos);

    freeDynamicGateway(&dynlibname_randlib, &gatewayname_randlib, &hRandlibLib, &ptr_gw_randlib);

    freeDynamicGateway(&dynlibname_arnoldi, &gatewayname_arnoldi, &hArnoldiLib, &ptr_gw_arnoldi);

    freeDynamicGateway(&dynlibname_symbolic, &gatewayname_symbolic, &hSymbolicLib, &ptr_gw_symbolic);

    freeDynamicGateway(&dynlibname_interpolation, &gatewayname_interpolation, &hInterpolationLib, &ptr_gw_interpolation);

    freeDynamicGateway(&dynlibname_statistics, &gatewayname_statistics, &hStatisticsLib, &ptr_gw_statistics);

    freeDynamicGateway(&dynlibname_signal_processing, &gatewayname_signal_processing, &hSignal_processingLib, &ptr_gw_signal_processing);

    freeDynamicGateway(&dynlibname_functions, &gatewayname_functions, &hFunctionsLib, &ptr_gw_functions);

    freeDynamicGateway(&dynlibname_xcos, &gatewayname_xcos, &hXcosLib, &ptr_gw_xcos);

    freeDynamicGateway(&dynlibname_scinotes, &gatewayname_scinotes, &hSciNotesLib, &ptr_gw_scinotes);

    freeDynamicGateway(&dynlibname_hdf5, &gatewayname_hdf5, &hHdf5Lib, &ptr_gw_hdf5);

    freeDynamicGateway(&dynlibname_graphic_export, &gatewayname_graphic_export, &hGraphic_exportLib, &ptr_gw_graphic_export);

    freeDynamicGateway(&dynlibname_action_binding, &gatewayname_action_binding, &hAction_bindingLib, &ptr_gw_action_binding);

    dynTerminateParallel();

    freeDynamicGateway(&dynlibname_ui_data, &gatewayname_ui_data, &hUi_dataLib, &ptr_gw_ui_data);
}

/*--------------------------------------------------------------------------*/
