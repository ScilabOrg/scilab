package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.forge.scirenderer.core.drawable.IDrawable;
import org.scilab.forge.scirenderer.core.maths.ITransformation;
import org.scilab.forge.scirenderer.core.maths.impl.FinalTransformation;
import org.scilab.forge.scirenderer.core.treemodel.ISceneNode;
import org.scilab.forge.scirenderer.core.treemodel.ISceneTree;
import org.scilab.modules.graphic_objects.JoGLView.scilab.ScilabDrawableBridge;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import java.util.Vector;

/**
 * @author Pierre Lando
 */
public class SciSceneTree implements ISceneTree {
    private final String id;

    public SciSceneTree(String id) {
        this.id = id;
    }

    public ISceneNode getRootNode() {
        return new NodeController(id);
    }

    public ISceneNode getParentNode(ISceneNode node) {
        if (node instanceof NodeController) {
            Object parent = GraphicController.getController().getProperty(((NodeController) node).getId(), GraphicObjectProperties.__GO_PARENT__);
            if (parent != null && parent instanceof String) {
                String parentId = (String) parent;
                if (!parentId.equals("")) {
                    return new NodeController(parentId);
                }
            }
        }
        return null;
    }

    public ISceneNode[] getChildren(ISceneNode node) {
        Vector<ISceneNode> v = new Vector<ISceneNode>();
        if (node instanceof NodeController) {
            Object children = GraphicController.getController().getProperty(((NodeController) node).getId(), GraphicObjectProperties.__GO_CHILDREN__);
            if (children instanceof String[]) {
                for (String child : (String[]) children) {
                    if (child != null && !child.equals("")) {
                        v.add(new NodeController(child));
                    }
                }
            }
        }
        return v.toArray(new ISceneNode[v.size()]);
    }

    public int getSize() {
        return getBranchSize(getRootNode());
    }

    private int getBranchSize(ISceneNode node) {
        int size = 1;
        for (ISceneNode childNode : getChildren(node)) {
            size += getBranchSize(childNode);
        }
        return size;
    }

    private class NodeController implements ISceneNode {
        private final ScilabDrawableBridge scilabDrawableBridge;
        protected NodeController(String id) {
            scilabDrawableBridge = new ScilabDrawableBridge(id);
        }

        public boolean isVisible() {
            Boolean isVisible = (Boolean) GraphicController.getController().getProperty(getId(), GraphicObjectProperties.__GO_VISIBLE__);
            return (isVisible == null) ? true : isVisible;
        }

        @Override
        public ITransformation getTransformation() {
            // TODO : use a NodeController factory.

            String type = scilabDrawableBridge.getType();

            // TODO : use Figure.type
            if ((type != null) && type.equals("Figure")) {
                Object object = GraphicController.getController().getObjectFromId(scilabDrawableBridge.getId());
                if (object instanceof Figure) {

                    /**
                     * In the figure we use a [0, 1]³ box instead of a [-1, 1]³ one.
                     */
                    float[] r = {
                        2, 0, 0, 0,
                        0, 2, 0, 0,
                        0, 0, 2, 0,
                        -1, -1, -1, 1
                    };

                    try {
                        return new FinalTransformation(r);
                    } catch (ITransformation.BadLengthException e) {
                        return null;
                    }
                }
            }


            if ((type != null) && type.equals("Axes")) {
                // The "tight_limits" is related to ticks. We don't use it right now.
                Object object = GraphicController.getController().getObjectFromId(scilabDrawableBridge.getId());
                if (object instanceof Axes) {
                    Axes axes = (Axes) object;

                    Double[] axesBounds = axes.getAxesBounds();
                    Double[] margins = axes.getMargins();

                    float t[] = new float[2];
                    float s[] = new float[2];


                    t[0] = (float) (axesBounds[0] + axesBounds[2] * margins[0]);
                    t[1] = (float) (1.0 - axesBounds[1] - axesBounds[3] * (1.0 - margins[3]));

                    s[0] = (float) ((1.0 - margins[0] - margins[1]) * axesBounds[2]);
                    s[1] = (float) ((1.0 - margins[2] - margins[3]) * axesBounds[3]);


                    float[] r = {
                            s[0], 0,    0, 0,
                            0,    s[1], 0, 0,
                            0,    0,    1, 0,
                            t[0], t[1], 0, 1
                    };
                    
                    try {
                        return new FinalTransformation(r);
                    } catch (ITransformation.BadLengthException e) {
                        return null;
                    }
                }
            }


            return null;
        }

        @Override
        public IDrawable getContent() {
            return scilabDrawableBridge;
        }

        public String getId() {
            return scilabDrawableBridge.getId();
        }
    }
}
