/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
#include <libxml/parser.h>

#include "TerminateCore.h"
/*--------------------------------------------------------------------------*/
#include "realmain.h" /* Get_no_startup_flag */
#include "inffic.h" /* get_sci_data_strings */
#include "scirun.h" /* scirun */
#include "getmodules.h"
#include "scimem.h" /* freegmem */
#include "tmpdir.h" /* tmpdirc */
#include "hashtable_core.h" /* destroy_hashtable_scilab_functions */
#include "filesmanagement.h"
#include "scilabmode.h"
#include "dynamic_gateways.h" /* freeAllDynamicGateways */
#include "h5_fileManagement.h" /* HDF5cleanup */

/*--------------------------------------------------------------------------*/
BOOL TerminateCorePart1(void)
{
    if ( Get_no_startup_flag() == 0)
    {
        char *quit_script = NULL;

        /* bug 3672 */
        if (getScilabMode() == SCILAB_STD)
        {
            quit_script = get_sci_data_strings(QUIT_ERRCATCH_ID);
        }
        else
        {
            quit_script = get_sci_data_strings(QUIT_ID);
        }

        /* launch scilab.quit script */
        C2F(scirun)(quit_script, (long int)strlen(quit_script));
    }
    return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL TerminateCorePart2(void)
{
    // Free stack and global stack
    C2F(freestacklastmemory)();
    C2F(freeglobalstacklastmemory)();

    freeStackCurrentMemory();
    freeGlobalStackCurrentMemory();

    DisposeModulesInfo();

    destroy_hashtable_scilab_functions();

    /* Close all scilab's files */
    TerminateScilabFilesList();

    /*
    * Cleanup function for the XML library.
    */
    xmlCleanupParser();

    HDF5cleanup();

    /** clean tmpfiles **/
    C2F(tmpdirc)();

    /* free dynamic gateways and dynamic libraries */
    freeAllDynamicGateways();

    return TRUE;
}
/*--------------------------------------------------------------------------*/
