/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.util.List;

import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.Controller;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort.DataType;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.input.InputPort;

/**
 * Perform an input port transformation between Scicos and Xcos.
 *
 * On this element we doesn't validate the Scicos values has they have been
 * already checked on the {@link BlockElement}.
 */
public final class InputPortElement extends AbstractElement<InputPort> {
    /**
     * The related block id in the model
     */
    private long id = 0;

    protected static final List<String> DATA_FIELD_NAMES = BlockElement.DATA_FIELD_NAMES;

    protected static final List<String> GRAPHICS_DATA_FIELD_NAMES_FULL = BlockGraphicElement.DATA_FIELD_NAMES_FULL;
    protected static final List<String> MODEL_DATA_FIELD_NAMES = BlockModelElement.DATA_FIELD_NAMES;

    private static final int GRAPHICS_PIN_INDEX = GRAPHICS_DATA_FIELD_NAMES_FULL.indexOf("pin");
    private static final int GRAPHICS_INIMPL_INDEX = GRAPHICS_DATA_FIELD_NAMES_FULL.indexOf("in_implicit");
    private static final int GRAPHICS_INSTYLE_INDEX = GRAPHICS_DATA_FIELD_NAMES_FULL.indexOf("in_style");
    private static final int GRAPHICS_INLABEL_INDEX = GRAPHICS_DATA_FIELD_NAMES_FULL.indexOf("in_label");

    private static final int MODEL_IN_DATALINE_INDEX = MODEL_DATA_FIELD_NAMES.indexOf("in");
    private static final int MODEL_IN_DATACOL_INDEX = MODEL_DATA_FIELD_NAMES.indexOf("in2");
    private static final int MODEL_IN_DATATYPE_INDEX = MODEL_DATA_FIELD_NAMES.indexOf("intyp");

    private int alreadyDecodedCount;
    private boolean allColumnsAreZeros = true;

    /**
     * Default constructor
     *
     * @param id
     *            the Scicos block parameters used by this element.
     */
    public InputPortElement(long id) {
        setID(id);
    }

    /**
     * @return the id
     */
    public long getID() {
        return id;
    }

    /**
     * @param id the id to set
     */
    public void setID(long id) {
        this.id = id;
    }

    /**
     * @return the number of input ports for the element.
     */
    public int getNumberOfInputPort() {
        final Controller controller = new Controller();
        VectorOfScicosID inputs = new VectorOfScicosID();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.INPUTS, inputs);
        return (int) inputs.size();
    }

    /**
     * Decode Scicos element into the block.
     *
     * @param id
     *            the id of the Scicos element.
     * @return the new port.
     * @throws ScicosFormatException
     *             on error.
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType,
     *      java.lang.Object)
     */
    public InputPort decode(long id) throws ScicosFormatException {

        InputPort port;

        port = allocatePort(id);

        port = beforeDecode(null, port);

        //decodeModel(port); // The info is already in the port
        //decodeGraphics(port); // The info is already in the port

        // Update the index counter
        alreadyDecodedCount++;

        port = afterDecode(null, port);

        return port;
    }

    /**
     * Allocate a port according to the explicit/implicit values.
     *
     * @return a new typed port
     */
    private InputPort allocatePort(long id) {
        final Controller controller = new Controller();
        boolean[] isImpl = {true};
        controller.getObjectProperty(id, Kind.PORT, ObjectProperties.IMPLICIT, isImpl);

        if (isImpl[0]) {
            return new ImplicitInputPort(id);
        } else {
            return new ExplicitInputPort(id);
        }
    }

    /**
     * Encode the instance into the element
     *
     * @param from
     *            the source instance
     * @param element
     *            the previously allocated element.
     * @return the element parameter
     * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object,
     *      org.scilab.modules.types.ScilabType)
     */
    @Override
    public ScilabType encode(InputPort from, ScilabType element) {
        ScilabMList data = (ScilabMList) element;

        if (data == null) {
            throw new IllegalArgumentException();
        }

        data = (ScilabMList) beforeEncode(from, data);

        encodeModel(from, data);
        encodeGraphics(from, data);

        // Update the index counter
        alreadyDecodedCount++;

        data = (ScilabMList) afterEncode(from, data);

        return data;
    }

    /**
     * Encode the data into the model fields.
     *
     * This method fills :
     * <ul>
     * <li>Block.model.in</li>
     * <li>Block.model.in2</li>
     * <li>Block.model.intyp</li>
     * </ul>
     *
     * @param from
     *            the source data
     */
    private void encodeModel(InputPort from, ScilabMList data) {
        ScilabDouble sciValues;
        double[][] values;

        ScilabMList model = (ScilabMList) data.get(DATA_FIELD_NAMES.indexOf("model"));

        // in
        sciValues = (ScilabDouble) model.get(MODEL_IN_DATALINE_INDEX);
        values = sciValues.getRealPart();
        int datalines = from.getDataLines();
        values[alreadyDecodedCount][0] = datalines;

        // in2
        sciValues = (ScilabDouble) model.get(MODEL_IN_DATACOL_INDEX);
        values = sciValues.getRealPart();
        int datacolumns = from.getDataColumns();
        if (datacolumns == 0) {
            datacolumns = 1;
        } else {
            allColumnsAreZeros = false;
        }
        values[alreadyDecodedCount][0] = datacolumns;

        // intyp
        sciValues = (ScilabDouble) model.get(MODEL_IN_DATATYPE_INDEX);
        values = sciValues.getRealPart();
        values[alreadyDecodedCount][0] = from.getDataType().getAsDouble();
    }

    /**
     * Encode the data into the graphic fields.
     *
     * This method fills :
     * <ul>
     * <li>Block.graphics.pin</li>
     * <li>Block.graphics.in_implicit</li>
     * </ul>
     *
     * @param from
     *            the source data
     */
    private void encodeGraphics(InputPort from, ScilabMList data) {
        ScilabDouble sciValues;
        ScilabString sciStrings;
        double[][] values;
        String[][] strings;

        ScilabMList graphics = (ScilabMList) data.get(DATA_FIELD_NAMES.indexOf("graphics"));

        // pin
        sciValues = (ScilabDouble) graphics.get(GRAPHICS_PIN_INDEX);
        values = sciValues.getRealPart();
        if (from.getEdgeCount() == 1) {
            // only set on valid connection
            values[alreadyDecodedCount][0] = ((BasicLink) from.getEdgeAt(0)).getOrdering();
        } else {
            values[alreadyDecodedCount][0] = 0.0;
        }

        // in_implicit
        sciStrings = (ScilabString) graphics.get(GRAPHICS_INIMPL_INDEX);
        strings = sciStrings.getData();
        strings[alreadyDecodedCount][0] = from.getType().getAsString();

        // in_style
        sciStrings = (ScilabString) graphics.get(GRAPHICS_INSTYLE_INDEX);
        strings = sciStrings.getData();
        strings[alreadyDecodedCount][0] = from.getStyle();

        // in_label
        sciStrings = (ScilabString) graphics.get(GRAPHICS_INLABEL_INDEX);
        strings = sciStrings.getData();
        if (from.getValue() != null) {
            strings[alreadyDecodedCount][0] = from.getValue().toString();
        } else {
            strings[alreadyDecodedCount][0] = "";
        }
    }

    /**
     * Clear Block.model.in2 if it contains only zeros.
     */
    public ScilabMList afterEncode(ScilabType ret) {
        ScilabMList data = (ScilabMList) ret;
        if (allColumnsAreZeros) {
            data.set(MODEL_IN_DATACOL_INDEX, new ScilabDouble());
        }
        return data;
    }
}
