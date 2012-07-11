/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __DYNAMIC_GATEWAYS_H__
#define __DYNAMIC_GATEWAYS_H__

/*
* When we terminate Scilab, we freed dynamic gateways
*/
void freeAllDynamicGateways(void);

/**
* graphic_objects gateway
* graphic_objects is loaded dynamically at runtime
* load gw_graphic_objects in SCI/modules/graphic_objects/sci_gateway/gw_graphic_objects.c
*/
int gw_dynamic_graphic_objects(void);

/**
* special_functions gateway
* special_functions is loaded dynamically at runtime
* load gw_special_functions in SCI/modules/special_functions/sci_gateway/gw_special_functions.c
*/
int gw_dynamic_special_functions(void);


/**
* ui_data gateway
* ui_data is loaded dynamically at runtime
* load gw_ui_data in SCI/modules/ui_data/sci_gateway/gw_ui_data.c
*/
int gw_dynamic_ui_data(void);

/**
* xcos gateway
* xcos is loaded dynamically at runtime
* load gw_xcos in SCI/modules/xcos/sci_gateway/gw_xcos.c
*/
int gw_dynamic_xcos(void);

/**
* interpolation gateway
* interpolation is loaded dynamically at runtime
* load gw_interpolation in SCI/modules/interpolation/sci_gateway/gw_interpolation.c
*/
int gw_dynamic_interpolation(void);

/**
* optimization gateway
* optimization is loaded dynamically at runtime
* load gw_optimization in SCI/modules/optimization/sci_gateway/gw_optimization.c
*/
int gw_dynamic_optimization(void);

/**
* UMFPACK gateway
* UMFPACK is loaded dynamically at runtime
* load gw_umfpack in SCI/modules/umfpack/sci_gateway/gw_umfpack.c
*/
int gw_dynamic_umfpack(void);

/**
* spreadsheet gateway
* spreadsheet is loaded dynamically at runtime
* load gw_spreadsheet in SCI/modules/spreadsheet/sci_gateway/gw_spreadsheet.c
*/
int gw_dynamic_spreadsheet(void);

/**
* sound gateway
* sound is loaded dynamically at runtime
* load gw_sound in SCI/modules/sound/sci_gateway/gw_sound.c
*/
int gw_dynamic_sound(void);

/**
* Scicos gateway
* Scicos is loaded dynamically at runtime
* load gw_scicos in SCI/modules/scicos/sci_gateway/gw_scicos.c
*/
int gw_dynamic_scicos(void);

/**
* randlib gateway
* randlib is loaded dynamically at runtime
* load gw_randlib in SCI/modules/randlib/sci_gateway/gw_randlib.c
*/
int gw_dynamic_randlib(void);

/**
* arnoldi gateway
* arnoldi is loaded dynamically at runtime
* load gw_arnoldi in SCI/modules/arnoldi/sci_gateway/gw_arnoldi.c
*/
int gw_dynamic_arnoldi(void);

/**
* symbolic gateway
* symbolic is loaded dynamically at runtime
* load gw_symbolic in SCI/modules/symbolic/sci_gateway/gw_symbolic.c
*/
int gw_dynamic_symbolic(void);

/**
* graphic_export gateway
* graphic_export is loaded dynamically at runtime
* load gw_graphic_export in SCI/modules/graphic_export/sci_gateway/gw_graphic_export.c
*/
int gw_dynamic_graphic_export(void);

/**
* action_binding gateway
* action_binding is loaded dynamically at runtime
* load gw_action_binding in SCI/modules/action_binding/sci_gateway/gw_action_binding.c
*/
int gw_dynamic_action_binding(void);

/**
* preferences gateway
* preferences is loaded dynamically at runtime
* load gw_preferences in SCI/modules/preferences/sci_gateway/gw_preferences.c
*/
int gw_dynamic_preferences(void);

/**
* xml gateway
* xml is loaded dynamically at runtime
* load xml in SCI/modules/xml/sci_gateway/gw_xml.c
*/
int gw_dynamic_xml(void);




#endif /* __DYNAMIC_GATEWAYS_H__ */
/*--------------------------------------------------------------------------*/


