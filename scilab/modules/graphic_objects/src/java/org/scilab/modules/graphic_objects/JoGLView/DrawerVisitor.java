package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.forge.scirenderer.canvas.interfaces.appearance.material.color.IColor;
import org.scilab.forge.scirenderer.canvas.interfaces.canvas.ICanvas;
import org.scilab.forge.scirenderer.canvas.interfaces.geometry.FinalRectangle;
import org.scilab.modules.graphic_objects.JoGLView.sciCanvas.ISciBuffersManager;
import org.scilab.modules.graphic_objects.JoGLView.sciCanvas.IScilabCanvas;
import org.scilab.modules.graphic_objects.arc.Arc;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axis.Axis;
import org.scilab.modules.graphic_objects.compound.Compound;
import org.scilab.modules.graphic_objects.fec.Fec;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;
import org.scilab.modules.graphic_objects.imageplot.Grayplot;
import org.scilab.modules.graphic_objects.imageplot.Matplot;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.legend.Legend;
import org.scilab.modules.graphic_objects.polyline.Polyline;
import org.scilab.modules.graphic_objects.rectangle.Rectangle;
import org.scilab.modules.graphic_objects.surface.Fac3d;
import org.scilab.modules.graphic_objects.surface.Plot3d;
import org.scilab.modules.graphic_objects.textObject.Text;
import org.scilab.modules.graphic_objects.textObject.TextObject;
import org.scilab.modules.graphic_objects.vectfield.Arrow;
import org.scilab.modules.graphic_objects.vectfield.Champ;
import org.scilab.modules.graphic_objects.vectfield.Segs;

/**
 * @author Pierre Lando
 */
public class DrawerVisitor implements IVisitor {
    private final IScilabCanvas canvas;

    private ColorMap colorMap;

    public DrawerVisitor(IScilabCanvas canvas) {
        this.canvas = canvas;
    }

    public ICanvas getCanvas() {
        return canvas;
    }

    private void askChildrenAcceptVisitor(String[] ids) {
        for (String id : ids) {
            GraphicObject object = GraphicController.getController().getObjectFromId(id);
            object.accept(this);
        }
    }

    @Override
    public void visit(Axes axes) {
        if (axes.getVisible()) {
            // Set current zone
            /**********************************************************************************************************/
            Double[] axesBounds = axes.getAxesBounds();
            Double[] margins = axes.getMargins();

            double x = (axesBounds[0] + axesBounds[2] * margins[0]);
            double y = (1.0 - axesBounds[1] - axesBounds[3] * (1.0 - margins[3]));
            double w = (1 - margins[0] - margins[1]) * axesBounds[2];
            double h = (1 - margins[2] - margins[3]) * axesBounds[3];

            canvas.setCurrentZone(new FinalRectangle(x+w/2, y+h/2, w, h));
            /**********************************************************************************************************/

            // Draw background
            // TODO : remove, draw a cube with 'back face culling'
            /**********************************************************************************************************/
            int background = axes.getBackground();
            IColor clearColor = colorMap.getScilabColor(background);
            canvas.clearZone(clearColor);
            canvas.clearZBuffer();
            /**********************************************************************************************************/

            askChildrenAcceptVisitor(axes.getChildren());
        }
    }

    @Override
    public void visit(Arc arc) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Axis axis) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Compound compound) {
        if (compound.getVisible()) {
            askChildrenAcceptVisitor(compound.getChildren());
        }
    }

    @Override
    public void visit(Fec fec) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Figure figure) {
        if (figure.getVisible()) {
            colorMap = figure.getColorMap();
            int background = figure.getBackground();
            IColor clearColor = colorMap.getScilabColor(background);

            canvas.setCoordinate(new FinalRectangle(.5,.5,1,1));
            canvas.setCurrentZone(new FinalRectangle(.5,.5,1,1));
            canvas.clearZone(clearColor);
            canvas.clearZBuffer();

            askChildrenAcceptVisitor(figure.getChildren());
        }
    }

    @Override
    public void visit(Grayplot grayplot) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Matplot matplot) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Label label) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Legend legend) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Polyline polyline) {

        IColor frontColor = colorMap.getScilabColor(polyline.getLineColor()); 
        canvas.getAppearanceControl().getMaterial().setDiffuseColor(frontColor);

        float lineWidth = polyline.getLineThickness().floatValue();
        canvas.getAppearanceControl().getLineProperties().setLineWidth(lineWidth);

        short pattern = polyline.getLineStyleAsEnum().asPattern();
        canvas.getAppearanceControl().getLineProperties().setLinePattern(pattern);


        ISciBuffersManager buffersManager = canvas.getBuffersManager();
        buffersManager.setVertexBuffer(polyline);


        //NativeGL.loadGLData(polyline.getIdentifier());
    }

    @Override
    public void visit(Rectangle rectangle) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Fac3d fac3d) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Plot3d plot3d) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Text text) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(TextObject textObject) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Arrow arrow) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Champ champ) {
        //To change body of implemented methods use File | Settings | File Templates.
    }

    @Override
    public void visit(Segs segs) {
        //To change body of implemented methods use File | Settings | File Templates.
    }
}
