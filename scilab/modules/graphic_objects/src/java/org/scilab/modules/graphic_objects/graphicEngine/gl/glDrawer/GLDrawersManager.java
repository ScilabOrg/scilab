/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.DefaultDrawersManager;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.Geometry.*;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.Zone.RenderingZoneGLDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.appearance.SolidColorGLDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.grouping.TransformGLDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.navigation.CameraGLDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.scilab.AxesGLDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.scilab.PolylineGLDrawer;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.shape.ShapeGLDrawer;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 *
 * Basic implementation of IGLDrawersManager.
 * Classic drawer are registered at the initialisation.
 */
public class GLDrawersManager extends DefaultDrawersManager<IGLDrawer> implements IGLDrawersManager {

    /**
     * Default constructor of GLDrawersManager.
     */
    public GLDrawersManager() {
        registerDrawer(RenderingZoneGLDrawer.getDrawer());

        // Geometry drawer
        registerDrawer(RectangleGLDrawer.getDrawer());
        registerDrawer(BoxGLDrawer.getDrawer());
        registerDrawer(CylinderGLDrawer.getDrawer());
        registerDrawer(IndexedTriangleSetGLDrawer.getDrawer());
        registerDrawer(TransformGLDrawer.getDrawer());
        registerDrawer(CameraGLDrawer.getDrawer());
        registerDrawer(ShapeGLDrawer.getDrawer());
        registerDrawer(SolidColorGLDrawer.getDrawer());

        //Scilab Drawer
        registerDrawer(PolylineGLDrawer.getDrawer());
        registerDrawer(AxesGLDrawer.getDrawer());
    }
}
