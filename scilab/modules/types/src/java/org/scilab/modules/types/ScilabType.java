/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2012-2012 - Scilab Enterprises - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.types;

import java.io.Externalizable;
import java.io.Serializable;

/**
 * This interface specifies what is a Scilab types<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 */
public abstract class ScilabType implements Externalizable, Serializable, Cloneable {

    private String varName;
    private boolean swaped;
    
    protected ScilabType() {
        varName = null;
        swaped = false;
    }
            
    protected ScilabType(String varName, boolean swaped) {
        this.varName = varName;
        this.swaped = swaped;
    }
    
    protected void setVarName(String varName) {
        this.varName = varName;
    }
    
    protected String getVarName() {
        return varName;
    }
    
    protected boolean isSwaped() {
        return swaped;
    }
    
    /**
     * Return the type of Scilab
     *
     * @return the type of Scilab
     * @since 5.4.0
     */
    public abstract ScilabTypeEnum getType();

    /**
     * Return the width (number of elements) of the stored data
     *
     * @return the width
     */
    public abstract int getWidth();

    /**
     * Return the height (number of elements) of the stored data
     *
     * @return the height
     */
    public abstract int getHeight();

    /**
     * Check if the current instance is empty.
     *
     * @return true, the there is no data; false otherwise.
     */
    public abstract boolean isEmpty();

    /**
     * Compare if obj is the same object or not
     *
     * @param obj
     *            the object we want to compare
     * @return true if the two objects are equals, false otherwise
     */
    public abstract boolean equals(Object obj);

    /**
     * Get a single object used to easily rebuild a Scilab variable from the
     * Java ScilabType
     *
     * @return an very simple object reflecting this ScilabType.
     */
    public abstract Object getSerializedObject();

    /**
     * Display the representation in the Scilab language of the type<br />
     * Note that the representation can be copied/pasted straight into Scilab
     *
     * @return the pretty print
     */
    public abstract String toString();
}
