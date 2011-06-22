/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.Drawer;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.sprite.Sprite;
import org.scilab.forge.scirenderer.sprite.SpriteAnchorPosition;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.graphic_objects.imageplot.Grayplot;
import org.scilab.modules.graphic_objects.imageplot.Matplot;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.rectangle.Rectangle;
import org.scilab.modules.graphic_objects.surface.Fac3d;
import org.scilab.modules.graphic_objects.surface.Plot3d;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.vectfield.Arrow;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.scilab.modules.graphic_objects.vectfield.Segs;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.mark.MarkSpriteManager;
import org.scilab.modules.renderer.JoGLView.text.TextManager;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

/**
 * @author Pierre Lando
 */
public class DrawerVisitor implements IVisitor, Drawer, GraphicView {
    private final Canvas canvas;
    private final Figure figure;
    private final DataManager dataManager;
    private final MarkSpriteManager markManager;
    private final TextManager textManager;
    private final AxesDrawer axesDrawer;

    private ColorMap colorMap;
    private DrawingTools drawingTools = null;

    public DrawerVisitor(Canvas canvas, Figure figure) {
        GraphicController.getController().register(this);

        this.canvas = canvas;
        this.figure = figure;

        this.dataManager = new DataManager(canvas);
        this.markManager = new MarkSpriteManager(canvas.getSpriteManager());
        this.textManager = new TextManager(canvas.getSpriteManager());
        this.axesDrawer = new AxesDrawer(this);
    }

    public DrawingTools getDrawingTools() {
        return drawingTools;
    }

    public Canvas getCanvas() {
        return canvas;
    }

    public ColorMap getColorMap() {
        return colorMap;
    }

    @Override
    public void draw(DrawingTools drawingTools) {
        long start = System.currentTimeMillis();
        this.drawingTools = drawingTools;
        figure.accept(this);
        long end = System.currentTimeMillis();
        long delta = end - start;
        System.out.println(delta + "ms | " + 1000.0 / delta + " FPS");
    }

    /**
     * Ask the given object to accept visitor.
     * @param ids array of object identifier.
     */
    public void askAcceptVisitor(String[] ids) {
        for (String id : ids) {
            GraphicObject object = GraphicController.getController().getObjectFromId(id);
            object.accept(this);
        }
    }

    @Override
    public void visit(Axes axes) {
        if (axes.getVisible()) {
            axesDrawer.draw(axes);
        }
    }

    @Override
    public void visit(Arc arc) {
        // TODO
        System.out.println("How can I draw an arc ?");
    }

    @Override
    public void visit(Axis axis) {
        // TODO
        System.out.println("How can I draw an axis ?");
    }

    @Override
    public void visit(Compound compound) {
        if (compound.getVisible()) {
            askAcceptVisitor(compound.getChildren());
        }
    }

    /*
     * To do:
     * -use polygon offset for wireframe rendering.
     */
    @Override
    public void visit(final Fec fec) {
        if (fec.getVisible()) {
            Geometry triangles = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.TRIANGLES;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(fec.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    return dataManager.getColorBuffer(fec.getIdentifier());
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getIndexBuffer(fec.getIdentifier());
                    return indices;
                }

				@Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    return FaceCullingMode.BOTH;
                }
            };

            Geometry wireframe = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.SEGMENTS;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(fec.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    return null;
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getWireIndexBuffer(fec.getIdentifier());
                    return indices;
                }
                
                @Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    return FaceCullingMode.BOTH;
                }
            };

            Appearance trianglesAppearance = new Appearance();
            drawingTools.draw(triangles, trianglesAppearance);

            if (fec.getLineMode())
            {
                Appearance wireframeAppearance = new Appearance();
                wireframeAppearance.setLineColor(ColorFactory.createColor(colorMap, fec.getLineColor()));
                drawingTools.draw(wireframe, wireframeAppearance);
            }
        }
    }

    @Override
    public void visit(Figure figure) {
        if (figure.getVisible()) {

            /**
             * Set the current {@see ColorMap}.
             */
            colorMap = figure.getColorMap();

            drawingTools.clear(ColorFactory.createColor(colorMap, figure.getBackground()));
            drawingTools.clearDepthBuffer();

            askAcceptVisitor(figure.getChildren());
        }
    }

    @Override
    public void visit(final Grayplot grayplot) {
        if (grayplot.getVisible()) {
            Geometry triangles = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.TRIANGLES;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(grayplot.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    return dataManager.getColorBuffer(grayplot.getIdentifier());
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getIndexBuffer(grayplot.getIdentifier());
                    return indices;
                }

                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    return FaceCullingMode.BOTH;
                }
            };

            Appearance trianglesAppearance = new Appearance();
            drawingTools.draw(triangles, trianglesAppearance);
        }
    }

    @Override
    public void visit(final Matplot matplot) {
        if (matplot.getVisible()) {
            Geometry triangles = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.TRIANGLES;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(matplot.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    return dataManager.getColorBuffer(matplot.getIdentifier());
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getIndexBuffer(matplot.getIdentifier());
                    return indices;
                }
                
                @Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    return FaceCullingMode.BOTH;
                }
            };

            Appearance trianglesAppearance = new Appearance();
            drawingTools.draw(triangles, trianglesAppearance);
        }
    }

    @Override
    public void visit(Label label) {
        // TODO
        System.out.println("How can I draw a label ?");
    }

    @Override
    public void visit(Legend legend) {
        // TODO
        System.out.println("How can I draw a legend ?");
    }

    @Override
    public void visit(final Polyline polyline) {
        if (polyline.getVisible()) {

            Geometry triangles = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.TRIANGLES;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(polyline.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    return null;
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getIndexBuffer(polyline.getIdentifier());
                    return indices;
                }
                
                @Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    return FaceCullingMode.BOTH;
                }
            };

            Geometry geometry = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.SEGMENTS;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(polyline.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    return null;
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getWireIndexBuffer(polyline.getIdentifier());
                    System.out.println("indices size : " + indices.getSize());
                    return indices;
                }

                @Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    return FaceCullingMode.BOTH;
                }
            };


            Appearance trianglesAppearance = new Appearance();
            trianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, polyline.getBackground()));

            drawingTools.draw(triangles, trianglesAppearance);

            Appearance appearance = new Appearance();
            appearance.setLineColor(ColorFactory.createColor(colorMap, polyline.getLineColor()));
            appearance.setLineWidth(polyline.getLineThickness().floatValue());
            appearance.setLinePattern(polyline.getLineStyleAsEnum().asPattern());


            drawingTools.draw(geometry, appearance);


            if (polyline.getMarkMode()) {
                Sprite sprite = markManager.getMarkSprite(polyline, colorMap);
                ElementsBuffer positions = dataManager.getVertexBuffer(polyline.getIdentifier());  // TODO : getMarkVertexBuffer
                drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
            }
        }
    }

    @Override
    public void visit(Rectangle rectangle) {
        // TODO
        System.out.println("How can I draw a rectangle ?");
    }

    /*
     * To do:
     * -use common code for both the Fac3d and Plot3d visit methods
     *  as they are mostly similar.
     */
    @Override
    public void visit(final Fac3d fac3d) {
        if (fac3d.getVisible()) {

            Geometry triangles = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.TRIANGLES;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(fac3d.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    if (fac3d.getColorFlag() >= 1) {
                        return dataManager.getColorBuffer(fac3d.getIdentifier());
                    }
                    else
                    {
                        return null;
                    }
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getIndexBuffer(fac3d.getIdentifier());
                    return indices;
                }
                
                @Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    if (fac3d.getHiddenColor() > 0) {
                        return FaceCullingMode.CW;
                    }
                    else {
                        return FaceCullingMode.BOTH;
                    }
                }
            };

            Geometry backTriangles = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.TRIANGLES;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(fac3d.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    return null;
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getIndexBuffer(fac3d.getIdentifier());
                    return indices;
                }

                @Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    return FaceCullingMode.CCW;
                }
            };

            Geometry wireframe = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.SEGMENTS;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(fac3d.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    return null;
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getWireIndexBuffer(fac3d.getIdentifier());
                    return indices;
                }
                
                @Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    return FaceCullingMode.BOTH;
                }
            };

            if (fac3d.getSurfaceMode()) {
                if (fac3d.getColorMode() != 0) {
                    /* Back-facing triangles */
                    if (fac3d.getHiddenColor() > 0) {
                        Appearance backTrianglesAppearance = new Appearance();
                        backTrianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, fac3d.getHiddenColor()));
                        drawingTools.draw(backTriangles, backTrianglesAppearance);
                    }

                    /* Front-facing triangles */
                    Appearance trianglesAppearance = new Appearance();

                    if (fac3d.getColorFlag() == 0) {
                        trianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, Math.abs(fac3d.getColorMode())));
                    }

                    drawingTools.draw(triangles, trianglesAppearance);
                }

                if (fac3d.getColorMode() >= 0 && fac3d.getLineThickness() > 0.0) {
                    Appearance wireframeAppearance = new Appearance();
                    wireframeAppearance.setLineColor(ColorFactory.createColor(colorMap, fac3d.getLineColor()));
                    wireframeAppearance.setLineWidth(fac3d.getLineThickness().floatValue());
                    drawingTools.draw(wireframe, wireframeAppearance);
                }
            }

            if (fac3d.getMarkMode()) {
                Sprite sprite = markManager.getMarkSprite(fac3d, colorMap);
                ElementsBuffer positions = dataManager.getVertexBuffer(fac3d.getIdentifier());  // TODO : getMarkVertexBuffer
                drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
            }

        }

    }

    @Override
    public void visit(final Plot3d plot3d) {
        if (plot3d.getVisible()) {

            Geometry triangles = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.TRIANGLES;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(plot3d.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    if (plot3d.getColorFlag() == 1) {
                        return dataManager.getColorBuffer(plot3d.getIdentifier());
                    }
                    else
                    {
                        return null;
                    }
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getIndexBuffer(plot3d.getIdentifier());
                    return indices;
                }
                
                @Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    if (plot3d.getHiddenColor() > 0) {
                        return FaceCullingMode.CW;
                    }
                    else {
                        return FaceCullingMode.BOTH;
                    }
                }
            };

            Geometry backTriangles = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.TRIANGLES;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(plot3d.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    return null;
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getIndexBuffer(plot3d.getIdentifier());
                    return indices;
                }

                @Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    return FaceCullingMode.CCW;
                }
            };

            Geometry wireframe = new Geometry() {
                @Override
                public DrawingMode getDrawingMode() {
                    return Geometry.DrawingMode.SEGMENTS;
                }

                @Override
                public ElementsBuffer getVertices() {
                    return dataManager.getVertexBuffer(plot3d.getIdentifier());
                }

                @Override
                public ElementsBuffer getColors() {
                    return null;
                }

                @Override
                public ElementsBuffer getNormals() {
                    return null;
                }

                @Override
                public IndicesBuffer getIndices() {
                    IndicesBuffer indices = dataManager.getWireIndexBuffer(plot3d.getIdentifier());
                    return indices;
                }
                
                @Override
                public IndicesBuffer getEdgesIndices() {
                    return null;
                }

                @Override
                public FaceCullingMode getFaceCullingMode() {
                    return FaceCullingMode.BOTH;
                }
            };

            if (plot3d.getSurfaceMode()) {
                if (plot3d.getColorMode() != 0) {

                    /* Back-facing triangles */
                    if (plot3d.getHiddenColor() > 0) {
                        Appearance backTrianglesAppearance = new Appearance();
                        backTrianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, plot3d.getHiddenColor()));
                        drawingTools.draw(backTriangles, backTrianglesAppearance);
                    }

                    /* Front-facing triangles */
                    Appearance trianglesAppearance = new Appearance();

                    if (plot3d.getColorFlag() == 0) {
                        trianglesAppearance.setFillColor(ColorFactory.createColor(colorMap, Math.abs(plot3d.getColorMode())));
                    }

                    drawingTools.draw(triangles, trianglesAppearance);
                }

                if (plot3d.getColorMode() >= 0 && plot3d.getLineThickness() > 0.0) {
                    Appearance wireframeAppearance = new Appearance();
                    wireframeAppearance.setLineColor(ColorFactory.createColor(colorMap, plot3d.getLineColor()));
                    wireframeAppearance.setLineWidth(plot3d.getLineThickness().floatValue());
                    drawingTools.draw(wireframe, wireframeAppearance);
                }
            }

            if (plot3d.getMarkMode()) {
                Sprite sprite = markManager.getMarkSprite(plot3d, colorMap);
                ElementsBuffer positions = dataManager.getVertexBuffer(plot3d.getIdentifier());  // TODO : getMarkVertexBuffer
                drawingTools.draw(sprite, SpriteAnchorPosition.CENTER, positions);
            }

        }

    }

    @Override
    public void visit(Text text) {
        if (text.getVisible()) {
            textManager.draw(drawingTools, colorMap, text);
        }
    }

    @Override
    public void visit(Arrow arrow) {
        // TODO
        System.out.println("How can I draw an arrow ?");
    }

    @Override
    public void visit(Champ champ) {
        // TODO
        System.out.println("How can I draw a champ ?");
    }

    @Override
    public void visit(Segs segs) {
        // TODO
        System.out.println("How can I draw a segs ?");
    }





    @Override
    public void updateObject(String id, String property) {
        if (GraphicObjectProperties.__GO_COLORMAP__.equals(property) && figure.getIdentifier().equals(id)) {
            dataManager.disposeAllColorBuffers();
            markManager.disposeAll();
            textManager.disposeAll();
            axesDrawer.disposeAll();
            canvas.redraw();
        } else if (isFigureChild(id)) {
            dataManager.update(id, property);
            markManager.update(id, property);
            textManager.update(id, property);
            axesDrawer.update(id, property);
            canvas.redraw();
        }
    }

    @Override
    public void createObject(String id) {
    }

    @Override
    public void deleteObject(String id) {
        dataManager.dispose(id);
        markManager.dispose(id);
        textManager.dispose(id);
        axesDrawer.dispose(id);
        canvas.redraw();
    }

    /**
     * Check if the given id correspond to a child of the current {@see Figure}.
     * @param id the given id.
     * @return true if the given id correspond to a child of the current {@see Figure}.
     */
    private boolean isFigureChild(String id) {
        String parentFigureID = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_PARENT_FIGURE__);
        return figure.getIdentifier().equals(parentFigureID);
    }
}

