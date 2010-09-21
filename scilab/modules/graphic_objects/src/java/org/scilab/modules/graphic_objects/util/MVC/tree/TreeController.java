package org.scilab.modules.graphic_objects.util.MVC.tree;

import org.scilab.modules.graphic_objects.util.MVC.AbstractDataController;
import org.scilab.modules.graphic_objects.util.MVC.IDataController;
import org.scilab.modules.graphic_objects.util.MVC.IDataEvent;
import org.scilab.modules.graphic_objects.util.MVC.IDataListener;
import org.scilab.modules.graphic_objects.util.MVC.tree.events.NodeInsertedEvent;
import org.scilab.modules.graphic_objects.util.MVC.tree.events.NodeRemovedEvent;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeContent;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeController;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 */
public class TreeController <nodeContentType extends INodeContent> extends AbstractDataController<ITreeModel<nodeContentType>> implements ITreeController <nodeContentType> {
    private final IDataListener nodeListener = new IDataListener() {

        
        public void update(IDataController sources, IDataEvent event) {
            fireUpdate(event);
        }
    };

    public TreeController() {
        super(new TreeModelImpl<nodeContentType>());
    }

    
    public INodeController<nodeContentType> getRootNode() {
        return getModel().getRootNode();
    }

    
    public INodeController<nodeContentType> insertNode(INodeController<nodeContentType> parentNode) {
        INodeController<nodeContentType> childNode = getModel().insertNode(parentNode);
        fireUpdate(new NodeInsertedEvent<nodeContentType>(childNode, parentNode));

        // TODO: the tree listen his nodes should be optional.
        childNode.addListener(nodeListener);
        return childNode; 
    }

    
    public INodeController<nodeContentType> insertNode() {
        return insertNode(getRootNode());
    }

    
    public void removeNode(INodeController<nodeContentType> node) {
        fireUpdate(new NodeRemovedEvent<nodeContentType>(node));
        getModel().removeNode(node);
    }

    
    public int getSize() {
        return getModel().getSize();
    }
}
