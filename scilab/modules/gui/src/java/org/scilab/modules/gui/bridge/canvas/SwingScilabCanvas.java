/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 * Copyright (C) 2007 - INRIA - Marouane BEN JELLOUL
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.canvas;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.implementation.jogl.JoGLCanvasFactory;
import org.scilab.modules.commons.OS;
import org.scilab.modules.graphic_objects.figure.Figure;
import org.scilab.modules.gui.bridge.tab.SwingScilabAxes;
import org.scilab.modules.gui.canvas.SimpleCanvas;
import org.scilab.modules.gui.events.GlobalEventWatcher;
import org.scilab.modules.gui.graphicWindow.PanelLayout;
import org.scilab.modules.gui.utils.Position;
import org.scilab.modules.gui.utils.Size;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

import javax.media.opengl.GL;
import javax.media.opengl.GLCanvas;
import javax.media.opengl.GLJPanel;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.event.FocusListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.image.BufferedImage;

/**
 * Swing implementation for Scilab Canvas in GUIs This implementation requires
 * JOGL
 *
 * @author Vincent COUVERT
 * @author Marouane BEN JELLOUL
 * @author Jean-Baptiste Silvy
 */
public class SwingScilabCanvas extends JPanel implements SimpleCanvas {

	private static final long serialVersionUID = 6101347094617535625L;

    /** The renderer canvas */
    private final Canvas rendererCanvas;

    /** The drawn figure */
    private final Figure figure;

    /** The drawer visitor used to draw the figure */
    private final DrawerVisitor drawerVisitor;

    /** The drawable component where the draw is performed */
    private final Component drawableComponent;

    /*
      * Using GLJPanel for MacOSX may lead to a deadlock on deletion.
      * Wrap call to removeNotify to ensure we are not outside Swing Thread
      * and PBuffer is not locked.
      */
	private final class MacOSXGLJPanel extends GLJPanel
	{
        private static final long serialVersionUID = -6166986369022555750L;

        private void superRemoveNotify()
	    {
	        super.removeNotify();
	    }

	    @Override
	    public void removeNotify() {
	        final MacOSXGLJPanel panel = this;
	            SwingUtilities.invokeLater(new Runnable() {
	                    public void run() {
	                        panel.superRemoveNotify();
	                }
	            });
	        }
	}

    public SwingScilabCanvas(int figureId, final Figure figure)
	{
	    super(new PanelLayout());
        this.figure = figure;

	    /*
	     * Even with the good Java 1.6 version
	     * MacOSX does not manage mixing ligthweight and heavyweight components
	     * Use MacOSXGLJPanel as OpenGL component for now since GLJPanel will
	     * lead to deadlock on deletion.
	     */
	    if (OS.get() == OS.MAC) {
		    GLJPanel glCanvas = new MacOSXGLJPanel();
            drawableComponent = glCanvas;
		    glCanvas.setEnabled(true);
		    add(glCanvas, PanelLayout.GL_CANVAS);

            rendererCanvas = JoGLCanvasFactory.createCanvas(glCanvas);
            drawerVisitor = new DrawerVisitor(drawableComponent, rendererCanvas, figure);
            rendererCanvas.setMainDrawer(drawerVisitor);

            drawableComponent.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseEntered(MouseEvent e) {
                    GlobalEventWatcher.setAxesUID(figure.getIdentifier());
                }
            });
	    }
	    else {
	    	GLCanvas glCanvas = new GLCanvas();
            drawableComponent = glCanvas;
	    	glCanvas.setEnabled(true);
	    	add(glCanvas, PanelLayout.GL_CANVAS);

            rendererCanvas = JoGLCanvasFactory.createCanvas(glCanvas);
            drawerVisitor = new DrawerVisitor(drawableComponent, rendererCanvas, figure);
            rendererCanvas.setMainDrawer(drawerVisitor);

            drawableComponent.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseEntered(MouseEvent e) {
                    GlobalEventWatcher.setAxesUID(figure.getIdentifier());
                }
            });
	    }
	}

    /**
     * Rendering canvas getter.
     * @return the SciRenderer canvas.
     */
    public Canvas getRendererCanvas() {
        return rendererCanvas;
    }

    /**
     * figure getter.
     * @return the MVC figure.
     */
    public Figure getFigure() {
        return figure;
    }

    /**
	 * Create a Scilab Canvas
	 *
	 * @param figureIndex index of the displayed figure
	 * @param antialiasingQuality Specify the number of pass to use for antialiasing.
     *                            If its value is 0, then antialiasing is disable.
	 * @return the created canvas
	 */
	public static SwingScilabCanvas createCanvas(int figureIndex, int antialiasingQuality) {
	    return null;
	}

    /**
     * Drawable component getter.
     * @return the drawable component.
     */
    Component getDrawableComponent() {
        return drawableComponent;
    }

	/**
	 * Draws a Scilab canvas
	 *
	 * @see org.scilab.modules.gui.canvas.SimpleCanvas#draw()
	 */
	public void draw() {
		this.setVisible(true);
		this.doLayout();
	}

	/**
	 * Gets the dimensions (width and height) of a Scilab Canvas
	 *
	 * @return the size of the canvas
	 * @see org.scilab.modules.gui.canvas.SimpleCanvas#getDims()
	 */
	public Size getDims() {
		return new Size(this.getWidth(), this.getHeight());
	}

	/**
	 * Gets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 *
	 * @return the position of the canvas
	 * @see org.scilab.modules.gui.canvas.SimpleCanvas#getPosition()
	 */
	public Position getPosition() {
		return new Position(this.getX(), this.getY());
	}

	/**
	 * Sets the dimensions (width and height) of a Scilab Canvas
	 *
	 * @param newSize
	 *            the size we want to set to the canvas
	 * @see org.scilab.modules.gui.canvas.SimpleCanvas#setDims(org.scilab.modules.gui.utils.Size)
	 */
	public void setDims(Size newSize) {
		// make suze size is not greater than the max size
		Dimension finalDim = new Dimension(newSize.getWidth(), newSize.getHeight());

		setSize(finalDim);
	}


	/**
	 * Sets the position (X-coordinate and Y-coordinate) of a Scilab canvas
	 *
	 * @param newPosition
	 *            the position we want to set to the canvas
	 * @see org.scilab.modules.gui.canvas.SimpleCanvas#setPosition(org.scilab.modules.gui.utils.Position)
	 */
	public void setPosition(Position newPosition) {
		this.setLocation(newPosition.getX(), newPosition.getY());
	}
	/**
	 * Get the Figure Index : the Scilab ID of the figure.
	 *
	 * @return the ID.
	 */
	public int getFigureIndex() {
		// to avoid storing the data everywhere
		return getParentAxes().getFigureId();
	}


	/**
	 * Set the background of the Canvas.
	 * @param red red channel
	 * @param green green channel
	 * @param blue blue channel
	 */
	public void setBackgroundColor(double red, double green, double blue) {
		this.setBackground(new Color((float) red, (float) green, (float) blue));
	}

	/**
	 * Disable the canvas befor closing
	 */
	public void close() {
	}

	/**
	 * Take a screenshot of the figure and put it into a BufferedImage
	 * @return a BufferedImage
	 */
	public BufferedImage dumpAsBufferedImage() {
		return null;
	}

	/**
	 * Set double buffer mode on or Off
	 * @param useSingleBuffer if true use single buffer if false use double buffering
	 */
	public void setSingleBuffered(boolean useSingleBuffer) {

	}

	/**
	 * @return the axes object containing the canvas
	 */
	private SwingScilabAxes getParentAxes() {
		return null;
	}

	/**
	 * Override set cursor in order to be able to modify the cursor
	 * on the axes and not on the canvas itself
	 * @param newCursor cursor to apply on the canvas
	 */
	public void setCursor(Cursor newCursor) {
		getParentAxes().setCursor(newCursor);
	}

	/**
	 * The canvas is not focusable, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void addFocusListener(FocusListener listener) {
		getParentAxes().addFocusListener(listener);
	}

	/**
	 * The canvas is not focusable, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void removeFocusListener(FocusListener listener) {
		getParentAxes().removeFocusListener(listener);
	}

	/**
	 * The canvas is not enabled, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void addMouseListener(MouseListener listener) {
		getParentAxes().addMouseListener(listener);
	}

	/**
	 * The canvas is not enabled, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void removeMouseListener(MouseListener listener) {
		getParentAxes().removeMouseListener(listener);
	}

	/**
	 * The canvas is not enabled, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void addMouseMotionListener(MouseMotionListener listener) {
		getParentAxes().addMouseMotionListener(listener);
	}

	/**
	 * The canvas is not enabled, so add the listener to the parent instead
	 * @param listener listener to add
	 */
	public void removeMouseMotionListener(MouseMotionListener listener) {
		getParentAxes().removeMouseMotionListener(listener);
	}
    @Override
    public void display() {
        // TODO Auto-generated method stub

    }
    @Override
    public boolean getAutoSwapBufferMode() {
        // TODO Auto-generated method stub
        return false;
    }
    @Override
    public GL getGL() {
        // TODO Auto-generated method stub
        return null;
    }
    @Override
    public void setAutoSwapBufferMode(boolean onOrOff) {
        // TODO Auto-generated method stub

    }



}
