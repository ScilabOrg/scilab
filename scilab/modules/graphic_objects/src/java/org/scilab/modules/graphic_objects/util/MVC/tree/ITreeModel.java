/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.util.MVC.tree;

import org.scilab.modules.graphic_objects.util.MVC.IDataModel;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeContent;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeController;

/**
 * Author: Lando Pierre
 * Date: 27 avril 2010
 */
public interface ITreeModel <nodeContentType extends INodeContent> extends IDataModel {
    /**
     * @return the root node for this model.
     */
    INodeController<nodeContentType> getRootNode();

    /**
     * Insert a child node in a grouping node.
     * @param parentNode where child node will be inserted.
     * @return the inserted node.
     */
    INodeController<nodeContentType> insertNode(INodeController<nodeContentType> parentNode);

    /**
     * Insert a child node in root node.
     * @return the inserted node.
     */
    INodeController<nodeContentType> insertNode();

    /**
     * Remove the given node and all is descendant from this data model.
     * If node is rootNode, root node is not removed (is descendant are).
     * @param node the given node.
     */
    void removeNode(INodeController<nodeContentType> node);

    /**
     * @return the number of node in this data model.
     */
    int getSize();
}
