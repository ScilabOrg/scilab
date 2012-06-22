/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;


import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.*;


import org.scilab.modules.gui.editor.Editor;
import org.scilab.modules.gui.editor.PolylineHandler;

import java.awt.MouseInfo;

import java.util.ArrayList;

import org.scilab.modules.gui.datatip.*;

/**
* Event listener for the figure editor.
*
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/

public class EditorEventListener implements KeyListener, MouseListener, MouseMotionListener {

    String windowUid;
    Editor editor;
    boolean isInRotation = false;
    public static String picked;
    public static ArrayList<String> PolylineUIDRedraw = new ArrayList<String>();
    public static ArrayList<Integer> CoordXRedraw = new ArrayList<Integer>();
    public static ArrayList<Integer> CoordYRedraw = new ArrayList<Integer>();
    public static int NumberCreatedDatatips = 0;

    public EditorEventListener(String uid) {
        windowUid = uid;
        editor = new Editor();
        editor.setFigure(uid);
    }

    public void keyPressed(KeyEvent keyEvent) {
    }

    public void keyReleased(KeyEvent arg0) {
        /* TODO add copy/cut/paste by keyboard shortcut*/
        if (arg0.isControlDown()) {
            if (arg0.getKeyCode() == KeyEvent.VK_C) {
                if (editor.getSelected() != null) {

                }
            }
        }
    }

    public void keyTyped(KeyEvent arg0) {
    }

    public void mouseClicked(MouseEvent arg0) {
    }

    public void mouseEntered(MouseEvent arg0) {
    }

    public void mouseExited(MouseEvent arg0) {
    }

    /**
    * On left mouse press: check if the user clicked over
    * a polyline and set it selected.
    * On right mouse press: pass event to editor to make
    * the popup menu visible.
    * @param arg0 MouseEvent
    */
    public void mousePressed(MouseEvent arg0) {
        if (arg0.getButton() == 1) {
            EntityPicker ep = new EntityPicker();
            String picked = ep.pick( windowUid, arg0.getX(), arg0.getY() );
            editor.setSelected(picked);
            int XCoord = MouseInfo.getPointerInfo().getLocation().x;
            int YCoord = MouseInfo.getPointerInfo().getLocation().y;
            if (picked != null) {
                new DatatipCreate (XCoord, YCoord, picked);
                PolylineUIDRedraw.add(picked);
                CoordXRedraw.add(XCoord);
                CoordYRedraw.add(YCoord);
                NumberCreatedDatatips++;
            }
        } else if (arg0.getButton() == 3) {
            new DatatipContextMenu ();
        }
    }

    /**
    * On right mouse release: pass event to editor to make
    * the popup menu visible.
    * @param arg0 MouseEvent
    */
    public void mouseReleased(MouseEvent arg0) {
        if (arg0.getButton() == 3) {
            if (!isInRotation) {
                //editor.onMouseClick(arg0);
            }
        }
        isInRotation = false;
    }

    public void mouseDragged(MouseEvent arg0) {
        isInRotation = true;
    }

    public void mouseMoved(MouseEvent arg0) {
    }

}

