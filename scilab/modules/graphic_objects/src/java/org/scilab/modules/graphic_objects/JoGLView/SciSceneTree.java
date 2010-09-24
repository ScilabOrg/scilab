package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.forge.scirenderer.core.drawable.IDrawable;
import org.scilab.forge.scirenderer.core.maths.ITransformation;
import org.scilab.forge.scirenderer.core.treemodel.ISceneNode;
import org.scilab.forge.scirenderer.core.treemodel.ISceneTree;
import org.scilab.modules.graphic_objects.JoGLView.scilab.ScilabDrawableBridge;
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
            return (Boolean) GraphicController.getController().getProperty(getId(), GraphicObjectProperties.__GO_VISIBLE__);
        }

        @Override
        public ITransformation getTransformation() {
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
