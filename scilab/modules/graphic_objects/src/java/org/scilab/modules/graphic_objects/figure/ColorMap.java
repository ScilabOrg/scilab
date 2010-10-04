/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects.figure;

/**
 * @author Pierre Lando
 */
public class ColorMap {
    /**
     * Number of channels for one color.
     */
    private static final int CHANNELS_NUMBER = 3;

    private static final Double[] BLACK_COLOR = new Double[]{0.,0.,0.};
    private static final Double[] WHITE_COLOR = new Double[]{1.,1.,1.};

    /**
     * The data of this ColorMap.
     */
    Double[] data = new Double[0];

    /**
     * Return a copy of the data of this color map.
     * @return a copy of the data of this color map.
     */
    public Double[] getData() {
        return data.clone();
    }

    /**
     * Return the number of color in this ColorMap
     * @return the number of color in this ColorMap
     */
    public int getSize() {
        return data.length / CHANNELS_NUMBER;
    }

    /**
     * Return the color corresponding to the given Scilab index.
     *
     * The first color in scilab ColorMap have index 1.
     * If index is 0, it will be considered has 1.
     * If index is -1 the color is black.
     * If index is -2 the color is white.
     * If index is last scilab index + 1, it will be considered has -1.
     * If index is last scilab index + 2, it will be considered has -2.
     * Other value should not be possible.
     * By the way we consider the nearest value.
     *
     * @param index the given Scilab index.
     * @return the color corresponding to the given Scilab index.
     */
    public Double[] getScilabColor(int index) {

        int lastScilabIndex = getSize();

        if (index > lastScilabIndex) {
            index = lastScilabIndex - index;
        }

        if (index < -2) {
            index = -2;
        } 

        if (index == 0) {
            index = 1;
        }

        if (index==-1) {
            return BLACK_COLOR.clone();
        } else if (index==-2) {
            return WHITE_COLOR.clone();
        } else {
            index = index - 1;
            Double[] returned = new Double[CHANNELS_NUMBER];
            System.arraycopy(data, index*CHANNELS_NUMBER, returned, 0, CHANNELS_NUMBER);
            return returned;
        }
    }

    /**
     * Set the data of this ColorMap
     * This function is package to avoid bad use (update model without using the controller).
     * @param data the new data.
     */
    void setData(Double[] data) {
        int length = data.length;
        length -= length %3;
        
        this.data = new Double[length];
        System.arraycopy(data, 0, this.data, 0, length);
    }
}
