/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.renderingZone;

import org.scilab.modules.graphic_objects.util.MVC.IDataModel;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.core.IGraphicTreeController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.navigation.ICameraController;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.IDrawable;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.layout.ILayoutPolicy;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 */
public interface IRenderingZoneModel extends IDataModel, IDrawable {
    public final String type = "RenderingZone";  

    /**
     * Set camera associated with this rendering zone.
     * @param camera is the new camera associated with this rendering zone.
     */
    void setCamera(ICameraController camera);

    /**
     * @return camera associated with this rendering zone.
     */
    ICameraController getCamera();

    /**
     * Set the data model of the scene rendered in this rendering zone.
     * @param graphicDataController the data controller of the scene rendered in this rendering zone.
     */
    void setDataController(IGraphicTreeController graphicDataController);

    /**
     * @return the data model of the scene rendered in this rendering zone.
     */
    IGraphicTreeController getDataController();

    /**
     * @param layoutPolicy is the new layout policy of this rendering zone.
     */
    void setLayoutPolicy(ILayoutPolicy layoutPolicy);

    /**
     * @return the layout policy of this rendering zone.
     */
    ILayoutPolicy getLayoutPolicy();
}
