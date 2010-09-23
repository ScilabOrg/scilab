package org.scilab.modules.graphic_objects.util.MVC.tree.events;

import org.scilab.modules.graphic_objects.util.MVC.IDataEvent;
import org.scilab.modules.graphic_objects.util.MVC.tree.ITreeModel;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeContent;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeController;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 */
public class NodeInsertedEvent<nodeContentType extends INodeContent> implements IDataEvent<ITreeModel<nodeContentType>> {
    public final INodeController childNode;
    public final INodeController parentNode;

    public NodeInsertedEvent(INodeController<? extends nodeContentType> childNode, INodeController<? extends nodeContentType> parentNode) {
        this.childNode = childNode;
        this.parentNode = parentNode;
    }
}
