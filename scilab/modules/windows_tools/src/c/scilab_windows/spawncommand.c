/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#include "PATH_MAX.h"
#include "MALLOC.h"
#include "tmpdir.h"
#include "FileExist.h"
#include "scilabmode.h"
#include "spawncommand.h"
#include "strdup_windows.h"
#include "charEncoding.h"
#include "getshortpathname.h"
#include "strdup_windows.h"
/*--------------------------------------------------------------------------*/
#define BUFSIZE 4096
#define LF_STR "\n"
#define CR '\r'
#define LF '\n'
#define BLANK L' '
#define NOTPRINTABLE -96
#define EMPTY_CHAR L'\0'
#define CMDLINE_FORMAT_DETACHED L"%ls /A /C \"%ls\""
#define CMDLINE_FORMAT_NOTDETACHED L"%ls /A /C \"%ls && echo DOS > %ls\""
#define OUTPUT_CHECK_FILENAME_FORMAT L"%ls\\DOS.OK"
/*--------------------------------------------------------------------------*/
pipeinfo SCILAB_WINDOWS_IMPEXP pipeSpawnOut = {INVALID_HANDLE_VALUE, NULL, 0};
pipeinfo SCILAB_WINDOWS_IMPEXP pipeSpawnErr = {INVALID_HANDLE_VALUE, NULL, 0};
/*--------------------------------------------------------------------------*/
static int GetNumberOfLines(char *lines);
static BOOL removeEOL(char *_string);
static BOOL removeNotPrintableCharacters(char *_string);
static char *convertLine(char *_string, BOOL DetachProcess);
/*--------------------------------------------------------------------------*/
int spawncommand(wchar_t *command, BOOL DetachProcess)
{
    wchar_t shellCmd[PATH_MAX];
    wchar_t *CmdLine = NULL;

    STARTUPINFOW si;
    PROCESS_INFORMATION pi;
    SECURITY_ATTRIBUTES sa;
    DWORD threadID;
    DWORD dwCreationFlags;
    BOOL ok = FALSE;
    HANDLE hProcess = NULL, h = NULL, pipeThreads[2];
    DWORD ExitCode = 0;

    if (wcscmp(command, L"") == 0)
    {
        // do nothing
        pipeSpawnOut.NumberOfLines = 0;
        pipeSpawnOut.OutputBuffer = NULL;

        pipeSpawnErr.NumberOfLines = 0;
        pipeSpawnErr.OutputBuffer = NULL;

        return 1;
    }

    hProcess = GetCurrentProcess();

    ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
    ZeroMemory(&si, sizeof(STARTUPINFOW));
    si.cb = sizeof(STARTUPINFO);
    si.dwFlags   = STARTF_USESTDHANDLES;
    si.hStdInput = INVALID_HANDLE_VALUE;

    ZeroMemory(&sa, sizeof(SECURITY_ATTRIBUTES));
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.lpSecurityDescriptor = NULL;
    sa.bInheritHandle = TRUE;

    /* create a non-inheritible pipe. */
    CreatePipe(&pipeSpawnOut.pipe, &h, &sa, 0);

    /* dupe the write side, make it inheritible, and close the original. */
    DuplicateHandle(hProcess, h, hProcess, &si.hStdOutput,
                    0, TRUE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE);

    /* Same as above, but for the error side. */
    CreatePipe(&pipeSpawnErr.pipe, &h, &sa, 0);
    DuplicateHandle(hProcess, h, hProcess, &si.hStdError,
                    0, TRUE, DUPLICATE_SAME_ACCESS | DUPLICATE_CLOSE_SOURCE);

    /* base command line */
    GetEnvironmentVariableW(L"ComSpec", shellCmd, PATH_MAX);

    if (DetachProcess)
    {
        int lenCmdLine = (int)(wcslen(shellCmd) + wcslen(command) + wcslen(CMDLINE_FORMAT_DETACHED));
        CmdLine = (wchar_t*) MALLOC((lenCmdLine + 1) * sizeof(wchar_t));
        swprintf(CmdLine, lenCmdLine, CMDLINE_FORMAT_DETACHED, shellCmd, command);

        dwCreationFlags = DETACHED_PROCESS;
    }
    else
    {
        int lenCmdLine = 0;
        wchar_t FileTMPDir[PATH_MAX + 16];
        BOOL bConvert = FALSE;

        wchar_t *TMPDirLong = getTMPDIRW();

        swprintf(FileTMPDir, PATH_MAX + 16, OUTPUT_CHECK_FILENAME_FORMAT, TMPDirLong);
        FREE(TMPDirLong);

        if (FileExistW(FileTMPDir))
        {
            DeleteFileW(FileTMPDir);
        }

        lenCmdLine = (int)(wcslen(shellCmd) + wcslen(command) + wcslen(CMDLINE_FORMAT_NOTDETACHED) +
                           wcslen(FileTMPDir));
        CmdLine = (wchar_t*)MALLOC((lenCmdLine + 1) * sizeof(wchar_t));
        swprintf(CmdLine, lenCmdLine, CMDLINE_FORMAT_NOTDETACHED, shellCmd, command, FileTMPDir);

        dwCreationFlags = 0;
    }

    ok = CreateProcessW(
             NULL,	    /* Module name. */
             CmdLine,	    /* Command line. */
             NULL,	    /* Process handle not inheritable. */
             NULL,	    /* Thread handle not inheritable. */
             TRUE,	    /* yes, inherit handles. */
             dwCreationFlags, /* No console for you. */
             NULL,	    /* Use parent's environment block. */
             NULL,	    /* Use parent's starting directory. */
             &si,	    /* Pointer to STARTUPINFO structure. */
             &pi);	    /* Pointer to PROCESS_INFORMATION structure. */

    if (!ok)
    {
        return 2;
    }

    if (CmdLine)
    {
        FREE(CmdLine);
        CmdLine = NULL;
    }

    /* close our references to the write handles that have now been inherited. */
    CloseHandle(si.hStdOutput);
    CloseHandle(si.hStdError);

    WaitForInputIdle(pi.hProcess, 5000);
    CloseHandle(pi.hThread);

    /* start the pipe reader threads. */
    pipeThreads[0] = CreateThread(NULL, 0, ReadFromPipe, &pipeSpawnOut, 0, &threadID);
    pipeThreads[1] = CreateThread(NULL, 0, ReadFromPipe, &pipeSpawnErr, 0, &threadID);

    /* block waiting for the process to end. */
    WaitForSingleObject(pi.hProcess, INFINITE);

    if ( GetExitCodeProcess(pi.hProcess, &ExitCode) == STILL_ACTIVE )
    {
        TerminateProcess(pi.hProcess, 0);
    }

    CloseHandle(pi.hProcess);

    /* wait for our pipe to get done reading */
    WaitForMultipleObjects(2, pipeThreads, TRUE, 500);
    CloseHandle(pipeThreads[0]);
    CloseHandle(pipeThreads[1]);

    return ExitCode;
}
/*--------------------------------------------------------------------------*/
int ClosePipeInfo (pipeinfo pipe)
{
    CloseHandle(pipe.pipe);
    if (pipe.OutputBuffer)
    {
        FREE(pipe.OutputBuffer);
        pipe.OutputBuffer = NULL;
        pipe.NumberOfLines = 0;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
DWORD WINAPI ReadFromPipe (LPVOID args)
{
    pipeinfo *pi = (pipeinfo *) args;
    int readSoFar = 0;
    DWORD dwRead;
    BOOL moreOutput = TRUE;
    unsigned char *op = NULL;

    pi->OutputBuffer = (unsigned char*) MALLOC(BUFSIZE);
    op = pi->OutputBuffer;

    while (moreOutput)
    {
        BOOL bres = ReadFile( pi->pipe, op, BUFSIZE - 1, &dwRead, NULL);

        moreOutput = bres || (dwRead != 0);

        if (moreOutput)
        {
            readSoFar += dwRead;
            pi->OutputBuffer  = (unsigned char*) REALLOC(pi->OutputBuffer , readSoFar + BUFSIZE);
            op = pi->OutputBuffer + readSoFar;
        }
    }
    *op = '\0';
    return 0;
}
/*--------------------------------------------------------------------------*/
int GetNumberOfLines(char *lines)
{
    int NumberOfLines = 0;
    if (lines)
    {
        char *buffer = strdup(lines);
        if (buffer)
        {
            int i = 0;
            char *line = strtok(buffer, LF_STR);

            while (line)
            {
                line = strtok(NULL, LF_STR);
                i++;
            }

            NumberOfLines = i;

            FREE(buffer);
            buffer = NULL;
        }
        if (NumberOfLines == 0)
        {
            NumberOfLines = 1;
        }
    }
    return NumberOfLines;
}
/*--------------------------------------------------------------------------*/
char **CreateOuput(pipeinfo *pipe, BOOL DetachProcess)
{
    char **OuputStrings = NULL;
    if (pipe)
    {
        if (pipe->OutputBuffer)
        {
            char *buffer = strdup((const char *)(pipe->OutputBuffer));
            if (buffer)
            {
                pipe->NumberOfLines = GetNumberOfLines(buffer);
                if (pipe->NumberOfLines)
                {
                    OuputStrings = (char**)MALLOC((pipe->NumberOfLines) * sizeof(char*));
                    if (OuputStrings)
                    {
                        char *line = strtok(buffer, LF_STR);
                        int i = 0;

                        while (line)
                        {
                            OuputStrings[i] = convertLine(line, DetachProcess);
                            line = strtok(NULL, LF_STR);
                            i++;
                            if (i > pipe->NumberOfLines)
                            {
                                break;
                            }
                        }
                    }
                }
                FREE(buffer);
                buffer = NULL;
            }
        }
    }
    return OuputStrings;
}
/*--------------------------------------------------------------------------*/
BOOL DetectDetachProcessInCommandLine(wchar_t *command)
{
    BOOL bOK = FALSE;
    if (command)
    {
        int i = (int)wcslen(command);
        for (i = (int)wcslen(command) - 1; i >= 0; i--)
        {
            if (command[i] == BLANK)
            {
                command[i] = EMPTY_CHAR;
            }
            else
            {
                break;
            }
        }
        i = (int)wcslen(command);
        if ( (i > 0) && (command[i - 1] == L'&') )
        {
            bOK = TRUE;
        }
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL removeEOL(char *_string)
{
    if (_string)
    {
        int len = (int)strlen(_string);
        if ( (_string[len - 1] == CR) || (_string[len - 1] == LF) )
        {
            _string[len - 1] = EMPTY_CHAR;
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL removeNotPrintableCharacters(char *_string)
{
    if (_string)
    {
        int j = 0;
        int len = (int)strlen(_string);
        BOOL bRemove = FALSE;
        for (j = 0; j < len; j++)
        {
            /* remove some no printable characters */
            if (_string[j] == NOTPRINTABLE)
            {
                _string[j] = BLANK;
                bRemove = TRUE;
            }
        }
        return bRemove;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
char *convertLine(char *_string, BOOL DetachProcess)
{
    char *convertedString = NULL;
    if (_string)
    {
        convertedString = strdup(_string);

        if (getScilabMode() == SCILAB_STD)
        {
            if ( (!DetachProcess) && (!IsValidUTF8(_string)) )
            {
                // We need to add detection of ANSI characters
                // in this case we do not convert from Oem to char
                OemToChar(_string, convertedString);
            }
        }
        else
        {
            // in -nw mode
            // chcp 65001 (to switch cmd to UNICODE)
            // and change font to Lucida (TrueType)
            if ( (DetachProcess) && (!IsValidUTF8(_string)) )
            {
                CharToOem(_string, convertedString);
            }
        }

        removeEOL(convertedString);
        removeNotPrintableCharacters(convertedString);

    }
    return convertedString;
}
/*--------------------------------------------------------------------------*/
int CallWindowsShell(char *command)
{
    int returnedExitCode = -1;

    wchar_t shellCmd[PATH_MAX];
    wchar_t *CmdLine = NULL;
    wchar_t * wcommand = NULL;
    size_t iCmdSize = 0;

    PROCESS_INFORMATION piProcInfo;
    STARTUPINFOW siStartInfo;
    SECURITY_ATTRIBUTES saAttr;

    DWORD ExitCode = 0;

    wchar_t *TMPDir = NULL;
    wchar_t FileTMPDir[PATH_MAX];

    if (strcmp(command, "") == 0)
    {
        // do nothing
        return 1;
    }

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION) );

    ZeroMemory( &siStartInfo, sizeof(STARTUPINFO) );
    siStartInfo.cb = sizeof(STARTUPINFO);
    siStartInfo.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    siStartInfo.wShowWindow  = SW_HIDE;
    siStartInfo.hStdInput = NULL;

    siStartInfo.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    siStartInfo.hStdError = GetStdHandle(STD_ERROR_HANDLE);

    GetEnvironmentVariableW(L"ComSpec", shellCmd, PATH_MAX);
    TMPDir = getTMPDIRW();
    swprintf(FileTMPDir, PATH_MAX, L"%s\\DOS.OK", TMPDir);
    if (TMPDir)
    {
        FREE(TMPDir);
        TMPDir = NULL;
    }

    wcommand = to_wide_string(command);
    iCmdSize = (wcslen(shellCmd) + wcslen(wcommand) + wcslen(FileTMPDir) + wcslen(L"%s /a /c \"%s\" && echo DOS>%s") + 1);
    CmdLine = (wchar_t*)MALLOC(iCmdSize * sizeof(wchar_t));
    swprintf(CmdLine, iCmdSize, L"%s /a /c \"%s\" && echo DOS>%s", shellCmd, wcommand, FileTMPDir);

    if (CreateProcessW(NULL, CmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &siStartInfo, &piProcInfo))
    {
        WaitForSingleObject(piProcInfo.hProcess, INFINITE);

        if ( GetExitCodeProcess(piProcInfo.hProcess, &ExitCode) == STILL_ACTIVE )
        {
            TerminateProcess(piProcInfo.hProcess, 0);
        }

        CloseHandle(piProcInfo.hProcess);

        if (CmdLine)
        {
            FREE(CmdLine);
            CmdLine = NULL;
        }

        if (FileExistW(FileTMPDir))
        {
            DeleteFileW(FileTMPDir);
        }

        returnedExitCode = (int)ExitCode;
    }
    else
    {
        CloseHandle(piProcInfo.hProcess);
        if (CmdLine)
        {
            FREE(CmdLine);
            CmdLine = NULL;
        }
    }
    return returnedExitCode;
}
/*--------------------------------------------------------------------------*/
