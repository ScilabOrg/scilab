/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * @author Pierre Lando
 */
public class InteractionManager implements RubberBoxListener {

    /** The rubber box */
    private RubberBox rubberBox;

    /** The drag, zoom, rotate interaction manager */
    DragZoomRotateInteraction dragZoomRotateInteraction;

    /**
     * Default constructor.
     * @param drawerVisitor parent drawer visitor.
     */
    public InteractionManager(DrawerVisitor drawerVisitor) {
        dragZoomRotateInteraction = new DragZoomRotateInteraction(drawerVisitor);
        dragZoomRotateInteraction.setEnable(true);

        rubberBox = new RubberBox(drawerVisitor);
        rubberBox.addListener(this);
        drawerVisitor.addPostRendering(rubberBox);
    }

    /**
     * Called to start zooming.
     */
    public void startInteractiveZoom() {
        dragZoomRotateInteraction.setEnable(false);
        rubberBox.setEnable(true);
    }

    @Override
    public void rubberBoxEnd(Vector3d firstPoint, Vector3d secondPoint) {
        dragZoomRotateInteraction.setEnable(true);
    }

    @Override
    public void rubberBoxStopped() {
        dragZoomRotateInteraction.setEnable(true);
    }
}
