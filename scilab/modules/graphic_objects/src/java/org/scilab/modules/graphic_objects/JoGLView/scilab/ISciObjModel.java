package org.scilab.modules.graphic_objects.JoGLView.scilab;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.IGeometryModel;

/**
 * Author: lando
 * Date: 21 sept. 2010
 */
public interface ISciObjModel extends IGeometryModel {

    /**
     * @return the id of the scilab object.
     */
    String getId();
}
