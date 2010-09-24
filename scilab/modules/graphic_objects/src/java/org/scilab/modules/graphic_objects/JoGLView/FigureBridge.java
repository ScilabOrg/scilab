package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.modules.graphic_objects.JoGLView.nativegl.NativeGL;
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
    private final String id;
    private JFrame frame;
    private Figure figure = null;

    private GLCanvas canvas;


    private FigureBridge(String id) {
        this.id = id;

        Object object = GraphicController.getController().getObjectFromId(id);
        if (object instanceof Figure) {
            canvas = new GLCanvas();
            GraphicController.getController().register(this);

            figure = (Figure) object;

            frame = new JFrame();
            frame.setSize(900, 750);
            frame.setVisible(true);

            canvas = new GLCanvas();
            SciSceneTree sceneTree = new SciSceneTree(id);

            //renderer = new SimpleGLRenderer(canvas);
            //renderer.setSceneTree(sceneTree);

            JPanel jContentPane = new JPanel(new BorderLayout());
            jContentPane.add(canvas, BorderLayout.CENTER);
            frame.add(jContentPane);

            updateObject(id, null); // TRICHEUR
        } else {
            // TODO
        }
    }

    public void updateObject(String id, String property) {
        if (id.equals(this.id)) {
            frame.setTitle(figure.getName());
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
        return new FigureBridge(id);
    }
}
