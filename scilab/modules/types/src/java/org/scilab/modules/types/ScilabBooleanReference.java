/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2010-2010 - DIGITEO - Clement DAVID
 *  Copyright (C) 2011-2011 - DIGITEO - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.types;

import java.io.ObjectStreamException;
import java.nio.IntBuffer;

/**
 * This class provides a representation on the Scilab boolean datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<br />
 * <code>
 * boolean [][]a={{true,false,true}, {true,true,true}};<br />
 * ScilabBoolean aMatrix = new ScilabBoolean(a);
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabBooleanReference extends ScilabBoolean {

    private IntBuffer intBuffer;
    private final int nbRows;
    private final int nbCols;

    /**
     * Create an object from an array of array of boolean
     *
     * @param data
     *            the array of boolean
     */
    public ScilabBooleanReference(String varName, IntBuffer data, int nbRows, int nbCols) {
        this.varName = varName;
        this.intBuffer = data;
        this.swaped = false;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.byref = true;
    }

    /**
     * Check the emptiness of the associated data.
     *
     * @return true, if the associated data array is empty.
     */
    @Override
    public boolean isEmpty() {
        return nbRows == 0 || nbCols == 0;
    }

    /**
     * {@inheritDoc}
     */
    public boolean getElement(final int i, final int j) {
        return intBuffer.get(i + nbRows * j) != 0;
    }

    /**
     * {@inheritDoc}
     */
    public void setElement(final int i, final int j, final boolean x) {
        intBuffer.put(i + nbRows * j, x ? 1 : 0);
    }

    /**
     * Change the value with the provided data
     *
     * @param data
     *            array of boolean
     */
    public void setData(boolean[][] data) {
        ScilabTypeUtils.setPart(intBuffer, data);
    }

    /**
     * Return the data
     *
     * @return the array of array of boolean
     */
    public boolean[][] getData() {
        boolean[][] b = new boolean[nbRows][nbCols];
        ScilabTypeUtils.setBuffer(b, intBuffer);

        return b;
    }

    /**
     * {@inheritDoc}
     */
    public Object getRawData() {
        return intBuffer;
    }

    /**
     * When this object is deserialized we want a ScilabBoolean, not a ScilabBooleanReference.
     * @return a ScilabDouble
     */
    private Object readResolve() throws ObjectStreamException {
        return new ScilabBoolean(varName, data, swaped);
    }

    /**
     * Return the height (number of element) of the stored data
     *
     * @return the height
     */
    @Override
    public int getHeight() {
        return nbRows;
    }

    /**
     * Return the width (number of element) of the stored data
     *
     * @return the width
     */
    @Override
    public int getWidth() {
        return nbCols;
    }
}
