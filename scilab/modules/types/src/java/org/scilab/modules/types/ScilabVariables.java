/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.types;

import java.util.ArrayList;
import java.util.List;
import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

import javax.swing.event.EventListenerList;

/**
 * Class to handle the Scilab data retrievment and conversion to a ScilabType
 * object. All the functions sendFoo are used from C++ and should not be used on
 * the Java side.
 *
 * An handler which implements ScilabVariablesHandler must be used to achieve
 * the retrievment. The id returned by addScilabVariablesHandler must be passed
 * from C/C++ to functions in the class org_modules_types::ScilabToJava to
 * guarantee that the correct handler would receive its datas.
 */
public final class ScilabVariables {

    private static final Map<Thread, ArrayList<ScilabType>> lists = new HashMap<Thread, ArrayList<ScilabType>>();
    private static final Vector<ScilabVariablesHandler> handlers = new Vector<ScilabVariablesHandler>();

    /**
     * Register a new handler
     *
     * @param handler
     *            the handler
     * @return the id to use from C/C++
     */
    public static final int addScilabVariablesHandler(ScilabVariablesHandler handler) {
        int i = 0;
        for (; i < handlers.size(); i++) {
            ScilabVariablesHandler h = handlers.get(i);
            if (h == handler) {
                return i;
            } else if (h == null) {
                handlers.set(i, handler);
                return i;
            }
        }
        handlers.add(handler);

        return i;
    }

    /**
     * Unregister an handler (the id must be considered as lost !!)
     *
     * @param handler
     *            the handler to remove
     */
    public static final void removeScilabVariablesHandler(ScilabVariablesHandler handler) {
        if (handler != null) {
            for (int i = 0; i < handlers.size(); i++) {
                if (handlers.get(i) == handler) {
                    handlers.set(i, null);
                }
            }
        }
    }

    /**
     * Unregister an handler with a given id (the id must be considered as lost
     * !!)
     *
     * @param id
     *            the handler id to remove
     */
    public static final void removeScilabVariablesHandler(int id) {
        if (id >= 0 && id < handlers.size()) {
            handlers.set(id, null);
        }
    }

    /**
     * Send double matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, double[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabDouble(null, data, null, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabDouble(varName, data, null, swaped));
        }
    }

    /**
     * Send complex matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param real
     *            the real data
     * @param img
     *            the imaginary data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, double[][] real, double[][] img, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabDouble(null, real, img, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabDouble(varName, real, img, swaped));
        }
    }

    /**
     * Send double matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendHandleData(String varName, int[] indexes, long[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabHandle(null, data, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabHandle(varName, data, swaped));
        }
    }

    /**
     * Send int32 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, int[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, false, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, false, swaped));
        }
    }

    /**
     * Send uint16 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendUnsignedData(String varName, int[] indexes, short[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, true, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, true, swaped));
        }
    }

    /**
     * Send int16 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, short[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, false, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, false, swaped));
        }
    }

    /**
     * Send uint8 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendUnsignedData(String varName, int[] indexes, byte[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, true, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, true, swaped));
        }
    }

    /**
     * Send boolean matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, boolean[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabBoolean(null, data, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabBoolean(varName, data, swaped));
        }
    }

    /**
     * Send int8 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, byte[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, false, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, false, swaped));
        }
    }

    /**
     * Send int64 or uint64 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, long[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, false, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, false, swaped));
        }
    }

    /**
     * Send uint32 matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendUnsignedData(String varName, int[] indexes, int[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabInteger(null, data, true, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabInteger(varName, data, true, swaped));
        }
    }

    /**
     * Send string matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, String[][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabString(null, data, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabString(varName, data, swaped));
        }
    }

    /**
     * Send double sparse matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param row
     *            the row number
     * @param col
     *            the col number
     * @param nbItem
     *            the number of non null elements
     * @param nbItemRow
     *            the number by row of non null elements
     * @param colPos
     *            the column position of the non null elements
     * @param data
     *            the data
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, int row, int col, int nbItem, int[] nbItemRow, int[] colPos, double[] data, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabSparse(null, row, col, nbItem, nbItemRow, colPos, data, null));
        } else {
            handlers.get(handlerId).handle(new ScilabSparse(varName, row, col, nbItem, nbItemRow, colPos, data, null));
        }
    }

    /**
     * Send double sparse matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param row
     *            the row number
     * @param col
     *            the col number
     * @param nbItem
     *            the number of non null elements
     * @param nbItemRow
     *            the number by row of non null elements
     * @param colPos
     *            the column position of the non null elements
     * @param real
     *            the real data
     * @param imag
     *            the imaginary data
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, int row, int col, int nbItem, int[] nbItemRow, int[] colPos, double[] real, double[] imag,
                                      int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabSparse(null, row, col, nbItem, nbItemRow, colPos, real, imag));
        } else {
            handlers.get(handlerId).handle(new ScilabSparse(varName, row, col, nbItem, nbItemRow, colPos, real, imag));
        }
    }

    /**
     * Send boolean sparse matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param row
     *            the row number
     * @param col
     *            the col number
     * @param nbItem
     *            the number of true elements
     * @param nbItemRow
     *            the number by row of true elements
     * @param colPos
     *            the column position of the true elements
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, int row, int col, int nbItem, int[] nbItemRow, int[] colPos, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabBooleanSparse(null, row, col, nbItem, nbItemRow, colPos));
        } else {
            handlers.get(handlerId).handle(new ScilabBooleanSparse(varName, row, col, nbItem, nbItemRow, colPos));
        }
    }

    /**
     * Send double polynomial matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param polyVarName
     *            the polynomial variable name
     * @param data
     *            the data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendPolynomial(String varName, int[] indexes, String polyVarName, double[][][] data, boolean swaped, int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabPolynomial(null, polyVarName, data, null, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabPolynomial(varName, polyVarName, data, null, swaped));
        }
    }

    /**
     * Send complex polynomial matrix
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param polyVarName
     *            the polynomial variable name
     * @param real
     *            the real data
     * @param img
     *            the imaginary data
     * @param swaped
     *            true if the matrix is stored row by row
     * @param handlerId
     *            the handler id
     */
    public static final void sendPolynomial(String varName, int[] indexes, String polyVarName, double[][][] real, double[][][] img, boolean swaped,
                                            int handlerId) {
        if (indexes.length != 0) {
            addElement(indexes, new ScilabPolynomial(null, polyVarName, real, img, swaped));
        } else {
            handlers.get(handlerId).handle(new ScilabPolynomial(varName, polyVarName, real, img, swaped));
        }
    }

    /**
     * Send list, tlist and mlist
     *
     * @param varName
     *            the variable name
     * @param indexes
     *            an integer array with the indexes of the (sub)*-list which
     *            will contain the data
     * @param type
     *            a char which must take the values 'l' for list or 'm' for
     *            mlist or 't' for tlist
     * @param handlerId
     *            the handler id
     */
    public static final void sendData(String varName, int[] indexes, char type, int handlerId) {
        String name = null;
        if (indexes.length == 0) {
            name = varName;
        }

        ScilabType var = null;
        switch (type) {
            case 'l':
                var = new ScilabList(name);
                break;
            case 'm':
                var = new ScilabMList(name);
                break;
            case 't':
                var = new ScilabTList(name);
                break;
        }

        if (indexes.length == 0) {
            lists.put(Thread.currentThread(), (ArrayList<ScilabType>) var);
        } else {
            addElement(indexes, var);
        }
    }

    /**
     * Call when the list filling is finished a * @param indexes an integer
     * array with the indexes of the (sub)*-list which will contain the data
     *
     * @param handlerId
     *            the handler id
     */
    public static final void closeList(int[] indexes, int handlerId) {
        Thread t = Thread.currentThread();
        ArrayList<ScilabType> var = lists.get(t);
        if (var != null && indexes.length == 0) {
            handlers.get(handlerId).handle((ScilabType) var);
            lists.remove(t);
        }
    }

    /**
     * Add an element to the list
     *
     * @param indexes
     *            the indexes where to put the variable
     * @param var
     *            the variable to put
     */
    private static final void addElement(int[] indexes, ScilabType data) {
        ArrayList<ScilabType> list = lists.get(Thread.currentThread());

        for (int i = 0; i < indexes.length - 1; i++) {
            list = (ArrayList<ScilabType>) list.get(indexes[i] - 1);
        }

        int n = indexes[indexes.length - 1] - 1;
        if (n < list.size()) {
            list.set(indexes[indexes.length - 1] - 1, data);
        } else {
            list.add(data);
        }
    }
}
