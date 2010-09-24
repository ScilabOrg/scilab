package org.scilab.modules.graphic_objects.util.MVC.tree.node;

import org.scilab.modules.graphic_objects.util.MVC.IDataController;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 */
public interface INodeController <nodeContentType extends INodeContent> extends INodeModel<nodeContentType>, IDataController {
}
