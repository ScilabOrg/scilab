/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.types;

import java.nio.ByteBuffer;
import java.nio.DoubleBuffer;
import java.nio.ShortBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;

final class ScilabTypeUtils {

    static final boolean equals(final DoubleBuffer buffer, final int nbRows, final int nbCols, final double[][] data, final boolean swaped) {
        if (!swaped) {
            final int r = data.length;
            final int c = r > 0 ? data[0].length : 0;
            if (r == nbRows && c == nbCols) {
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        if (buffer.get(i + r * j) != data[i][j]) {
                            return false;
                        }
                    }
                }
                return true;
            }
        } else {
            final int c = data.length;
            final int r = c > 0 ? data[0].length : 0;
            if (r == nbRows && c == nbCols) {
                buffer.clear();
                for (int i = 0; i < c; i++) {
                    buffer.position(r * i);
                    buffer.limit(r * i + r);
                    if (!buffer.equals(buffer.wrap(data[i]))) {
                        buffer.clear();
                        return false;
                    }
                }
                buffer.clear();
                return true;
            }
        }
        return false;
    }

    static final boolean equals(final ByteBuffer buffer, final int nbRows, final int nbCols, final byte[][] data, final boolean swaped) {
        if (!swaped) {
            final int r = data.length;
            final int c = r > 0 ? data[0].length : 0;
            if (r == nbRows && c == nbCols) {
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        if (buffer.get(i + r * j) != data[i][j]) {
                            return false;
                        }
                    }
                }
                return true;
            }
        } else {
            final int c = data.length;
            final int r = c > 0 ? data[0].length : 0;
            if (r == nbRows && c == nbCols) {
                buffer.clear();
                for (int i = 0; i < c; i++) {
                    buffer.position(r * i);
                    buffer.limit(r * i + r);
                    if (!buffer.equals(buffer.wrap(data[i]))) {
                        buffer.clear();
                        return false;
                    }
                }
                buffer.clear();
                return true;
            }
        }
        return false;
    }

    static final boolean equals(final ShortBuffer buffer, final int nbRows, final int nbCols, final short[][] data, final boolean swaped) {
        if (!swaped) {
            final int r = data.length;
            final int c = r > 0 ? data[0].length : 0;
            if (r == nbRows && c == nbCols) {
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        if (buffer.get(i + r * j) != data[i][j]) {
                            return false;
                        }
                    }
                }
                return true;
            }
        } else {
            final int c = data.length;
            final int r = c > 0 ? data[0].length : 0;
            if (r == nbRows && c == nbCols) {
                buffer.clear();
                for (int i = 0; i < c; i++) {
                    buffer.position(r * i);
                    buffer.limit(r * i + r);
                    if (!buffer.equals(buffer.wrap(data[i]))) {
                        buffer.clear();
                        return false;
                    }
                }
                buffer.clear();
                return true;
            }
        }
        return false;
    }

    static final boolean equals(final IntBuffer buffer, final int nbRows, final int nbCols, final int[][] data, final boolean swaped) {
        if (!swaped) {
            final int r = data.length;
            final int c = r > 0 ? data[0].length : 0;
            if (r == nbRows && c == nbCols) {
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        if (buffer.get(i + r * j) != data[i][j]) {
                            return false;
                        }
                    }
                }
                return true;
            }
        } else {
            final int c = data.length;
            final int r = c > 0 ? data[0].length : 0;
            if (r == nbRows && c == nbCols) {
                buffer.clear();
                for (int i = 0; i < c; i++) {
                    buffer.position(r * i);
                    buffer.limit(r * i + r);
                    if (!buffer.equals(buffer.wrap(data[i]))) {
                        buffer.clear();
                        return false;
                    }
                }
                buffer.clear();
                return true;
            }
        }
        return false;
    }

    static final boolean equals(final LongBuffer buffer, final int nbRows, final int nbCols, final long[][] data, final boolean swaped) {
        if (!swaped) {
            final int r = data.length;
            final int c = r > 0 ? data[0].length : 0;
            if (r == nbRows && c == nbCols) {
                for (int i = 0; i < r; i++) {
                    for (int j = 0; j < c; j++) {
                        if (buffer.get(i + r * j) != data[i][j]) {
                            return false;
                        }
                    }
                }
                return true;
            }
        } else {
            final int c = data.length;
            final int r = c > 0 ? data[0].length : 0;
            if (r == nbRows && c == nbCols) {
                buffer.clear();
                for (int i = 0; i < c; i++) {
                    buffer.position(r * i);
                    buffer.limit(r * i + r);
                    if (!buffer.equals(buffer.wrap(data[i]))) {
                        buffer.clear();
                        return false;
                    }
                }
                buffer.clear();
                return true;
            }
        }
        return false;
    }
}
