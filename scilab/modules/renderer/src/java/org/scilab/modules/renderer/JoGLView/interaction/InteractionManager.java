/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.interaction;

import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * @author Pierre Lando
 */
public class InteractionManager implements RubberBoxListener {

    /** The rubber box */
    private RubberBox rubberBox;

    /** The drag, zoom, rotate interaction manager */
    private final DragZoomRotateInteraction dragZoomRotateInteraction;

    /** Parent drawer visitor */
    private DrawerVisitor drawerVisitor;

    /**
     * Default constructor.
     * @param drawerVisitor parent drawer visitor.
     */
    public InteractionManager(DrawerVisitor drawerVisitor) {
        this.drawerVisitor = drawerVisitor;
        dragZoomRotateInteraction = new DragZoomRotateInteraction(drawerVisitor);
        dragZoomRotateInteraction.setEnable(true);
    }

    /**
     * Finalize method.
     * @exception Throwable the <code>Exception</code> raised by this method
     */
    public void finalize() throws Throwable {
        if (rubberBox != null) {
            rubberBox.removeListener(this);
        }
        super.finalize();
    }

    /**
     * Called to start zooming.
     */
    public void startInteractiveZoom() {
        dragZoomRotateInteraction.setEnable(false);
        rubberBox = new ZoomRubberBox(drawerVisitor);
        rubberBox.addListener(this);
        drawerVisitor.addPostRendering(rubberBox);
        rubberBox.setEnable(true);
    }

    public int[] startClickRubberBox() {
        final Object lock = new Object();
        dragZoomRotateInteraction.setEnable(false);
        rubberBox = new RubberBox(drawerVisitor);
        rubberBox.addListener(new RubberBoxListener() {
            @Override
            public void rubberBoxEnd() {
                synchronized (lock) {
                    lock.notify();
                }
                dragZoomRotateInteraction.setEnable(true);
                rubberBox = null;
            }
        });
        drawerVisitor.addPostRendering(rubberBox);
        rubberBox.setEnable(true);
        try {
            synchronized (lock) {
                lock.wait();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
    
    public int[] startDragRubberBox() {
        final Object lock = new Object();
        dragZoomRotateInteraction.setEnable(false);
        rubberBox = new RubberBox(drawerVisitor);
        rubberBox.addListener(new RubberBoxListener() {
            @Override
            public void rubberBoxEnd() {
                synchronized (lock) {
                    lock.notify();
                }
                dragZoomRotateInteraction.setEnable(true);
                rubberBox = null;
            }
        });
        drawerVisitor.addPostRendering(rubberBox);
        rubberBox.setEnable(true);
        try {
            synchronized (lock) {
                lock.wait();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }
    
    @Override
    public void rubberBoxEnd() {
        dragZoomRotateInteraction.setEnable(true);
        rubberBox = null;
    }
}
