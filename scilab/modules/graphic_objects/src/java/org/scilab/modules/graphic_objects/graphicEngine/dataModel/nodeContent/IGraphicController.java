package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent;

import org.scilab.modules.graphic_objects.util.MVC.IDataController;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeContent;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 */
public interface IGraphicController<modelType extends IGraphicModel> extends IDataController, IGraphicModel, INodeContent<modelType> {
}
