package org.scilab.modules.graphic_objects.util.MVC.tree;

import org.scilab.modules.graphic_objects.util.MVC.IDataController;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeContent;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public interface ITreeController <nodeContentType extends INodeContent> extends ITreeModel<nodeContentType>, IDataController {
}
