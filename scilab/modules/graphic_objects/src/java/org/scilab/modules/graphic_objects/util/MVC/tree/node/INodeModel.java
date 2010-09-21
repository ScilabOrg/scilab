package org.scilab.modules.graphic_objects.util.MVC.tree.node;

import org.scilab.modules.graphic_objects.util.MVC.IDataModel;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 */
public interface INodeModel <nodeContentType extends INodeContent> extends IDataModel{

    void setVisible(boolean visible);

    boolean isVisible();

    nodeContentType getContent();

    void setContent(nodeContentType content);

    INodeController<nodeContentType> getParentNode();

    INodeController<nodeContentType>[] getChildren();
}
