package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.modules.renderer.JoGLView.postRendering.PostRendered;

import javax.swing.event.EventListenerList;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Point;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

/**
 * @author Pierre Lando
 */
public class RubberBox implements PostRendered, MouseListener, MouseMotionListener {

    /** Rubber box color */
    private static final Color RUBBER_BOX_COLOR = new Color(.2f, .3f, .4f);

    /** Rubber box thickness */
    private static final float RUBBER_BOX_THICKNESS = 1;

    /** Rubber box pattern */
    private static final short RUBBER_BOX_PATTERN = (short) 0xFAFA;

    /** The mouse button used to perform a rubber box */
    private static final int MOUSE_MODIFIER = MouseEvent.BUTTON1_MASK;

    /** Rubber box appearance */
    private static Appearance appearance;

    /** Listened component */
    private final Component component;

    /** Parent canvas */
    private final Canvas canvas;

    /** The event listener list */
    private final EventListenerList listenerList = new EventListenerList();

    /** Rubber box geometry */
    private DefaultGeometry geometry;

    /** Current listening status. */
    private boolean isEnable;

    /** Rubber box visibility */
    private boolean isVisible;
    private Point firstPoint;
    private Point secondPoint;

    public RubberBox(Component component, Canvas canvas) {
        this.component = component;
        this.canvas = canvas;
        isVisible = false;
        isEnable = false;
    }

    @Override
    public void draw(DrawingTools drawingTools) throws SciRendererException {
        if (isVisible) {
            updateVertexData(drawingTools);
            drawingTools.draw(getGeometry(drawingTools), getAppearance());
        }
    }

    /**
     * Rubber box geometry getter.
     * @param drawingTools the drawing tools.
     * @return the rubber box geometry.
     */
    public Geometry getGeometry(DrawingTools drawingTools) {
        if (geometry == null) {
            geometry = new DefaultGeometry();

            BuffersManager bufferManager = drawingTools.getCanvas().getBuffersManager();
            ElementsBuffer vertexBuffer = bufferManager.createElementsBuffer();
            vertexBuffer.setData(new float[] {
                    0, 0, 0, 1,
                    0, 0, 0, 1,
                    0, 0, 0, 1,
                    0, 0, 0, 1
            }, 4);
            IndicesBuffer indicesBuffer = bufferManager.createIndicesBuffer();
            indicesBuffer.setData(new int[] {
                    0, 1, 2, 1, 2, 3, 0, 3
            });

            geometry.setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
            geometry.setFillDrawingMode(Geometry.FillDrawingMode.NONE);
            geometry.setVertices(vertexBuffer);
            geometry.setWireIndices(indicesBuffer);
        }
        return geometry;
    }

    public Appearance getAppearance() {
        if (appearance == null) {
            appearance = new Appearance();
            appearance.setLineColor(RUBBER_BOX_COLOR);
            appearance.setLineWidth(RUBBER_BOX_THICKNESS);
            appearance.setLinePattern(RUBBER_BOX_PATTERN);
        }
        return appearance;
    }

    public void setEnable(boolean isEnable) {
        if (this.isEnable != isEnable) {
            this.isEnable = isEnable;
            if (isEnable) {
                isVisible = false;
                component.addMouseListener(this);
            } else {
                component.removeMouseListener(this);
                component.removeMouseMotionListener(this);
            }
        }
    }

    /** Notify all listener that the rubber box have ended */
    private void fireRubberBox() {
        for (RubberBoxListener rubberBoxListener : listenerList.getListeners(RubberBoxListener.class)) {
            rubberBoxListener.rubberBoxEnd(firstPoint, secondPoint);
        }
    }

    public void addListener(RubberBoxListener rubberBoxListener) {
        listenerList.add(RubberBoxListener.class, rubberBoxListener);
    }

    public void removeListener(RubberBoxListener rubberBoxListener) {
        listenerList.remove(RubberBoxListener.class, rubberBoxListener);
    }


    @Override
    public void mouseClicked(MouseEvent e) {
        if (e.getModifiers() == MOUSE_MODIFIER) {
            if (isVisible) {
                component.removeMouseMotionListener(this);
                isVisible = false;
                canvas.redraw();
                fireRubberBox();
            } else {
                firstPoint = bound(e.getPoint());
                secondPoint = bound(e.getPoint());
                isVisible = true;
                component.addMouseMotionListener(this);
            }
        }
    }

    @Override
    public void mouseMoved(MouseEvent e) {
        if (isVisible) {
            secondPoint = bound(e.getPoint());
            canvas.redraw();
        }
    }

    /**
     * bound the given point to canvas.
     * @param point the given point.
     * @return a point bounded in the canvas.
     */
    private Point bound(Point point) {
        Dimension dimension = canvas.getDimension();
        return new Point(
                Math.max(1, Math.min(point.x, dimension.width)),
                Math.max(1, Math.min(point.y, dimension.height))
        );
    }

    /** Update geometry
     * @param drawingTools the drawing tools are needed to set geometry.
     */
    private void updateVertexData(DrawingTools drawingTools) {
        Geometry geom = getGeometry(drawingTools);
        float height = (float) canvas.getDimension().getHeight() + 1;
        geom.getVertices().setData(new float[] {
                firstPoint.x, height - firstPoint.y, 0, 1,
                secondPoint.x, height - firstPoint.y, 0, 1,
                secondPoint.x, height - secondPoint.y, 0, 1,
                firstPoint.x, height - secondPoint.y, 0, 1
        }, 4);
    }

    @Override
    public void mousePressed(MouseEvent e) {
    }

    @Override
    public void mouseReleased(MouseEvent e) {
    }

    @Override
    public void mouseEntered(MouseEvent e) {
    }

    @Override
    public void mouseExited(MouseEvent e) {
    }

    @Override
    public void mouseDragged(MouseEvent e) {
    }
}
