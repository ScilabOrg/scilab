/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Allan CORNET
 * Copyright (C) 2012 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dropFiles.h"
#include "MALLOC.h"
#include "storeCommand.h" /* storecommand */
#include "FindFileExtension.h"
#include "URIFileToFilename.h"
#include "../../../string/includes/stricmp.h"
#include "../../../core/src/c/with_module.h"
#if _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#define BIN_EXTENSION_FILE ".bin"
#define SAV_EXTENSION_FILE ".sav"
#define COS_EXTENSION_FILE ".cos"
#define XCOS_EXTENSION_FILE ".xcos"
#define COSF_EXTENSION_FILE ".cosf"
#define SCI_EXTENSION_FILE ".sci"
#define SCE_EXTENSION_FILE ".sce"
#define TST_EXTENSION_FILE ".tst"
#define DEM_EXTENSION_FILE ".dem"
#define SCG_EXTENSION_FILE ".scg"
#define ZCOS_EXTENSION_FILE ".zcos"
#define SOD_EXTENSION_FILE ".sod"
/*--------------------------------------------------------------------------*/
#define FORMAT_BIN_SCE_EXTENSION_FILES "load('%s');"
#define FORMAT_COS_COSF_XCOS_EXTENSION_FILES "xcos('%s');"
#define FORMAT_SCI_EXTENSION_FILES "exec('%s');"
#define FORMAT_SCE_TST_EXTENSION_FILES "exec('%s');"
#define FORMAT_SCG_EXTENSION_FILES "load('%s');"
#define FORMAT_UNKNOW_EXTENSION_FILES "disp(gettext('Unknown file type : %s'));"
#define XCOS_NOT_INSTALLED "disp(gettext('Please install xcos module.'))"
/*--------------------------------------------------------------------------*/
static char *getCommandByFileExtension(char *File, char *FileExtension);
static char *buildCommand(char *format, char *filename);
static BOOL LaunchFilebyExtension(char *File);
/*--------------------------------------------------------------------------*/
BOOL dropFiles(char **files)
{
    int len = 0;

    while (files[len])
    {
        char *convertfile = URIFileToFilename(files[len]);

        if (convertfile)
        {
            BOOL bCheck = LaunchFilebyExtension(convertfile);
            if (convertfile)
            {
                FREE(convertfile);
                convertfile = NULL;
            }
            if (!bCheck)
            {
                return bCheck;
            }
        }
        len++;
    }
    return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL LaunchFilebyExtension(char *File)
{
    BOOL bOK = FALSE;

    char *CommandLine = NULL;
    char *FileExtension = NULL;

    FileExtension = FindFileExtension(File);
    CommandLine = getCommandByFileExtension(File, FileExtension);

    if (CommandLine)
    {
        StoreCommand(CommandLine);
        bOK = TRUE;

        FREE(CommandLine);
        CommandLine = NULL;
    }

    if (FileExtension)
    {
        FREE(CommandLine);
        CommandLine = NULL;
    }

    return bOK;
}
/*--------------------------------------------------------------------------*/
static char *getCommandByFileExtension(char *File, char *FileExtension)
{
    char *command = NULL;

    if (FileExtension)
    {
        if ( (stricmp(FileExtension, BIN_EXTENSION_FILE) == 0) ||
                (stricmp(FileExtension, SAV_EXTENSION_FILE) == 0) ||
                (stricmp(FileExtension, SOD_EXTENSION_FILE) == 0))
        {
            command = buildCommand(FORMAT_BIN_SCE_EXTENSION_FILES, File);
        }
        else if ( (stricmp(FileExtension, COS_EXTENSION_FILE) == 0) ||
                  (stricmp(FileExtension, COSF_EXTENSION_FILE) == 0) ||
                  (stricmp(FileExtension, ZCOS_EXTENSION_FILE) == 0) ||
                  (stricmp(FileExtension, XCOS_EXTENSION_FILE) == 0))
        {
            if (with_module("xcos"))
            {
                command = buildCommand(FORMAT_COS_COSF_XCOS_EXTENSION_FILES, File);
            }
            else
            {
                command = strdup(XCOS_NOT_INSTALLED);
            }
        }
        else if (stricmp(FileExtension, SCI_EXTENSION_FILE) == 0)
        {
            command = buildCommand(FORMAT_SCI_EXTENSION_FILES, File);
        }
        else if ( (stricmp(FileExtension, SCE_EXTENSION_FILE) == 0) || (stricmp(FileExtension, TST_EXTENSION_FILE) == 0) || (stricmp(FileExtension, DEM_EXTENSION_FILE) == 0) )
        {
            command = buildCommand(FORMAT_SCE_TST_EXTENSION_FILES, File);
        }
        else if (stricmp(FileExtension, SCG_EXTENSION_FILE) == 0)
        {
            command = buildCommand(FORMAT_SCG_EXTENSION_FILES, File);
        }
        else
        {
            command = buildCommand(FORMAT_UNKNOW_EXTENSION_FILES, File);
        }
    }
    return command;
}
/*--------------------------------------------------------------------------*/
static char *buildCommand(char *format, char *filename)
{
    char *command = NULL;

    if (format && filename)
    {
        command = (char*)MALLOC( (strlen(filename) + strlen(format) + 1) * sizeof(char) );
        if (command)
        {
            sprintf(command, format, filename);
        }
    }

    return command;
}
/*--------------------------------------------------------------------------*/
