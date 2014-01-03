/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.ruler.graduations;

import java.text.DecimalFormat;
import java.text.DecimalFormatSymbols;
import java.text.FieldPosition;

/**
 * TinyIntervalFormat allows to format 3.0001 into "3+1e-4" or 2.9999 in "3-1e-4"
 * It is useful to represent values in a tiny intervall to avoid to have the same representation
 * for different values.
 * For a number 3.0001, 3 is called the base and 1e-4 the fractional part.
 */
public class TinyIntervalFormat extends DecimalFormat {

    private static final DecimalFormat simpleFormat = new DecimalFormat("0.######");

    private DecimalFormat fracFormat;

    /**
     * Constructor
     * @param basePattern the pattern to represent the base
     * @param fracPattern the pattern to represent frac
     */
    public TinyIntervalFormat(String basePattern, String fracPattern) {
        super(basePattern);
        fracFormat = new DecimalFormat(fracPattern);
    }

    /**
     * {@inheritDoc}
     */
    public StringBuffer format(double number, StringBuffer result, FieldPosition fieldPosition) {
        double[] parts = getParts(number);

        if (parts[1] == 0) {
            return super.format(number, result, fieldPosition);
        }

        if (parts[0] != 0) {
            if (parts[0] < 10) {
                result = simpleFormat.format(parts[0], result, fieldPosition);
            } else {
                result = super.format(parts[0], result, fieldPosition);
            }

            if (parts[1] > 0) {
                result.append("+");
            } else if (parts[1] < 0) {
                result.append("-");
            }

            return fracFormat.format(Math.abs(parts[1]), result, fieldPosition);
        }

        return fracFormat.format(parts[1], result, fieldPosition);
    }

    /**
     * {@inheritDoc}
     */
    public void setDecimalFormatSymbols(DecimalFormatSymbols newSymbols) {
        super.setDecimalFormatSymbols(newSymbols);
        fracFormat.setDecimalFormatSymbols(newSymbols);
        simpleFormat.setDecimalFormatSymbols(newSymbols);
    }

    /**
     * Get the base and the frac part of the number x
     * @param x the value
     * @return the parts
     */
    private static final double[] getParts(final double x) {
        double p = 1;
        double y = x;
        double f = y - Math.round(y);
        while (Math.abs(f) > 1e-2) {
            y *= 10;
            p *= 10;
            f = y - Math.round(y);
        }

        return new double[] {Math.round(y) / p, f / p};
    }
}
