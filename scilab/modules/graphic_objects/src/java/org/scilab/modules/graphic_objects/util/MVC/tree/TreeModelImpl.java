package org.scilab.modules.graphic_objects.util.MVC.tree;

import org.scilab.modules.graphic_objects.util.MVC.tree.node.AbstractNodeController;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeContent;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeController;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeModel;

import java.util.LinkedList;
import java.util.List;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 */
public class TreeModelImpl <nodeContentType extends INodeContent> implements ITreeModel<nodeContentType> {
    private final NodeController<nodeContentType> rootNode = new NodeController<nodeContentType>();

    
    public INodeController <nodeContentType> getRootNode() {
        return rootNode;
    }

    
    public INodeController<nodeContentType> insertNode(INodeController<nodeContentType> parentNode) {
        if (parentNode instanceof NodeController) {
            NodeController<nodeContentType> child = new NodeController<nodeContentType>();
            ((NodeController) parentNode).addChild(child);
            child.setParentNode(parentNode);

            return child;
        } else {
            return null;
        }
    }

    
    public INodeController<nodeContentType> insertNode() {
        return insertNode(getRootNode());
    }

    
    public void removeNode(INodeController<nodeContentType> node) {
        if (node instanceof NodeController) {
            if (node == getRootNode()) {
                rootNode.removeAllChild();
            } else {
                INodeController<nodeContentType> parentNode = node.getParentNode();
                ((NodeController) parentNode).removeChild(node);
            }
        }
    }

    
    public int getSize() {
        return nodeCount(rootNode);
    }

    private static int nodeCount(INodeController node) {
        int r = 1;
        for (INodeController child : node.getChildren()) {
            r+= nodeCount(child);
        }
        return r;
    }

    private class NodeController<nodeContentType extends INodeContent> extends AbstractNodeController<nodeContentType> implements INodeController<nodeContentType> {
        public NodeController() {
            super(new NodeModel<nodeContentType>());
        }

        
        public void setVisible(boolean visible) {
            if (isVisible() != visible) {
                getModel().setVisible(visible);
                fireUpdate();
            }
        }

        
        public boolean isVisible() {
            return getModel().isVisible();
        }

        
        public INodeController<nodeContentType> getParentNode() {
            return getModel().getParentNode();
        }

        
        public INodeController<nodeContentType>[] getChildren() {
            return getModel().getChildren();
        }
        
        
        protected NodeModel<nodeContentType> getModel() {
            return (NodeModel<nodeContentType>) super.getModel();
        }

        protected void setParentNode(INodeController<nodeContentType> parentNode) {
            getModel().setParentNode(parentNode);
        }

        protected void addChild(INodeController<nodeContentType> child) {
            getModel().addChild(child);
        }

        protected void removeChild(INodeController<nodeContentType> child) {
            getModel().removeChild(child);
        }

        protected void removeAllChild() {
            getModel().removeAllChild();
        }
    }

    private class NodeModel<nodeContentType extends INodeContent> implements INodeModel<nodeContentType> {
        private final List<INodeController<nodeContentType>> children =  new LinkedList<INodeController<nodeContentType>>();
        private INodeController<nodeContentType>  parentNode;

        private nodeContentType content;
        private boolean visible = true;

        
        public void setVisible(boolean visible) {
            this.visible = visible;
        }

        
        public boolean isVisible() {
            return visible;
        }

        
        public nodeContentType getContent() {
            return content;
        }

        
        public void setContent(nodeContentType content) {
            this.content = content;
        }

        
        public INodeController<nodeContentType> getParentNode() {
            return parentNode;
        }

        
        public INodeController<nodeContentType> [] getChildren() {
            return children.toArray(new INodeController[children.size()]);
        }

        protected void setParentNode(INodeController<nodeContentType> parentNode) {
            this.parentNode = parentNode;
        }

        protected void addChild(INodeController<nodeContentType> child) {
            children.add(child);
        }

        protected void removeChild(INodeController<nodeContentType> child) {
            children.remove(child);
        }

        protected void removeAllChild() {
            children.clear();
        }
    }
}
