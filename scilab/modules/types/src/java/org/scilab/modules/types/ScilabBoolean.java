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
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.types;

import java.io.IOException;
import java.io.ObjectInput;
import java.io.ObjectOutput;
import java.nio.IntBuffer;
import java.util.Arrays;

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
public class ScilabBoolean implements ScilabType {

    private static final long serialVersionUID = 6511497080095473901L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_boolean;

    private static final int VERSION = 0;

    private IntBuffer intBuffer;
    private final boolean byref;
    private final int nbRows;
    private final int nbCols;

    /* the boolean data */
    private boolean[][] data;
    private String varName;
    private boolean swaped;

    /**
     * Create an empty object
     */
    public ScilabBoolean() {
        data = null;
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Create an object from an array of array of boolean
     *
     * @param data
     *            the array of boolean
     */
    public ScilabBoolean(boolean[][] data) {
        this();
        this.data = data;
    }

    /**
     * Create an object from an array of array of boolean
     *
     * @param data
     *            the array of boolean
     */
    public ScilabBoolean(String varName, boolean[][] data, boolean swaped) {
        this.varName = varName;
        this.data = data;
        this.swaped = swaped;
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Create an object from an array of array of boolean
     *
     * @param data
     *            the array of boolean
     */
    public ScilabBoolean(String varName, IntBuffer data, int nbRows, int nbCols) {
        this.varName = varName;
        this.intBuffer = data;
        this.swaped = false;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.byref = true;
    }

    /**
     * Create a scalar boolean from a boolean
     *
     * @param value
     *            the boolean
     */
    public ScilabBoolean(boolean value) {
        this();
        this.data = new boolean[1][1];
        this.data[0][0] = value;
    }

    public final boolean isReference() {
        return byref;
    }

    public final boolean getElement(final int i, final int j) {
        if (byref) {
            return intBuffer.get(i + nbRows * j) != 0;
        } else {
            return data[i][j];
        }
    }

    public final void setElement(final int i, final int j, final boolean x) {
        if (byref) {
            intBuffer.put(i + nbRows * j, x ? 1 : 0);
        } else {
            data[i][j] = x;
        }
    }

    /**
     * Change the value with the provided data
     *
     * @param data
     *            array of boolean
     */
    public void setData(int[][] data) {
        if (byref) {
            setPart(intBuffer, data);
        } else {
            this.data = new boolean[data.length][data[0].length];
            for (int i = 0; i < data.length; i++) {
                for (int j = 0; j < data[0].length; j++) {
                    this.data[i][j] = data[i][j] != 0;
                }
            }
        }
    }

    /**
     * Change the value with the provided data
     *
     * @param data
     *            array of boolean
     */
    public void setData(boolean[][] data) {
        if (byref) {
            setPart(intBuffer, data);
        } else {
            this.data = data;
        }
    }

    /**
     * Return the data
     *
     * @return the array of array of boolean
     */
    public boolean[][] getData() {
        if (byref) {
            boolean[][] b = new boolean[nbRows][nbCols];
            intBuffer.clear();
            for (int i = 0; i < nbRows; i++) {
                for (int j = 0; j < nbCols; j++) {
                    b[i][j] = intBuffer.get(i + nbRows * j) != 0;
                }
            }
            return b;
        } else {
            return data;
        }
    }

    /**
     * {@inheritDoc}
     */
    public String getVarName() {
        return varName;
    }

    /**
     * {@inheritDoc}
     */
    public boolean isSwaped() {
        return swaped;
    }

    private static final void setPart(final IntBuffer buffer, final int[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c == buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.put(part[i]);
            }
        }
    }

    private static final void setPart(final IntBuffer buffer, final boolean[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c == buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                for (int j = 0; j < r; j++) {
                    buffer.put(i + c * j, part[i][j] ? 1 : 0);
                }
            }
        }
    }

    /**
     * Return the type of Scilab
     *
     * @return the type of Scilab
     * @since 5.4.0
     */
    @Override
    public ScilabTypeEnum getType() {
        return type;
    }

    /**
     * Return the height (number of element) of the stored data
     *
     * @return the height
     */
    @Override
    public int getHeight() {
        if (isEmpty()) {
            return 0;
        }
        if (byref) {
            return nbRows;
        } else {
            return data.length;
        }
    }

    /**
     * Return the width (number of element) of the stored data
     *
     * @return the width
     */
    @Override
    public int getWidth() {
        if (isEmpty()) {
            return 0;
        }
        if (byref) {
            return nbCols;
        } else {
            return data[0].length;
        }
    }

    /**
     * If it is empty / uninitialized
     *
     * @return true if empty
     */
    @Override
    public boolean isEmpty() {
        return (data == null);
    }

    private final boolean equals(final IntBuffer buffer, final boolean[][] data) {
        final int c = data.length;
        final int r = c > 0 ? data[0].length : 0;
        if (r == nbRows && c == nbCols) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                for (int j = 0; j < r; j++) {
                    if ((buffer.get(i + c * j) != 0) != data[i][j]) {
                        return false;
                    }
                }
            }

            return true;
        }

        return false;
    }

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabBoolean) {
            ScilabBoolean sciBoolean = (ScilabBoolean) obj;

            if (byref && sciBoolean.byref) {
                intBuffer.clear();
                sciBoolean.intBuffer.clear();
                return intBuffer.equals(sciBoolean.intBuffer);
            } else if (!byref && !sciBoolean.byref) {
                return Arrays.deepEquals(this.data, sciBoolean.data);
            } else if (byref && !sciBoolean.byref) {
                return equals(intBuffer, sciBoolean.data);
            } else {
                return equals(sciBoolean.intBuffer, data);
            }
        } else {
            return false;
        }
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        return getData();
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int version = in.readInt();
        switch (version) {
            case 0:
                data = (boolean[][]) in.readObject();
                varName = (String) in.readObject();
                swaped = in.readBoolean();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabBoolean with a version " + version + " does not exists");
        }
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(VERSION);
        out.writeObject(data);
        out.writeObject(varName);
        out.writeBoolean(swaped);
    }

    /**
     * Display the representation in the Scilab language of the type<br />
     * Note that the representation can be copied/pasted straight into Scilab
     *
     * @return the pretty print
     */
    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();

        if (isEmpty()) {
            return "[]";
        }

        result.append("[");
        final int h = getHeight();
        final int w = getWidth();
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                if (getElement(i, j)) {
                    result.append("%t");
                } else {
                    result.append("%f");
                }
                if (j != w - 1) {
                    result.append(", ");
                }
            }
            if (i != h - 1) {
                result.append(" ; ");
            }
        }
        result.append("]");
        return result.toString();
    }
}
