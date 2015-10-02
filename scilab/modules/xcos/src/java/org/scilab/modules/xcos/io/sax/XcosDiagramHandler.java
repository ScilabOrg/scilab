package org.scilab.modules.xcos.io.sax;

import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Optional;
import java.util.Vector;
import java.util.stream.IntStream;
import java.util.stream.Stream;

import org.scilab.modules.xcos.JavaController;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfInt;
import org.scilab.modules.xcos.VectorOfScicosID;
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
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.graph.model.XcosCell;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.commandcontrol.CommandControlLink;
import org.scilab.modules.xcos.link.explicit.ExplicitLink;
import org.scilab.modules.xcos.link.implicit.ImplicitLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.ExplicitInputPort;
import org.scilab.modules.xcos.port.input.ImplicitInputPort;
import org.scilab.modules.xcos.port.output.ExplicitOutputPort;
import org.scilab.modules.xcos.port.output.ImplicitOutputPort;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxPoint;
import com.sun.istack.internal.Nullable;

/**
 * Implement a diagram SAX handler to decode the document as a stream.
 */
public class XcosDiagramHandler extends DefaultHandler {

    private static enum HandledElementsCategory {
        JGRAPHX,
        BLOCK,
        LINK,
        PORT,
        RAW_DATA,
        CUSTOM
    }

    /**
     * Contains all the handled elements to Xcos by category to ease dispatch.
     *
     * Note: the names correspond to each Element localName.
     */
    private static enum HandledElement {
        add(HandledElementsCategory.RAW_DATA),
        AfficheBlock(HandledElementsCategory.BLOCK),
        Array(HandledElementsCategory.RAW_DATA),
        BasicBlock(HandledElementsCategory.BLOCK),
        BigSom(HandledElementsCategory.BLOCK),
        CommandControlLink(HandledElementsCategory.LINK),
        CommandPort(HandledElementsCategory.PORT),
        ConstBlock(HandledElementsCategory.BLOCK),
        ControlPort(HandledElementsCategory.PORT),
        data(HandledElementsCategory.RAW_DATA),
        EventInBlock(HandledElementsCategory.BLOCK),
        EventOutBlock(HandledElementsCategory.BLOCK),
        ExplicitInBlock(HandledElementsCategory.BLOCK),
        ExplicitInputPort(HandledElementsCategory.PORT),
        ExplicitLink(HandledElementsCategory.LINK),
        ExplicitOutBlock(HandledElementsCategory.BLOCK),
        ExplicitOutputPort(HandledElementsCategory.PORT),
        GainBlock(HandledElementsCategory.BLOCK),
        GroundBlock(HandledElementsCategory.BLOCK),
        ImplicitInBlock(HandledElementsCategory.BLOCK),
        ImplicitInputPort(HandledElementsCategory.PORT),
        ImplicitLink(HandledElementsCategory.LINK),
        ImplicitOutBlock(HandledElementsCategory.BLOCK),
        ImplicitOutputPort(HandledElementsCategory.PORT),
        mxCell(HandledElementsCategory.JGRAPHX),
        mxGeometry(HandledElementsCategory.JGRAPHX),
        mxGraphModel(HandledElementsCategory.JGRAPHX),
        mxPoint(HandledElementsCategory.JGRAPHX),
        mxUndoManager(HandledElementsCategory.JGRAPHX),
        Orientation(HandledElementsCategory.CUSTOM),
        PrintBlock(HandledElementsCategory.BLOCK),
        Product(HandledElementsCategory.BLOCK),
        RoundBlock(HandledElementsCategory.BLOCK),
        ScilabBoolean(HandledElementsCategory.RAW_DATA),
        ScilabDouble(HandledElementsCategory.RAW_DATA),
        ScilabInteger(HandledElementsCategory.RAW_DATA),
        ScilabString(HandledElementsCategory.RAW_DATA),
        SplitBlock(HandledElementsCategory.BLOCK),
        Summation(HandledElementsCategory.BLOCK),
        SuperBlock(HandledElementsCategory.BLOCK),
        SuperBlockDiagram(HandledElementsCategory.CUSTOM),
        TextBlock(HandledElementsCategory.BLOCK),
        VoltageSensorBlock(HandledElementsCategory.BLOCK),
        XcosDiagram(HandledElementsCategory.CUSTOM),
        ;

        private static Map<String, HandledElement> getMap() {
            Map<String, HandledElement> map = new HashMap<>(HandledElement.values().length);
            Stream.of(HandledElement.values()).forEach(e -> map.put(e.name(), e));
            return map;
        }

        private final HandledElementsCategory category;

        private HandledElement(final HandledElementsCategory category) {
            this.category = category;
        }
    }

    /*
     * Utilities classes and methods
     */
    private static class UnresolvedReference {
        private long uid;
        private Kind kind;
        private ObjectProperties property;

        public UnresolvedReference(long uid, Kind kind, ObjectProperties property) {
            super();
            this.uid = uid;
            this.kind = kind;
            this.property = property;
        }

        public void execute(JavaController controller, long v) {
            controller.setObjectProperty(uid, kind, property, v);
        }
    }

    /**
     * Local implementation of a Stack as the java default one inherit from {@link Vector}.
     *
     * @param <E> the contained item klass
     */
    private static class Stack<E> {
        ArrayList<E> stack;

        public void pop() {
            stack.remove(stack.size() - 1);
        }

        public void push(E e) {
            stack.add(e);
        }

        public E peek() {
            return stack.get(stack.size() - 1);
        }

        public Stream<E> stream() {
            // stream in a reversed order (traditional stack usage)
            return IntStream.range(0, stack.size())
                   .mapToObj(i->stack.get(stack.size() - 1 - i));
        }
    }

    private XcosCell lookupForParentCell() {
        // TODO in fact we can use the depth (eg. size) of the stack to retrieve this value
        // is it necessary to improve performance (over safety) there ?
        Optional<XcosCell> parentBlock = parents.stream()
                                         .filter(o -> o instanceof XcosCell)
                                         .map(o -> (XcosCell) o)
                                         .findFirst();

        return parentBlock.orElse(null);
    }

    /*
     * Instance data
     */
    private final XcosDiagram root;
    private final JavaController controller;
    private final Map<String, HandledElement> map;

    /*
     * Current state of the parser
     */
    private Stack<Object> parents = new Stack<>();
    private Stack<HashMap<String, Long>> allChildren = new Stack<>();
    private HashMap<String, ArrayList<UnresolvedReference> > unresolvedReferences = new HashMap<>();


    public XcosDiagramHandler(final XcosDiagram content) {
        this.root = content;
        this.controller = new JavaController();

        this.map = HandledElement.getMap();
    }

    /*
     * Implemented method
     */

    @Override
    public void startElement(String uri, String localName, String qName, Attributes atts) throws SAXException {
        HandledElement found = map.get(localName);
        Object localParent = null;

        switch (found.category) {
            case JGRAPHX:
                localParent = startJGraphX(found, atts);
                break;
            case BLOCK:
                localParent = startBlock(found, atts);
                break;
            case LINK:
                localParent = startLink(found, atts);
                break;
            case PORT:
                localParent = startPort(found, atts);
                break;
            case RAW_DATA:
                localParent = startRawData(found, atts);
                break;
            case CUSTOM:
                localParent = startCustom(found, atts);
                break;
            default:
                // not handled element
                // something weird happens, let's continue
                break;
        }

        parents.push(localParent);
    }

    private Object startJGraphX(HandledElement found, Attributes atts) {
        String v;

        switch (found) {
            case mxCell: {
                v = atts.getValue("parent");
                if (v != null) {
                    long parentUID = allChildren.peek().getOrDefault(v, 0l);
                    if (parentUID != 0) {
                        v = atts.getValue("value");
                        if (v != null) {
                            Kind kind = controller.getKind(parentUID);
                            controller.setObjectProperty(parentUID, kind, ObjectProperties.LABEL, v);
                        }
                    }
                }

                return null;
            }
            case mxGeometry: {
                mxGeometry g = new mxGeometry();

                v = atts.getValue("height");
                if (v != null) {
                    g.setHeight(Double.valueOf(v));
                }
                v = atts.getValue("width");
                if (v != null) {
                    g.setWidth(Double.valueOf(v));
                }
                v = atts.getValue("x");
                if (v != null) {
                    g.setX(Double.valueOf(v));
                }
                v = atts.getValue("y");
                if (v != null) {
                    g.setY(Double.valueOf(v));
                }

                return g;
            }
            case mxGraphModel:
                // ignored on purpose, the logic is handled by the diagram
                return null;
            case mxPoint: {
                // defensive programming
                if (!(parents.peek() instanceof mxGeometry)) {
                    return null;
                }
                mxGeometry parent = (mxGeometry) parents.peek();

                mxPoint p = new mxPoint();

                v = atts.getValue("x");
                if (v != null) {
                    p.setX(Double.valueOf(v));
                }
                v = atts.getValue("y");
                if (v != null) {
                    p.setY(Double.valueOf(v));
                }

                v = atts.getValue("as");
                if ("sourcePoint".equals(v)) {
                    parent.setSourcePoint(p);
                } else if ("targetPoint".equals(v)) {
                    parent.setTargetPoint(p);
                }
                return p;
            }
            case mxUndoManager:
                // ignored on purpose, the undo manager is not serialized at all
                return null;
            default:
                throw new IllegalArgumentException();
        }
    }

    private BasicBlock startBlock(HandledElement found, Attributes atts) {
        String v;
        BasicBlock block;
        final long uid = controller.createObject(Kind.BLOCK);

        switch (found) {
            case AfficheBlock:
                block = new AfficheBlock(uid);
                break;
            case BasicBlock:
                block = new BasicBlock(uid);
                break;
            case BigSom:
                block = new BigSom(uid);
                break;
            case ConstBlock:
                //			 FIXME: why not needed anymore
                block = new BasicBlock(uid);
                break;
            case EventInBlock:
                block = new EventInBlock(uid);
                break;
            case EventOutBlock:
                block = new EventOutBlock(uid);
                break;
            case ExplicitInBlock:
                block = new ExplicitInBlock(uid);
                break;
            case ExplicitOutBlock:
                block = new ExplicitOutBlock(uid);
                break;
            case GainBlock:
                //			 FIXME: why not needed anymore
                block = new BasicBlock(uid);
                break;
            case GroundBlock:
                block = new GroundBlock(uid);
                break;
            case ImplicitInBlock:
                block = new ImplicitInBlock(uid);
                break;
            case ImplicitOutBlock:
                block = new ImplicitOutBlock(uid);
                break;
            case PrintBlock:
                //			 FIXME: why not needed anymore
                block = new BasicBlock(uid);
                break;
            case Product:
                block = new Product(uid);
                break;
            case RoundBlock:
                block = new RoundBlock(uid);
                break;
            case SplitBlock:
                block = new SplitBlock(uid);
                break;
            case Summation:
                block = new Summation(uid);
                break;
            case SuperBlock:
                block = new SuperBlock(uid);
                break;
            case TextBlock:
                block = new TextBlock(uid);
                break;
            case VoltageSensorBlock:
                block = new VoltageSensorBlock(uid);
                break;
            default:
                throw new IllegalArgumentException();
        }

        /*
         * Set the attributes
         */
        v = atts.getValue("id");
        if (v != null) {
            block.setId(v);
        }



        insertChild(block);
        return block;
    }

    private BasicLink startLink(HandledElement found, Attributes atts) {
        String v;
        BasicLink link;
        final long uid = controller.createObject(Kind.LINK);

        switch (found) {
            case CommandControlLink:
                link = new CommandControlLink(uid);
                break;
            case ExplicitLink:
                link = new ExplicitLink(uid);
                break;
            case ImplicitLink:
                link = new ImplicitLink(uid);
                break;
            default:
                throw new IllegalArgumentException();
        }

        /*
         * Set the attributes
         */
        v = atts.getValue("id");
        if (v != null) {
            controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.UID, v);
        }

        v = atts.getValue("source");
        if (v != null) {
            // if the attribute is present then the connected port is already decoded and present in the map
            long src = allChildren.peek().get(v);
            controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.SOURCE_PORT, src);
        }

        v = atts.getValue("target");
        if (v != null) {
            // if the attribute is present then the connected port is already decoded and present in the map
            long dst = allChildren.peek().get(v);
            controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.DESTINATION_PORT, dst);
        }

        v = atts.getValue("style");
        if (v != null) {
            controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.STYLE, v);
        }

        v = atts.getValue("value");
        if (v != null) {
            controller.setObjectProperty(uid, Kind.LINK, ObjectProperties.LABEL, v);
        }

        insertChild(link);
        return link;
    }

    /**
     * Insert a child into the current sub-diagram
     *
     * Manage hierarchy association :
     *  - PARENT_BLOCK
     *  - PARENT_DIAGRAM
     *  - CHILDREN
     */
    private void insertChild(final XcosCell cell) {
        long parentUID;
        Kind parentKind;

        final XcosCell parentBlock = lookupForParentCell();
        if (parentBlock != null) {
            parentUID = parentBlock.getUID();
            parentKind = parentBlock.getKind();

            controller.setObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.PARENT_BLOCK, parentUID);
        } else {
            parentUID = ((XcosCell)root.getDefaultParent()).getUID();
            parentKind = Kind.DIAGRAM;
        }
        controller.setObjectProperty(cell.getUID(), cell.getKind(), ObjectProperties.PARENT_DIAGRAM, ((XcosCell)root.getDefaultParent()).getUID());

        VectorOfScicosID children = new VectorOfScicosID();
        controller.getObjectProperty(parentUID, parentKind, ObjectProperties.CHILDREN, children);

        children.add(cell.getUID());
        controller.referenceObject(cell.getUID());

        controller.setObjectProperty(parentUID, parentKind, ObjectProperties.CHILDREN, children);
    }

    private BasicPort startPort(HandledElement found, Attributes atts) {
        BasicPort port;
        ObjectProperties relatedProperty;
        final long uid = controller.createObject(Kind.PORT);

        /*
         * Allocate the port with the right class to set default properties
         */

        switch (found) {
            case CommandPort:
                port = new CommandPort(uid);
                relatedProperty = ObjectProperties.EVENT_OUTPUTS;
                break;
            case ControlPort:
                port = new ControlPort(uid);
                relatedProperty = ObjectProperties.EVENT_INPUTS;
                break;
            case ExplicitInputPort:
                port = new ExplicitInputPort(uid);
                relatedProperty = ObjectProperties.INPUTS;
                break;
            case ExplicitOutputPort:
                port = new ExplicitOutputPort(uid);
                relatedProperty = ObjectProperties.OUTPUTS;
                break;
            case ImplicitInputPort:
                port = new ImplicitInputPort(uid);
                relatedProperty = ObjectProperties.INPUTS;
                break;
            case ImplicitOutputPort:
                port = new ImplicitOutputPort(uid);
                relatedProperty = ObjectProperties.OUTPUTS;
                break;
            default:
                throw new IllegalArgumentException();
        }

        /*
         * Setup the properties
         */
        String v;

        v = atts.getValue("style");
        if (v != null) {
            controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.STYLE, v);
        }

        VectorOfInt datatype = new VectorOfInt();
        controller.getObjectProperty(uid, Kind.PORT, ObjectProperties.DATATYPE, datatype);

        v = atts.getValue("dataType");
        if (v != null) {
            datatype.set(0, BasicPort.DataType.valueOf(v).ordinal());
        }
        v = atts.getValue("dataColumns");
        if (v != null) {
            datatype.set(1, Integer.valueOf(v));
        }
        v = atts.getValue("dataLines");
        if (v != null) {
            datatype.set(2, Integer.valueOf(v));
        }

        controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.DATATYPE, datatype);

        v = atts.getValue("initialState");
        if (v != null) {
            controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.FIRING, Double.valueOf(v));
        }

        /*
         * Associate to the parent block
         */

        int ordering = 0;
        long parent = 0l;

        v = atts.getValue("ordering");
        if (v != null) {
            ordering = Integer.valueOf(v) - 1;
        }

        v = atts.getValue("parent");
        if (v != null) {
            parent = allChildren.peek().get(v);
        }

        VectorOfScicosID associatedPorts = new VectorOfScicosID();
        controller.getObjectProperty(parent, Kind.BLOCK, relatedProperty, associatedPorts);

        associatedPorts.add(ordering, uid);
        controller.referenceObject(uid);

        controller.setObjectProperty(uid, Kind.PORT, ObjectProperties.SOURCE_BLOCK, parent);
        controller.setObjectProperty(parent, Kind.BLOCK, relatedProperty, associatedPorts);

        return port;
    }

    private Object startRawData(HandledElement found, Attributes atts) {
        switch (found) {
            case add:
                break;
            case Array:
                break;
            case data:
                break;
            case ScilabBoolean:
                break;
            case ScilabDouble:
                break;
            case ScilabInteger:
                break;
            case ScilabString:
                break;
            default:
                throw new IllegalArgumentException();
        }

        return null;
    }

    private Object startCustom(HandledElement found, final Attributes atts) {
        long uid = 0l;
        String v;

        switch (found) {
            case Orientation: {
                // defensive programming
                if (!(parents.peek() instanceof BasicPort)) {
                    return null;
                }
                BasicPort parent = (BasicPort) parents.peek();

                v = atts.getValue("value");
                if (v != null) {
                    final String orientationString = v;
                    Optional<Orientation> orientation = Stream.of(Orientation.values())
                                                        .filter( o -> o.name().equals(orientationString)).findFirst();
                    if (orientation.isPresent()) {
                        parent.setOrientation(orientation.get());
                    }
                }

                return null;
            }
            case XcosDiagram:
                // do not allocate this is already allocated as #root
                uid = root.getUID();

                /*
                 * Decode some graph properties
                 */
                v = atts.getValue("savedFile");
                if (v != null) {
                    controller.setObjectProperty(uid, Kind.DIAGRAM, ObjectProperties.PATH, v);
                }

                /*
                 * Decode simulation properties
                 */
                VectorOfDouble properties = new VectorOfDouble();
                controller.getObjectProperty(uid, Kind.DIAGRAM, ObjectProperties.PROPERTIES, properties);

                v = atts.getValue("finalIntegrationTime");
                if (v != null) {
                    properties.set(ScicosParameters.FINAL_INTEGRATION_TIME, Double.valueOf(v));
                }
                v = atts.getValue("integratorAbsoluteTolerance");
                if (v != null) {
                    properties.set(ScicosParameters.INTEGRATOR_ABSOLUTE_TOLERANCE, Double.valueOf(v));
                }
                v = atts.getValue("integratorRelativeTolerance");
                if (v != null) {
                    properties.set(ScicosParameters.INTEGRATOR_RELATIVE_TOLERANCE, Double.valueOf(v));
                }
                v = atts.getValue("toleranceOnTime");
                if (v != null) {
                    properties.set(ScicosParameters.TOLERANCE_ON_TIME, Double.valueOf(v));
                }
                v = atts.getValue("maxIntegrationTimeinterval");
                if (v != null) {
                    properties.set(ScicosParameters.MAX_INTEGRATION_TIME_INTERVAL, Double.valueOf(v));
                }
                v = atts.getValue("maximumStepSize");
                if (v != null) {
                    properties.set(ScicosParameters.MAXIMUM_STEP_SIZE, Double.valueOf(v));
                }
                v = atts.getValue("realTimeScaling");
                if (v != null) {
                    properties.set(ScicosParameters.REAL_TIME_SCALING, Double.valueOf(v));
                }
                v = atts.getValue("solver");
                if (v != null) {
                    properties.set(ScicosParameters.SOLVER, Double.valueOf(v));
                }

                controller.setObjectProperty(uid, Kind.DIAGRAM, ObjectProperties.PROPERTIES, properties);

            // no break on purpose, we decode non-root specific properties later
            case SuperBlockDiagram:
                final Kind kind;
                if (uid == 0l) {
                    XcosCell parent = lookupForParentCell();
                    uid = parent.getUID();
                    kind = parent.getKind();
                } else {
                    kind = Kind.DIAGRAM;
                }


                v = atts.getValue("gridEnabled");
                if (v != null) {
                    // FIXME should be pushed to the model
                    root.setGridEnabled(Integer.valueOf(v) == 1);
                }

                allChildren.push(new HashMap<>());
                break;
            default:
                throw new IllegalArgumentException();
        }

        return null;
    }

    @Override
    public void endElement(String uri, String localName, String qName) throws SAXException {
        HandledElement found = map.get(localName);

        switch (found.category) {
            case JGRAPHX:
                endJGraphX(found);
                break;
            case BLOCK:
                endBlock(found);
                break;
            case LINK:
                endLink(found);
                break;
            case PORT:
                endPort(found);
                break;
            case RAW_DATA:
                endRawData(found);
                break;
            case CUSTOM:
                endCustom(found);
                break;
            default:
                // not handled element
                // something weird happens, let's continue
                break;
        }

        parents.pop();
    }

    private void endJGraphX(HandledElement found) {
    }

    private void endBlock(HandledElement found) {
    }

    private void endLink(HandledElement found) {
    }

    private void endPort(HandledElement found) {
    }

    private void endRawData(HandledElement found) {
    }

    private void endCustom(HandledElement found) {
        switch (found) {
            case Orientation:
                break;
            case SuperBlockDiagram:
            // no break on purpose
            case XcosDiagram:
                allChildren.pop();
                break;
            default:
                throw new IllegalArgumentException();
        }

        // FIXME manage each link source
        // FIXME manage each link target

    }
}
