/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
import java.nio.DoubleBuffer;
import java.nio.IntBuffer;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * This class provides a representation on the Scilab Sparse datatype<br>
 * <br>
 * This class is {@link java.io.Serializable} and any modification could impact
 * load and store of data (Xcos files, Javasci saved data, etc...).<br>
 * <br>
 * //TODO Sly : ajouter de la doc
 *
 * @see org.scilab.modules.javasci.Scilab
 */
public class ScilabSparse implements ScilabType {

    private static final long serialVersionUID = 879625048944109684L;
    private static final ScilabTypeEnum type = ScilabTypeEnum.sci_sparse;

    private static final int VERSION = 0;

    private IntBuffer nbItemBuffer;
    private IntBuffer colPosBuffer;
    private DoubleBuffer realBuffer;
    private DoubleBuffer imaginaryBuffer;
    private final boolean byref;

    private int rows;
    private int cols;
    private int nbItem;
    private int[] nbItemRow;
    private int[] colPos;
    private double[] realPart;
    private double[] imaginaryPart;
    private String varName;

    /**
     * Default constructor
     */
    public ScilabSparse() {
        this.byref = false;
    }

    /**
     * Constructor with a unique value.
     *
     * @param data
     *            the unique value
     */
    public ScilabSparse(double data) {
        if (data != 0) {
            nbItem = 1;
            rows = cols = 1;
            nbItemRow = new int[] { 1 };
            colPos = new int[] { 0 };
            realPart = new double[] { data };
        }
        this.byref = false;
    }

    /**
     * Constructor with a unique complex value.
     *
     * @param realData
     *            the real part
     * @param imagData
     *            the complex part
     */
    public ScilabSparse(double realData, double imagData) {
        if (realData != 0 || imagData != 0) {
            nbItem = 1;
            rows = cols = 1;
            nbItemRow = new int[] { 1 };
            colPos = new int[] { 0 };
            realPart = new double[] { realData };
            imaginaryPart = new double[] { imagData };
        }
        this.byref = false;
    }

    /**
     * Constructor
     *
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param data
     *            the non null data
     * @param check
     *            if true the parameters validity is checked
     */
    public ScilabSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] data, boolean check) throws ScilabSparseException {
        this(rows, cols, nbItem, nbItemRow, colPos, data);
        if (check) {
            checkValidity(rows, cols, nbItem, nbItemRow, colPos);

            if (realPart.length != nbItem) {
                throw new ScilabSparseException("Invalid length for the array realPart: its length must be equal to the number of non-null items.");
            }
        }
    }

    /**
     * Constructor
     *
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param real
     *            the non null real data
     */
    public ScilabSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] real) {
        this.rows = rows;
        this.cols = cols;
        this.nbItem = nbItem;
        this.nbItemRow = nbItemRow;
        this.colPos = colPos;
        this.realPart = real;
        this.byref = false;
    }

    /**
     * Constructor
     *
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param real
     *            the non null real data
     * @param imag
     *            the non null imaginary data
     * @param check
     *            if true the parameters validity is checked
     */
    public ScilabSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] real, double[] imag, boolean check)
    throws ScilabSparseException {
        this(rows, cols, nbItem, nbItemRow, colPos, real, imag);
        if (check) {
            checkValidity(rows, cols, nbItem, nbItemRow, colPos);

            if (realPart.length != nbItem) {
                throw new ScilabSparseException("Invalid length for the array realPart: its length must be equal to the number of non-null items.");
            }

            if (imaginaryPart.length != nbItem) {
                throw new ScilabSparseException("Invalid length for the array imaginaryPart: its length must be equal to the number of non-null items.");
            }
        }
    }

    /**
     * Constructor
     *
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param real
     *            the non null real data
     * @param imag
     *            the non null imaginary data
     */
    public ScilabSparse(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] real, double[] imag) {
        this(rows, cols, nbItem, nbItemRow, colPos, real);
        this.imaginaryPart = imag;
    }

    /**
     * Constructor
     *
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     */
    static final void checkValidity(int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos) throws ScilabSparseException {
        if (nbItem > rows * cols || nbItem < 0) {
            throw new ScilabSparseException("Invalid number of items: between 0 and " + rows * cols + " expected.");
        }

        if (nbItemRow.length > rows) {
            throw new ScilabSparseException("Invalid length for the array nbItemRow: a length between 0 and " + rows + " expected.");
        }

        int s = 0;
        for (int i = 0; i < nbItemRow.length; i++) {
            if (nbItemRow[i] > cols) {
                throw new ScilabSparseException("Invalid number of non-null items in nbItemRow at position " + i + ".");
            }
            s += nbItemRow[i];
        }

        if (s != nbItem) {
            throw new ScilabSparseException("Invalid array nbItemRow: the total sum is not equal to the number of non-null items.");
        }

        if (colPos.length != nbItem) {
            throw new ScilabSparseException("Invalid length for the array colPos: its length must be equal to the number of non-null items.");
        }

        for (int i = 0; i < nbItem; i++) {
            if (colPos[i] >= cols || colPos[i] < 0) {
                throw new ScilabSparseException("Invalid column position at position " + i + ".");
            }
        }
    }

    /**
     * Constructor with a matrix of real data.
     *
     * @param data
     *            the data
     */
    public ScilabSparse(double[][] data) {
        if (data.length != 0 && data[0].length != 0) {
            rows = data.length;
            cols = data[0].length;
            nbItemRow = new int[rows];
            List<Integer> listCol = new ArrayList<Integer>();
            List<Double> listVal = new ArrayList<Double>();

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (data[i][j] != 0) {
                        ++nbItem;
                        ++nbItemRow[i];
                        listCol.add(j);
                        listVal.add(data[i][j]);
                    }
                }
            }

            colPos = new int[listCol.size()];
            realPart = new double[colPos.length];
            int i = 0;
            for (Integer c : listCol) {
                colPos[i++] = c;
            }
            i = 0;
            for (Double x : listVal) {
                realPart[i++] = x;
            }
        }
        this.byref = false;
    }

    /**
     * Constructor with a matrix of complex numbers
     *
     * @param realData
     *            the real part of the data
     * @param imagData
     *            the imaginary part of the data
     */
    public ScilabSparse(double[][] realData, double[][] imagData) {
        if (realData.length != 0 && realData[0].length != 0) {
            rows = realData.length;
            cols = realData[0].length;
            nbItemRow = new int[rows];
            List<Integer> listCol = new ArrayList<Integer>();
            List<Double> listReal = new ArrayList<Double>();
            List<Double> listImag = new ArrayList<Double>();

            for (int i = 0; i < rows; i++) {
                for (int j = 0; j < cols; j++) {
                    if (realData[i][j] != 0 || imagData[i][j] != 0) {
                        ++nbItem;
                        ++nbItemRow[i];
                        listCol.add(j);
                        listReal.add(realData[i][j]);
                        listImag.add(imagData[i][j]);
                    }
                }
            }

            colPos = new int[listCol.size()];
            this.realPart = new double[colPos.length];
            this.imaginaryPart = new double[colPos.length];
            int i = 0;
            for (Integer c : listCol) {
                colPos[i++] = c;
            }
            i = 0;
            for (Double x : listReal) {
                this.realPart[i++] = x;
            }
            i = 0;
            for (Double x : listImag) {
                this.imaginaryPart[i++] = x;
            }
        }
        this.byref = false;
    }

    /**
     * Constructor
     *
     * @param varName
     *            the variable name
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param real
     *            the non null real data
     * @param imag
     *            the non null imaginary data
     */
    public ScilabSparse(String varName, int rows, int cols, int nbItem, int[] nbItemRow, int[] colPos, double[] real, double[] imag) {
        this(rows, cols, nbItem, nbItemRow, colPos, real, imag);
        this.varName = varName;
    }

    /**
     * Constructor
     *
     * @param varName
     *            the variable name
     * @param rows
     *            the number of rows
     * @param cols
     *            the number of cols
     * @param nbItem
     *            the number of non null items
     * @param nbItemRow
     *            contains the number of true in each rows
     * @param colPos
     *            the column position of each non null item
     * @param real
     *            the non null real data
     * @param imag
     *            the non null imaginary data
     */
    public ScilabSparse(String varName, int rows, int cols, int nbItem, IntBuffer nbItemRow, IntBuffer colPos, DoubleBuffer real, DoubleBuffer imag) {
        this.varName = varName;
        this.rows = rows;
        this.cols = cols;
        this.nbItem = nbItem;
        this.nbItemBuffer = nbItemRow;
        this.colPosBuffer = colPos;
        this.realBuffer = real;
        this.imaginaryBuffer = imag;
        this.byref = true;
    }

    public final boolean isReference() {
        return byref;
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
     * Check the emptiness of the associated data.
     *
     * @return true, if the associated data array is empty.
     */
    @Override
    public boolean isEmpty() {
        return rows == 0 && cols == 0;
    }

    public final int getColPosElement(final int i) {
        if (byref) {
            return colPosBuffer.get(i);
        } else {
            return colPos[i];
        }
    }

    public final void setColPosElement(final int i, final int x) {
        if (byref) {
            colPosBuffer.put(i, x);
        } else {
            colPos[i] = x;
        }
    }

    public final int getNbItemElement(final int i) {
        if (byref) {
            return nbItemBuffer.get(i);
        } else {
            return nbItemRow[i];
        }
    }

    public final void setNbItemElement(final int i, final int x) {
        if (byref) {
            nbItemBuffer.put(i, x);
        } else {
            nbItemRow[i] = x;
        }
    }

    public final double getRealElement(final int i) {
        if (byref) {
            return realBuffer.get(i);
        } else {
            return realPart[i];
        }
    }

    public final double getImaginaryElement(final int i) {
        if (byref) {
            return imaginaryBuffer.get(i);
        } else {
            return imaginaryPart[i];
        }
    }

    public final double[] getElement(final int i) {
        if (byref) {
            if (isReal()) {
                return new double[] {realBuffer.get(i), 0};
            } else {
                return new double[] {realBuffer.get(i), imaginaryBuffer.get(i)};
            }
        } else {
            if (isReal()) {
                return new double[] {realPart[i], 0};
            } else {
                return new double[] {realPart[i], imaginaryPart[i]};
            }
        }
    }

    public final void setRealElement(final int i, final double x) {
        if (byref) {
            realBuffer.put(i, x);
        } else {
            realPart[i] = x;
        }
    }

    public final void setImaginaryElement(final int i, final double x) {
        if (byref) {
            imaginaryBuffer.put(i, x);
        } else {
            imaginaryPart[i] = x;
        }
    }

    public final void setElement(final int i, final double x, final double y) {
        if (byref) {
            realBuffer.put(i, x);
            imaginaryBuffer.put(i, y);
        } else {
            realPart[i] = x;
            imaginaryPart[i] = y;
        }
    }

    /**
     * Check if the current data doesn't have an imaginary part.
     *
     * @return true, if the data are real only.
     */
    public boolean isReal() {
        if (byref) {
            return imaginaryBuffer == null;
        } else {
            return (imaginaryPart == null);
        }
    }

    /**
     * Get the real part of the data.
     *
     * @return the real part.
     */
    public DoubleBuffer getRealBuffer() {
        return realBuffer;
    }

    /**
     * Get the real part of the data.
     *
     * @return the real part.
     */
    public double[] getRealPart() {
        if (byref) {
            double[] d = new double[realBuffer.capacity()];
            realBuffer.clear();
            realBuffer.get(d);
            return d;
        } else {
            return realPart;
        }
    }

    /**
     * Set the real part of the data.
     *
     * @param realPart
     *            the real part.
     */
    public void setRealPart(double[] realPart) {
        if (byref) {
            realBuffer.clear();
            realBuffer.put(realPart);
        } else {
            this.realPart = realPart;
        }
    }

    /**
     * Get the imaginary part of the data.
     *
     * @return the imaginary part.
     */
    public DoubleBuffer getImaginaryBuffer() {
        return imaginaryBuffer;
    }

    /**
     * Get the imaginary part of the data.
     *
     * @return the imaginary part.
     */
    public double[] getImaginaryPart() {
        if (byref) {
            double[] d = new double[imaginaryBuffer.capacity()];
            imaginaryBuffer.clear();
            imaginaryBuffer.get(d);
            return d;
        } else {
            return imaginaryPart;
        }
    }

    /**
     * Set the imaginary part of the data.
     *
     * @param imaginaryPart
     *            the imaginary part.
     */
    public void setImaginaryPart(double[] imaginaryPart) {
        if (byref) {
            imaginaryBuffer.clear();
            imaginaryBuffer.put(imaginaryPart);
        } else {
            this.imaginaryPart = imaginaryPart;
        }
    }

    /**
     * Get the number of non null items in the matrix.
     *
     * @return the number of non null items.
     */
    public int getNbNonNullItems() {
        return nbItem;
    }

    /**
     * Set the number of non null items in the matrix.
     *
     * @param nbItem
     *            the number of non null items.
     */
    public void setNbNonNullItems(int nbItem) {
        this.nbItem = nbItem;
    }

    /**
     * Get the number of non null items by row.
     *
     * @return an integer array.
     */
    public int[] getNbItemRow() {
        if (byref) {
            int[] i = new int[nbItemBuffer.capacity()];
            nbItemBuffer.clear();
            nbItemBuffer.get(i);
            return i;
        } else {
            return nbItemRow;
        }
    }

    /**
     * Set the number of non null items by row.
     *
     * @param nbItemRow
     *            an integer array.
     */
    public void setNbItemRow(int[] nbItemRow) {
        if (byref) {
            nbItemBuffer.clear();
            nbItemBuffer.put(nbItemRow);
        } else {
            this.nbItemRow = nbItemRow;
        }
    }

    /**
     * Get the column positions of the non null items.
     *
     * @return an integer array.
     */
    public int[] getScilabColPos() {
        int[] cp = new int[colPos.length];
        if (byref) {
            colPosBuffer.clear();
            colPosBuffer.get(cp);
        } else {
            for (int i = 0; i < colPos.length; i++) {
                cp[i] = colPos[i] + 1;
            }
        }

        return cp;
    }

    /**
     * Get the column positions of the non null items.
     *
     * @return an integer array.
     */
    public int[] getColPos() {
        if (byref) {
            int[] i = new int[colPosBuffer.capacity()];
            colPosBuffer.clear();
            colPosBuffer.get(i);
            for (int k = 0; k < i.length; k++) {
                i[k]--;
            }
            return i;
        } else {
            return colPos;
        }
    }

    /**
     * Set the column positions of the non null items.
     *
     * @param colPos
     *            an integer array.
     */
    public void setColPos(int[] colPos) {
        if (byref) {
            int[] cp = new int[colPos.length];
            for (int i = 0; i < colPos.length; i++) {
                cp[i] = colPos[i] + 1;
            }
            colPosBuffer.clear();
            colPosBuffer.put(cp);
        } else {
            this.colPos = colPos;
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
        return false;
    }

    /**
     * Get the real part of the full sparse matrix
     *
     * @return the full real matrix
     */
    public double[][] getFullRealPart() {
        int prev = 0;
        int j = 0;
        double[][] d = new double[rows][cols];
        if (byref) {
            realBuffer.clear();
            for (int i = 0; i < nbItemBuffer.capacity(); i++) {
                final int n = nbItemBuffer.get(i);
                for (; j < prev + n; j++) {
                    d[i][colPosBuffer.get(j) - 1] = realBuffer.get(j);
                }
                prev += n;
            }
        } else {
            for (int i = 0; i < nbItemRow.length; i++) {
                for (; j < prev + nbItemRow[i]; j++) {
                    d[i][colPos[j]] = realPart[j];
                }
                prev += nbItemRow[i];
            }
        }

        return d;
    }

    /**
     * Get the imaginary part of the full sparse matrix
     *
     * @return the full imaginary matrix
     */
    public double[][] getFullImaginaryPart() {
        int prev = 0;
        int j = 0;
        double[][] d = new double[rows][cols];
        if (byref) {
            imaginaryBuffer.clear();
            for (int i = 0; i < nbItemBuffer.capacity(); i++) {
                final int n = nbItemBuffer.get(i);
                for (; j < prev + n; j++) {
                    d[i][colPosBuffer.get(j) - 1] = imaginaryBuffer.get(j);
                }
                prev += n;
            }
        } else {
            for (int i = 0; i < nbItemRow.length; i++) {
                for (; j < prev + nbItemRow[i]; j++) {
                    d[i][colPos[j]] = imaginaryPart[j];
                }
                prev += nbItemRow[i];
            }
        }
        return d;
    }

    /**
     * Get the full sparse matrix representation as an array 2 x rows x cols If
     * d = getFullMatrix(), then d[0] contains realpart and d[1] the imaginary
     * one.
     *
     * @return the full matrix components
     */
    public double[][][] getFullMatrix() {
        int prev = 0;
        int j = 0;
        double[][][] d = new double[2][rows][cols];
        if (byref) {
            realBuffer.clear();
            imaginaryBuffer.clear();
            for (int i = 0; i < nbItemBuffer.capacity(); i++) {
                final int n = nbItemBuffer.get(i);
                for (; j < prev + n; j++) {
                    final int p = colPosBuffer.get(j) - 1;
                    d[0][i][p] = realBuffer.get(j);
                    d[1][i][p] = imaginaryBuffer.get(j);
                }
                prev += n;
            }
        } else {
            for (int i = 0; i < nbItemRow.length; i++) {
                for (; j < prev + nbItemRow[i]; j++) {
                    d[0][i][colPos[j]] = realPart[j];
                    d[1][i][colPos[j]] = imaginaryPart[j];
                }
                prev += nbItemRow[i];
            }
        }

        return d;
    }

    /**
     * Get complex matrix as a serialized form
     *
     * @return the serialized matrix with complex values
     */
    // TODO Sly : faire qque chose ici...
    public double[] getSerializedSparseMatrix() {
        return new double[0];
    }

    /**
     * @return the height of the data matrix
     * @see org.scilab.modules.types.ScilabType#getHeight()
     */
    @Override
    public int getHeight() {
        return rows;
    }

    /**
     * @return the width of the data matrix
     * @see org.scilab.modules.types.ScilabType#getWidth()
     */
    @Override
    public int getWidth() {
        return cols;
    }

    static final boolean equals(final DoubleBuffer buffer, final double[] data) {
        buffer.clear();
        return buffer.equals(DoubleBuffer.wrap(data));
    }

    static final boolean equals(final IntBuffer buffer, final int[] data) {
        buffer.clear();
        return buffer.equals(IntBuffer.wrap(data));
    }

    /**
     * @see org.scilab.modules.types.ScilabType#equals(Object)
     */
    @Override
    public boolean equals(Object obj) {
        if (obj instanceof ScilabSparse) {
            ScilabSparse sciSparse = (ScilabSparse) obj;
            if (this.getNbNonNullItems() == sciSparse.getNbNonNullItems()) {
                if (byref && sciSparse.byref) {
                    nbItemBuffer.clear();
                    sciSparse.nbItemBuffer.clear();
                    if (!nbItemBuffer.equals(sciSparse.nbItemBuffer)) {
                        return false;
                    }
                    colPosBuffer.clear();
                    sciSparse.colPosBuffer.clear();
                    if (!colPosBuffer.equals(sciSparse.colPosBuffer)) {
                        return false;
                    }
                } else if (!byref && !sciSparse.byref) {
                    if (!compareNbItemRow(this.nbItemRow, sciSparse.nbItemRow) || !Arrays.equals(this.colPos, sciSparse.colPos)) {
                        return false;
                    }
                } else if (byref && !sciSparse.byref) {
                    if (!compareNbItemRow(sciSparse.nbItemRow, this.nbItemBuffer) || !equals(this.colPosBuffer, sciSparse.getScilabColPos())) {
                        return false;
                    }
                } else {
                    if (!compareNbItemRow(this.nbItemRow, sciSparse.nbItemBuffer) || !equals(sciSparse.colPosBuffer, this.getScilabColPos())) {
                        return false;
                    }
                }
            }

            if (this.isReal() && sciSparse.isReal()) {
                if (byref && sciSparse.byref) {
                    realBuffer.clear();
                    sciSparse.realBuffer.clear();
                    return realBuffer.equals(sciSparse.realBuffer);
                } else if (!byref && !sciSparse.byref) {
                    return Arrays.equals(this.getRealPart(), sciSparse.getRealPart());
                } else if (byref && !sciSparse.byref) {
                    return equals(this.realBuffer, sciSparse.getRealPart());
                } else {
                    return equals(sciSparse.realBuffer, this.getRealPart());
                }
            }
            if ((this.isReal() && !sciSparse.isReal()) || (!this.isReal() && sciSparse.isReal())) {
                return false;
            } else {
                if (byref && sciSparse.byref) {
                    realBuffer.clear();
                    imaginaryBuffer.clear();
                    sciSparse.realBuffer.clear();
                    sciSparse.imaginaryBuffer.clear();
                    return realBuffer.equals(sciSparse.realBuffer) && imaginaryBuffer.equals(sciSparse.imaginaryBuffer);
                } else if (!byref && !sciSparse.byref) {
                    return Arrays.equals(this.getRealPart(), sciSparse.getRealPart()) && Arrays.equals(this.getImaginaryPart(), sciSparse.getImaginaryPart());
                } else if (byref && !sciSparse.byref) {
                    return equals(this.realBuffer, sciSparse.getRealPart()) && equals(this.imaginaryBuffer, sciSparse.getImaginaryPart());
                } else {
                    return equals(sciSparse.realBuffer, this.getRealPart()) && equals(sciSparse.imaginaryBuffer, this.getImaginaryPart());
                }
            }
        }

        return false;
    }

    /**
     * Compare two arrays containing the number of items by row. For example {1,
     * 2, 3, 4} is equal to {1, 2, 3, 4, 0, 0, 0, 0}/
     *
     * @param a
     *            an array
     * @param b
     *            an other array
     * @return true if the arrays are equal
     */
    static final boolean compareNbItemRow(final int[] a, final int[] b) {
        int[] c, d;
        if (a.length <= b.length) {
            c = a;
            d = b;
        } else {
            c = b;
            d = a;
        }

        int i = 0;
        for (; i < c.length; i++) {
            if (c[i] != d[i]) {
                return false;
            }
        }

        for (; i < d.length; i++) {
            if (d[i] != 0) {
                return false;
            }
        }

        return true;
    }

    /**
     * Compare two arrays containing the number of items by row. For example {1,
     * 2, 3, 4} is equal to {1, 2, 3, 4, 0, 0, 0, 0}/
     *
     * @param a
     *            an array
     * @param b
     *            an other array
     * @return true if the arrays are equal
     */
    static final boolean compareNbItemRow(final int[] a, final IntBuffer b) {
        b.clear();
        final int n = b.capacity();
        if (a.length == n) {
            return b.equals(IntBuffer.wrap(a));
        }

        if (a.length < n) {
            b.limit(a.length);
            boolean eq = b.equals(IntBuffer.wrap(a));
            if (!eq) {
                b.clear();
                return false;
            }

            int[] zeros = new int[n - a.length];
            b.position(a.length);
            b.limit(n);
            eq = b.equals(IntBuffer.wrap(zeros));
            b.clear();

            return eq;
        } else {
            IntBuffer buf = IntBuffer.wrap(a);
            buf.limit(n);
            boolean eq = b.equals(buf);
            if (!eq) {
                return false;
            }

            int[] zeros = new int[a.length - n];
            buf.position(n);
            buf.limit(a.length);
            eq = buf.equals(IntBuffer.wrap(zeros));

            return eq;
        }
    }

    /**
     * {@inheritDoc}
     */
    public Object getSerializedObject() {
        if (isReal()) {
            return new Object[] { new int[] { getHeight(), getWidth() }, getNbItemRow(), getScilabColPos(), getRealPart() };
        } else {
            return new Object[] { new int[] { getHeight(), getWidth() }, getNbItemRow(), getScilabColPos(), getRealPart(), getImaginaryPart() };
        }
    }

    @Override
    public void readExternal(ObjectInput in) throws IOException, ClassNotFoundException {
        int version = in.readInt();
        switch (version) {
            case 0:
                rows = in.readInt();
                cols = in.readInt();
                nbItem = in.readInt();
                nbItemRow = (int[]) in.readObject();
                colPos = (int[]) in.readObject();
                realPart = (double[]) in.readObject();
                imaginaryPart = (double[]) in.readObject();
                varName = (String) in.readObject();
                break;
            default:
                throw new ClassNotFoundException("A class ScilabSparse with a version " + version + " does not exists");
        }
    }

    @Override
    public void writeExternal(ObjectOutput out) throws IOException {
        out.writeInt(VERSION);
        out.writeInt(rows);
        out.writeInt(cols);
        out.writeInt(nbItem);
        out.writeObject(getNbItemRow());
        out.writeObject(getColPos());
        out.writeObject(getRealPart());
        out.writeObject(getImaginaryPart());
        out.writeObject(varName);
    }

    /**
     * Display the representation in the Scilab language of the type<br />
     * Note that the representation can be copied/pasted straight into Scilab
     *
     * @return a Scilab-like String representation of the data.
     * @see java.lang.Object#toString()
     */
    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();

        if (isEmpty()) {
            result.append("[]");
            return result.toString();
        }

        result.append("sparse([");
        int j = 0;
        int prev = 0;
        final int inc = byref ? 0 : 1;
        final int len = byref ? nbItemBuffer.capacity() : nbItemRow.length;
        for (int i = 0; i < len; i++) {
            final int n = getNbItemElement(i);
            for (; j < prev + n; j++) {
                result.append(Integer.toString(i + 1));
                result.append(", ");
                result.append(Integer.toString(getColPosElement(j) + inc));
                if (j < nbItem - 1) {
                    result.append(" ; ");
                }
            }
            prev += n;
        }

        result.append("], [");
        for (int i = 0; i < nbItem; i++) {
            if (isReal()) {
                result.append(Double.toString(getRealElement(i)));
            } else {
                result.append(Double.toString(getRealElement(i)));
                final double im = getImaginaryElement(i);
                if (im != 0) {
                    result.append("+");
                    result.append(Double.toString(im));
                    result.append("*%i");
                }
            }
            if (i < nbItem - 1) {
                result.append(" ; ");
            } else {
                result.append("]");
            }
        }

        result.append(", [");
        result.append(Integer.toString(rows));
        result.append(", ");
        result.append(Integer.toString(cols));

        result.append("])");

        return result.toString();
    }
}
