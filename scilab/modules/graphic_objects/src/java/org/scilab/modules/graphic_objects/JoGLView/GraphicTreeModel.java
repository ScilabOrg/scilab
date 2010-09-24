package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.modules.graphic_objects.JoGLView.scilab.SciObjController;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.IGraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.IGraphicModel;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.util.MVC.AbstractDataController;
import org.scilab.modules.graphic_objects.util.MVC.tree.ITreeModel;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeController;

import java.util.Vector;

/**
 * Author: lando
 * Date: 21 sept. 2010
 */
public class GraphicTreeModel implements ITreeModel<IGraphicController> {
    private final String id;

    public GraphicTreeModel(String id) {
        this.id = id;
    }

    public INodeController<IGraphicController> getRootNode() {
        return new NodeController(id);
    }

    public INodeController<IGraphicController> insertNode(INodeController<IGraphicController> parentNode) {
        return null;
    }
    public INodeController<IGraphicController> insertNode() {
        return null;
    }
    public void removeNode(INodeController<IGraphicController> node) {}

    public int getSize() {
        return getBranchSize(getRootNode());
    }

    private int getBranchSize(INodeController<IGraphicController> node) {
        int size = 1;
        for (INodeController<IGraphicController> childNode : node.getChildren()) {
            size += getBranchSize(childNode);
        }
        return size;
    }

    private class NodeController extends AbstractDataController implements INodeController<IGraphicController> {
        private final String id;
        protected NodeController(String id) {
            super(null);
            this.id = id;
        }

        public void setVisible(boolean visible) {}

        public boolean isVisible() {
            return (Boolean) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_VISIBLE__);
        }

        public IGraphicController getContent() {
            return new SciObjController(id);
        }

        public void setContent(IGraphicController content) {}

        public INodeController<IGraphicController> getParentNode() {
            Object parent = GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_PARENT__);
            if (parent != null && parent instanceof String) {
                String parentId = (String) parent;
                if (!parentId.equals("")) {
                    return new NodeController(parentId);
                }
            }
            return null;
        }

        public INodeController<IGraphicController>[] getChildren() {
            Object children = GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__);
            Vector<INodeController<IGraphicController>> v = new Vector<INodeController<IGraphicController>>();
            if (children instanceof String[]) {
                for (String child : (String[]) children) {
                    if (child != null && !child.equals("")) {
                        v.add(new NodeController(child));
                    }
                }
            }
            return v.toArray(new INodeController[v.size()]);
        }
    }
}
