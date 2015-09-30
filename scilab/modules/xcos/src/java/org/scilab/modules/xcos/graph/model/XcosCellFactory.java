/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph.model;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.synchronousScilabExec;

import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.EnumSet;
import java.util.Optional;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.preferences.Component.Link;
import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.UpdateStatus;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfScicosID;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosView;
import org.scilab.modules.xcos.XcosViewListener;
import org.scilab.modules.xcos.block.AfficheBlock;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.block.io.EventInBlock;
import org.scilab.modules.xcos.block.io.EventOutBlock;
import org.scilab.modules.xcos.block.io.ExplicitInBlock;
import org.scilab.modules.xcos.block.io.ExplicitOutBlock;
import org.scilab.modules.xcos.block.io.ImplicitInBlock;
import org.scilab.modules.xcos.block.io.ImplicitOutBlock;
import org.scilab.modules.xcos.block.positionning.BigSom;
import org.scilab.modules.xcos.block.positionning.GroundBlock;
import org.scilab.modules.xcos.block.positionning.Product;
import org.scilab.modules.xcos.block.positionning.RoundBlock;
import org.scilab.modules.xcos.block.positionning.Summation;
import org.scilab.modules.xcos.block.positionning.VoltageSensorBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;

/**
 * Ease the creation of any {@link Kind} of graphical object
 */
public final class XcosCellFactory {

    // DAC: As this is the constructor for all the block classes, this class is
    // very coupled with *Block classes
    // CSOFF: ClassDataAbstractionCoupling
    /**
     * List the specific block interface function name. <BR>
     * <BR>
     * <EM>Specific instance must be registered before generic ones in order
     * to serialized all the non-default values.</EM>
     */
    public static enum BlockInterFunction {
        /** @see TextBlock */
        TEXT_f(TextBlock.class),
        /** @see SuperBlock */
        DSUPER(SuperBlock.class),
        /** @see SuperBlock */
        SUPER_f(SuperBlock.class),
        /** @see AfficheBlock */
        AFFICH_m(AfficheBlock.class),
        /** @see AfficheBlock */
        AFFICH_f(AfficheBlock.class),
        /** @see ExplicitInBlock */
        IN_f(ExplicitInBlock.class),
        /** @see ExplicitOutBlock */
        OUT_f(ExplicitOutBlock.class),
        /** @see ImplicitInBlock */
        INIMPL_f(ImplicitInBlock.class),
        /** @see ImplicitOutBlock */
        OUTIMPL_f(ImplicitOutBlock.class),
        /** @see EventInBlock */
        CLKINV_f(EventInBlock.class),
        /** @see EventOutBlock */
        CLKOUTV_f(EventOutBlock.class),
        /** @see EventOutBlock */
        CLKOUT_f(EventOutBlock.class),
        /** @see SplitBlock */
        SPLIT_f(SplitBlock.class),
        /** @see SplitBlock */
        IMPSPLIT_f(SplitBlock.class),
        /** @see SplitBlock */
        CLKSPLIT_f(SplitBlock.class),
        /** @see GroundBlock */
        Ground(GroundBlock.class),
        /** @see VoltageSensorBlock */
        VoltageSensor(VoltageSensorBlock.class),
        /** @see RoundBlock */
        SUM_f(RoundBlock.class),
        /** @see RoundBlock */
        PROD_f(RoundBlock.class),
        /** @see RoundBlock */
        CLKSOM_f(RoundBlock.class),
        /** @see RoundBlock */
        CLKSOMV_f(RoundBlock.class),
        /** @see BigSom */
        BIGSOM_f(BigSom.class),
        /** @see Summation */
        SUMMATION(Summation.class),
        /** @see Product */
        PRODUCT(Product.class),
        /** @see BasicBlock */
        BASIC_BLOCK(BasicBlock.class);

        private final Class<? extends BasicBlock> klass;

        /**
         * Default constructor
         *
         * @param block
         *            The reference instance
         */
        private BlockInterFunction(Class<? extends BasicBlock> klass) {
            this.klass = klass;
        }

        /**
         * @return the class to instantiate
         */
        public Class<? extends BasicBlock> getKlass() {
            return klass;
        }
    }

    // CSON: ClassDataAbstractionCoupling

    /** Default singleton constructor */
    private XcosCellFactory() {
        // This class is a static singleton
    }

    /*
     * Diagram management
     */

    private static class DiagramLoadedListener extends XcosViewListener {
        private long uid;

        public DiagramLoadedListener() {
            uid = 0;
        }

        public long getUID() {
            return uid;
        }

        /**
         * When a diagram is created then store it for later use.
         */
        @Override
        public void objectCreated(long uid, Kind kind) {
            if (!Kind.DIAGRAM.equals(kind)) {
                return;
            }

            this.uid = uid;
        }
    }

    /**
     * Allocate a Java XcosDiagram from a COSF file.
     *
     * This method execute the file and register a
     *
     * @param controller the controller
     * @param filename the file to execute
     * @return an allocated XcosDiagram
     */
    public static XcosDiagram createDiagramFromCOSF(final JavaController controller, String filename) {
        XcosView view = (XcosView) JavaController.lookup_view(Xcos.class.getSimpleName());

        DiagramLoadedListener diagramLoaded = new DiagramLoadedListener();
        view.addXcosViewListener(diagramLoaded, EnumSet.allOf(Kind.class), true, EnumSet.of(ObjectProperties.CHILDREN));

        XcosDiagram diagram;
        try {
            synchronousScilabExec("exec(\"" + filename + "\", -1)");
            diagram = new XcosDiagram(diagramLoaded.getUID(), Kind.DIAGRAM);
            insertChildren(controller, diagram);
        } catch (InterpreterException e1) {
            diagram = null;
        } finally {
            view.removeXcosViewListener(diagramLoaded);
        }

        return diagram;
    }

    private static void insertChildren(JavaController controller, XcosDiagram diagram) {
        VectorOfScicosID children = new VectorOfScicosID();
        controller.getObjectProperty(diagram.getUID(), diagram.getKind(), ObjectProperties.CHILDREN, children);
        final int childrenLen = children.size();

        /*
         * Allocation some pre-sized stash data
         */
        final ArrayList<BasicLink> links = new ArrayList<>(childrenLen / 2);
        final ArrayList<BasicPort> ports = new ArrayList<>(childrenLen);

        /*
         * Create the XcosCell objects and store some of them for later use
         */
        XcosCell[] cells = new XcosCell[childrenLen];
        for (int i = 0; i < childrenLen; i++) {
            final long uid = children.get(i);
            final Kind kind = controller.getKind(uid);

            switch (kind) {
                case ANNOTATION:
                case BLOCK:
                    BasicBlock b = createBlock(controller, uid, kind);
                    cells[i] = b;
                    b.getTypedChildrenIndexes(BasicPort.class).stream()
                    .map(index -> b.getChildAt(index))
                    .filter(c -> c instanceof BasicPort)
                    .forEach( c -> ports.add((BasicPort) c));
                    break;
                case LINK:
                    BasicLink l = createLink(controller, uid, kind);
                    cells[i] = l;
                    links.add(l);
                    break;
                default:
                    break;
            }
        }

        /*
         * Relink the links on the XcosCell part
         */
        Comparator<XcosCell> compare = (c1, c2) -> (int) (c1.getUID() - c2.getUID());
        Collections.sort(ports, compare);
        for (BasicLink l : links) {
            long[] src = new long[1];
            controller.getObjectProperty(l.getUID(), l.getKind(), ObjectProperties.SOURCE_PORT, src);

            long[] dest = new long[1];
            controller.getObjectProperty(l.getUID(), l.getKind(), ObjectProperties.DESTINATION_PORT, dest);

            int srcIndex = Collections.binarySearch(ports, new XcosCell(src[0], Kind.PORT), compare);
            if (srcIndex > 0) {
                l.setSource(ports.get(srcIndex));
            }

            int destIndex = Collections.binarySearch(ports, new XcosCell(dest[0], Kind.PORT), compare);
            if (destIndex > 0) {
                l.setTarget(ports.get(destIndex));
            }
        }

        XcosCell[] allChildren = new XcosCell[cells.length + links.size()];
        System.arraycopy(cells, 0, allChildren, 0, cells.length);
        System.arraycopy(links.toArray(), 0, allChildren, cells.length, links.size());
        diagram.addCells(allChildren);
    }



    /*
     * Block and Annotation management
     */

    /**
     * Instantiate a new block with the specified interface function.
     *
     * @param func
     *            the interface function
     * @return A new instance of a block.
     */
    public static BasicBlock createBlock(BlockInterFunction func) {
        return createBlock(func, func.name());
    }

    /**
     * Instantiate a new block with the specified UID value and interface function
     *
     * @param uid
     *            The associated UID value
     * @param interfaceFunction the interface function
     * @return A new instance of a block.
     */
    public static BasicBlock createBlock(String interfaceFunction) {
        Optional<BlockInterFunction> func = EnumSet.allOf(BlockInterFunction.class).stream()
                                            .filter(f -> f.name().equals(interfaceFunction))
                                            .findFirst();

        final BasicBlock block;
        if (func.isPresent()) {
            block = createBlock(func.get());
        } else {
            block = createBlock(BlockInterFunction.BASIC_BLOCK, interfaceFunction);
        }
        block.setStyle(interfaceFunction);

        return block;
    }

    private static class BlockLoadedListener extends XcosViewListener {
        private long uid;

        public BlockLoadedListener() {
            uid = 0;
        }

        public long getUID() {
            return uid;
        }

        /**
         * When a unique block is created then store it for later use.
         */
        @Override
        public void objectCreated(long uid, Kind kind) {
            if (!EnumSet.of(Kind.BLOCK, Kind.ANNOTATION).contains(kind)) {
                return;
            }

            this.uid = uid;
        }

        /**
         * When a composite block is created we track the PARENT_BLOCK / CHILDREN association to store the parent.
         */
        @Override
        public void propertyUpdated(long uid, Kind kind, ObjectProperties property, UpdateStatus status) {
            if (status != UpdateStatus.SUCCESS || property != ObjectProperties.CHILDREN) {
                return;
            }
            if (!EnumSet.of(Kind.BLOCK, Kind.ANNOTATION).contains(kind)) {
                return;
            }

            this.uid = uid;
        }
    }

    private static BasicBlock createBlock(BlockInterFunction func, String interfaceFunction) {
        return createBlock(new JavaController(), func, interfaceFunction);
    }

    private static BasicBlock createBlock(final JavaController controller, BlockInterFunction func, String interfaceFunction) {
        XcosView view = (XcosView) JavaController.lookup_view(Xcos.class.getSimpleName());

        BlockLoadedListener blockLoaded = new BlockLoadedListener();
        view.addXcosViewListener(blockLoaded, EnumSet.allOf(Kind.class), true, EnumSet.of(ObjectProperties.CHILDREN));

        BasicBlock block;
        try {
            synchronousScilabExec(ScilabDirectHandler.BLK + " = " + buildCall(interfaceFunction, "define"));
            block = createBlock(controller, func, interfaceFunction, blockLoaded.getUID());
        } catch (InterpreterException e1) {
            block = null;
        } finally {
            view.removeXcosViewListener(blockLoaded);
        }

        return block;
    }

    private static BasicBlock createBlock(final JavaController controller, long uid, Kind kind) {
        String[] interfaceFunction = new String[1];
        controller.getObjectProperty(uid, kind, ObjectProperties.INTERFACE_FUNCTION, interfaceFunction);

        return createBlock(controller, interfaceFunction[0], uid);
    }

    private static BasicBlock createBlock(final JavaController controller, String interfaceFunction, long uid) {
        Optional<BlockInterFunction> optFunc = EnumSet.allOf(BlockInterFunction.class).stream()
                                               .filter(f -> f.name().equals(interfaceFunction))
                                               .findFirst();

        final BlockInterFunction func;
        if (optFunc.isPresent()) {
            func = optFunc.get();
        } else {
            func = BlockInterFunction.BASIC_BLOCK;
        }

        return createBlock(controller, func, interfaceFunction, uid);
    }

    /**
     * Instantiate a new block with the specified interface function and uid.
     *
     * @param controller the Java controller to use
     * @param func the interface function as an enum
     * @param interfaceFunction the interface function name
     * @param uid the allocated uid
     * @return A new instance of a block.
     */
    private static BasicBlock createBlock(final JavaController controller, BlockInterFunction func, String interfaceFunction, long uid) {
        BasicBlock block = null;
        try {
            block = func.getKlass().getConstructor(Long.TYPE).newInstance(uid);
        } catch (InstantiationException | IllegalAccessException | IllegalArgumentException | InvocationTargetException	| NoSuchMethodException | SecurityException e) {
            // Something goes wrong, print it.
            e.printStackTrace();
            return block;
        }

        /*
         * Synchronize model information back to the JGraphX data
         */
        insertPortChildren(controller, block);

        block.setStyle(interfaceFunction);

        VectorOfDouble geom = new VectorOfDouble(4);
        controller.getObjectProperty(block.getUID(), block.getKind(), ObjectProperties.GEOMETRY, geom);
        block.setGeometry(new mxGeometry(geom.get(0), geom.get(1), geom.get(2), geom.get(3)));

        return block;
    }

    /*
     * Port management
     */

    /**
     * Helper used to create port children on a parent block.
     *
     * This method does not manage the model transaction and should be used to preset the children of a block out of an {@link XcosDiagram}.
     *
     * @param controller is the shared controller instance
     * @param parent is the parent {@link mxCell} to modify
     */
    private static void insertPortChildren(final JavaController controller, final XcosCell parent) {
        final EnumSet<ObjectProperties> properties = EnumSet.of(ObjectProperties.INPUTS, ObjectProperties.OUTPUTS, ObjectProperties.EVENT_INPUTS, ObjectProperties.EVENT_OUTPUTS);
        insertPortChildren(controller, properties, parent);
    }


    /**
     * Helper used to create port children on a parent block.
     *
     * This method does not manage the model transaction and should be used to preset the children of a block out of an {@link XcosDiagram}.
     *
     * @param controller is the shared controller instance
     * @param properties specify the kind of port to insert and should be some of : <UL>
     *        <LI>{@link ObjectProperties#INPUTS}
     *        <LI>{@link ObjectProperties#OUTPUTS}
     *        <LI>{@link ObjectProperties#EVENT_INPUTS}
     *        <LI>{@link ObjectProperties#EVENT_OUTPUTS}
     * @param parent is the parent {@link mxCell} to modify
     */
    private static void insertPortChildren(final JavaController controller, final EnumSet<ObjectProperties> properties, final XcosCell parent) {
        for (ObjectProperties property : properties) {
            insertPortChildren(controller, property, parent);
        }
    }

    /**
     * Helper used to create port children on a parent block.
     *
     * This method does not manage the model transaction and should be used to preset the children of a block out of an {@link XcosDiagram}.
     *
     * @param controller is the shared controller instance
     * @param property specify the kind of port to insert and should be one of : <UL>
     *        <LI>{@link ObjectProperties#INPUTS}
     *        <LI>{@link ObjectProperties#OUTPUTS}
     *        <LI>{@link ObjectProperties#EVENT_INPUTS}
     *        <LI>{@link ObjectProperties#EVENT_OUTPUTS}
     * @param parent is the parent {@link mxCell} to modify
     */
    private static void insertPortChildren(final JavaController controller, final ObjectProperties property, final XcosCell parent) {
        if (parent.getKind() != Kind.BLOCK) {
            return;
        }

        VectorOfScicosID modelChildren = new VectorOfScicosID();
        controller.getObjectProperty(parent.getUID(), parent.getKind(), property, modelChildren);

        mxICell[] children = new mxICell[modelChildren.size()];
        for (int i = 0; i < children.length; i++) {
            children[i] = createPort(controller, modelChildren.get(i), property);
        }
        Arrays.stream(children).forEach(c -> parent.insert(c));
    }

    /**
     * Create a port for a specific uid
     *
     * @param controller is the shared controller instance
     * @param uid represent the allocated UID on the MVC
     * @param property specify the kind of port to create and should be one of : <UL>
     *        <LI>{@link ObjectProperties#INPUTS}
     *        <LI>{@link ObjectProperties#OUTPUTS}
     *        <LI>{@link ObjectProperties#EVENT_INPUTS}
     *        <LI>{@link ObjectProperties#EVENT_OUTPUTS}
     * @return a newly allocated port
     */
    private static final BasicPort createPort(final JavaController controller, long uid, final ObjectProperties property) {
        boolean[] isImplicit = {false};

        switch (property) {
            case INPUTS:
                controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.IMPLICIT, isImplicit);
                if (isImplicit[0]) {
                    return new ImplicitInputPort(uid);
                } else {
                    return new ExplicitInputPort(uid);
                }
            case OUTPUTS:
                controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.IMPLICIT, isImplicit);
                if (isImplicit[0]) {
                    return new ImplicitOutputPort(uid);
                } else {
                    return new ExplicitOutputPort(uid);
                }
            case EVENT_INPUTS:
                return new ControlPort(uid);
            case EVENT_OUTPUTS:
                return new CommandPort(uid);
            default:
                return null;
        }
    }

    /*
     * Link management
     */

    private static BasicLink createLink(JavaController controller, long uid, Kind kind) {
        int[] type = new int[1];
        controller.getObjectProperty(uid, kind, ObjectProperties.KIND, type);

        switch (type[0]) {
            case -1:
                return new CommandControlLink(uid);
            case 1:
                return new ExplicitLink(uid);
            case 2:
                return new ImplicitLink(uid);
            default:
                return null;
        }
    }

}
