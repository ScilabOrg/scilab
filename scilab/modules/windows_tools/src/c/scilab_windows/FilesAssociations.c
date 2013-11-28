/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "TextToPrint.h"
#include <windows.h>
#include <Winuser.h>
#include <shlwapi.h>
#include "version.h"
#include "FilesAssociations.h"
#include "MALLOC.h"
#include "FindScilab.h"
#include "wmcopydata.h"
#include "strdup_windows.h"
#include "MutexClosingScilab.h"
#include "with_module.h"
#include "FileExist.h"
#include "getshortpathname.h"
/*--------------------------------------------------------------------------*/
static void ReplaceSlash(char *pathout, char *pathin);
static void ExtensionFileIntoLowerCase(char *fichier);
static BOOL isGoodExtension(char *chainefichier, char *ext);
static BOOL IsAScicosFileCOS(char *chainefichier);
static BOOL IsAScicosFileCOSF(char *chainefichier);
static BOOL IsAScicosFileXCOS(char *chainefichier);
static BOOL IsAScicosFileZCOS(char *chainefichier);
static BOOL IsASciNotesFileSCE(char *chainefichier);
static BOOL IsASciNotesFileSCI(char *chainefichier);
static BOOL IsASciNotesFileTST(char *chainefichier);
/*--------------------------------------------------------------------------*/
#define MSG_SCIMSG1 "%s -e load(getlongpathname('%s'));disp(getlongpathname('%s')+ascii(32)+'loaded');"
#define MSG_SCIMSG2_XCOS "%s -e xcos(getlongpathname('%s'));"
#define MSG_SCIMSG3_XCOS "execstr('xcos(getlongpathname(''%s''));','errcatch');"
#define MSG_SCIMSG4 "%s -e exec(getlongpathname('%s'));"
#define MSG_SCIMSG5_EDITOR "%s -e editor(getlongpathname('%s'));"
/* we try to launch scilab editor */
#define MSG_SCIMSG6_EDITOR "execstr('editor(getlongpathname(''%s''));','errcatch');"
#define MSG_SCIMSG7 "Scilab Communication"
/*--------------------------------------------------------------------------*/
/* Teste si le fichier a une extension .sav ou .bin*/
/* retourne TRUE si c'est le cas sinon FALSE */
BOOL IsABinOrSavFile(char *chainefichier)
{
    if (isGoodExtension(chainefichier, ".BIN") || isGoodExtension(chainefichier, ".SAV")
            || isGoodExtension(chainefichier, ".SOD"))
    {
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL IsAScicosFile(char *chainefichier)
{
    if (IsAScicosFileCOS(chainefichier) ||
            IsAScicosFileCOSF(chainefichier) ||
            IsAScicosFileXCOS(chainefichier) ||
            IsAScicosFileZCOS(chainefichier))
    {
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL IsAScicosFileCOS(char *chainefichier)
{
    return isGoodExtension(chainefichier, ".COS");
}
/*--------------------------------------------------------------------------*/
BOOL IsAScicosFileCOSF(char *chainefichier)
{
    return isGoodExtension(chainefichier, ".COSF");
}
/*--------------------------------------------------------------------------*/
BOOL IsAScicosFileXCOS(char *chainefichier)
{
    return isGoodExtension(chainefichier, ".XCOS");
}
/*--------------------------------------------------------------------------*/
BOOL IsAScicosFileZCOS(char *chainefichier)
{
    return isGoodExtension(chainefichier, ".ZCOS");
}
/*--------------------------------------------------------------------------*/
BOOL IsASciNotesFile(char *chainefichier)
{
    if (IsASciNotesFileSCE(chainefichier) ||
            IsASciNotesFileSCI(chainefichier) ||
            IsASciNotesFileTST(chainefichier))
    {
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL IsASciNotesFileSCE(char *chainefichier)
{
    return isGoodExtension(chainefichier, ".SCE");
}
/*--------------------------------------------------------------------------*/
BOOL IsASciNotesFileSCI(char *chainefichier)
{
    return isGoodExtension(chainefichier, ".SCI");
}
/*--------------------------------------------------------------------------*/
BOOL IsASciNotesFileTST(char *chainefichier)
{
    return isGoodExtension(chainefichier, ".TST");
}
/*--------------------------------------------------------------------------*/
int CommandByFileExtension(char *fichier, int OpenCode, char *Cmd)
{
    int ReturnedValue = 0;
    if (FileExist(fichier))
    {
        BOOL bConverted = FALSE;
        char FinalFileName[(MAX_PATH * 2) + 1];
        char *ShortPath = NULL;
        char PathWScilex[(MAX_PATH * 2) + 1];


        /* Recuperation du nom du fichier au format 8.3 */
        ShortPath = getshortpathname(fichier, &bConverted);
        GetShortPathName(fichier, ShortPath, MAX_PATH);
        ReplaceSlash(FinalFileName, ShortPath);
        if (ShortPath)
        {
            FREE(ShortPath);
            ShortPath = NULL;
        }

        GetModuleFileName ((HINSTANCE)GetModuleHandle(NULL), PathWScilex, MAX_PATH);
        ReturnedValue = 1;

        switch (OpenCode)
        {
            case 0:
            default: /* -O Open file with editor */
            {
                if (!HaveAnotherWindowScilab() || haveMutexClosingScilab())
                {
                    if (with_module("scinotes"))
                    {
                        wsprintf(Cmd, MSG_SCIMSG5_EDITOR, PathWScilex, FinalFileName);
                    }
                    else
                    {
                        MessageBox(NULL, "Please install editor module.", "Error", MB_ICONSTOP);
                        exit(0);
                    }
                }
                else
                {
                    char *ScilabDestination = NULL;

                    if (with_module("scinotes"))
                    {
                        wsprintf(Cmd, MSG_SCIMSG6_EDITOR, FinalFileName);
                    }
                    else
                    {
                        MessageBox(NULL, "Please install editor module.", "Error", MB_ICONSTOP);
                        exit(0);
                    }

                    ScilabDestination = getLastScilabFound();
                    if (ScilabDestination)
                    {
                        SendCommandToAnotherScilab(MSG_SCIMSG7, ScilabDestination, Cmd);
                        FREE(ScilabDestination);
                        exit(0);
                    }
                    else
                    {
                        if (with_module("scinotes"))
                        {
                            wsprintf(Cmd, MSG_SCIMSG5_EDITOR, PathWScilex, FinalFileName);
                        }
                        else
                        {
                            MessageBox(NULL, "Please install editor module.", "Error", MB_ICONSTOP);
                            exit(0);
                        }
                    }
                }
            }
            break;

            case 1: /* -X eXecute file */
            {
                if (IsABinOrSavFile(FinalFileName) == TRUE)
                {
                    /* C'est un fichier .BIN ou .SAV d'ou load */
                    wsprintf(Cmd, MSG_SCIMSG1, PathWScilex, FinalFileName, FinalFileName);
                }
                else
                {
                    if (IsAScicosFile(fichier) == TRUE)
                    {
                        ExtensionFileIntoLowerCase(FinalFileName);
                        if (!HaveAnotherWindowScilab() || haveMutexClosingScilab())
                        {
                            if (with_module("xcos"))
                            {
                                wsprintf(Cmd, MSG_SCIMSG2_XCOS, PathWScilex, FinalFileName);
                            }
                            else
                            {
                                MessageBox(NULL, "Please install xcos module.", "Error", MB_ICONSTOP);
                                exit(0);
                            }
                        }
                        else
                        {
                            char *ScilabDestination = NULL;

                            if (with_module("xcos"))
                            {
                                wsprintf(Cmd, MSG_SCIMSG3_XCOS, FinalFileName);
                            }
                            else
                            {
                                MessageBox(NULL, "Please install xcos module.", "Error", MB_ICONSTOP);
                                exit(0);
                            }

                            ScilabDestination = getLastScilabFound();
                            if (ScilabDestination)
                            {
                                SendCommandToAnotherScilab(MSG_SCIMSG7, ScilabDestination, Cmd);
                                FREE(ScilabDestination);
                                exit(0);
                            }
                            else
                            {
                                if (with_module("xcos"))
                                {
                                    wsprintf(Cmd, MSG_SCIMSG2_XCOS, PathWScilex, FinalFileName);
                                }
                                else
                                {
                                    MessageBox(NULL, "Please install xcos module.", "Error", MB_ICONSTOP);
                                    exit(0);
                                }
                            }
                        }
                    }
                    else
                    {
                        wsprintf(Cmd, MSG_SCIMSG4, PathWScilex, FinalFileName);
                    }
                }
            }
            break;

            case 2: /* -P Print file */
            {
                PrintFile(fichier);
                strcpy(Cmd, " ");
                exit(0);
            }
            break;
        }
    }
    return ReturnedValue;
}
/*--------------------------------------------------------------------------*/
static void ExtensionFileIntoLowerCase(char *fichier)
{
    char *tmpfile = NULL;
    char *buffer = NULL;
    char *lastdot = NULL;
    char *ext = NULL;

    tmpfile = strdup(fichier);
    buffer = strtok(tmpfile, ".");
    while (buffer = strtok(NULL, "."))
    {
        lastdot = buffer;
    }
    /* le dernier . permet d'avoir l'extension */
    ext = _strlwr(lastdot); /* Fichier en Majuscule */

    strcpy(&fichier[strlen(fichier) - strlen(ext)], ext);

    FREE(tmpfile);
}
/*--------------------------------------------------------------------------*/
static void ReplaceSlash(char *pathout, char *pathin)
{
    int i = 0;
    int len_pathin = (int)strlen(pathin);
    for (i = 0; i < len_pathin; i++)
    {
        if (pathin[i] == '\\')
        {
            pathout[i] = '/';
        }
        else
        {
            pathout[i] = pathin[i];
        }
    }
    pathout[i] = '\0';
}
/*--------------------------------------------------------------------------*/
static BOOL isGoodExtension(char *chainefichier, char *ext)
{
    char *ExtensionFilename = PathFindExtension(chainefichier);
    if (ExtensionFilename)
    {
        if (_stricmp(ExtensionFilename, ext) == 0)
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
