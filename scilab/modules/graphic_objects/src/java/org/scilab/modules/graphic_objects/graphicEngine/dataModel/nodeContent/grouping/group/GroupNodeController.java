package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.group;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;

/**
 * Author: Lando Pierre
 * Date: june 14, 2010
 */
public class GroupNodeController extends AbstractGraphicController<IGroupNodeModel> implements IGroupNodeController {

    public GroupNodeController(IGroupNodeModel model) {
        super(model);
    }

    public GroupNodeController() {
        super(new GroupNodeModelImpl());
    }
}
