/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.gui.graphicWindow;

import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_CHILDREN__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_TYPE__;
import static org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties.__GO_UICONTROL__;

import java.awt.BorderLayout;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.util.HashMap;

import javax.media.opengl.GLJPanel;
import javax.swing.JComponent;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvasFactory;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;
import org.scilab.modules.gui.bridge.pushbutton.SwingScilabPushButton;
import org.scilab.modules.gui.tab.ScilabTab;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.textbox.ScilabTextBox;
import org.scilab.modules.gui.textbox.TextBox;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * @author Pierre Lando
 */
public class FigureBridge implements GraphicView {

    private final Window frame = ScilabWindow.createWindow();
    private final GLJPanel glpanel = new GLJPanel();
    private final Tab tab = ScilabTab.createTab("scilab");
    private final TextBox infoTextBox = ScilabTextBox.createTextBox();
    private final String id;

    private final HashMap<String,SwingScilabPushButton> children = new HashMap<String, SwingScilabPushButton>();

    /**
     * Default constructor.
     * @param id the id of the figure represented by this frame.
     * @throws Exception when the given id is not a figure id.
     */
    private FigureBridge(final String id) throws Exception {
        Object type = GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);
        if (type instanceof String && type.equals("Figure")) {
            // TODO : use static Figure.type instead of "Figure".

            this.id = id;
            Object object = GraphicController.getController().getObjectFromId(id);

            if (object instanceof Figure) {
                final Figure figure = (Figure) object;

                Canvas rendererCanvas = JoGLCanvasFactory.createCanvas(glpanel);

                rendererCanvas.setMainDrawer(new DrawerVisitor(rendererCanvas, figure));

                /*
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
                */

                glpanel.addMouseListener(new MouseListener() {
                    MouseEvent beginEvent;
                    final MouseMotionListener mouseMotionListener = new MouseMotionListener() {

                        @Override
                        public void mouseDragged(MouseEvent e) {
                            int dx = e.getX() - beginEvent.getX();
                            int dy = e.getY() - beginEvent.getY();

                            String[] children = (String[]) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__);
                            for (String child : children) {
                                Double[] angles = (Double[]) GraphicController.getController().getProperty(child, GraphicObjectProperties.__GO_ROTATION_ANGLES__);
                                angles[0] -= dy / 4.0;
                                angles[1] -= dx / 4.0;
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
                            glpanel.addMouseMotionListener(mouseMotionListener);
                            beginEvent = e;
                        }
                    }

                    @Override
                    public void mouseReleased(MouseEvent e) {
                        if (e.getButton() == MouseEvent.BUTTON1) {
                            System.out.println("End");
                            glpanel.removeMouseMotionListener(mouseMotionListener);
                        }
                    }

                    @Override
                    public void mouseEntered(MouseEvent e) {}

                    @Override
                    public void mouseExited(MouseEvent e) {}
                });

                tab.setCallback(null);
                JComponent jComp = ((JComponent)tab.getAsSimpleTab());
                jComp.setLayout(new BorderLayout());
                jComp.add(glpanel, BorderLayout.CENTER);
                tab.addInfoBar(infoTextBox);

                frame.addTab(tab);

                GraphicController.getController().register(this);
                frame.setVisible(true);
                tab.setVisible(true);
                glpanel.setVisible(true);
                updateGUI();
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
        if (object != null && (object instanceof Figure)) {
            Figure figure = (Figure) object;

            String name = figure.getName();
            Integer figureId = figure.getId();
            if ((name != null) && (figureId != null)) {
                name = name.replaceFirst("%d", figureId.toString());
                tab.setName(name);
            }

            String infoMessage = figure.getInfoMessage();
            if ((infoMessage == null) || (infoMessage.length()==0)) {
                infoTextBox.setText("");
            } else {
                infoTextBox.setText(infoMessage);
            }
        }
    }

    @Override
    public void updateObject(String id, String property) {
        System.out.println("[UPDATE] "+id+ ((String) GraphicController.getController().getProperty(id, __GO_TYPE__))+" Property = "+property);       
        
        /*
         * Check if someone is not adding me a child
         */
        if (id != null && id.equals(this.id) && property.equals(__GO_CHILDREN__))
        {
            
            String[] allChildren =  (String []) GraphicController.getController().getProperty(id,__GO_CHILDREN__);
            
            for (int i = 0; i < allChildren.length ; ++i) {
                   if (!children.containsKey(allChildren[i])) {                        
                       
                       String childType = (String) GraphicController.getController().getProperty(allChildren[i],__GO_TYPE__);
                       
                       if (childType.equals(__GO_UICONTROL__)) {
                           System.out.println("[!!!!!] I Have a new Uicontrol Child !!!");
                           SwingScilabPushButton button = new SwingScilabPushButton();
                           button.setText("Hello...");
                           button.setVisible(true);
                           button.setDims(new Size(200, 200));
                           button.setPosition(new Position(0, 0));
                       
                           children.put(allChildren[i], button);
                           //((SwingScilabTab) tab.getAsSimpleTab()).addMember(button);
                           glpanel.add(button);
                       }
                       else {
                          children.put(allChildren[i],null);
                       }
                   }
               }
            }
        
        if(children.containsKey(id))
        {
            updateGUI();
        }
    }

    @Override
    public void createObject(String id) {
        System.out.println("[CREATE] " + id + " "+((String) GraphicController.getController().getProperty(id, __GO_TYPE__)));
    }

    @Override
    public void deleteObject(String id) {
            System.out.println("[DELETE] " + id + " " + ((String) GraphicController.getController().getProperty(id, __GO_TYPE__)));
    }

    public static FigureBridge createFigure(String id) {
        try {
            return new FigureBridge(id);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }
}
