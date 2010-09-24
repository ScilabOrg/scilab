package org.scilab.modules.graphic_objects.JoGLView.scilab;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

/**
 * @author Pierre Lando
 */
public class ScilabDrawableBridge implements IScilabDrawableBridge {
    private final String id;
    private final String type;

	public ScilabDrawableBridge(String id) {
	    this.id = id;
		type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
	}

    @Override
    public String getType() {
        return type;
	}

    @Override
    public String getId() {
	    return id;
	}
}
