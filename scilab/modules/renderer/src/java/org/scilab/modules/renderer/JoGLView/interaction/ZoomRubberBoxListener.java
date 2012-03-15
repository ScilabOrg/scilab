package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.Camera;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;

import java.awt.Component;
import java.awt.Point;

/**
 * @author Pierre Lando
 */
public class ZoomRubberBoxListener extends FigureInteraction implements RubberBoxListener {

    /** The parent rubber box */
    private final RubberBox rubberBox;

    /**
     * Default constructor.
     * @param figure listened figure.
     * @param component listened component.
     * @param rubberBox parent rubber box.
     */
    public ZoomRubberBoxListener(Figure figure, Component component, RubberBox rubberBox) {
        super(figure, component);
        this.rubberBox = rubberBox;
    }

    @Override
    public void rubberBoxEnd(Point firstPoint, Point secondPoint) {
        performZoom(firstPoint, secondPoint);
        rubberBox.setEnable(false);
    }

    @Override
    protected void changeEnable(boolean isEnable) {
        rubberBox.setEnable(isEnable);
    }

    /**
     * Actually perform the zoom
     * @param firstPoint first rubber box corner.
     * @param secondPoint second rubber box corner.
     */
    private void performZoom(Point firstPoint, Point secondPoint) {
        Axes axes = getUnderlyingAxes(firstPoint);
        if (axes.getCamera().getView().equals(Camera.ViewType.VIEW_2D)) {
            Double[] bounds = axes.getDisplayedBounds().clone();
            double[] p1 = AxesDrawer.compute2dViewFromPixelCoordinates(axes, new double[]{firstPoint.x, firstPoint.y});
            double[] p2 = AxesDrawer.compute2dViewFromPixelCoordinates(axes, new double[]{secondPoint.x, secondPoint.y});
            bounds[0] = Math.min(p1[0], p2[0]);
            bounds[1] = Math.max(p1[0], p2[0]);

            bounds[2] = Math.min(p1[1], p2[1]);
            bounds[3] = Math.max(p1[1], p2[1]);

            Boolean zoomed = tightZoomBounds(axes, bounds);
            GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_BOX__, bounds);
            GraphicController.getController().setProperty(axes.getIdentifier(), GraphicObjectProperties.__GO_ZOOM_ENABLED__, zoomed);
        }
    }
}
