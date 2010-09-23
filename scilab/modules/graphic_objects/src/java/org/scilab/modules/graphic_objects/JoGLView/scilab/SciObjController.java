package org.scilab.modules.graphic_objects.JoGLView.scilab;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;

/**
 * Author: lando
 * Date: 21 sept. 2010
 */
public class SciObjController extends AbstractGraphicController<ISciObjModel> implements ISciObjController {
    
    public SciObjController(ISciObjModel model) {
        super(model);
    }

    public SciObjController(String id) {
        super(new SciObjModelImpl(id));
    }

    public String getId() {
        return getModel().getId();
    }
}
