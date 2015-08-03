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

import static java.util.Arrays.asList;

import java.util.List;
import java.util.logging.Logger;

import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.Controller;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

/**
 * Perform a block transformation between Scicos and Xcos.
 */
// CSOFF: ClassDataAbstractionCoupling
// CSOFF: ClassFanOutComplexity
public final class BlockElement extends AbstractElement<BasicBlock> {
    protected static final List<String> DATA_FIELD_NAMES = asList("Block", "graphics", "model", "gui", "doc");

    /** Mutable field to easily get the data through methods */
    private ScilabMList data;

    /** Element used to decode/encode Scicos model part into a BasicBlock */
    private final BlockModelElement modelElement;

    /** Element used to decode/encode Scicos model part into a BasicBlock */
    private final BlockGraphicElement graphicElement;

    /*
     * Decoder state
     */

    /**
     * Current block ordering, the ordering change on each {@link BlockElement}
     * instance so be careful when allocated a new {@link BlockElement}.
     */
    private int ordering;

    /**
     * Default constructor.
     *
     * The state change on each {@link BlockElement} instance so be careful when
     * allocated a new {@link BlockElement}.
     */
    public BlockElement(long id) {
        graphicElement = new BlockGraphicElement();
        modelElement = new BlockModelElement();
    }

    /**
     * Decode the element into the block.
     *
     * @param into
     *            the target, if null a new instance is allocated and returned.
     * @return the decoded block.
     * @throws ScicosFormatException
     *             when e decoding error occurred.
     * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.ScilabType,
     *      java.lang.Object)
     */
    public BasicBlock decode(BasicBlock into) throws ScicosFormatException {
        BasicBlock block = into;

        block = beforeDecode(null, block);

        /*
         * Allocate and setup ports
         */
        InputPortElement inElement = new InputPortElement(block.getID());
        final Controller controller = new Controller();
        VectorOfScicosID inputs = new VectorOfScicosID();
        controller.getObjectProperty(block.getID(), Kind.BLOCK, ObjectProperties.INPUTS, inputs);
        final int numberOfInputPorts = (int) inputs.size();
        for (int i = 0; i < numberOfInputPorts; ++i) {
            final BasicPort port = inElement.decode(inputs.get(i));

            // do not use BasicPort#addPort() to avoid the view update
            port.setOrdering(i + 1);
            block.insert(port, i);
        }

        OutputPortElement outElement = new OutputPortElement(block.getID());
        VectorOfScicosID outputs = new VectorOfScicosID();
        controller.getObjectProperty(block.getID(), Kind.BLOCK, ObjectProperties.OUTPUTS, outputs);
        final int numberOfOutputPorts = (int) outputs.size();
        for (int i = 0; i < numberOfOutputPorts; i++) {
            final BasicPort port = outElement.decode(outputs.get(i));

            // do not use BasicPort#addPort() to avoid the view update
            port.setOrdering(i + 1);
            block.insert(port, numberOfInputPorts + i);
        }

        /*
         * Apply the zoom factor to the dimensions of the model
         */
        graphicElement.applyFactor(block);
        modelElement.decode(block);

        /*
         * Set state dependent information.
         */
        block.setOrdering(ordering);
        ordering++;

        block = afterDecode(null, block);

        return block;
    }

    /**
     * Use the Scicos documentation structure to get the previous Xcos IDs.
     *
     * @param scilabType
     *            the scicos documentation field.
     * @param into
     *            the target instance.
     */
    private void fillDocStructure(ScilabType scilabType, BasicBlock into) {
        /*
         * The double type is used as the default one, generate on empty field.
         */
        if (scilabType instanceof ScilabDouble) {
            return;
        }

        /*
         * Classical behavior
         */
        ScilabList list = (ScilabList) scilabType;

        if (list.size() > 0 && list.get(0) instanceof ScilabString) {
            String uid = ((ScilabString) list.get(0)).getData()[0][0];
            if (isValidUid(uid)) {
                into.setId(uid);
                return;
            }
        }
    }

    /**
     * @param uid
     *            The uid to check
     * @return true if the uid is valid, false otherwise
     */
    private boolean isValidUid(String uid) {
        final String[] components = uid.split(":");

        boolean valid = components.length == 3;
        if (valid) {
            try {
                Integer.parseInt(components[0], 16);
                Long.parseLong(components[1], 16);
                Integer.parseInt(components[2], 16);
            } catch (IllegalArgumentException e) {
                valid = false;
            }
        }
        return valid;
    }

    // CSON: CyclomaticComplexity
    // CSON: NPathComplexity

    /**
     * Test if the current instance can be used to decode the element
     *
     * @param element
     *            the current element
     * @return true, if the element can be decoded, false otherwise
     * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.ScilabType)
     */
    @Override
    public boolean canDecode(ScilabType element) {
        data = (ScilabMList) element;

        final String type = ((ScilabString) data.get(0)).getData()[0][0];
        return type.equals(DATA_FIELD_NAMES.get(0));
    }

    /**
     * {@inheritDoc}
     *
     * Clear cell warnings before encoding
     */
    @Override
    public ScilabType beforeEncode(BasicBlock from, ScilabType element) {
        XcosDiagram graph = from.getParentDiagram();
        if (graph == null) {
            from.setParentDiagram(Xcos.findParent(from));
            graph = from.getParentDiagram();
            Logger.getLogger(BlockElement.class.getName()).finest("Parent diagram was null");
        }
        if (graph.getAsComponent() != null) {
            graph.getAsComponent().removeCellOverlays(from);
        }
        return super.beforeEncode(from, element);
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
    public ScilabType encode(BasicBlock from, ScilabType element) {
        data = (ScilabMList) element;

        int field = 0;
        ScilabType base;

        if (data == null) {
            data = allocateElement();
            setupPortSize(from);
        }

        data = (ScilabMList) beforeEncode(from, data);

        field++;
        base = data.get(field);
        base = graphicElement.encode(from, null);
        data.set(field, base);

        field++;
        base = data.get(field);
        base = modelElement.encode(from, null);
        data.set(field, base);

        field++;
        base = new ScilabString(from.getInterfaceFunctionName());
        data.set(field, base);

        field++;
        base = data.get(field);
        ((ScilabList) base).add(new ScilabString(from.getId()));

        /*
         * Encoding the InputPorts and OutputPorts using their own elements
         */
        final InputPortElement inElement = new InputPortElement(from.getID());
        final OutputPortElement outElement = new OutputPortElement(from.getID());
        final int numberOfPorts = from.getChildCount();

        // Assume the children are sorted by type
        for (int i = 0; i < numberOfPorts; i++) {
            final Object instance = from.getChildAt(i);

            if (instance instanceof InputPort) {
                inElement.encode((InputPort) instance, data);
            } else if (instance instanceof OutputPort) {
                outElement.encode((OutputPort) instance, data);
            }
        }

        /*
         * Post-process for element shared fields
         */
        final int MODEL_INDEX = BlockElement.DATA_FIELD_NAMES.indexOf("model");
        data.set(MODEL_INDEX, inElement.afterEncode(data.get(MODEL_INDEX)));
        data.set(MODEL_INDEX, outElement.afterEncode(data.get(MODEL_INDEX)));

        data = (ScilabMList) afterEncode(from, data);

        return data;
    }

    /**
     * Set the port size per type.
     *
     * @param from
     *            the source block
     */
    private void setupPortSize(BasicBlock from) {
        // Getting children size per type.
        int in;
        int out;
        int ein;
        int eout;

        in = BasicBlockInfo.getAllTypedPorts(from, false, InputPort.class).size();
        out = BasicBlockInfo.getAllTypedPorts(from, false, OutputPort.class).size();
        ein = BasicBlockInfo.getAllTypedPorts(from, false, ControlPort.class).size();
        eout = BasicBlockInfo.getAllTypedPorts(from, false, CommandPort.class).size();

        // Setup the graphics and model ports size
        graphicElement.setPortsSize(in, out, ein, eout);
        modelElement.setPortsSize(in, out, ein, eout);
    }

    /**
     * Allocate a new element
     *
     * @return the new element
     */
    private ScilabMList allocateElement() {
        ScilabMList element = new ScilabMList(DATA_FIELD_NAMES.toArray(new String[0]));
        element.add(new ScilabMList()); // graphics
        element.add(new ScilabMList()); // model
        element.add(new ScilabString()); // gui
        element.add(new ScilabList()); // doc
        return element;
    }

    @Override
    public BasicBlock decode(ScilabType element, BasicBlock into) throws ScicosFormatException {
        // TODO Auto-generated method stub
        // AbstractElement decodings won't take ScilabTypes anymore
        return null;
    }
}
// CSON: ClassFanOutComplexity
// CSON: ClassDataAbstractionCoupling
