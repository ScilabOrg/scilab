package org.scilab.modules.graphic_objects.graphicEngine.dataModel.core;

import org.scilab.modules.graphic_objects.JoGLView.GraphicTreeModel;
import org.scilab.modules.graphic_objects.util.MVC.tree.ITreeModel;
import org.scilab.modules.graphic_objects.util.MVC.tree.TreeController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.IGraphicController;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 *
 * Implementation of IGraphicTreeController
 *
 */
public class GraphicTreeController extends TreeController<IGraphicController> implements IGraphicTreeController {

    GraphicTreeController() {
        
    }

    public GraphicTreeController(GraphicTreeModel graphicTreeModel) {
        super(graphicTreeModel);
    }
}
