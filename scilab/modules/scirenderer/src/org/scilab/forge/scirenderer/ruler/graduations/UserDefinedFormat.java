/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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
import java.util.Formatter;
import java.util.IllegalFormatConversionException;
import java.util.Locale;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Use a user defined format to format ticks label.
 */
public class UserDefinedFormat extends DecimalFormat {

    private String format;
    private double scale;
    private double translation;
    private DecimalFormat fallback;

    /**
     * Constructor
     * @param basePattern the pattern to represent the base
     * @param fracPattern the pattern to represent frac
     */
    public UserDefinedFormat(DecimalFormat fallback, String format, double scale, double translation) {
        super();
        this.format = format;
        this.scale = scale;
        this.translation = translation;
        this.fallback = fallback;
    }

    public String getFormat() {
        return format;
    }

    /**
     * {@inheritDoc}
     */
    public StringBuffer format(double number, StringBuffer result, FieldPosition fieldPosition) {
        final double d = scale * (number - translation);
        if (format != null && !format.isEmpty()) {
            try {
                Formatter fmt = new Formatter(Locale.US);
                fmt.format(Locale.US, format, d);
                return result.append(fmt.toString());
            } catch (IllegalFormatConversionException e) {
                try {
                    Formatter fmt = new Formatter(Locale.US);
                    fmt.format(Locale.US, format, (long) d);
                    return result.append(fmt.toString());
                } catch (Exception ee) { }
            } catch (ArrayIndexOutOfBoundsException e) {
                // Java bug in Formatter format("%.1g", 0.) or something like that is faulty
                // So what is following is just a crappy workaround to replace g by f...
                if (d == 0) {
                    Pattern pat = Pattern.compile("([^%]*%[-#+ 0,(]?\\.[01])[gG](.*)");
                    Matcher match = pat.matcher(format);
                    if (match.find() && match.groupCount() == 2) {
                        StringBuilder buffer = new StringBuilder(format.length());
                        buffer.append(format.substring(0, match.start())).append(match.group(1)).append('f').append(match.group(2));
                        format = buffer.toString();

                        try {
                            Formatter fmt = new Formatter(Locale.US);
                            fmt.format(Locale.US, format, d);
                            return result.append(fmt.toString());
                        } catch (Exception ee) { }
                    }
                }
            } catch (Exception e) { }
        }

        return fallback.format(number, result, fieldPosition);
    }

    /**
     * {@inheritDoc}
     */
    public void setDecimalFormatSymbols(DecimalFormatSymbols newSymbols) {
        fallback.setDecimalFormatSymbols(newSymbols);
    }
}
