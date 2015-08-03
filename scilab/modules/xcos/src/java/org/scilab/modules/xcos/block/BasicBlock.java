/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.block;

import java.awt.Cursor;
import java.awt.MouseInfo;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.Deque;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.logging.Level;
import java.util.logging.Logger;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.graph.ScilabGraphUniqueObject;
import org.scilab.modules.graph.actions.CopyAction;
import org.scilab.modules.graph.actions.CutAction;
import org.scilab.modules.graph.actions.DeleteAction;
import org.scilab.modules.graph.actions.base.DefaultAction;
import org.scilab.modules.graph.utils.ScilabGraphConstants;
import org.scilab.modules.graph.utils.StyleMap;
import org.scilab.modules.gui.bridge.contextmenu.SwingScilabContextMenu;
import org.scilab.modules.gui.contextmenu.ContextMenu;
import org.scilab.modules.gui.contextmenu.ScilabContextMenu;
import org.scilab.modules.gui.events.callback.CommonCallBack;
import org.scilab.modules.gui.menu.Menu;
import org.scilab.modules.gui.menu.ScilabMenu;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.gui.menuitem.ScilabMenuItem;
import org.scilab.modules.types.ScilabDouble;
import org.scilab.modules.types.ScilabList;
import org.scilab.modules.types.ScilabTList;
import org.scilab.modules.types.ScilabMList;
import org.scilab.modules.types.ScilabString;
import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.Controller;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.UpdateStatus;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfString;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.XcosView;
import org.scilab.modules.xcos.actions.EditFormatAction;
import org.scilab.modules.xcos.actions.ShowHideShadowAction;
import org.scilab.modules.xcos.block.actions.BlockDocumentationAction;
import org.scilab.modules.xcos.block.actions.BlockParametersAction;
import org.scilab.modules.xcos.block.actions.BorderColorAction;
import org.scilab.modules.xcos.block.actions.FilledColorAction;
import org.scilab.modules.xcos.block.actions.FlipAction;
import org.scilab.modules.xcos.block.actions.MirrorAction;
import org.scilab.modules.xcos.block.actions.RegionToSuperblockAction;
import org.scilab.modules.xcos.block.actions.RotateAction;
import org.scilab.modules.xcos.block.actions.ViewDetailsAction;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockAction;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionBottom;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionCenter;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionLeft;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionMiddle;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionRight;
import org.scilab.modules.xcos.block.actions.alignement.AlignBlockActionTop;
import org.scilab.modules.xcos.graph.PaletteDiagram;
import org.scilab.modules.xcos.graph.SuperBlockDiagram;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.BasicBlockInfo;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;
import org.scilab.modules.xcos.utils.BlockPositioning;
import org.scilab.modules.xcos.utils.XcosConstants;
import org.scilab.modules.xcos.utils.XcosEvent;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import com.mxgraph.model.mxIGraphModel;
import com.mxgraph.util.mxConstants;
import com.mxgraph.util.mxEventObject;
import com.mxgraph.util.mxUtils;

/**
 * A block on the diagram
 */
// CSOFF: ClassDataAbstractionCoupling
// CSOFF: ClassFanOutComplexity
@SuppressWarnings(value = { "serial" })
public class BasicBlock extends ScilabGraphUniqueObject implements Serializable {
    /**
     * Sorted kind of input, useful to sort them by kind
     */
    private static final Class<?>[] sortedChildrenClass = {InputPort.class, OutputPort.class, ControlPort.class, CommandPort.class, Object.class};

    /**
     * The block id in the model
     */
    private long id = 0;

    /*
     * Property names
     */

    /**
     * Property name of interfaceFunctionName
     */
    public static final String INTERFACE_FUNCTION_NAME = "interfaceFunctionName";
    /**
     * Property name of simulationFunctionName
     */
    public static final String SIMULATION_FUNCTION_NAME = "simulationFunctionName";
    /**
     * Property name of simulationFunctionType
     */
    public static final String SIMULATION_FUNCTION_TYPE = "simulationFunctionType";
    /**
     * Property name of realParameters
     */
    public static final String REAL_PARAMETERS = "realParameters";
    /**
     * Property name of integerParameters
     */
    public static final String INTEGER_PARAMETERS = "integerParameters";
    /**
     * Property name of objectsParameters
     */
    public static final String OBJECTS_PARAMETERS = "objectsParameters";
    /**
     * Property name of dependsOnU
     */
    public static final String DEPENDS_ON_U = "dependsOnU";
    /**
     * Property name of dependsOnT
     */
    public static final String DEPENDS_ON_T = "dependsOnT";
    /**
     * Property name of blockType
     */
    public static final String BLOCK_TYPE = "blockType";
    /**
     * Property name of ordering
     */
    public static final String ORDERING = "ordering";
    /**
     * Property name of exprs
     */
    public static final String EXPRS = "exprs";
    /**
     * Property name of nbZerosCrossing
     */
    public static final String NB_ZEROS_CROSSING = "nbZerosCrossing";
    /**
     * Property name of nmode
     */
    public static final String NMODE = "nmode";
    /**
     * Property name of state
     */
    public static final String STATE = "state";
    /**
     * Property name of dState
     */
    public static final String D_STATE = "dState";
    /**
     * Property name of oDState
     */
    public static final String O_D_STATE = "oDState";
    /**
     * Property name of equations
     */
    public static final String EQUATIONS = "equations";

    /*
     * Default values
     */

    /**
     * Default interface function name
     */
    public static final String DEFAULT_INTERFACE_FUNCTION = "xcos_block";
    /**
     * Default simulation function name
     */
    public static final String DEFAULT_SIMULATION_FUNCTION = "xcos_simulate";

    /*
     * Local constants
     */

    private static final String PARENT_DIAGRAM_WAS_NULL = "Parent diagram was null";
    private static final double DEFAULT_POSITION_X = 10.0;
    private static final double DEFAULT_POSITION_Y = 10.0;
    private static final double DEFAULT_WIDTH = 40.0;
    private static final double DEFAULT_HEIGHT = 40.0;

    private static final PropertyChangeListener STYLE_UPDATER = new UpdateStyleFromInterfunction();
    private static final Logger LOG = Logger.getLogger(BasicBlock.class.getName());

    /**
     * @return the id
     *
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
     * Sort the children list in place.
     *
     * The sort put inputs then outputs the control then command ports. The
     * local port order is preserved.The sort is performed in place and do not
     * emit any event.
     *
     *
     * @param children
     *            the children to sort
     */
    public static final void sort(List<?> children) {
        final List<Object> reference = new ArrayList<Object>(children);

        Collections.sort(children, new Comparator<Object>() {
            @Override
            public int compare(Object o1, Object o2) {
                // diff is the major sorting by kind
                int diff = compareByChildClass(o1, o2);

                if (o1 instanceof BasicPort && o2 instanceof BasicPort) {
                    // first sort with the port list index
                    final int diffIndexOf = Integer.signum(reference.indexOf(o1) - reference.indexOf(o2));
                    // then sort with the ordering value
                    final int diffOrdering = Integer.signum(((BasicPort) o1).getOrdering() - ((BasicPort) o2).getOrdering());
                    // then sort with the port position value
                    final mxGeometry o1Geom = ((BasicPort) o1).getGeometry();
                    final mxGeometry o2Geom = ((BasicPort) o2).getGeometry();
                    final int diffPosition = Integer.signum((int) (o2Geom.getX() - o1Geom.getX() - o2Geom.getY() + o1Geom.getY()));

                    // voting is performed with these equivalent 3 selector
                    diff = diff + diffIndexOf + diffOrdering + diffPosition;
                }

                return diff;
            }
        });
    }

    /**
     * Internal method to get a base index to compare with depending on the cell
     * type.
     *
     * @param cell
     *            the cell
     * @return the base index
     */
    private static final int compareByChildClass(final Object o1, final Object o2) {
        int o1Index = 0;
        int o2Index = 0;

        for (int i = 0; i < sortedChildrenClass.length; i++) {
            final Class<?> klass = sortedChildrenClass[i];

            if (klass.isInstance(o1)) {
                o1Index = i;
                break;
            }
        }
        for (int i = 0; i < sortedChildrenClass.length; i++) {
            final Class<?> klass = sortedChildrenClass[i];

            if (klass.isInstance(o2)) {
                o2Index = i;
                break;
            }
        }

        final int base = o1Index - o2Index;
        return base * (Integer.MAX_VALUE / sortedChildrenClass.length);
    }

    /**
     * Manage events for block parameters.
     *
     * The property name is the field name, is one of:
     * <ol>
     * <li>"interfaceFunctionName"
     * <li>"simulationFunctionName"
     * <li>"simulationFunctionType"
     * <li>"exprs"
     * <li>"realParameters"
     * <li>"integerParameters"
     * <li>"objectsParameters"
     * <li>"nbZerosCrossing"
     * <li>"nmode"
     * <li>"state"
     * <li>"dState"
     * <li>"oDState"
     * <li>"equations"
     * <li>"dependsOnU"
     * <li>"dependsOnT"
     * <li>"blockType"
     * <li>"ordering"
     * </ol>
     *
     * you can easily access to then by using property name constants.
     */
    private PropertyChangeSupport parametersPCS = new PropertyChangeSupport(this);

    private transient XcosDiagram parentDiagram;
    // Child diagram (in case of Superblock, stored in RPAR
    private ScilabMList childDiagram;

    private boolean isMirrored;

    // TODO : Must make this types evolve, but for now keep a strong link to
    // Scilab
    // !! WARNING !!
    // exprs = [] ; rpar = [] ; ipar = [] ; opar = list()

    private int ordering;
    private boolean locked;

    /**
     * Represent a simulation function type compatible with Scilab/Scicos
     * function type descriptors.
     */
    public static enum SimulationFunctionType {
        /** event select; reduced at compilation */
        ESELECT(-2.0),
        /** if then else; reduced at compilation */
        IFTHENELSE(-1.0),
        /** first common block */
        DEFAULT(0.0),
        /** first native block */
        TYPE_1(1.0),
        /** second native block */
        TYPE_2(2.0),
        /** third native block */
        TYPE_3(3.0),
        /** forth native block */
        C_OR_FORTRAN(4.0),
        /** Scilab blocks */
        SCILAB(5.0),
        /** Debug blocks */
        DEBUG(99),
        /** dynamic {@link #TYPE_1} Fortran blocks (fortran_block.sci) */
        DYNAMIC_FORTRAN_1(1001.0),
        /** dynamic {@link #TYPE_1} C blocks (c_block.sci) */
        DYNAMIC_C_1(2001.0),
        /** Explicit dynamic {@link #TYPE_4} blocks (CBLOCK.sci) */
        DYNAMIC_EXPLICIT_4(2004.0),
        /** Implicit {@link #TYPE_1} Fortran blocks (DIFF_f.sci) */
        OLDBLOCKS(10001.0),
        /** Implicit {@link #C_OR_FORTRAN} blocks */
        IMPLICIT_C_OR_FORTRAN(10004.0),
        /** Implicit dynamic {@link #TYPE_4} blocks (CBLOCK.sci) */
        DYNAMIC_IMPLICIT_4(12004.0),
        /** Modelica {@link #C_OR_FORTRAN} blocks */
        MODELICA(30004.0),
        /** Magic types */
        UNKNOWN(5.0);

        private double value;

        /**
         * Default constructor
         *
         * @param scilabValue
         *            Scilab/Scicos function type descriptor
         */
        private SimulationFunctionType(double scilabValue) {
            value = scilabValue;
        }

        /**
         * Get the Java descriptor from the Scilab descriptor.
         *
         * @param scilabValue
         *            Scilab/Scicos function type descriptor
         * @return The corresponding java descriptor
         */
        public static SimulationFunctionType convertScilabValue(int scilabValue) {
            for (SimulationFunctionType iter : SimulationFunctionType.values()) {
                if (iter.getAsDouble() == scilabValue) {
                    return iter;
                }
            }
            return UNKNOWN;
        }

        /**
         * Get the Scilab Descriptor from the Java Descriptor
         *
         * @return The corresponding Scilab/Scicos descriptor
         */
        public double getAsDouble() {
            return value;
        }
    };

    /**
     * Update the source block when the interfunction change.
     */
    private static final class UpdateStyleFromInterfunction implements PropertyChangeListener, Serializable {

        /**
         * Default constructor.
         */
        public UpdateStyleFromInterfunction() {
        }

        /**
         * Update the label on interfunction change.
         *
         * @param evt
         *            the property change event.
         * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
         */
        @Override
        public void propertyChange(PropertyChangeEvent evt) {
            final BasicBlock source = (BasicBlock) evt.getSource();

            /*
             * Put the interfunction at the start of the style map to preserve
             * style modification.
             *
             * oldStyle="SUPER_f;fillColor=red" newStyle="DSUPER;fillColor=red"
             *
             * and not newStyle="fillColor=red;DSUPER"
             */
            final StyleMap style = new StyleMap((String) evt.getNewValue());
            style.putAll(source.getStyle());
            style.remove(evt.getOldValue());

            source.setStyle(style.toString());
        }

    }

    /**
     * Trace the parameters change on the {@link Logger}.
     *
     * This listener is only installed if the trace is enable.
     */
    private static final class TraceParametersListener implements PropertyChangeListener, Serializable {
        private static TraceParametersListener instance;

        /**
         * Default constructor.
         */
        private TraceParametersListener() {
            super();
        }

        /**
         * @return the instance
         */
        public static TraceParametersListener getInstance() {
            if (instance == null) {
                instance = new TraceParametersListener();
            }
            return instance;
        }

        /**
         * Trace.
         *
         * @param evt
         *            the event
         * @see java.beans.PropertyChangeListener#propertyChange(java.beans.PropertyChangeEvent)
         */
        @Override
        public void propertyChange(PropertyChangeEvent evt) {
            if (LOG.isLoggable(Level.FINEST)) {
                LOG.finest(evt.getPropertyName() + ": " + evt.getOldValue() + ", " + evt.getNewValue());
            }
        }
    }

    /**
     * Default constructor.
     */
    public BasicBlock() {
        super();
        // Create a block in the Scilab console and retrieve its model id
        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        if (handler == null) {
            return;
        }

        try {
            XcosView blockView = new XcosView();
            JavaController.register_view("blockLog", blockView);
            ScilabInterpreterManagement.synchronousScilabExec("scicos_block();");
            setID(blockView.getId(Kind.BLOCK));
            JavaController.unregister_view(blockView);
        } catch (final InterpreterException e) {
            e.printStackTrace();
        } finally {
            handler.release();
        }
        // Increment the block ID so it is not deleted on the next Constructor call
        final Controller controller = new Controller();
        controller.referenceObject(getID());

        setDefaultValues();
        setVisible(true);
        setVertex(true);

        if (getStyle().isEmpty() && !getInterfaceFunctionName().isEmpty()) {
            setStyle(getInterfaceFunctionName());
        }

        parametersPCS.addPropertyChangeListener(INTERFACE_FUNCTION_NAME, STYLE_UPDATER);

        /*
         * Trace block parameters change if applicable.
         */
        if (LOG.isLoggable(Level.FINEST)) {
            parametersPCS.addPropertyChangeListener(TraceParametersListener.getInstance());
        }
    }

    /**
     * Constructor by id.
     */
    public BasicBlock(long id) {
        super();
        setID(id);
        setDefaultValues();
        setVisible(true);
        setVertex(true);

        if (getStyle().isEmpty() && !getInterfaceFunctionName().isEmpty()) {
            setStyle(getInterfaceFunctionName());
        }

        parametersPCS.addPropertyChangeListener(INTERFACE_FUNCTION_NAME, STYLE_UPDATER);

        /*
         * Trace block parameters change if applicable.
         */
        if (LOG.isLoggable(Level.FINEST)) {
            parametersPCS.addPropertyChangeListener(TraceParametersListener.getInstance());
        }
    }

    /**
     * @param label
     *            block label
     */
    protected BasicBlock(String label) {
        this();
        setDefaultValues();
    }

    /**
     * @param label
     *            block label
     * @param style
     *            initial style
     */
    protected BasicBlock(String label, String style) {
        this(label);
        setStyle(style);
    }

    /**
     * Initialize the block with the default values
     */
    protected void setDefaultValues() {
        setVisible(true);
        setVertex(true);
        setConnectable(false);
        setGeometry(new mxGeometry(DEFAULT_POSITION_X, DEFAULT_POSITION_Y, DEFAULT_WIDTH, DEFAULT_HEIGHT));
        setValue("");
        setStyle("");
    }

    /**
     * @return parent diagram
     */
    public XcosDiagram getParentDiagram() {
        return parentDiagram;
    }

    /**
     * @param parentDiagram
     *            parent diagram
     */
    public void setParentDiagram(XcosDiagram parentDiagram) {
        final Controller controller = new Controller();
        controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.PARENT_DIAGRAM, parentDiagram.getID());
        this.parentDiagram = parentDiagram;
    }

    /**
     * @return interface function name
     */
    public String getInterfaceFunctionName() {
        final Controller controller = new Controller();
        String[] interfaceFunctionName = {""};
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, interfaceFunctionName);
        return interfaceFunctionName[0];
    }

    /**
     * @param interfaceFunctionName
     *            interface function name
     */
    public void setInterfaceFunctionName(String interfaceFunctionName) {
        final Controller controller = new Controller();
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.INTERFACE_FUNCTION, interfaceFunctionName) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(INTERFACE_FUNCTION_NAME, null, interfaceFunctionName); // FIXME: get model's 'interface function' for fallback (oldValue)?
        }
    }

    /**
     * @return simulation function name
     */
    public String getSimulationFunctionName() {
        final Controller controller = new Controller();
        String[] simulationFunctionName = {""};
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_FUNCTION_NAME, simulationFunctionName);
        return simulationFunctionName[0];
    }

    /**
     * @param simulationFunctionName
     *            simulation function name
     */
    public void setSimulationFunctionName(String simulationFunctionName) {
        final Controller controller = new Controller();
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_FUNCTION_NAME, simulationFunctionName) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(SIMULATION_FUNCTION_NAME, null, simulationFunctionName); // FIXME: get model's 'simulation function' for fallback (oldValue)?
        }
    }

    /**
     * @return simulation function type
     */
    public SimulationFunctionType getSimulationFunctionType() {
        final Controller controller = new Controller();
        int[] simulationFunctionType = {0};
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_FUNCTION_API, simulationFunctionType);
        return SimulationFunctionType.convertScilabValue(simulationFunctionType[0]);
    }

    /**
     * @param scilabValue
     *            simulation function type
     */
    public void setSimulationFunctionType(int scilabValue) {
        final Controller controller = new Controller();
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_FUNCTION_API, scilabValue) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(SIMULATION_FUNCTION_TYPE, null, scilabValue); // FIXME: get model's 'simulation type' for fallback (oldValue)?
        }
    }

    /**
     * @return real parameter ( rpar )
     */
    public ScilabType getRealParameters() {
        final Controller controller = new Controller();
        VectorOfScicosID children = new VectorOfScicosID();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.CHILDREN, children);

        if (children.isEmpty()) {
            VectorOfDouble Rpar = new VectorOfDouble();
            controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.RPAR, Rpar);
            double rpar[][] = new double[(int) Rpar.size()][1];
            for (int i = 0; i < Rpar.size(); ++i) {
                rpar[i][0] = Rpar.get(i);
            }
            return new ScilabDouble(rpar);
        } else {
            return childDiagram;
        }
    }

    /**
     * @param realParameters
     *            reaL parameter ( rpar )
     */
    public void setRealParameters(ScilabType realParameters) {
        if (realParameters instanceof ScilabDouble) {
            ScilabDouble Rpar = (ScilabDouble) realParameters;
            final Controller controller = new Controller();
            VectorOfDouble rpar = new VectorOfDouble(Rpar.getHeight());
            for (int i = 0;  i < Rpar.getHeight(); ++i) {
                rpar.set(i, Rpar.getRealElement(i, 0));
            }
            if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.RPAR, rpar) == UpdateStatus.SUCCESS) {
                parametersPCS.firePropertyChange(REAL_PARAMETERS, null, realParameters); // FIXME: get model's 'rpar' for fallback (oldValue)?
            }
        } else if (realParameters instanceof ScilabMList) {
            final ScilabType oldValue = this.childDiagram;
            this.childDiagram = (ScilabMList) realParameters;
            parametersPCS.firePropertyChange(REAL_PARAMETERS, oldValue, realParameters);
        }
    }

    /**
     * @return integer parameter ( ipar )
     */
    public ScilabType getIntegerParameters() {
        final Controller controller = new Controller();
        VectorOfDouble Ipar = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.IPAR, Ipar);
        double ipar[][] = new double[(int) Ipar.size()][1];
        for (int i = 0; i < Ipar.size(); ++i) {
            ipar[i][0] = Ipar.get(i);
        }
        return new ScilabDouble(ipar);
    }

    /**
     * @param integerParameters
     *            integer parameter ( ipar )
     */
    public void setIntegerParameters(ScilabType integerParameters) {
        if (integerParameters instanceof ScilabList) {
            // List clears previous value
            final Controller controller = new Controller();
            VectorOfInt ipar = new VectorOfInt();
            controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.IPAR, ipar);
            parametersPCS.firePropertyChange(INTEGER_PARAMETERS, null, integerParameters); // FIXME: get model's 'ipar' for fallback (oldValue)?
        }
        if (integerParameters instanceof ScilabDouble) {
            ScilabDouble Ipar = (ScilabDouble) integerParameters;
            final Controller controller = new Controller();
            VectorOfInt ipar = new VectorOfInt(Ipar.getHeight());
            for (int i = 0;  i < Ipar.getHeight(); ++i) {
                ipar.set(i, (int) Ipar.getRealElement(i, 0));
            }
            if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.IPAR, ipar) == UpdateStatus.SUCCESS) {
                parametersPCS.firePropertyChange(INTEGER_PARAMETERS, null, integerParameters); // FIXME: get model's 'ipar' for fallback (oldValue)?
            }
        }
    }

    /**
     * @return object parameter ( opar )
     */
    public ScilabType getObjectsParameters() {
        final Controller controller = new Controller();
        VectorOfDouble Opar = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.OPAR, Opar);
        // FIXME: find how to call ScilabType opar = vec2var(Opar);
        return null;
    }

    /**
     * @param objectsParameters
     *            object parameter ( opar )
     */
    public void setObjectsParameters(ScilabType objectsParameters) {
        final Controller controller = new Controller();
        VectorOfDouble opar = new VectorOfDouble(); // Stub
        // FIXME: find how to call VectorOfDouble opar = var2vec(objectsParameters);
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.OPAR, opar) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(OBJECTS_PARAMETERS, null, objectsParameters); // FIXME: get model's 'opar' for fallback (oldValue)?
        }
    }

    /**
     * @return ?
     */
    public boolean isDependsOnU() {
        final Controller controller = new Controller();
        VectorOfInt DepUT = new VectorOfInt();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_DEP_UT, DepUT);
        return (DepUT.get(0) == 1 ? true : false);
    }

    /**
     * @param dependsOnU
     *            ?
     */
    public void setDependsOnU(boolean dependsOnU) {
        final Controller controller = new Controller();
        VectorOfInt DepUT = new VectorOfInt();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_DEP_UT, DepUT);
        DepUT.set(0, (dependsOnU ? 1 : 0));
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_DEP_UT, DepUT) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(DEPENDS_ON_U, null, dependsOnU); // FIXME: get model's 'deput' for fallback (oldValue)?
        }
    }

    /**
     * @return ?
     */
    public boolean isDependsOnT() {
        final Controller controller = new Controller();
        VectorOfInt DepUT = new VectorOfInt();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_DEP_UT, DepUT);
        return (DepUT.get(1) == 1 ? true : false);
    }

    /**
     * @param dependsOnT
     *            ?
     */
    public void setDependsOnT(boolean dependsOnT) {
        final Controller controller = new Controller();
        VectorOfInt DepUT = new VectorOfInt();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_DEP_UT, DepUT);
        DepUT.set(1, (dependsOnT ? 1 : 0));
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_DEP_UT, DepUT) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(DEPENDS_ON_U, null, dependsOnT); // FIXME: get model's 'deput' for fallback (oldValue)?
        }
    }

    /**
     * @return block type
     */
    public String getBlockType() {
        final Controller controller = new Controller();
        String[] blocktype = {""};
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_BLOCKTYPE, blocktype);
        return blocktype[0];
    }

    /**
     * @param blockType
     *            block type
     */
    public void setBlockType(String blockType) {
        final Controller controller = new Controller();
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.SIM_BLOCKTYPE, blockType) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(BLOCK_TYPE, null, blockType); // FIXME: get model's 'blocktype' for fallback (oldValue)?
        }
    }

    /**
     * @return order value
     */
    public int getOrdering() {
        return ordering;
    }

    /**
     * @param ordering
     *            order value
     */
    public void setOrdering(int ordering) {
        if (this.ordering != ordering) {

            final int oldValue = this.ordering;
            this.ordering = ordering;
            parametersPCS.firePropertyChange(ORDERING, oldValue, ordering);
        }
    }

    /**
     * @return expression
     */
    public ScilabType getExprs() {
        final Controller controller = new Controller();
        VectorOfDouble Exprs = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.EXPRS, Exprs);
        // FIXME: find how to call ScilabType exprs = vec2var(Exprs);
        return null;
    }

    /**
     * @param exprs
     *            expression
     */
    public void setExprs(ScilabType exprs) {
        if (exprs != null) {
            final Controller controller = new Controller();
            VectorOfDouble Exprs = new VectorOfDouble(); // Stub
            // FIXME: find how to call VectorOfDouble Exprs = var2vec(exprs);
            if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.EXPRS, Exprs) == UpdateStatus.SUCCESS) {
                parametersPCS.firePropertyChange(EXPRS, null, exprs); // FIXME: get model's 'exprs' for fallback (oldValue)?
            }
        }
    }

    /**
     * @return the expression as an object array
     */
    public Object[] getExprsFormat() {
        // evaluate emptiness
        if (getExprs() == null || getExprs().isEmpty() || getExprs().getHeight() == 0 || getExprs().getWidth() == 0) {
            return new String[0];
        }

        List<String[]> stack = getString(null, getExprs());

        int len = 0;
        for (Object[] strings : stack) {
            len += strings.length;
        }

        final Object[] array = new Object[len];
        int start = 0;
        for (Object[] strings : stack) {
            System.arraycopy(strings, 0, array, start, strings.length);
            start += strings.length;
        }

        return array;
    }

    /**
     * Append the data recursively to the stack
     *
     * @param currentStack
     *            the current stack
     * @param data
     *            the data to append
     * @return the stack
     */
    private List<String[]> getString(List<String[]> currentStack, ScilabType data) {
        final List<String[]> stack;

        if (currentStack == null) {
            stack = new LinkedList<String[]>();
        } else {
            stack = currentStack;
        }

        if (data instanceof List) {
            /*
             * Container case (ScilabList, ScilabMList, ScilabTList)
             */

            @SuppressWarnings("unchecked")
            final List<ScilabType> list = (List<ScilabType>) data;

            for (final ScilabType scilabType : list) {
                getString(stack, scilabType);
            }
        } else if (data instanceof ScilabString) {
            /*
             * native case (only ScilabString supported)
             */

            final String[][] scilabData = ((ScilabString) data).getData();
            final int height = data.getHeight();
            final int width = data.getWidth();

            final String[] array = new String[height * width];
            for (int i = 0; i < height; ++i) {
                System.arraycopy(scilabData[i], 0, array, i * width, width);
            }

            stack.add(array);
        }

        return stack;
    }

    /**
     * @return zero crossing value
     */
    public ScilabType getNbZerosCrossing() {
        final Controller controller = new Controller();
        VectorOfInt Nbzcross = new VectorOfInt();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.NZCROSS, Nbzcross);
        double nbzcross[][] = new double[(int) Nbzcross.size()][1];
        for (int i = 0; i < Nbzcross.size(); ++i) {
            nbzcross[i][0] = Nbzcross.get(i);
        }
        return new ScilabDouble(nbzcross);
    }

    /**
     * @param nbZerosCrossing
     *            zero crossing value
     */
    public void setNbZerosCrossing(ScilabType nbZerosCrossing) {
        ScilabDouble NbZerosCrossing = (ScilabDouble) nbZerosCrossing;
        final Controller controller = new Controller();
        VectorOfInt nbzcross = new VectorOfInt(NbZerosCrossing.getHeight());
        for (int i = 0;  i < NbZerosCrossing.getHeight(); ++i) {
            nbzcross.set(i, (int) NbZerosCrossing.getRealElement(i, 0));
        }
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.NZCROSS, nbzcross) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(NB_ZEROS_CROSSING, null, nbZerosCrossing); // FIXME: get model's 'nbzcross' for fallback (oldValue)?
        }
    }

    /**
     * @return nmode
     */
    public ScilabType getNmode() {
        final Controller controller = new Controller();
        VectorOfInt Nmode = new VectorOfInt();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.NMODE, Nmode);
        double nmode[][] = new double[(int) Nmode.size()][1];
        for (int i = 0; i < Nmode.size(); ++i) {
            nmode[i][0] = Nmode.get(i);
        }
        return new ScilabDouble(nmode);
    }

    /**
     * @param nmode
     *            nmode
     */
    public void setNmode(ScilabType nmode) {
        ScilabDouble Nmode = (ScilabDouble) nmode;
        final Controller controller = new Controller();
        VectorOfInt nbmode = new VectorOfInt(Nmode.getHeight());
        for (int i = 0;  i < Nmode.getHeight(); ++i) {
            nbmode.set(i, (int) Nmode.getRealElement(i, 0));
        }
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.NMODE, nbmode) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(NMODE, null, nmode); // FIXME: get model's 'nmode' for fallback (oldValue)?
        }
    }

    /**
     * @return current state
     */
    public ScilabType getState() {
        final Controller controller = new Controller();
        VectorOfDouble State = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.STATE, State);
        double state[][] = new double[(int) State.size()][1];
        for (int i = 0; i < State.size(); ++i) {
            state[i][0] = State.get(i);
        }
        return new ScilabDouble(state);
    }

    /**
     * @param state
     *            new state
     */
    public void setState(ScilabType state) {
        ScilabDouble State = (ScilabDouble) state;
        final Controller controller = new Controller();
        VectorOfDouble stateVector = new VectorOfDouble(State.getHeight());
        for (int i = 0;  i < State.getHeight(); ++i) {
            stateVector.set(i, State.getRealElement(i, 0));
        }
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.STATE, stateVector) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(STATE, null, state); // FIXME: get model's 'state' for fallback (oldValue)?
        }
    }

    /**
     * @return current dstate
     */
    public ScilabType getDState() {
        final Controller controller = new Controller();
        VectorOfDouble Dstate = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.DSTATE, Dstate);
        double dstate[][] = new double[(int) Dstate.size()][1];
        for (int i = 0; i < Dstate.size(); ++i) {
            dstate[i][0] = Dstate.get(i);
        }
        return new ScilabDouble(dstate);
    }

    /**
     * @param dState
     *            new dstate
     */
    public void setDState(ScilabType dState) {
        ScilabDouble Dstate = (ScilabDouble) dState;
        final Controller controller = new Controller();
        VectorOfDouble dstateVector = new VectorOfDouble(Dstate.getHeight());
        for (int i = 0;  i < Dstate.getHeight(); ++i) {
            dstateVector.set(i, Dstate.getRealElement(i, 0));
        }
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.DSTATE, dstateVector) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(D_STATE, null, dState); // FIXME: get model's 'dstate' for fallback (oldValue)?
        }
    }

    /**
     * @return current odstate
     */
    public ScilabType getODState() {
        final Controller controller = new Controller();
        VectorOfDouble Odstate = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.ODSTATE, Odstate);
        // FIXME: find how to call ScilabType odstate = vec2var(Odstate);
        return null;
    }

    /**
     * @param oDState
     *            new odstate
     */
    public void setODState(ScilabType oDState) {
        final Controller controller = new Controller();
        VectorOfDouble odstate = new VectorOfDouble(); // Stub
        // FIXME: find how to call VectorOfDouble odstate = var2vec(oDState);
        if (controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.ODSTATE, odstate) == UpdateStatus.SUCCESS) {
            parametersPCS.firePropertyChange(O_D_STATE, null, oDState); // FIXME: get model's 'odstate' for fallback (oldValue)?
        }
    }

    /**
     * @return equations
     */
    public ScilabType getEquations() {
        final Controller controller = new Controller();
        VectorOfString Equations = new VectorOfString();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.EQUATIONS, Equations);
        if (Equations.isEmpty()) {
            return new ScilabList();
        }
        // FIXME: Decode Equations into a ScilabTList
        return null;
    }

    /**
     * @param equations
     *            equations
     */
    public void setEquations(ScilabType equations) {
        if (equations instanceof ScilabTList) {
            ScilabTList Equations = (ScilabTList) equations;
            // FIXME: decode Equations into a VectorOfDouble
        }
        //if ((this.equations == null && equations != null) || !this.equations.equals(equations)) {

        //    final ScilabType oldValue = this.equations;
        //    this.equations = equations;
        //    parametersPCS.firePropertyChange(EQUATIONS, oldValue, equations);
        //}
    }

    /**
     * @return locked status
     */
    public synchronized boolean isLocked() {
        return locked;
    }

    /**
     * @param locked
     *            change locked status
     */
    public synchronized void setLocked(boolean locked) {
        this.locked = locked;
    }

    /**
     * @param port
     *            to remove
     */
    public void removePort(BasicPort port) {
        if (port.getEdgeCount() != 0 && getParentDiagram() != null) {
            getParentDiagram().removeCells(new Object[] { port.getEdgeAt(0) });
        }
        remove(port);
    }

    /**
     * Add a port on the block.
     *
     * This call should only be used when a port reordering operation must be
     * performed.
     *
     * @param port
     *            The port to be added to the block
     */
    public void addPort(BasicPort port) {
        insert(port);
        port.setOrdering(BasicBlockInfo.getAllTypedPorts(this, false, port.getClass()).size());
        BlockPositioning.updateBlockView(this);
    }

    /**
     * @return command ports initial state
     */
    public ScilabDouble getAllCommandPortsInitialStates() {
        final List<CommandPort> cmdPorts = BasicBlockInfo.getAllTypedPorts(this, false, CommandPort.class);
        if (cmdPorts.isEmpty()) {
            return new ScilabDouble();
        }

        double[][] data = new double[cmdPorts.size()][1];
        for (int i = 0; i < cmdPorts.size(); ++i) {
            data[i][0] = cmdPorts.get(i).getInitialState();
        }

        return new ScilabDouble(data);
    }

    /**
     * @return name and type of the simulation function
     */
    public ScilabType getSimulationFunctionNameAndType() {
        if (getSimulationFunctionType() == SimulationFunctionType.DEFAULT) {
            return new ScilabString(getSimulationFunctionName());
        }
        ScilabList data = new ScilabList();

        data.add(new ScilabString(getSimulationFunctionName()));
        data.add(new ScilabDouble(getSimulationFunctionType().getAsDouble()));

        return data;
    }

    /**
     * Does the block update and register on the undo manager
     *
     * @param modifiedBlock
     *            the new settings
     */
    public void updateBlockSettings(BasicBlock modifiedBlock) {
        if (modifiedBlock == null) {
            return;
        }

        /*
         * Update the block settings
         */
        updateFields(modifiedBlock);

        /*
         * Update the children ports
         */
        updateChildren(modifiedBlock);

        /*
         * If the block is in a superblock then update it.
         */
        if (getParentDiagram() instanceof SuperBlockDiagram) {
            SuperBlock block = ((SuperBlockDiagram) getParentDiagram()).getContainer();

            XcosDiagram graph = block.getParentDiagram();
            if (graph == null) {
                setParentDiagram(Xcos.findParent(block));
                graph = block.getParentDiagram();
                LOG.finest(PARENT_DIAGRAM_WAS_NULL);
            }

            graph.fireEvent(new mxEventObject(XcosEvent.SUPER_BLOCK_UPDATED, XcosConstants.EVENT_BLOCK_UPDATED, block));
        }
    }

    /**
     * Update the instance field.
     *
     * @param modifiedBlock
     *            the modified instance
     */
    private void updateFields(BasicBlock modifiedBlock) {
        if (modifiedBlock == null) {
            return;
        }

        setDependsOnT(modifiedBlock.isDependsOnT());
        setDependsOnU(modifiedBlock.isDependsOnU());
        setExprs(modifiedBlock.getExprs());

        setRealParameters(modifiedBlock.getRealParameters());
        setIntegerParameters(modifiedBlock.getIntegerParameters());
        setObjectsParameters(modifiedBlock.getObjectsParameters());

        setState(modifiedBlock.getState());
        setDState(modifiedBlock.getDState());
        setODState(modifiedBlock.getODState());

        setBlockType(modifiedBlock.getBlockType());
        setSimulationFunctionName(modifiedBlock.getSimulationFunctionName());
        setSimulationFunctionType((int) modifiedBlock.getSimulationFunctionType().getAsDouble());

        setNbZerosCrossing(modifiedBlock.getNbZerosCrossing());
        setNmode(modifiedBlock.getNmode());

        setEquations(modifiedBlock.getEquations());
        setStyle(modifiedBlock.getStyle());
    }

    /**
     * Update the children of the block.
     *
     * @param modifiedBlock
     *            the new block instance
     */
    private void updateChildren(BasicBlock modifiedBlock) {
        if (modifiedBlock == null) {
            return;
        }

        XcosDiagram graph = getParentDiagram();
        if (graph == null) {
            setParentDiagram(Xcos.findParent(this));
            graph = getParentDiagram();
            LOG.finest(PARENT_DIAGRAM_WAS_NULL);
        }

        /*
         * Checked as port classes only
         */
        Set < Class <? extends mxICell >> types = new HashSet < Class <? extends mxICell >> (Arrays.asList(InputPort.class, OutputPort.class, ControlPort.class,
                CommandPort.class));

        Map < Class <? extends mxICell > , Deque<mxICell >> annotatedOlds = getTypedChildren(types);
        Map < Class <? extends mxICell > , Deque<mxICell >> annotatedNews = modifiedBlock.getTypedChildren(types);

        getParentDiagram().getModel().beginUpdate();
        try {
            for (Class <? extends mxICell > klass : types) {
                final Deque<mxICell> olds = annotatedOlds.get(klass);
                final Deque<mxICell> news = annotatedNews.get(klass);

                // updated ports
                while (!olds.isEmpty() && !news.isEmpty()) {
                    mxICell previous = olds.poll();
                    mxICell modified = news.poll();

                    final int previousIndex = children.indexOf(previous);

                    // relink
                    if (previous.getEdgeCount() != 0) {
                        final mxICell edge = previous.getEdgeAt(0);
                        final boolean isOutgoing = previous == edge.getTerminal(true);
                        previous.removeEdge(edge, isOutgoing);
                        modified.insertEdge(edge, isOutgoing);
                    }

                    getParentDiagram().removeCells(new Object[] { previous }, false);
                    getParentDiagram().addCells(new Object[] { modified }, this, previousIndex);

                    // Clone the geometry to avoid empty geometry on new cells.
                    getParentDiagram().getModel().setGeometry(modified, (mxGeometry) previous.getGeometry().clone());

                }

                // removed ports
                if (!olds.isEmpty()) {
                    getParentDiagram().removeCells(olds.toArray(), true);
                }

                // added ports
                if (!news.isEmpty()) {
                    getParentDiagram().addCells(news.toArray(), this);
                }
            }
        } finally {
            getParentDiagram().getModel().endUpdate();
        }
    }

    /**
     * Format the children as a typed map for the given class set.
     *
     * @param types
     *            the classes to search for.
     * @return a map which linked foreach type the corresponding cell list.
     */
    private Map < Class <? extends mxICell > , Deque<mxICell >> getTypedChildren(Set < Class <? extends mxICell >> types) {
        Map < Class <? extends mxICell > , Deque<mxICell >> oldPorts = new HashMap < Class <? extends mxICell > , Deque<mxICell >> ();

        // Allocate all types set
        for (Class <? extends mxICell > type : types) {
            oldPorts.put(type, new LinkedList<mxICell>());
        }

        if (getChildCount() <= 0) {
            return oldPorts;
        }

        // sort children according to the ordering parameter (useful on
        // scilab-5.2.x diagrams)
        sort(children);

        // children lookup
        for (Object cell : children) {

            Class <? extends Object > klass = cell.getClass();
            while (klass != null) {
                if (types.contains(klass)) {
                    break;
                }
                klass = klass.getSuperclass();
            }

            final Deque<mxICell> current = oldPorts.get(klass);
            if (current != null) {
                current.add((mxICell) cell);
            }
        }

        return oldPorts;
    }

    /**
     * Sort the children list in place.
     *
     * The sort put inputs then outputs the control then command ports. The
     * local port order is preserved.The sort is performed in place and do not
     * emit any event.
     */
    public void sortChildren() {
        if (getChildCount() <= 0) {
            return;
        }

        sort(children);
    }

    /**
     * @param context
     *            parent diagram context
     */
    public void openBlockSettings(String[] context) {
        final XcosDiagram graph;
        if (getParentDiagram() == null) {
            setParentDiagram(Xcos.findParent(this));
            graph = getParentDiagram();
            LOG.finest(PARENT_DIAGRAM_WAS_NULL);
        } else {
            graph = getParentDiagram();
        }
        if (graph instanceof PaletteDiagram) {
            return;
        }

        if (context == null) {
            throw new IllegalArgumentException();
        }

        // prevent to open twice
        if (isLocked()) {
            return;
        }

        graph.setCellsLocked(true);
        graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.WAIT_CURSOR));

        // sort children according to the ordering parameter (useful on
        // scilab-5.2.x diagrams)
        sortChildren();

        final ScilabDirectHandler handler = ScilabDirectHandler.acquire();
        if (handler == null) {
            return;
        }

        try {
            // Write scs_m
            handler.writeBlock(this);
            // Write context
            handler.writeContext(context);

            final ActionListener action = new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    LOG.finest("Updating data.");

                    graph.getView().clear(this, true, true);

                    // Now read new Block
                    graph.getModel().beginUpdate();
                    try {
                        final BasicBlock modifiedBlock = handler.readBlock();
                        updateBlockSettings(modifiedBlock);

                        graph.fireEvent(new mxEventObject(XcosEvent.ADD_PORTS, XcosConstants.EVENT_BLOCK_UPDATED, BasicBlock.this));
                    } catch (ScicosFormatException ex) {
                        LOG.severe(ex.toString());
                    } finally {
                        graph.getModel().endUpdate();
                        setLocked(false);

                        handler.release();

                        graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
                        graph.setCellsLocked(false);
                    }
                }
            };

            setLocked(true);
            ScilabInterpreterManagement.asynchronousScilabExec(action, "blk = xcosBlockInterface", getInterfaceFunctionName().toCharArray(), "set",
                    ScilabDirectHandler.BLK.toCharArray(), ScilabDirectHandler.CONTEXT.toCharArray());
        } catch (InterpreterException e) {
            LOG.severe(e.toString());
            setLocked(false);

            handler.release();

            graph.getAsComponent().getGraphControl().setCursor(Cursor.getPredefinedCursor(Cursor.DEFAULT_CURSOR));
            graph.setCellsLocked(false);
        }
    }

    /**
     * @return tooltip text
     */
    public String getToolTipText() {
        StringBuilder result = new StringBuilder();
        result.append(ScilabGraphConstants.HTML_BEGIN);
        result.append("Block Name : " + getInterfaceFunctionName() + ScilabGraphConstants.HTML_NEWLINE);
        result.append("Simulation : " + getSimulationFunctionName() + ScilabGraphConstants.HTML_NEWLINE);

        if (getParentDiagram() instanceof PaletteDiagram) {
            if (getIntegerParameters() != null) {
                result.append("Integer parameters : " + getIntegerParameters() + ScilabGraphConstants.HTML_NEWLINE);
            }

            if (getRealParameters() != null && getRealParameters().getHeight() != 0 && getRealParameters().getWidth() != 0) {
                result.append("Real parameters : " + getRealParameters() + ScilabGraphConstants.HTML_NEWLINE);
            }

            if (getObjectsParameters() != null) {
                result.append("Object parameters : " + getObjectsParameters() + ScilabGraphConstants.HTML_NEWLINE);
            }
        } else {
            result.append("UID : " + getId() + ScilabGraphConstants.HTML_NEWLINE);
            final int length = getStyle().length();
            result.append("Style : ");
            if (length > XcosConstants.MAX_CHAR_IN_STYLE) {
                result.append(getStyle().substring(0, XcosConstants.MAX_CHAR_IN_STYLE));
                result.append(XcosMessages.DOTS);
            } else {
                result.append(getStyle());
            }
            result.append(ScilabGraphConstants.HTML_NEWLINE);
            result.append("Flip : " + getFlip() + ScilabGraphConstants.HTML_NEWLINE);
            result.append("Mirror : " + getMirror() + ScilabGraphConstants.HTML_NEWLINE);
            result.append("Input ports : " + BasicBlockInfo.getAllTypedPorts(this, false, InputPort.class).size() + ScilabGraphConstants.HTML_NEWLINE);
            result.append("Output ports : " + BasicBlockInfo.getAllTypedPorts(this, false, OutputPort.class).size() + ScilabGraphConstants.HTML_NEWLINE);
            result.append("Control ports : " + BasicBlockInfo.getAllTypedPorts(this, false, ControlPort.class).size() + ScilabGraphConstants.HTML_NEWLINE);
            result.append("Command ports : " + BasicBlockInfo.getAllTypedPorts(this, false, CommandPort.class).size() + ScilabGraphConstants.HTML_NEWLINE);
        }

        result.append("x : " + getGeometry().getX() + ScilabGraphConstants.HTML_NEWLINE);
        result.append("y : " + getGeometry().getY() + ScilabGraphConstants.HTML_NEWLINE);
        result.append("w : " + getGeometry().getWidth() + ScilabGraphConstants.HTML_NEWLINE);
        result.append("h : " + getGeometry().getHeight() + ScilabGraphConstants.HTML_NEWLINE);
        result.append(ScilabGraphConstants.HTML_END);
        return result.toString();
    }

    /**
     * @param graph
     *            parent graph
     */
    public void openContextMenu(ScilabGraph graph) {
        ContextMenu menu = null;
        if (getParentDiagram() instanceof PaletteDiagram) {
            menu = createPaletteContextMenu(graph);
        } else {
            menu = createContextMenu(graph);
        }
        menu.setVisible(true);
    }

    /**
     * @param graph
     *            parent graph
     * @return context menu
     */
    // CSOFF: JavaNCSS
    public ContextMenu createPaletteContextMenu(ScilabGraph graph) {
        ContextMenu menu = ScilabContextMenu.createContextMenu();

        final List<XcosDiagram> allDiagrams = Xcos.getInstance().openedDiagrams();

        if (allDiagrams.size() == 0) {
            // No diagram opened: should never happen if Xcos opens an empty
            // diagram when it is launched
            MenuItem addTo = ScilabMenuItem.createMenuItem();

            addTo.setText(XcosMessages.ADDTO_NEW_DIAGRAM);
            addTo.setCallback(new CommonCallBack(XcosMessages.ADDTO_NEW_DIAGRAM) {
                @Override
                public void callBack() {

                    XcosDiagram theDiagram = new XcosDiagram();
                    BasicBlock block = (BasicBlock) BlockFactory.createClone(BasicBlock.this);
                    theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
                    mxGeometry geom = BasicBlock.this.getGeometry();
                    setDefaultPosition(geom);
                    theDiagram.getModel().setGeometry(block, geom);

                    XcosTab.get(theDiagram).setVisible(true);
                    BlockPositioning.updateBlockView(block);
                }
            });

            menu.add(addTo);

        } else if (allDiagrams.size() == 1) {
            // A single diagram opened: add to this diagram
            MenuItem addTo = ScilabMenuItem.createMenuItem();

            addTo.setText(XcosMessages.ADDTO + " " + XcosTab.get(allDiagrams.get(0)).getName());
            final XcosDiagram theDiagram = allDiagrams.get(0);
            addTo.setCallback(new CommonCallBack(theDiagram.getTitle()) {
                private static final long serialVersionUID = -99601763227525686L;

                @Override
                public void callBack() {
                    BasicBlock block = (BasicBlock) BlockFactory.createClone(BasicBlock.this);
                    theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
                    mxGeometry geom = BasicBlock.this.getGeometry();
                    setDefaultPosition(geom);
                    theDiagram.getModel().setGeometry(block, geom);
                    BlockPositioning.updateBlockView(block);
                    block.setParentDiagram(theDiagram);
                }
            });

            menu.add(addTo);

        } else {
            // The user has to choose
            Menu addTo = ScilabMenu.createMenu();

            addTo.setText(XcosMessages.ADDTO);

            for (int i = 0; i < allDiagrams.size(); i++) {
                MenuItem diagram = ScilabMenuItem.createMenuItem();
                final XcosDiagram theDiagram = allDiagrams.get(i);
                diagram.setText(XcosTab.get(allDiagrams.get(i)).getName());
                diagram.setCallback(new CommonCallBack(theDiagram.getTitle()) {
                    private static final long serialVersionUID = 3345416658377835057L;

                    @Override
                    public void callBack() {
                        BasicBlock block = (BasicBlock) BlockFactory.createClone(BasicBlock.this);
                        theDiagram.getModel().add(theDiagram.getDefaultParent(), block, 0);
                        mxGeometry geom = BasicBlock.this.getGeometry();
                        setDefaultPosition(geom);
                        theDiagram.getModel().setGeometry(block, geom);
                        BlockPositioning.updateBlockView(block);
                    }
                });
                addTo.add(diagram);
            }

            menu.add(addTo);
        }

        menu.getAsSimpleContextMenu().addSeparator();

        MenuItem help = ScilabMenuItem.createMenuItem();
        help.setText(XcosMessages.BLOCK_DOCUMENTATION);
        help.setCallback(new CommonCallBack(XcosMessages.BLOCK_DOCUMENTATION) {
            private static final long serialVersionUID = -1480947262397441951L;

            @Override
            public void callBack() {
                InterpreterManagement.requestScilabExec("help " + getInterfaceFunctionName());
            }
        });
        menu.add(help);

        menu.setVisible(true);

        ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo()
                .getLocation().y);

        return menu;
    }

    // CSON: JavaNCSS

    /**
     * @param graph
     *            parent graph
     * @return context menu
     */
    // CSOFF: JavaNCSS
    public ContextMenu createContextMenu(ScilabGraph graph) {
        ContextMenu menu = ScilabContextMenu.createContextMenu();
        Map < Class <? extends DefaultAction > , Menu > menuList = new HashMap < Class <? extends DefaultAction > , Menu > ();

        MenuItem value = BlockParametersAction.createMenu(graph);
        menuList.put(BlockParametersAction.class, value);
        menu.add(value);
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        value = CutAction.cutMenu(graph);
        menuList.put(CutAction.class, value);
        menu.add(value);
        value = CopyAction.copyMenu(graph);
        menuList.put(CopyAction.class, value);
        menu.add(value);
        value = DeleteAction.createMenu(graph);
        menuList.put(DeleteAction.class, value);
        menu.add(value);
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        value = RegionToSuperblockAction.createMenu(graph);
        menuList.put(RegionToSuperblockAction.class, value);
        menu.add(value);
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        Menu format = ScilabMenu.createMenu();
        format.setText(XcosMessages.FORMAT);
        menu.add(format);
        value = RotateAction.createMenu(graph);
        menuList.put(RotateAction.class, value);
        format.add(value);
        value = MirrorAction.createMenu(graph);
        menuList.put(MirrorAction.class, value);
        format.add(value);
        value = FlipAction.createMenu(graph);
        menuList.put(FlipAction.class, value);
        format.add(value);
        value = ShowHideShadowAction.createMenu(graph);
        menuList.put(ShowHideShadowAction.class, value);
        format.add(value);
        /*--- */
        format.addSeparator();
        /*--- */
        Menu alignMenu = ScilabMenu.createMenu();
        alignMenu.setText(XcosMessages.ALIGN_BLOCKS);
        alignMenu.add(AlignBlockActionLeft.createMenu(graph));
        alignMenu.add(AlignBlockActionCenter.createMenu(graph));
        alignMenu.add(AlignBlockActionRight.createMenu(graph));
        alignMenu.addSeparator();
        alignMenu.add(AlignBlockActionTop.createMenu(graph));
        alignMenu.add(AlignBlockActionMiddle.createMenu(graph));
        alignMenu.add(AlignBlockActionBottom.createMenu(graph));
        menuList.put(AlignBlockAction.class, alignMenu);
        format.add(alignMenu);
        /*--- */
        format.addSeparator();
        /*--- */
        if (graph.getSelectionCells().length > 1) {
            format.add(BorderColorAction.createMenu(graph));
            format.add(FilledColorAction.createMenu(graph));
        } else {
            format.add(EditFormatAction.createMenu(graph));
        }
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        menu.add(ViewDetailsAction.createMenu(graph));
        /*--- */
        menu.getAsSimpleContextMenu().addSeparator();
        /*--- */
        menu.add(BlockDocumentationAction.createMenu(graph));

        ((SwingScilabContextMenu) menu.getAsSimpleContextMenu()).setLocation(MouseInfo.getPointerInfo().getLocation().x, MouseInfo.getPointerInfo()
                .getLocation().y);

        customizeMenu(menuList);

        return menu;
    }

    // CSON: JavaNCSS

    /**
     * @return flip status
     */
    public boolean getFlip() {
        final Controller controller = new Controller();
        VectorOfDouble angle = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.ANGLE, angle);
        return (angle.get(0) == 1 ? true : false);
    }

    /**
     * @param flip
     *            value
     */
    @SuppressWarnings("deprecation")
    public void setFlip(boolean flip) {
        final Controller controller = new Controller();
        VectorOfDouble angle = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.ANGLE, angle);
        angle.set(0, (flip ? 1 : 0));
        controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.ANGLE, angle);

        if (getParentDiagram() != null) {
            final mxIGraphModel model = getParentDiagram().getModel();
            mxUtils.setCellStyles(model, new Object[] { this }, ScilabGraphConstants.STYLE_FLIP, Boolean.toString(flip));
        }
    }

    /**
     * invert flip status
     */
    public void toggleFlip() {
        BlockPositioning.toggleFlip(this);
    }

    /**
     * Override this to customize contextual menu
     *
     * @param menuList
     *            list of menu
     */
    protected void customizeMenu(Map < Class <? extends DefaultAction > , Menu > menuList) {
        // To be overridden by sub-classes
    }

    /**
     * @return mirror value
     */
    public boolean getMirror() {
        return isMirrored;
    }

    /**
     * @param mirror
     *            new mirror value
     */
    @SuppressWarnings("deprecation")
    public void setMirror(boolean mirror) {
        isMirrored = mirror;
        if (getParentDiagram() != null) {
            final mxIGraphModel model = getParentDiagram().getModel();
            mxUtils.setCellStyles(model, new Object[] { this }, ScilabGraphConstants.STYLE_MIRROR, Boolean.toString(mirror));
        }
    }

    /**
     * invert mirror value
     */
    public void toggleMirror() {
        BlockPositioning.toggleMirror(this);
    }

    /**
     *
     */
    public void toggleAntiClockwiseRotation() {
        BlockPositioning.toggleAntiClockwiseRotation(this);

    }

    /**
     * @return current angle
     */
    public int getAngle() {
        final Controller controller = new Controller();
        VectorOfDouble angle = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.ANGLE, angle);
        return (int) angle.get(1);
    }

    /**
     * @param angle
     *            new block angle
     */
    @SuppressWarnings("deprecation")
    public void setAngle(int angle) {
        final Controller controller = new Controller();
        VectorOfDouble Angle = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.ANGLE, Angle);
        Angle.set(1, angle);
        controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.ANGLE, Angle);

        if (getParentDiagram() != null) {
            mxUtils.setCellStyles(getParentDiagram().getModel(), new Object[] { this }, mxConstants.STYLE_ROTATION, Integer.toString(angle));
        }
    }

    /**
     * @param style
     *            new block style (as one-liner)
     */
    @Override
    public void setStyle(String style) {
        /**
         * Update the model, but not at instantiation (id=0)
         */
        if (id != 0) {
            final Controller controller = new Controller();
            controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.STYLE, style);
        }

        /**
         * Update the view
         */
        super.setStyle(style);
    }

    /**
     * Useful when we need to update local properties with mxCell style
     * properties
     */
    public void updateFieldsFromStyle() {
        StyleMap map = new StyleMap(getStyle());

        final Controller controller = new Controller();
        VectorOfDouble angle = new VectorOfDouble();
        controller.getObjectProperty(getID(), Kind.BLOCK, ObjectProperties.ANGLE, angle);
        if (map.get(mxConstants.STYLE_ROTATION) != null) {
            angle.set(1, Double.parseDouble(map.get(mxConstants.STYLE_ROTATION)));
        } else {
            angle.set(1, 0);
        }

        angle.set(0, (Boolean.parseBoolean(map.get(ScilabGraphConstants.STYLE_FLIP)) ? 1 : 0));
        controller.setObjectProperty(getID(), Kind.BLOCK, ObjectProperties.ANGLE, angle);
        isMirrored = Boolean.parseBoolean(map.get(ScilabGraphConstants.STYLE_MIRROR));
    }

    /**
     * Set the default block position on the geom
     *
     * @param geom
     *            the current geom
     */
    private void setDefaultPosition(mxGeometry geom) {
        geom.setX(DEFAULT_POSITION_X);
        geom.setY(DEFAULT_POSITION_Y);
    }

    /**
     * Get the parameters change support.
     *
     * The property name for each event is the field name, so one of: -
     * "interfaceFunctionName" - "simulationFunctionName" -
     * "simulationFunctionType" - "exprs" - "realParameters" -
     * "integerParameters" - "objectsParameters" - "nbZerosCrossing" - "nmode" -
     * "state" - "dState" - "oDState" - "equations" - "dependsOnU" -
     * "dependsOnT" - "blockType" - "ordering"
     *
     * @return the associated {@link PropertyChangeSupport} instance
     */
    protected PropertyChangeSupport getParametersPCS() {
        return parametersPCS;
    }

    /*
     * Overriden methods from jgraphx
     */

    /**
     * @return always false
     * @see com.mxgraph.model.mxCell#isConnectable()
     */
    @Override
    public boolean isConnectable() {
        return false;
    }

    /**
     * Re-associate fields with the new instance.
     *
     * @return a new clone instance
     * @throws CloneNotSupportedException
     *             never
     * @see com.mxgraph.model.mxCell#clone()
     */
    @Override
    public Object clone() throws CloneNotSupportedException {
        BasicBlock clone = (BasicBlock) super.clone();
        final Controller controller = new Controller();
        long newId = controller.cloneObject(getID(), true);
        clone.setID(newId);

        /* Reinstall the PropertyChangeSupport and all of it listeners */
        clone.parametersPCS = new PropertyChangeSupport(clone);
        PropertyChangeSupport pcs = getParametersPCS();
        for (PropertyChangeListener iter : pcs.getPropertyChangeListeners()) {
            clone.parametersPCS.addPropertyChangeListener(iter);
        }

        return clone;
    }

    /**
     * {@inheritDoc}
     *
     * Sync the specific child {@link EditFormatAction#HASH_IDENTIFIER}
     */
    @Override
    public mxICell insert(mxICell child, int index) {
        /*
         * Update the id if this is an identifier cell (herited identifier)
         */
        if (child.getId().endsWith(XcosDiagram.HASH_IDENTIFIER)) {
            child.setId(getId() + XcosDiagram.HASH_IDENTIFIER);
        }

        return super.insert(child, index);
    }

    @Override
    public String toString() {
        final StringBuilder str = new StringBuilder();
        str.append(getInterfaceFunctionName());
        str.append("\n");
        for (Object c : children) {
            str.append(c);
            str.append("\n");
        }

        return str.toString();
    }
}
// CSON: ClassDataAbstractionCoupling
// CSON: ClassFanOutComplexity
