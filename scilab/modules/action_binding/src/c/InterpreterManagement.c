/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <signal.h> /* for SIGINT */
#include "InterpreterManagement.h"
#include "storeCommand.h"
#include "sigbas.h"
/*--------------------------------------------------------------------------*/
int putCommandInScilabQueue(char *command)
{
    return StoreCommand(command);
}
/*--------------------------------------------------------------------------*/
/*
 * requestScilabExec
 *
 * WARNING : if the command is taking some time, scilab will not do anything else
 * before the command returns.
 */
int requestScilabExec(char *command)
{
    return StoreCommandWithFlag(command, 1);
}
/*--------------------------------------------------------------------------*/
int interruptScilab(void)
{
    int scilabSignal = SIGINT;
    C2F(sigbas)(&scilabSignal);
    return 0;
}
/*--------------------------------------------------------------------------*/
