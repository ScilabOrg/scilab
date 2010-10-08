package org.scilab.modules.renderer.JoGLView;

import org.scilab.forge.scirenderer.canvas.glimpl.canvas.AbstractGLCanvas;
import org.scilab.forge.scirenderer.canvas.glimpl.canvas.IGLCanvas;
import org.scilab.modules.graphic_objects.NativeGL;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicObject.IVisitor;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;

import javax.media.opengl.GLCanvas;
import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;

/**
 * @author Pierre Lando
 */
public class FigureBridge implements GraphicView {
    private final static String DEFAULT_INFO_MESSAGE = " ";


    private final String id;
    private final JFrame frame = new JFrame();
    private final GLCanvas canvas = new GLCanvas();
    private final JLabel infoMessageLabel = new JLabel(DEFAULT_INFO_MESSAGE);


    private FigureBridge(final String id) throws Exception {
        Object type = GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
        if (type instanceof String && type.equals("Figure")) {
            // TODO : use static Figure.type instead of "Figure".

            this.id = id;
            Object object = GraphicController.getController().getObjectFromId(id);

            if (object instanceof Figure) {
                final Figure figure = (Figure) object;
                IGLCanvas page = new AbstractGLCanvas(canvas) {
                    private IVisitor visitor = null;
                    
                    @Override
                    public void performDraw() {
                        figure.accept(getVisitor());
                    }

                    private IVisitor getVisitor() {
                        if (visitor == null) {
                            visitor = new DrawerVisitor(this);
                        }
                        return visitor;
                    }
                };

                canvas.addMouseListener(new MouseListener() {
                    MouseEvent beginEvent;
                    final MouseMotionListener mouseMotionListener = new MouseMotionListener() {

                        @Override
                        public void mouseDragged(MouseEvent e) {
                            int dx = e.getX() - beginEvent.getX();
                            int dy = e.getY() - beginEvent.getY();

                            String[] children = (String[]) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__);
                            for (String child : children) {
                                Double[] angles = (Double[]) GraphicController.getController().getProperty(child, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
                                angles[0] += dy / 4.0;
                                angles[1] += dx / 4.0;
                                GraphicController.getController().setProperty(child, GraphicObjectProperties.__GO_ROTATION_ANGLES__, angles);
                            }

                            beginEvent = e;
                        }

                        @Override
                        public void mouseMoved(MouseEvent e) {}
                    };

                    @Override
                    public void mouseClicked(MouseEvent e) {}

                    @Override
                    public void mousePressed(MouseEvent e) {
                        if (e.getButton() == MouseEvent.BUTTON1) {
                            System.out.println("Begin");
                            canvas.addMouseMotionListener(mouseMotionListener);
                            beginEvent = e;
                        }
                    }

                    @Override
                    public void mouseReleased(MouseEvent e) {
                        if (e.getButton() == MouseEvent.BUTTON1) {
                            System.out.println("End");
                            canvas.removeMouseMotionListener(mouseMotionListener);
                        }
                    }

                    @Override
                    public void mouseEntered(MouseEvent e) {}

                    @Override
                    public void mouseExited(MouseEvent e) {}
                });

                JPanel jContentPane = new JPanel(new BorderLayout());
                jContentPane.add(canvas, BorderLayout.CENTER);
                jContentPane.add(infoMessageLabel, BorderLayout.SOUTH);

                GraphicController.getController().register(this);

                updateGUI();

                frame.add(jContentPane);
                frame.setSize(900, 750);
                frame.setVisible(true);
            }
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

        if (id != null && property != null && property.equals(GraphicObjectProperties.__GO_DATA_MODEL__)) {
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
