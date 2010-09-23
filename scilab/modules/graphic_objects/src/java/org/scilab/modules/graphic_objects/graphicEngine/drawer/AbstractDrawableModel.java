package org.scilab.modules.graphic_objects.graphicEngine.drawer;

import javax.swing.event.EventListenerList;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;

/**
 * Author: Lando Pierre
 * Date: june 24, 2010
 *
 * This class implements common methods of drawable model.  
 *
 */
public abstract class AbstractDrawableModel implements IDrawable {
    private final EventListenerList listeners = new EventListenerList();

    
    public void addMouseListener(MouseListener listener) {
        listeners.add(MouseListener.class, listener);
    }

    
    public void removeMouseListener(MouseListener listener) {
        listeners.remove(MouseListener.class, listener);
    }

    
    public MouseListener[] getMouseListeners() {
        return listeners.getListeners(MouseListener.class);
    }

    
    public void addMouseMotionListener(MouseMotionListener listener) {
        listeners.add(MouseMotionListener.class, listener);
    }

    
    public void removeMouseMotionListener(MouseMotionListener listener) {
        listeners.remove(MouseMotionListener.class, listener);
    }

    
    public MouseMotionListener[] getMouseMotionListeners() {
        return listeners.getListeners(MouseMotionListener.class);
    }

    
    public void addMouseWheelListener(MouseWheelListener listener) {
        listeners.add(MouseWheelListener.class, listener);
    }

    
    public void removeMouseWheelListener(MouseWheelListener listener) {
        listeners.remove(MouseWheelListener.class, listener);
    }

    
    public MouseWheelListener[] getMouseWheelListeners() {
        return listeners.getListeners(MouseWheelListener.class);
    }
}
