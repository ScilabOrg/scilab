package org.scilab.modules.graphic_objects.JoGLView;

import org.scilab.modules.graphic_objects.JoGLView.nativegl.NativeGL;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.core.GraphicTreeController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.core.IGraphicTreeController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.AbstractGraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.IGraphicModel;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.navigation.CameraController;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCanvas.GLCanvasImpl;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.graphic_objects.util.FACTORY.AbstractFactoryManager;
import org.scilab.modules.graphic_objects.util.FACTORY.IFactory;

import javax.swing.*;
import java.awt.*;

/**
 * Author: lando
 * Date: 20 sept. 2010
 */
public class FigureBridge implements SciBridge, GraphicView {
    private final String id;
    private JFrame frame;
    private Figure figure = null;

    private GLCanvasImpl canvas;
    private GraphicTreeController graphicTree;


    private FigureBridge(String id) {
        this.id = id;

        canvas = new GLCanvasImpl();

        Object object = GraphicController.getController().getObjectFromId(id);
        if (object instanceof Figure) {
            GraphicController.getController().register(this);

            figure = (Figure) object;

            frame = new JFrame();
            frame.setSize(900, 750);
            frame.setVisible(true);

            canvas = new GLCanvasImpl();
            graphicTree = new GraphicTreeController(new GraphicTreeModel(id));
            canvas.getRenderingZone().setDataController(graphicTree);
            canvas.getRenderingZone().setCamera(new CameraController());

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

        if (graphicTree != null) {
            graphicTree.endUpdate();
        }
    }

    public void createObject(String id) {

    }

    public void deleteObject(String id) {
        // TODO.
    }


    public static IFactory<FigureBridge> getFactory() {
        return singleton;
    } 

    private static IFactory<FigureBridge> singleton = new IFactory<FigureBridge> () {

        // TODO remove that.
        public FigureBridge createStuff() {
            return null;
        }

        public FigureBridge createStuff(Object object) {
            if (object instanceof String) {
                return new FigureBridge((String) object);
            } else {
                // TODO : ??
                return null;
            }
        }
    };
}
