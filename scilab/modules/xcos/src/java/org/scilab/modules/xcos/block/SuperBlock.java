/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.block;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.block.actions.CodeGenerationAction;
import org.scilab.modules.xcos.block.actions.SuperblockMaskCreateAction;
import org.scilab.modules.xcos.block.actions.SuperblockMaskCustomizeAction;
import org.scilab.modules.xcos.block.actions.SuperblockMaskRemoveAction;
import org.scilab.modules.xcos.block.io.ContextUpdate.IOBlocks;
import org.scilab.modules.xcos.block.io.EventInBlock;
import org.scilab.modules.xcos.block.io.EventOutBlock;
import org.scilab.modules.xcos.block.io.ExplicitInBlock;
import org.scilab.modules.xcos.block.io.ExplicitOutBlock;
import org.scilab.modules.xcos.block.io.ImplicitInBlock;
import org.scilab.modules.xcos.block.io.ImplicitOutBlock;
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.swing.GraphComponent;
import org.scilab.modules.xcos.io.XcosCodec;
import org.scilab.modules.xcos.io.scicos.DiagramElement;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.w3c.dom.Node;

import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxEvent;
import com.mxgraph.util.mxEventObject;

/**
 * A SuperBlock contains an entire diagram on it. Thus it can be easily
 * customized by the user.
 * 
 * A SuperBlock can be created from any part of the diagram y selecting blocks
 * and applying the
 * {@link org.scilab.modules.xcos.block.actions.RegionToSuperblockAction}.
 * 
 * It can also appear to users as a normal block by applying a mask on it. In
 * this case the creator can use any SuperBlock context defined variable on a
 * prompt to the user.
 * 
 * @see SuperBlockDiagram
 * @see SuperblockMaskCreateAction
 * @see SuperblockMaskCustomizeAction
 * @see SuperblockMaskRemoveAction
 */
// CSOFF: ClassDataAbstractionCoupling
// CSOFF: ClassFanOutComplexity
public final class SuperBlock extends BasicBlock {
    /**
     * The interfunction name (linked to Xcos-core)
     */
    public static final String INTERFUNCTION_NAME = "SUPER_f";

    /**
     * The simulation name (linked to Xcos-core)
     */
    private static final String SIMULATION_NAME = "super";
    /**
     * The simulation name on a masked status (linked to Xcos-core)
     */
    private static final String MASKED_SIMULATION_NAME = "csuper";
    /**
     * The interfunction name on a masked status (linked to Xcos-core)
     */
    private static final String MASKED_INTERFUNCTION_NAME = "DSUPER";

    private SuperBlockDiagram child;

    /**
     * Constructor
     */
    public SuperBlock() {
        super();
    }

    /**
     * @param label
     *            block label
     */
    protected SuperBlock(String label) {
        this();
        setValue(label);
    }

    /**
     * @param masked
     *            masked super block
     */
    protected SuperBlock(boolean masked) {
        this();
        if (masked) {
            mask();
        }
    }

    /**
     * @param label
     *            block label
     * @param masked
     *            masked super block
     */
    protected SuperBlock(String label, boolean masked) {
        this(label);
        if (masked) {
            mask();
        }
    }

    /**
     * Initialize the block with the default values
     */
    @Override
    protected void setDefaultValues() {
        super.setDefaultValues();
        setInterfaceFunctionName(INTERFUNCTION_NAME);
        setSimulationFunctionName(SIMULATION_NAME);
        setRealParameters(new ScilabMList());
        setIntegerParameters(new ScilabDouble());
        setObjectsParameters(new ScilabList());
        setExprs(new ScilabDouble());
        setBlockType("h");
        setNbZerosCrossing(new ScilabDouble(0));
        setNmode(new ScilabDouble(0));
    }

    /**
     * openBlockSettings this method is called when a double click occurred on a
     * super block
     * 
     * @param context
     *            parent diagram context
     * @see BasicBlock#openBlockSettings(String[])
     */
    @Override
    public void openBlockSettings(String[] context) {

        // prevent to open twice
        if (isLocked()) {
            return;
        }

        /*
         * Do nothing when something happen on the Palette
         */
        if (getParentDiagram() instanceof PaletteDiagram) {
            return;
        }

        /*
         * Specific case when we want to generate code.
         */
        if (getChild() == null
                && getSimulationFunctionType().compareTo(
                        SimulationFunctionType.DEFAULT) != 0) {
            return;
        }

        /*
         * When the block is masked it perform actions like any other blocks.
         */
        if (isMasked()) {
            super.openBlockSettings(context);
            return;
        }

        try {
            // Lock the block because we are really performing actions
            setLocked(true);

            /*
             * Compatibility with older diagrams.
             * 
             * Before Scilab 5.2.2, saved diagrams don't contains XML children
             * but use a pseudo scs_m structure instead.
             * 
             * In this case child was null and we need to reconstruct child
             * diagram from scs_m.
             */
            if (getChild() == null
                    || getChild().getChildVertices(
                            getChild().getDefaultParent()).length == 0) {
                child = null;
                createChildDiagram();
            } else {
                // reassociate (useful on clone and load operation)
                getChild().setContainer(this);
                getChild().setComponent(new GraphComponent(getChild()));

                getChild().initComponent();
                getChild().installStylesheet();

                getChild().installListeners();
                getChild().installSuperBlockListeners();
            }

            /*
             * Construct the view or set it visible.
             */
            if (XcosTab.get(getChild()) == null) {
                XcosTab.restore(getChild());
            } else {
                XcosTab.get(getChild()).setCurrent();
            }

            updateAllBlocksColor();
            getChild().setModifiedNonRecursively(false);

            getChild().fireEvent(new mxEventObject(mxEvent.ROOT));
            getChild().getView().invalidate();

            /*
             * Update the cells from the context values.
             */
            getChild().updateCellsContext();
        } finally {
            setLocked(false);
        }
    }

    /**
     * Action to be performed when the diagram is closed or whenever we have to
     * commit a child modification.
     * 
     * This method does not handle tab closing operation.
     */
    public void syncParameters() {
        /*
         * Do not ask the user, the diagram is saved and closed.
         * 
         * By this way we are sure that the main scs_m structure is always
         * valid.
         */
        if (getChild().isModified()) {
            setRealParameters(new DiagramElement().encode(getChild()));
            getChild().setModified(true);
            getChild().setModifiedNonRecursively(false);
        }
    }

    /**
     * @param graph
     *            parent diagram
     */
    @Override
    public void openContextMenu(ScilabGraph graph) {
        ContextMenu menu = null;

        if (getParentDiagram() instanceof PaletteDiagram) {
            menu = createPaletteContextMenu(graph);
        } else {
            menu = createContextMenu(graph);
            menu.getAsSimpleContextMenu().addSeparator();
            menu.add(CodeGenerationAction.createMenu(graph));

            Menu maskMenu = ScilabMenu.createMenu();
            maskMenu.setText(XcosMessages.SUPERBLOCK_MASK);

            if (isMasked()) {
                maskMenu.add(SuperblockMaskRemoveAction.createMenu(graph));
                menu.add(maskMenu);
            } else {
                maskMenu.add(SuperblockMaskCreateAction.createMenu(graph));
            }
            maskMenu.add(SuperblockMaskCustomizeAction.createMenu(graph));
            menu.add(maskMenu);

        }
        menu.setVisible(true);
    }

    /**
     * @return status
     */
    public boolean createChildDiagram() {
        return createChildDiagram(false);
    }

    /**
     * @param generatedUID
     *            does we need to generated a new unique ID
     * @return status
     */
    public boolean createChildDiagram(boolean generatedUID) {
        if (child == null) {
            child = new SuperBlockDiagram(this);
            child.installListeners();

            final DiagramElement element = new DiagramElement();
            if (!element.canDecode(getRealParameters())) {
                return false;
            }

            try {
                element.decode(getRealParameters(), child, false);
            } catch (ScicosFormatException e) {
                LogFactory.getLog(SuperBlock.class).error(e);
                return false;
            }

            child.installSuperBlockListeners();
            updateAllBlocksColor();
            // only for loading and generate sub block UID
            if (generatedUID) {
                child.generateUID();
            }

            final XcosDiagram parent = Xcos.findParent(this);
            if (parent != null) {
                Xcos.getInstance().addDiagram(parent.getSavedFile(), child);
            }
        } else {
            return false;
        }

        return true;
    }

    /**
     * @return diagram
     */
    public SuperBlockDiagram getChild() {
        return child;
    }

    /**
     * @param child
     *            update diagram
     */
    public void setChild(SuperBlockDiagram child) {
        this.child = child;
    }

    /**
     * @param <T>
     *            The type to work on
     * @param klass
     *            the class instance to work on
     * @return list of typed block
     */
    @SuppressWarnings("unchecked")
    protected <T extends BasicBlock> List<T> getAllTypedBlock(Class<T> klass) {
        List<T> list = new ArrayList<T>();
        if (child == null) {
            return list;
        }

        int blockCount = child.getModel().getChildCount(
                child.getDefaultParent());

        for (int i = 0; i < blockCount; i++) {
            Object cell = child.getModel().getChildAt(child.getDefaultParent(),
                    i);
            if (klass.isInstance(cell)) {
                // According to the test we are sure that the cell is an
                // instance of T. Thus we can safely cast it.
                list.add((T) cell);
            }
        }
        return list;
    }

    /**
     * @param <T>
     *            The type to work on
     * @param klasses
     *            the class instance list to work on
     * @return list of typed block
     */
    protected <T extends BasicBlock> List<T> getAllTypedBlock(Class<T>[] klasses) {
        final List<T> list = new ArrayList<T>();
        for (Class<T> klass : klasses) {
            list.addAll(getAllTypedBlock(klass));
        }
        return list;
    }

    /**
     * @param blocks
     *            in/output blocks
     * @return greater block value
     */
    protected int getBlocksConsecutiveUniqueValueCount(
            List<? extends BasicBlock> blocks) {
        if (blocks == null) {
            return 0;
        }

        int count = blocks.size();
        int[] array = new int[blocks.size()];

        // initialize
        for (int i = 0; i < array.length; i++) {
            array[i] = 0;
        }

        // populate
        for (int i = 0; i < array.length; i++) {
            final ScilabDouble data = (ScilabDouble) ((BasicBlock) blocks
                    .get(i)).getIntegerParameters();

            if (data.getWidth() < 1 || data.getHeight() < 1) {
                continue;
            }
            final int index = (int) data.getRealPart()[0][0];

            if (index <= array.length) {
                array[index - 1] = 1;
            }
        }

        // parse
        for (int i = 0; i < array.length; i++) {
            if (array[i] == 0) {
                count = i;
                break;
            }
        }

        return count;
    }

    /**
     * force blocks update
     */
    @SuppressWarnings("unchecked")
    public void updateAllBlocksColor() {
        updateBlocksColor(getAllTypedBlock(new Class[] { ExplicitInBlock.class,
                ImplicitInBlock.class }));
        updateBlocksColor(getAllTypedBlock(new Class[] {
                ExplicitOutBlock.class, ImplicitOutBlock.class }));

        updateBlocksColor(getAllTypedBlock(EventInBlock.class));
        updateBlocksColor(getAllTypedBlock(EventOutBlock.class));

        if (child != null) {
            child.getAsComponent().validate();
            child.getView().validate();
        }
    }

    /**
     * @param blocks
     *            block list
     */
    private void updateBlocksColor(List<? extends BasicBlock> blocks) {
        if (blocks == null || blocks.size() == 0 || child == null) {
            return;
        }

        try {
            child.getModel().beginUpdate();

            final int countUnique = getBlocksConsecutiveUniqueValueCount(blocks);
            boolean[] isDone = new boolean[countUnique];

            // Initialize
            Arrays.fill(isDone, false);

            for (int i = 0; i < blocks.size(); i++) {
                final ScilabDouble data = (ScilabDouble) ((BasicBlock) blocks
                        .get(i)).getIntegerParameters();

                if (data.getWidth() < 1 || data.getHeight() < 1) {
                    continue;
                }
                final int index = (int) data.getRealPart()[0][0];

                if (index > countUnique || isDone[index - 1]) {
                    child.getAsComponent().setCellWarning(blocks.get(i),
                            XcosMessages.WRONG_PORT_NUMBER);
                    child.getView().invalidate(blocks.get(i));
                } else {
                    isDone[index - 1] = true;
                    child.getAsComponent().setCellWarning(blocks.get(i), null);
                    child.getView().invalidate(blocks.get(i));
                }
            }

        } finally {
            child.getModel().endUpdate();
        }
    }

    /**
     * update super block ports in parent diagram
     */
    public void updateExportedPort() {
        if (child == null) {
            return;
        }
        if (getParentDiagram() == null) {
            setParentDiagram(Xcos.findParent(this));
        }

        final Map<IOBlocks, List<mxICell>> blocksMap = IOBlocks
                .getAllBlocks(this);
        final Map<IOBlocks, List<mxICell>> portsMap = IOBlocks
                .getAllPorts(this);
        for (IOBlocks block : IOBlocks.values()) {
            final int blockCount = blocksMap.get(block).size();
            int portCount = portsMap.get(block).size();

            // add ports if required
            while (blockCount > portCount) {
                try {
                    BasicPort port;
                    port = block.getReferencedPortClass().newInstance();
                    addPort(port);
                } catch (InstantiationException e) {
                    LogFactory.getLog(SuperBlock.class).error(e);
                } catch (IllegalAccessException e) {
                    LogFactory.getLog(SuperBlock.class).error(e);
                }
                portCount++;
            }

            // remove ports if required
            while (portCount > blockCount) {
                removePort((BasicPort) portsMap.get(block).get(portCount - 1));
                portCount--;
            }
        }
        getParentDiagram().fireEvent(
                new mxEventObject(XcosEvent.SUPER_BLOCK_UPDATED,
                        XcosConstants.EVENT_BLOCK_UPDATED, this));
    }

    /**
     * Mask the SuperBlock
     */
    public void mask() {
        setInterfaceFunctionName(MASKED_INTERFUNCTION_NAME);
        setSimulationFunctionName(MASKED_SIMULATION_NAME);
        setIntegerParameters(new ScilabDouble(1));
    }

    /**
     * Unmask the SuperBlock
     */
    public void unmask() {
        setInterfaceFunctionName(INTERFUNCTION_NAME);
        setSimulationFunctionName(SIMULATION_NAME);
        setIntegerParameters(new ScilabDouble());
    }

    /**
     * @return True is the SuperBlock is masked, false otherwise
     */
    public boolean isMasked() {
        return getInterfaceFunctionName().compareTo(INTERFUNCTION_NAME) != 0;
    }

    /**
     * Customize the parent diagram on name change
     * 
     * @param value
     *            the new name
     * @see com.mxgraph.model.mxCell#setValue(java.lang.Object)
     */
    @Override
    public void setValue(Object value) {
        super.setValue(value);

        if (value == null) {
            return;
        }

        if (getChild() != null) {
            getChild().setTitle(FileUtils.toValidCIdentifier(value.toString()));
            setRealParameters(new DiagramElement().encode(getChild()));
        }
    }

    /**
     * Clone the child safely.
     * 
     * @return a new clone instance
     * @throws CloneNotSupportedException
     *             never
     * @see org.scilab.modules.xcos.block.BasicBlock#clone()
     */
    @Override
    public Object clone() throws CloneNotSupportedException {
        SuperBlock clone = (SuperBlock) super.clone();

        // clone the diagram
        if (child != null) {
            clone.child = (SuperBlockDiagram) child.clone();
            clone.child.setContainer(clone);
        }

        return clone;

    }

    /*
     * Serializable custom implementation need to handle any copy / DnD case.
     */

    /**
     * Encode the block as xml
     * 
     * @param out
     *            the output stream
     * @throws IOException
     *             on error
     */
    private void writeObject(java.io.ObjectOutputStream out) throws IOException {
        out.writeObject(new XcosCodec().encode(this));
    }

    /**
     * Decode the block as xml
     * 
     * @param in
     *            the input stream
     * @throws IOException
     *             on error
     * @throws ClassNotFoundException
     *             on error
     */
    private void readObject(java.io.ObjectInputStream in) throws IOException,
            ClassNotFoundException {
        new XcosCodec().decode((Node) in.readObject(), this);

        /*
         * Specific post serialization things
         */
        if (this.child == null) {
            this.child = new SuperBlockDiagram(this);
            this.child.installListeners();
        } else {
            this.child.setContainer(this);
        }
        this.child.installSuperBlockListeners();
    }
}
// CSON: ClassDataAbstractionCoupling
// CSON: ClassFanOutComplexity
