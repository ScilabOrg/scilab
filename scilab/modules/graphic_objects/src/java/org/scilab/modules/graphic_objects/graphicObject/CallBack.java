/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *  Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.graphic_objects.graphicObject;

import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.UpdateStatus;

/**
 * Abstract class to manage all callbacks.
 *
 * @author Bruno JOFRET
 */
public class CallBack {

    /**
     * Unmanaged command type constant
     */
    public static final int UNTYPED = -1;
    /**
     * Scilab instruction command type constant
     */
    public static final int SCILAB_INSTRUCTION = 0;
    public static final int SCILAB_NOT_INTERRUPTIBLE_INSTRUCTION = 10;
    /**
     * C or Fortran function type constant
     */
    public static final int C_FORTRAN = 1;
    /**
     * Scilab function type constant
     */
    public static final int SCILAB_FUNCTION = 2;
    public static final int SCILAB_NOT_INTERRUPTIBLE_FUNCTION = 12;
    /**
     * Scilab function type constant (not trapped by scilab event listeners)
     */
    public static final int SCILAB_OUT_OF_XCLICK_AND_XGETMOUSE = -2;
    /**
     * Java function type constant
     */
    public static final int JAVA = 3;
    /**
     * Java function type constant (not trapped by scilab event listeners)
     */
    public static final int JAVA_OUT_OF_XCLICK_AND_XGETMOUSE = -3;

    /**
     * Scilab instruction without GCBO setting (old addmenu compatibility)
     */
    public static final int SCILAB_INSTRUCTION_WITHOUT_GCBO = 4;

    /**
     * The Command to Store and remember.
     */
    private String command;

    /**
     * The command type
     */
    private int commandType;

    /**
     * Constructor to be seen by specifics CallBack.
     * @param command the command associated to the ccallback
     */
    public CallBack(String command) {
        this(command, UNTYPED);
    }

    /**
     * Constructor to be seen by specifics CallBack.
     * @param command the command associated to the ccallback
     * @param commandType the command type
     */
    public CallBack(String command, int commandType) {
        this.command = command;
        this.commandType =  commandType;
    }

    /**
     * @param command sets the command
     */
    public UpdateStatus setCommand(String command) {
        this.command = command;
        return UpdateStatus.Success;
    }

    /**
     * @return the command if it's a Scilab instruction.
     */
    public String getCommand() {
        return command;
    }

    /**
     * @param commandType sets the command
     */
    public UpdateStatus setCommandType(int commandType) {
        this.commandType = commandType;
        return UpdateStatus.Success;
    }

    /**
     * @return the command type
     */
    public int getCommandType() {
        return commandType;
    }
}
