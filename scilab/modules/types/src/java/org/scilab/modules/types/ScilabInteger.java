/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
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
import java.nio.ByteBuffer;
import java.nio.IntBuffer;
import java.nio.LongBuffer;
import java.nio.ShortBuffer;
import java.util.Arrays;

/**
 * This class provides a representation on the Scilab Integer datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * Example:<br />
 * <code>
 * byte [][]a={{32,42,41}, {12,13,32}};<br />
 * ScilabInteger aMatrix = new ScilabInteger(a, true); // true = unsigned
 * </code>
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabInteger implements ScilabType {

    private static final long serialVersionUID = 1759633801332932450L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_ints;

    private static final int VERSION = 0;

    private LongBuffer longBuffer;
    private IntBuffer intBuffer;
    private ShortBuffer shortBuffer;
    private ByteBuffer byteBuffer;
    private final boolean byref;
    private final int nbRows;
    private final int nbCols;

    private long[][] longData;
    private short[][] shortData;
    private int[][] intData;
    private byte[][] byteData;
    private ScilabIntegerTypeEnum precision;
    private String varName;
    private boolean swaped;

    /**
     * Default constructor
     */
    public ScilabInteger() {
        byref = false;
        nbRows = 0;
        nbCols = 0;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(byte[][] data, boolean bUnsigned) {
        this.setData(data, bUnsigned);
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(short[][] data, boolean bUnsigned) {
        this.setData(data, bUnsigned);
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(int[][] data, boolean bUnsigned) {
        this.setData(data, bUnsigned);
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(long[][] data, boolean bUnsigned) {
        this.setData(data, bUnsigned);
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(String varName, byte[][] data, boolean bUnsigned, boolean swaped) {
        this.setData(data, bUnsigned);
        this.varName = varName;
        this.swaped = swaped;
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(String varName, short[][] data, boolean bUnsigned, boolean swaped) {
        this.setData(data, bUnsigned);
        this.varName = varName;
        this.swaped = swaped;
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(String varName, int[][] data, boolean bUnsigned, boolean swaped) {
        this.setData(data, bUnsigned);
        this.varName = varName;
        this.swaped = swaped;
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    public ScilabInteger(String varName, long[][] data, boolean bUnsigned, boolean swaped) {
        this.setData(data, bUnsigned);
        this.varName = varName;
        this.swaped = swaped;
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    ScilabInteger(String varName, ByteBuffer data, int nbRows, int nbCols, boolean bUnsigned) {
        this.varName = varName;
        this.byteBuffer = data;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint8 : ScilabIntegerTypeEnum.sci_int8;
        this.byref = true;
        this.swaped = false;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    ScilabInteger(String varName, ShortBuffer data, int nbRows, int nbCols, boolean bUnsigned) {
        this.varName = varName;
        this.shortBuffer = data;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint16 : ScilabIntegerTypeEnum.sci_int16;
        this.byref = true;
        this.swaped = false;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    ScilabInteger(String varName, IntBuffer data, int nbRows, int nbCols, boolean bUnsigned) {
        this.varName = varName;
        this.intBuffer = data;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint32 : ScilabIntegerTypeEnum.sci_int32;
        this.byref = true;
        this.swaped = false;
    }

    /**
     * Constructor with values
     *
     * @param data
     *            the values
     * @param bUnsigned
     *            true, if the values are unsigned; false if they are signed.
     */
    ScilabInteger(String varName, LongBuffer data, int nbRows, int nbCols, boolean bUnsigned) {
        this.varName = varName;
        this.longBuffer = data;
        this.nbRows = nbRows;
        this.nbCols = nbCols;
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint64 : ScilabIntegerTypeEnum.sci_int64;
        this.byref = true;
        this.swaped = false;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(byte value) {
        this.byteData = new byte[1][1];
        this.byteData[0][0] = value;
        this.precision = ScilabIntegerTypeEnum.sci_int8;
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(short value) {
        this.shortData = new short[1][1];
        this.shortData[0][0] = value;
        this.precision = ScilabIntegerTypeEnum.sci_int16;
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(int value) {
        this.intData = new int[1][1];
        this.intData[0][0] = value;
        this.precision = ScilabIntegerTypeEnum.sci_int32;
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(long value) {
        this.longData = new long[1][1];
        this.longData[0][0] = value;
        this.precision = ScilabIntegerTypeEnum.sci_int64;
        this.byref = false;
        this.nbRows = 0;
        this.nbCols = 0;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(byte value, boolean bUnsigned) {
        this(value);
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint8 : ScilabIntegerTypeEnum.sci_int8;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(short value, boolean bUnsigned) {
        this(value);
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint16 : ScilabIntegerTypeEnum.sci_int16;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(int value, boolean bUnsigned) {
        this(value);
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint32 : ScilabIntegerTypeEnum.sci_int32;
    }

    /**
     * Constructor with single signed value
     *
     * @param value
     *            the unique value
     */
    public ScilabInteger(long value, boolean bUnsigned) {
        this(value);
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint64 : ScilabIntegerTypeEnum.sci_int64;
    }

    public final boolean isReference() {
        return byref;
    }

    public final byte getByteElement(final int i, final int j) {
        if (byref) {
            return byteBuffer.get(i + nbRows * j);
        } else {
            return byteData[i][j];
        }
    }

    public final short getShortElement(final int i, final int j) {
        if (byref) {
            return shortBuffer.get(i + nbRows * j);
        } else {
            return shortData[i][j];
        }
    }

    public final int getIntElement(final int i, final int j) {
        if (byref) {
            return intBuffer.get(i + nbRows * j);
        } else {
            return intData[i][j];
        }
    }

    public final long getLongElement(final int i, final int j) {
        if (byref) {
            return longBuffer.get(i + nbRows * j);
        } else {
            return longData[i][j];
        }
    }

    public final long getElement(final int i, final int j) {
        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                return (long) getByteElement(i, j);
            case sci_int16:
            case sci_uint16:
                return (long) getShortElement(i, j);
            case sci_int32:
            case sci_uint32:
                return (long) getIntElement(i, j);
            case sci_int64:
            case sci_uint64:
                return getLongElement(i, j);
        }

        return 0;
    }

    public final void setByteElement(final int i, final int j, final byte x) {
        if (byref) {
            byteBuffer.put(i + nbRows * j, x);
        } else {
            byteData[i][j] = x;
        }
    }

    public final void setShortElement(final int i, final int j, final short x) {
        if (byref) {
            shortBuffer.put(i + nbRows * j, x);
        } else {
            shortData[i][j] = x;
        }
    }

    public final void setIntElement(final int i, final int j, final int x) {
        if (byref) {
            intBuffer.put(i + nbRows * j, x);
        } else {
            intData[i][j] = x;
        }
    }

    public final void setLongElement(final int i, final int j, final long x) {
        if (byref) {
            longBuffer.put(i + nbRows * j, x);
        } else {
            longData[i][j] = x;
        }
    }

    public final void setElement(final int i, final int j, final long x) {
        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                setByteElement(i, j, (byte) x);
                break;
            case sci_int16:
            case sci_uint16:
                setShortElement(i, j, (short) x);
                break;
            case sci_int32:
            case sci_uint32:
                setIntElement(i, j, (int) x);
                break;
            case sci_int64:
            case sci_uint64:
                setLongElement(i, j, x);
        }
    }

    /**
     * Set the current values
     *
     * @param data
     *            the values to set
     * @param bUnsigned
     *            true, if these values are unsigned; false otherwise.
     */
    public void setData(byte[][] data, boolean bUnsigned) {
        if (byref) {
            setPart(byteBuffer, data);
        } else {
            this.byteData = data;
        }
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint8 : ScilabIntegerTypeEnum.sci_int8;
    }

    /**
     * Set the current values
     *
     * @param data
     *            the values to set
     * @param bUnsigned
     *            true, if these values are unsigned; false otherwise.
     */
    public void setData(short[][] data, boolean bUnsigned) {
        if (byref) {
            setPart(shortBuffer, data);
        } else {
            this.shortData = data;
        }
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint16 : ScilabIntegerTypeEnum.sci_int16;
    }

    /**
     * Set the current values
     *
     * @param data
     *            the values to set
     * @param bUnsigned
     *            true, if these values are unsigned; false otherwise.
     */
    public void setData(int[][] data, boolean bUnsigned) {
        if (byref) {
            setPart(intBuffer, data);
        } else {
            this.intData = data;
        }
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint32 : ScilabIntegerTypeEnum.sci_int32;
    }

    /**
     * Set the current values
     *
     * @param data
     *            the values to set
     * @param bUnsigned
     *            true, if these values are unsigned; false otherwise.
     */
    public void setData(long[][] data, boolean bUnsigned) {
        if (byref) {
            setPart(longBuffer, data);
        } else {
            this.longData = data;
        }
        this.precision = bUnsigned ? ScilabIntegerTypeEnum.sci_uint64 : ScilabIntegerTypeEnum.sci_int64;
    }

    private static final void setPart(final ByteBuffer buffer, final byte[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c == buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.put(part[i]);
            }
        }
    }

    private static final void setPart(final ShortBuffer buffer, final short[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c == buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.put(part[i]);
            }
        }
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

    private static final void setPart(final LongBuffer buffer, final long[][] part) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c == buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.put(part[i]);
            }
        }
    }

    private static final void setBuffer(final byte[][] part, final ByteBuffer buffer) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c == buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.get(part[i]);
            }
        }
    }

    private static final void setBuffer(final short[][] part, final ShortBuffer buffer) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c == buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.get(part[i]);
            }
        }
    }

    private static final void setBuffer(final int[][] part, final IntBuffer buffer) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c == buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.get(part[i]);
            }
        }
    }

    private static final void setBuffer(final long[][] part, final LongBuffer buffer) {
        final int c = part.length;
        final int r = c > 0 ? part[0].length : 0;
        if (r * c == buffer.capacity()) {
            buffer.clear();
            for (int i = 0; i < c; i++) {
                buffer.get(part[i]);
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
     * If the precision is not 64, all values will be converted to long
     * (attention, the convertion can be long) if precision is 64, just return
     * the data
     *
     * @return the values
     */
    public long[][] getData() {
        final int h = this.getHeight();
        final int w = this.getWidth();
        long[][] convertedMatrix = new long[h][w];

        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                for (int i = 0; i < h; i++) {
                    for (int j = 0; j < w; j++) {
                        convertedMatrix[i][j] = Long.valueOf(getByteElement(i, j));
                    }
                }
                return convertedMatrix;
            case sci_int16:
            case sci_uint16:
                for (int i = 0; i < h; i++) {
                    for (int j = 0; j < w; j++) {
                        convertedMatrix[i][j] = Long.valueOf(getShortElement(i, j));
                    }
                }
                return convertedMatrix;
            case sci_int32:
            case sci_uint32:
                for (int i = 0; i < h; i++) {
                    for (int j = 0; j < w; j++) {
                        convertedMatrix[i][j] = Long.valueOf(getIntElement(i, j));
                    }
                }
                return convertedMatrix;
            case sci_int64:
            case sci_uint64:
                return longData;
        }
        return null;
    }

    /**
     * Returns the value as the form of short
     *
     * @return the values as short
     */
    public short[][] getDataAsShort() {
        if (byref) {
            short[][] d = new short[nbRows][nbCols];
            setBuffer(d, shortBuffer);
            return d;
        } else {
            return shortData;
        }
    }

    /**
     * Returns the value as the form of byte
     *
     * @return the values as byte
     */
    public byte[][] getDataAsByte() {
        if (byref) {
            byte[][] d = new byte[nbRows][nbCols];
            setBuffer(d, byteBuffer);
            return d;
        } else {
            return byteData;
        }
    }

    /**
     * Returns the value as the form of int
     *
     * @return the values as int
     */
    public int[][] getDataAsInt() {
        if (byref) {
            int[][] d = new int[nbRows][nbCols];
            setBuffer(d, intBuffer);
            return d;
        } else {
            return intData;
        }
    }

    /**
     * Returns the value as the form of long Only for Scilab 6.X
     *
     * @return the values as long
     */
    public long[][] getDataAsLong() {
        if (byref) {
            long[][] d = new long[nbRows][nbCols];
            setBuffer(d, longBuffer);
            return d;
        } else {
            return longData;
        }
    }

    /**
     * Returns the value as the form of short
     *
     * @return the values as short
     */
    public ShortBuffer getDataAsShortBuffer() {
        return shortBuffer;
    }

    /**
     * Returns the value as the form of byte
     *
     * @return the values as byte
     */
    public ByteBuffer getDataAsByteBuffer() {
        return byteBuffer;
    }

    /**
     * Returns the value as the form of int
     *
     * @return the values as int
     */
    public IntBuffer getDataAsIntBuffer() {
        return intBuffer;
    }

    /**
     * Returns the value as the form of long Only for Scilab 6.X
     *
     * @return the values as long
     */
    public LongBuffer getDataAsLongBuffer() {
        return longBuffer;
    }

    /**
     * @return the precision of the values
     */
    public ScilabIntegerTypeEnum getPrec() {
        return precision;
    }

    /**
     * @return true, if the values are signed, false otherwise.
     */
    public boolean isUnsigned() {
        switch (precision) {
            case sci_int8:
            case sci_int16:
            case sci_int32:
            case sci_int64:
                return false;
            case sci_uint8:
            case sci_uint16:
            case sci_uint32:
            case sci_uint64:
                return true;
        }
        return false;
    }

    /**
     * Manage the old representation of IntegerType
     *
     * @param typeName
     *            the typeName (TYPE8, TYPE16, TYPE32, TYPE64)
     * @param unsigned
     *            unsigned or not
     * @return the converted type to ScilabIntegerTypeEnum. null is cannot
     *         convert
     */
    public static ScilabIntegerTypeEnum convertOldType(String typeName, boolean unsigned) {
        if (typeName.equals("TYPE8")) {
            return unsigned ? ScilabIntegerTypeEnum.sci_uint8 : ScilabIntegerTypeEnum.sci_int8;
        }
        if (typeName.equals("TYPE16")) {
            return unsigned ? ScilabIntegerTypeEnum.sci_uint16 : ScilabIntegerTypeEnum.sci_int16;
        }
        if (typeName.equals("TYPE32")) {
            return unsigned ? ScilabIntegerTypeEnum.sci_uint32 : ScilabIntegerTypeEnum.sci_int32;
        }
        if (typeName.equals("TYPE64")) {
            return unsigned ? ScilabIntegerTypeEnum.sci_uint64 : ScilabIntegerTypeEnum.sci_int64;
        }

        return null;
    }

    /**
     * @return the height of the value matrix
     * @see org.scilab.modules.types.ScilabType#getHeight()
     */
    @Override
    public int getHeight() {
        if (this.getPrec() == null) {
            return 0;
        }

        if (byref) {
            return nbRows;
        }

        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                if (byteData == null) {
                    return 0;
                }
                return byteData.length;
            case sci_int16:
            case sci_uint16:
                if (shortData == null) {
                    return 0;
                }
                return shortData.length;
            case sci_int32:
            case sci_uint32:
                if (intData == null) {
                    return 0;
                }
                return intData.length;
            case sci_int64:
            case sci_uint64:
                if (longData == null) {
                    return 0;
                }
                return longData.length;
            default:
                return 0;
        }
    }

    /**
     * @return the width of the value matrix
     * @see org.scilab.modules.types.ScilabType#getWidth()
     */
    @Override
    public int getWidth() {
        if (this.getPrec() == null) {
            return 0;
        }

        if (byref) {
            return nbCols;
        }

        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                if (byteData == null) {
                    return 0;
                }
                return byteData[0].length;
            case sci_int16:
            case sci_uint16:
                if (shortData == null) {
                    return 0;
                }
                return shortData[0].length;
            case sci_int32:
            case sci_uint32:
                if (intData == null) {
                    return 0;
                }
                return intData[0].length;
            case sci_int64:
            case sci_uint64:
                if (longData == null) {
                    return 0;
                }
                return longData[0].length;
            default:
                return 0;
        }
    }

    // int32(X), int8(x) , int16([x,x,x;x,x,x])
    // uint32(X), uint8(x) , uint16([x,x,x;x,x,x])

    /**
     * @return true, if there is no values; false otherwise.
     */
    @Override
    public boolean isEmpty() {
        if (this.getPrec() == null) {
            return true;
        }
        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                return byref ? byteBuffer == null : byteData == null;
            case sci_int16:
            case sci_uint16:
                return byref ? shortBuffer == null : shortData == null;
            case sci_int32:
            case sci_uint32:
                return byref ? intBuffer == null : intData == null;
            case sci_int64:
            case sci_uint64:
                return byref ? longBuffer == null : longData == null;
            default:
                return true;
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

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabInteger) {
            ScilabInteger sciInteger = (ScilabInteger) obj;
            if (getPrec() != sciInteger.getPrec()) {
                return false;
            }

            switch (getPrec()) {
                case sci_int8:
                case sci_uint8:
                    if (byref && sciInteger.byref) {
                        byteBuffer.clear();
                        sciInteger.byteBuffer.clear();
                        return byteBuffer.equals(sciInteger.byteBuffer);
                    } else if (!byref && !sciInteger.byref) {
                        return Arrays.deepEquals(byteData, sciInteger.byteData);
                    } else if (byref && !sciInteger.byref) {
                        return ScilabTypeUtils.equals(byteBuffer, nbRows, nbCols, sciInteger.byteData, sciInteger.swaped);
                    } else {
                        return ScilabTypeUtils.equals(sciInteger.byteBuffer, sciInteger.nbRows, sciInteger.nbCols, byteData, this.swaped);
                    }
                case sci_int16:
                case sci_uint16:
                    if (byref && sciInteger.byref) {
                        shortBuffer.clear();
                        sciInteger.shortBuffer.clear();
                        return shortBuffer.equals(sciInteger.shortBuffer);
                    } else if (!byref && !sciInteger.byref) {
                        return Arrays.deepEquals(shortData, sciInteger.shortData);
                    } else if (byref && !sciInteger.byref) {
                        return ScilabTypeUtils.equals(shortBuffer, nbRows, nbCols, sciInteger.shortData, sciInteger.swaped);
                    } else {
                        return ScilabTypeUtils.equals(sciInteger.shortBuffer, sciInteger.nbRows, sciInteger.nbCols, shortData, this.swaped);
                    }
                case sci_int32:
                case sci_uint32:
                    if (byref && sciInteger.byref) {
                        intBuffer.clear();
                        sciInteger.intBuffer.clear();
                        return intBuffer.equals(sciInteger.intBuffer);
                    } else if (!byref && !sciInteger.byref) {
                        return Arrays.deepEquals(intData, sciInteger.intData);
                    } else if (byref && !sciInteger.byref) {
                        return ScilabTypeUtils.equals(intBuffer, nbRows, nbCols, sciInteger.intData, sciInteger.swaped);
                    } else {
                        return ScilabTypeUtils.equals(sciInteger.intBuffer, sciInteger.nbRows, sciInteger.nbCols, intData, this.swaped);
                    }
                case sci_int64:
                case sci_uint64:
                    if (byref && sciInteger.byref) {
                        longBuffer.clear();
                        sciInteger.longBuffer.clear();
                        return longBuffer.equals(sciInteger.longBuffer);
                    } else if (!byref && !sciInteger.byref) {
                        return Arrays.deepEquals(longData, sciInteger.longData);
                    } else if (byref && !sciInteger.byref) {
                        return ScilabTypeUtils.equals(longBuffer, nbRows, nbCols, sciInteger.longData, sciInteger.swaped);
                    } else {
                        return ScilabTypeUtils.equals(sciInteger.longBuffer, sciInteger.nbRows, sciInteger.nbCols, longData, this.swaped);
                    }
            }
        }

        return false;
    }

    private Object getCorrectData() {
        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                return getDataAsByte();
            case sci_int16:
            case sci_uint16:
                return getDataAsShort();
            case sci_int32:
            case sci_uint32:
                return getDataAsInt();
            case sci_int64:
            case sci_uint64:
                return getDataAsLong();
        }
        return null;
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        return new Object[] { new int[] { this.getPrec().swigValue() }, getCorrectData() };
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int version = in.readInt();
        switch (version) {
            case 0:
                precision = ScilabIntegerTypeEnum.swigToEnum(in.readInt());
                Object data = in.readObject();
                switch (precision) {
                    case sci_int8:
                    case sci_uint8:
                        byteData = (byte[][]) data;
                        break;
                    case sci_int16:
                    case sci_uint16:
                        shortData = (short[][]) data;
                        break;
                    case sci_int32:
                    case sci_uint32:
                        intData = (int[][]) data;
                        break;
                    case sci_int64:
                    case sci_uint64:
                        longData = (long[][]) data;
                        break;
                }
                varName = (String) in.readObject();
                swaped = in.readBoolean();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabInteger with a version " + version + " does not exists");
        }
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(VERSION);
        out.writeInt(getPrec().swigValue());
        out.writeObject(getCorrectData());
        out.writeObject(varName);
        out.writeBoolean(swaped);
    }

    /**
     * Display the representation in the Scilab language of the type<br />
     * Note that the representation can be copied/pasted straight into Scilab
     *
     * @return the pretty-printed values
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();

        if (isEmpty()) {
            result.append("int([])");
            return result.toString();
        }

        if (isUnsigned()) {
            result.append("u");
        }
        result.append("int");

        switch (this.getPrec()) {
            case sci_int8:
            case sci_uint8:
                result.append("8");
                break;

            case sci_int16:
            case sci_uint16:
                result.append("16");
                break;

            case sci_int32:
            case sci_uint32:
                result.append("32");
                break;

            case sci_int64:
            case sci_uint64:
                result.append("64");
                break;

            default:
                break;
        }

        result.append("([");
        appendData(result);
        result.append("])");

        return result.toString();
    }

    /**
     * Put each value on the buffer.
     *
     * @param result
     *            the current buffer
     */
    private void appendData(StringBuilder result) {
        final int h = getHeight();
        final int w = getWidth();
        for (int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                result.append(getElement(i, j));

                if (j != w - 1) {
                    result.append(", ");
                }
            }
            if (i != h - 1) {
                result.append(" ; ");
            }
        }
    }
}
