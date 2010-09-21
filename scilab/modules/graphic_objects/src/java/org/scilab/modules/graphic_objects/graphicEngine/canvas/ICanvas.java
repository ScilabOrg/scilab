/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.canvas;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.IDrawersManager;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.IRenderingZoneController;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;

/**
 * Author: Lando Pierre
 * Date: 18 mai 2010
 *
 * Interface for a frame, the frame is where the rendering is performed.
 * The frame contain a drawer manager and a data model.
 *
 */
public interface ICanvas<drawersManagerType extends IDrawersManager> {

    /**
     * Set the renderingZone of this frame.
     * @param renderingZone is the new renderingZone of this frame.
     * //TODO add more rendering zone.
     */
    void setRenderingZone(IRenderingZoneController renderingZone);

    /**
     * @return the renderingZone of this frame.
     */ // TODO add more rendering zone, get by location. 
    IRenderingZoneController getRenderingZone();

    /**
     * Set the drawer manager of this frame.
     * @param drawerManager is the new drawer manager of this frame.
     */
    void setDrawersManager(drawersManagerType drawerManager);

    /**
     * @return the drawer manager of this frame.
     */
    drawersManagerType getDrawersManager();

    /**
     * @return the width of this frame.
     */
    int getWidth();

    /**
     * @return the height of this frame.
     */
    int getHeight();

    /**
     * Adds the specified mouse listener to receive mouse events from this component. If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse listener.
     */
    void addMouseListener(MouseListener l);

    /**
     * Removes the specified mouse listener so that it no longer receives mouse events from this component. This method performs no function, nor does it throw an exception, if the listener specified by the argument was not previously added to this component. If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse listener.
     */
    void removeMouseListener(MouseListener l);

    /**
     * Returns an array of all the mouse listeners registered on this component.
     * @return all of this component's MouseListeners or an empty array if no mouse listeners are currently registered
     */
    MouseListener[] getMouseListeners();

    /**
     * Adds the specified mouse motion listener to receive mouse motion events from this component. If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse motion listener.
     */
    void addMouseMotionListener(MouseMotionListener l);

    /**
     * Removes the specified mouse motion listener so that it no longer receives mouse motion events from this component. This method performs no function, nor does it throw an exception, if the listener specified by the argument was not previously added to this component. If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse motion listener.
     */
    void removeMouseMotionListener(MouseMotionListener l);

    /**
     * Returns an array of all the mouse motion listeners registered on this component.
     * @return all of this component's MouseMotionListeners or an empty array if no mouse motion listeners are currently registered
     */
    MouseMotionListener[] getMouseMotionListeners();

    /**
     * Adds the specified mouse wheel listener to receive mouse wheel events from this component. If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse wheel listener.
     */
    void addMouseWheelListener(MouseWheelListener l);

    /**
     * Removes the specified mouse wheel listener so that it no longer receives mouse wheel events from this component. This method performs no function, nor does it throw an exception, if the listener specified by the argument was not previously added to this component. If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse wheel listener.
     */
    void removeMouseWheelListener(MouseWheelListener l);

    /**
     * Returns an array of all the mouse wheel listeners registered on this component.
     * @return all of this component's MouseWheelListeners or an empty array if no mouse wheel listeners are currently registered
     */
    MouseWheelListener[] getMouseWheelListeners();

    /**
     * @param x the x coordinate in pixel.
     * @param y the y coordinate in pixel.
     * @return the underlying info at the given coordinate.
     */
    ILocationInfo getLocationInfo(int x, int y);

    /**
     * @param e the mouse event.
     * @return the underlying info at the given mouse event.
     */
    ILocationInfo getLocationInfo(MouseEvent e);

    /**
     * Notify the canvas that the data model content need to be redraw.
     */
    void display();
}
