package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

import java.awt.Point;

/**
 * @author Pierre Lando
 */
public class InteractionManager {
    /** Parent drawer visitor */
    private DrawerVisitor drawerVisitor;

    /** The rubber box */
    private RubberBox rubberBox;

    /** The drag, zoom, rotate interaction manager */
    DragZoomRotateInteraction dragZoomRotateInteraction;

    /**
     * Default constructor.
     * @param drawerVisitor parent drawer visitor.
     */
    public InteractionManager(DrawerVisitor drawerVisitor) {
        this.drawerVisitor = drawerVisitor;

        dragZoomRotateInteraction = new DragZoomRotateInteraction(drawerVisitor.getFigure(), drawerVisitor.getComponent());
        dragZoomRotateInteraction.setEnable(true);

        rubberBox = new RubberBox(drawerVisitor.getComponent(), drawerVisitor.getCanvas());
        rubberBox.addListener(new ZoomRubberBoxListener(drawerVisitor.getFigure(), drawerVisitor.getComponent(), rubberBox));
        rubberBox.addListener(new RubberBoxListener() {
            @Override
            public void rubberBoxEnd(Point firstPoint, Point secondPoint) {
                dragZoomRotateInteraction.setEnable(true);
            }
        });

        drawerVisitor.addPostRendering(rubberBox);
    }

    /**
     * Called to start zooming.
     */
    public void startInteractiveZoom() {
        dragZoomRotateInteraction.setEnable(false);
        rubberBox.setEnable(true);
    }
}
