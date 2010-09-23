/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.drawer;

import org.scilab.modules.graphic_objects.util.MVC.IDataModel;

import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;

/**
 * Author: Lando Pierre
 * Date: june 10, 2010
 *
 * Interface for drawable objects.
 *
 */
public interface IDrawable extends IDataModel {
    /**
     * @return the type of this drawable.
     */
    String getType();

    /**
     * Adds the specified mouse listener to receive mouse events from this drawable.
     * If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse listener.
     */
    void addMouseListener(MouseListener l);

    /**
     * Removes the specified mouse listener so that it no longer receives mouse events from this drawable.
     * This method performs no function, nor does it throw an exception, if the listener specified by the argument was not previously added to this drawable.
     * If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse listener.
     */
    void removeMouseListener(MouseListener l);

    /**
     * Returns an array of all the mouse listeners registered on this drawable.
     * @return all of this drawable's MouseListeners or an empty array if no mouse listeners are currently registered.
     */
    MouseListener[] getMouseListeners();

    /**
     * Adds the specified mouse motion listener to receive mouse motion events from this drawable.
     * If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse motion listener.
     */
    void addMouseMotionListener(MouseMotionListener l);

    /**
     * Removes the specified mouse motion listener so that it no longer receives mouse motion events from this drawable.
     * This method performs no function, nor does it throw an exception, if the listener specified by the argument was not previously added to this drawable.
     * If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse motion listener.
     */
    void removeMouseMotionListener(MouseMotionListener l);

    /**
     * Returns an array of all the mouse motion listeners registered on this drawable.
     * @return all of this drawable's MouseMotionListeners or an empty array if no mouse motion listeners are currently registered.
     */
    MouseMotionListener[] getMouseMotionListeners();


    /**
     * Adds the specified mouse wheel listener to receive mouse wheel events from this drawable.
     * If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse wheel listener.
     */
    void addMouseWheelListener(MouseWheelListener l);

    /**
     * Removes the specified mouse wheel listener so that it no longer receives mouse wheel events from this drawable.
     * This method performs no function, nor does it throw an exception, if the listener specified by the argument was not previously added to this drawable.
     * If listener l is null, no exception is thrown and no action is performed.
     * @param l the mouse wheel listener.
     */
    void removeMouseWheelListener(MouseWheelListener l);

    /**
     * Returns an array of all the mouse wheel listeners registered on this drawable.
     * @return all of this drawable's MouseWheelListeners or an empty array if no mouse wheel listeners are currently registered.
     */
    MouseWheelListener[] getMouseWheelListeners();
}
