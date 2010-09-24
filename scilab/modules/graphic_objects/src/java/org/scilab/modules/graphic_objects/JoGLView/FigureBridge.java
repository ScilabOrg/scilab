package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.forge.scirenderer.impl.opengl.renderer.simple.SimpleGLRenderer;
import org.scilab.modules.graphic_objects.JoGLView.nativegl.NativeGL;
import org.scilab.modules.graphic_objects.JoGLView.renderer.RendererFactory;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;

import javax.media.opengl.GLCanvas;
import javax.swing.*;
import java.awt.*;

/**
 * @author Pierre Lando
 */
public class FigureBridge implements GraphicView {
    private final static String DEFAULT_INFO_MESSAGE = " ";


    private final String id;
    private final JFrame frame = new JFrame();
    private final GLCanvas canvas = new GLCanvas();
    private JLabel infoMessageLabel = new JLabel(DEFAULT_INFO_MESSAGE);


    private FigureBridge(String id) throws Exception {
        Object type = GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
        if (type instanceof String && type.equals("Figure")) {
            // TODO : use static Figure.type instead of "Figure".

            this.id = id;

            SimpleGLRenderer renderer = RendererFactory.getRenderer(canvas);
            renderer.setSceneTree(new SciSceneTree(id));

            JPanel jContentPane = new JPanel(new BorderLayout());
            jContentPane.add(canvas, BorderLayout.CENTER);
            jContentPane.add(infoMessageLabel, BorderLayout.SOUTH);

            GraphicController.getController().register(this);

            updateGUI();

            frame.add(jContentPane);
            frame.setSize(900, 750);
            frame.setVisible(true);
        } else {
            throw new Exception("this id is not a figure");
        }
    }

    /**
     * This method update the figure name showed on the frame.
     * For compatibility, it replace the first occurrence of "%d" by the figure id.  
     */
    private void updateGUI() {

        Object object = GraphicController.getController().getObjectFromId(id);
        if (object instanceof Figure) {
            Figure figure = (Figure) object;

            String name = figure.getName();
            Integer figureId = figure.getId();
            if (name != null) {
                if (figureId == null) {
                    figureId = -1;
                }
                name = name.replaceFirst("%d", figureId.toString());
            }
            frame.setTitle(name);

            String infoMessage = figure.getInfoMessage();
            if ((infoMessage == null) || (infoMessage.length()==0)) {
                infoMessageLabel.setText(DEFAULT_INFO_MESSAGE);
            } else {
                infoMessageLabel.setText(infoMessage);
            }
        }
    }

    public void updateObject(String id, String property) {
        if (id != null && id.equals(this.id)) {
            updateGUI();
        }        

        if (property != null && property.equals(GraphicObjectProperties.__GO_DATA_MODEL__)) {
            NativeGL.updateObject(id);
        }

        canvas.display();
    }

    @Override
    public void createObject(String id) {}

    @Override
    public void deleteObject(String id) {}

    public static FigureBridge createFigure(String id) {
        try {
            return new FigureBridge(id);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
