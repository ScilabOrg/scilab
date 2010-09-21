package org.scilab.modules.graphic_objects.graphicEngine.drawer;

import org.scilab.modules.graphic_objects.util.MVC.AbstractDataController;

import javax.swing.event.EventListenerList;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;

/**
 * Author: Lando Pierre
 * Date: june 24, 2010
 *
 * This class implements common methods of drawable controller.
 */
public abstract class AbstractDrawableController<modelType extends IDrawable> extends AbstractDataController<modelType> implements IDrawable {
    private final EventListenerList listeners = new EventListenerList();
    
    protected AbstractDrawableController(modelType model) {
        super(model);
    }

    
    public void addMouseListener(MouseListener listener) {
        getModel().addMouseListener(listener);
    }

    
    public void removeMouseListener(MouseListener listener) {
        getModel().removeMouseListener(listener);
    }

    
    public MouseListener[] getMouseListeners() {
        return getModel().getMouseListeners();
    }

    
    public void addMouseMotionListener(MouseMotionListener listener) {
        getModel().addMouseMotionListener(listener);
    }

    
    public void removeMouseMotionListener(MouseMotionListener listener) {
        getModel().removeMouseMotionListener(listener);
    }

    
    public MouseMotionListener[] getMouseMotionListeners() {
        return getModel().getMouseMotionListeners();
    }

    
    public void addMouseWheelListener(MouseWheelListener listener) {
        getModel().addMouseWheelListener(listener);
    }

    
    public void removeMouseWheelListener(MouseWheelListener listener) {
        getModel().removeMouseWheelListener(listener);
    }

    
    public MouseWheelListener[] getMouseWheelListeners() {
        return getModel().getMouseWheelListeners();
    }
}
