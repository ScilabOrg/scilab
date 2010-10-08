package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.forge.scirenderer.canvas.interfaces.appearance.material.color.IColor;
import org.scilab.forge.scirenderer.canvas.interfaces.canvas.ICanvas;
import org.scilab.forge.scirenderer.canvas.interfaces.geometry.FinalBox;
import org.scilab.forge.scirenderer.canvas.interfaces.geometry.FinalRectangle;
import org.scilab.forge.scirenderer.canvas.interfaces.tranformations.*;
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
    private static final double DEFAULT_ALPHA = 0.0;
    private static final double DEFAULT_THETA = 270.0;


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

            canvas.getZoneControl().setCurrentZone(new FinalRectangle(x+w/2, y+h/2, w, h));
            /**********************************************************************************************************/

            // Draw background
            // TODO : remove, draw a cube with 'back face culling'
            /**********************************************************************************************************/
            int background = axes.getBackground();
            IColor clearColor = colorMap.getScilabColor(background);
            canvas.getZoneControl().clearZone(clearColor);
            canvas.getZoneControl().clearZBuffer();
            /**********************************************************************************************************/


            // Compute transformation matrix.
            /**
             * Data -> centered data (translation)
             * centered data -> scaled data (scale, depend on cube scaling)
             * scaled data -> rotated data
             * rotated data -> final 'in cube' data (scale, depend on isoview).
             * set cube good ratio ? 
             */
            /**********************************************************************************************************/

            // TODO : a vérifer. en fait FRect c'est qui ?
            Double[] bounds = axes.getDataBounds();
            System.out.println("Bounds : " + bounds[0] + ", " + bounds[1] + ", " + bounds[2] + ", " + bounds[3] + ", " + bounds[4] + ", " + bounds[5]);

            double alpha = axes.getRotationAngles()[0];
            double theta = axes.getRotationAngles()[1];
            double scaleX;
            double scaleY;
            double scaleZ;
            
            ITransformation tmp;

            double ratio = canvas.getZoneControl().getFinalAspectRatio();
            double nRatio = 1;
            double dRatio = 1;
            if (ratio < 1.0) {
                dRatio = ratio;
            } else {
                nRatio = 1/ratio;
            }

            tmp = new Scale(nRatio, dRatio, 1);

            tmp.times(new Rotation(DEFAULT_ALPHA - alpha, 1.0, 0.0, 0.0));
            tmp.times(new Rotation(DEFAULT_THETA - theta, 0.0, 0.0, 1.0));

            if (!axes.getCubeScaling()) {
                scaleX = (bounds[1] - bounds[0]);
                scaleY = (bounds[3] - bounds[2]);
                scaleZ = (bounds[5] - bounds[4]);
                tmp.times(new Scale(scaleX, scaleY, scaleZ));
            }

            double tmpv[] = tmp.getMatrix16dv();
            scaleX = 1 / (Math.abs(tmpv[0]) + Math.abs(tmpv[4]) + Math.abs(tmpv[8]));
            scaleY = 1 / (Math.abs(tmpv[1]) + Math.abs(tmpv[5]) + Math.abs(tmpv[9]));
            scaleZ = 1 / (Math.abs(tmpv[2]) + Math.abs(tmpv[6]) + Math.abs(tmpv[10]));

            if (axes.getIsoview()) {
                double minScale = Math.min(scaleX, scaleY);
                tmp = (new Scale(minScale, minScale, scaleZ)).times(tmp);
            } else {
                tmp = (new Scale(scaleX, scaleY, scaleZ)).times(tmp);
            }

            scaleX = 2/(bounds[1] - bounds[0]);
            scaleY = 2/(bounds[3] - bounds[2]);
            scaleZ = 2/(bounds[5] - bounds[4]);

            tmp.times(new Scale(scaleX, scaleY, scaleZ));

            tmp.times(new Translate(
               - (bounds[0] + bounds[1]) / 2.0,
               - (bounds[2] + bounds[3]) / 2.0,
               - (bounds[4] + bounds[5]) / 2.0
            ));

            tmp =   (new Scale(
                        axes.getAxes()[0].getReverse() ? -1 : 1,
                        axes.getAxes()[1].getReverse() ? -1 : 1,
                        axes.getAxes()[2].getReverse() ? -1 : 1
                    )).times(tmp);

            System.out.println(tmp);

            canvas.getTransformationsStack().push(tmp);

            canvas.getAppearanceControl().setFillLineMode(false, true);

            canvas.getTransformationsStack().push(new Translate(.5,.5,0));
            canvas.draw(new FinalBox(1, 1, 2));
            canvas.getTransformationsStack().pop();
            askChildrenAcceptVisitor(axes.getChildren());

            canvas.getTransformationsStack().pop();
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
            askChildrenAcceptVisitor(compound.getChildren());
        }
    }

    @Override
    public void visit(Fec fec) {
        // TODO
        System.out.println("How can I draw a fec ?");
    }

    @Override
    public void visit(Figure figure) {
        if (figure.getVisible()) {
            colorMap = figure.getColorMap();
            int background = figure.getBackground();
            IColor clearColor = colorMap.getScilabColor(background);

            canvas.setCoordinate(new FinalRectangle(.5,.5,1,1));
            canvas.getZoneControl().setCurrentZone(new FinalRectangle(.5,.5,1,1));
            canvas.getZoneControl().clearZone(clearColor);
            canvas.getZoneControl().clearZBuffer();

            askChildrenAcceptVisitor(figure.getChildren());
        }
    }

    @Override
    public void visit(Grayplot grayplot) {
        // TODO
        System.out.println("How can I draw a grayplot ?");
    }

    @Override
    public void visit(Matplot matplot) {
        // TODO
        System.out.println("How can I draw a matplot ?");
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
    public void visit(Polyline polyline) {

        canvas.getAppearanceControl().setFillLineMode(true, false);

        IColor frontColor = colorMap.getScilabColor(polyline.getLineColor());
        canvas.getAppearanceControl().getFillProperties().getMaterial().setDiffuseColor(frontColor);

        float lineWidth = polyline.getLineThickness().floatValue();
        canvas.getAppearanceControl().getLineProperties().setLineWidth(lineWidth);

        short pattern = polyline.getLineStyleAsEnum().asPattern();
        canvas.getAppearanceControl().getLineProperties().setLinePattern(pattern);


        ISciBuffersManager buffersManager = canvas.getBuffersManager();
        buffersManager.setVertexBuffer(polyline);
    }

    @Override
    public void visit(Rectangle rectangle) {
        // TODO
        System.out.println("How can I draw a rectangle ?");
    }

    @Override
    public void visit(Fac3d fac3d) {
        // TODO
        System.out.println("How can I draw a fac3d ?");
    }

    @Override
    public void visit(Plot3d plot3d) {
        // TODO
        System.out.println("How can I draw a plot3d ?");
    }

    @Override
    public void visit(Text text) {
        // TODO
        System.out.println("How can I draw a text ?");
    }

    @Override
    public void visit(TextObject textObject) {
        // TODO
        System.out.println("How can I draw a textObject ?");
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
}
