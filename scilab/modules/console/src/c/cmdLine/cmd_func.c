/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Karim Mamode
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*/

#include		<wchar.h>
#include		<wctype.h>
#include		<stdlib.h>
#include		<stdio.h>
#include		<unistd.h>
#include		<curses.h>
#include		<termios.h>
#include		<term.h>
#include		"history.h"
#include		"reader.h"
#include		"cap_func.h"
#include		"goto_func.h"
#include		"charctl.h"
#include		"init_tc_shell.h"
#include		"aff_prompt.h"
#include		"cmd_func.h"
#include		"MALLOC.h"

/*
 * If last key was '1'
 * it means this could be arrow key plus control key
 */
static void caseCtrlAndArrowKey(DoublyLinkedList * listCmd, unsigned int *cursorLocation)
{
    if (getwchar() == L';' && getwchar() == L'5')
    {
        switch (getwchar())
        {
        case L'C':
            nextWord(listCmd->data, cursorLocation);
            break;
        case L'D':
            previousWord(listCmd->data, cursorLocation);
            break;
        }
    }
}

static void caseHomeOrEndKey(DoublyLinkedList * listCmd, unsigned int *cursorLocation)
{
    switch (getwchar())
    {
    case L'H':
        begLine(listCmd->data, cursorLocation);
        break;
    case L'F':
        endLine(listCmd->data, cursorLocation);
        break;
    }
}

/*
 * If second key was L'['
 * It means this could be an arrow key or delete key.
 */
static void caseDelOrArrowKey(DoublyLinkedList ** listCmd, unsigned int *cursorLocation)
{
    switch (getwchar())
    {
    case L'A':
        previousCmd(listCmd, cursorLocation);
        break;
    case L'B':
        nextCmd(listCmd, cursorLocation);
        break;
    case L'C':
        gotoRight((*listCmd)->data, cursorLocation);
        break;
    case L'D':
        gotoLeft((*listCmd)->data, cursorLocation);
        break;
    case L'1':
        caseCtrlAndArrowKey(*listCmd, cursorLocation);
        break;
    case L'3':
        if (getwchar() == L'~')
        {
            rmChar((*listCmd)->data, SCI_DELETE, cursorLocation);
            break;
        }
    }
}

/*
 * If last key was Meta...
 */
static void caseMetaKey(DoublyLinkedList ** listCmd, unsigned int *cursorLocation)
{
    switch (getwchar())
    {
    case L'f':
    case L'F':
        nextWord((*listCmd)->data, cursorLocation);
        break;
    case L'b':
    case L'B':
        previousWord((*listCmd)->data, cursorLocation);
        break;
    case L'[':
        caseDelOrArrowKey(listCmd, cursorLocation);
        break;
    case L'O':
        caseHomeOrEndKey(*listCmd, cursorLocation);
        break;
    }
}

/*
 * Read keyboard a first time.
 */
static int getKey(DoublyLinkedList ** listCmd, unsigned int *cursorLocation)
{
    int key;

    key = getwchar();
    switch (key)
    {
    case CTRL_A:
        begLine((*listCmd)->data, cursorLocation);
        return 0;
    case CTRL_B:
        gotoLeft((*listCmd)->data, cursorLocation);
        return 0;
    case CTRL_E:
        endLine((*listCmd)->data, cursorLocation);
        return 0;
    case CTRL_F:
        gotoRight((*listCmd)->data, cursorLocation);
        return 0;
    case CTRL_K:
        deleteLineFromCurs((*listCmd)->data, cursorLocation);
        return 0;
    case CTRL_N:
        nextCmd(listCmd, cursorLocation);
        return 0;
    case CTRL_P:
        previousCmd(listCmd, cursorLocation);
        return 0;
    case ESCAPE:
        caseMetaKey(listCmd, cursorLocation);
        break;
    case SCI_BACKSPACE:
        rmChar((*listCmd)->data, SCI_BACKSPACE, cursorLocation);
        return 0;
    default:
        if (key == L'\n')
            return '\n';
        addChar((*listCmd)->data, key, cursorLocation);
        return 0;
    }
}

/*
 * If there is a string the function save it.
 * else The function write the saved string.
 */
void memCmd(DoublyLinkedList * cmd, int cursorLocation)
{
    static DoublyLinkedList *memList;

    static int i;

    if (cmd != NULL)
    {
        memList = cmd;
        i = cursorLocation;
    }
    else
    {
/* TODO comment */
        i = cursorLocation;
        cursorLocation = wcslen(memList->data);
        printf(SCI_PRINT_WSTRING, memList->data);
/* TODO probably useless. We are doing a buffering word by word */
        fflush(stdout);
        while (cursorLocation != i)
        {
            gotoLeft(memList, 0);
        }
    }
}

DoublyLinkedList *getNewCmd(DoublyLinkedList * lastCmd)
{
    DoublyLinkedList *newCmd;

    newCmd = MALLOC(sizeof(wchar_t));
    if (lastCmd != NULL)
    {
        lastCmd->next = newCmd;
    }
    newCmd->prev = lastCmd;
    newCmd->next = NULL;
    return newCmd;
}

void getCmd(DoublyLinkedList ** listCmd)
{
    int bin;

    unsigned int cursorLocation = 0;

    bin = 1;
    while (bin)
    {
        memCmd(*listCmd, cursorLocation);
        if (getKey(listCmd, &cursorLocation) == '\n')
        {
            putchar('\n');
            bin = 0;
        }
    }
}

DoublyLinkedList *freeCmd(DoublyLinkedList ** cmd)
{
    DoublyLinkedList *save;

    save = *cmd;
    *cmd = (*cmd)->prev;
    free(save->data);
    free(save);
    if (*cmd)
        (*cmd)->next = NULL;
    return *cmd;
}

void deleteHistory(DoublyLinkedList * cmd, int limit)
{
    while (limit && cmd->prev)
    {
        cmd = cmd->prev;
        limit--;
    }
    if (!limit)
    {
        cmd = cmd->next;
        free(cmd->prev->data);
        free(cmd->prev);
        cmd->prev = NULL;
    }
}
