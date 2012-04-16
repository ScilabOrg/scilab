/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.bridge.canvas;

import java.awt.Color;
import java.awt.Component;
import java.awt.Cursor;
import java.awt.Dimension;
import java.awt.Event;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Graphics;
import java.awt.HeadlessException;
import java.awt.Image;
import java.awt.MenuComponent;
import java.awt.MenuContainer;
import java.awt.event.ComponentListener;
import java.awt.event.FocusListener;
import java.awt.event.HierarchyBoundsListener;
import java.awt.event.HierarchyListener;
import java.awt.event.InputMethodListener;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.MouseWheelListener;
import java.awt.image.ImageObserver;
import java.beans.PropertyChangeListener;
import java.io.Serializable;
import java.util.Calendar;
import java.util.StringTokenizer;

import javax.accessibility.Accessible;
import javax.accessibility.AccessibleContext;
import javax.media.nativewindow.NativeSurface;
import javax.media.opengl.GL;
import javax.media.opengl.GLAnimatorControl;
import javax.media.opengl.GLAutoDrawable;
import javax.media.opengl.GLDrawableFactory;
import javax.media.opengl.GLProfile;
import javax.media.opengl.awt.GLCanvas;
import javax.media.opengl.GLCapabilities;
import javax.media.opengl.GLContext;
import javax.media.opengl.GLEventListener;
import javax.media.opengl.GLException;
import javax.media.opengl.GLRunnable;
import javax.media.opengl.awt.GLJPanel;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.utils.Debug;
import org.scilab.modules.localization.Messages;

public class SwingScilabCanvasImpl implements GLAutoDrawable, ImageObserver, MenuContainer, Accessible, Serializable {

    private static final long serialVersionUID = -3110280842744630282L;

    static boolean forceGLCanvas = false;
    static boolean noGLJPanel = false;

    static {
	long lastTime = Calendar.getInstance().getTimeInMillis();

        Debug.DEBUG("SwingScilabCanvasImpl", "=======================================");
        String OS_NAME = System.getProperty("os.name");
        Debug.DEBUG("SwingScilabCanvasImpl", "os.name="+OS_NAME);
        String OS_ARCH = System.getProperty("os.arch");
        Debug.DEBUG("SwingScilabCanvasImpl", "os.arch="+OS_ARCH);
        Debug.DEBUG("SwingScilabCanvasImpl", "=======================================");

        /*
         * Bug #7526: moved out of the try/catch block since it contains
         * no OpenGL calls.
         */
        if ( OS_NAME.contains("Windows") && OS_ARCH.equals("amd64") ) {
            // bug 3919 : JOGL x64 doesn't like x64 remote desktop on Windows
            // @TODO : bug report to JOGL
            String REMOTEDESKTOP = System.getenv("SCILAB_MSTS_SESSION");
            if (REMOTEDESKTOP != null) {
                if ( REMOTEDESKTOP.equals("OK") ) {
                    noGLJPanel = true;
                }
            }

            if (noGLJPanel) {
                /** Inform the users */
                InterpreterManagement.requestScilabExec(
                    String.format("disp(\"%s\"), disp(\"%s\")", 
                        String.format(Messages.gettext("WARNING: Due to your configuration limitations, Scilab switched in a mode where mixing uicontrols and graphics is not available. Type %s for more information."), "\"\"help usecanvas\"\""), 
                        String.format(Messages.gettext("In some cases, %s fixes the issue"),"\"\"system_setproperty(''jogl.gljpanel.nohw'','''');\"\"")
                    )
                );
            }
        }

        /*
         * Bug #7526: this code block causes a crash on Windows and Intel Graphic HD cards
         * with graphics drivers older than 8.15.10.2279 and is therefore not executed if
         * Windows is the OS (the tests on driver and OpenGL versions are Linux-specific).
         * Consequently, updateMaxCanvasSize, which determines the canvas size, is not called
         * any more on Windows. However, it is still called when an actual graphics window is created
         * (by the renderer module's SciRenderer init method), which still allows getting the maximum
         * canvas dimensions, so this should not be an issue.
         */
        if (!OS_NAME.contains("Windows")) {

	    try {
	        GLCanvas tmpCanvas = new GLCanvas(new GLCapabilities(GLProfile.getDefault()));
	        Frame tmpFrame = new Frame();
	        tmpFrame.add(tmpCanvas);
	        tmpFrame.setVisible(true);

	        tmpCanvas.getContext().makeCurrent();
	        GL gl = tmpCanvas.getGL();

	        String GL_VENDOR = gl.glGetString(GL.GL_VENDOR);
	        Debug.DEBUG("SwingScilabCanvasImpl", "GL_VENDOR="+GL_VENDOR);
	        String GL_RENDERER = gl.glGetString(GL.GL_RENDERER);
	        Debug.DEBUG("SwingScilabCanvasImpl", "GL_RENDERER="+GL_RENDERER);
	        String GL_VERSION = gl.glGetString(GL.GL_VERSION);
	        Debug.DEBUG("SwingScilabCanvasImpl", "GL_VERSION="+GL_VERSION);
	        //Debug.DEBUG("SwingScilabCanvasImpl", "GL_EXTENSIONS="+gl.glGetString(GL.GL_EXTENSIONS));
	        Debug.DEBUG("SwingScilabCanvasImpl", "=======================================");
	        //System.getProperties().list(System.err);

	        // get maximum axes size
	        //RenderingCapabilities.updateMaxCanvasSize(gl);

	        tmpCanvas.getContext().release();
	        tmpFrame.remove(tmpCanvas);
	        tmpFrame.setVisible(false);
	        tmpFrame.dispose();
	        Debug.DEBUG("SwingScilabCanvasImpl", "Testing time = "+(Calendar.getInstance().getTimeInMillis() - lastTime)+"ms");

	        noGLJPanel = false;

	        // By default disable GLJPanel on Linux
	        if (OS_NAME.contains("Linux")) {
                    noGLJPanel = true;
                    // Linux && NVIDIA
                    if (GL_VENDOR.contains("NVIDIA")) {
                        noGLJPanel = false;
                    }
                    // Linux && ATI
                    if (GL_VENDOR.contains("ATI")) {
                        StringTokenizer stSpace = new StringTokenizer(GL_VERSION, " ");
                        StringTokenizer stDot = new StringTokenizer(stSpace.nextToken(), ".");
                        int majorVersion = Integer.parseInt(stDot.nextToken());
                        int minorVersion = Integer.parseInt(stDot.nextToken());
                        int releaseVersion = Integer.parseInt(stDot.nextToken());
                        // Only OpenGL version newer than 2.1.7873 works
                        // available through ATI 8.8 installer
		        // and driver newer than 8.52.3
			Debug.DEBUG("SwingScilabCanvasImpl", "majorVersion = "
					+ majorVersion + " minorVersion = " + minorVersion
					+ " releaseVersion = " + releaseVersion);
                        if (majorVersion > 2
                            || majorVersion == 2 && minorVersion > 1
                            || majorVersion == 2 && minorVersion == 1 && releaseVersion >= 7873) {
                            noGLJPanel = false;
		        }
                    }
	        }

            if (noGLJPanel) {
                /** Inform the users */
                InterpreterManagement.requestScilabExec(
                    String.format("disp(\"%s\"), disp(\"%s\")", 
                        String.format(Messages.gettext("WARNING: Due to your configuration limitations, Scilab switched in a mode where mixing uicontrols and graphics is not available. Type %s for more information."), "\"\"help usecanvas\"\""), 
                        String.format(Messages.gettext("In some cases, %s fixes the issue"),"\"\"system_setproperty(''jogl.gljpanel.nohw'','''');\"\"")
                    )
                );
            }
        }
        catch (GLException e) {
            noGLJPanel = true;
            /** Inform the users */
            InterpreterManagement.requestScilabExec(
                String.format("disp(\"%s\"), disp(\"%s\")", 
                    Messages.gettext("Due to your video card drivers limitations, that are not able to manage OpenGL, Scilab will not be able to draw any graphics. Please update your driver."), 
                    String.format(Messages.gettext("In some cases, %s fixes the issue"),"\"\"system_setproperty(''jogl.gljpanel.nohw'','''');\"\"")
                )
            );
        }
        catch (HeadlessException e) {
            // do not print anything on a CLI only environment
            noGLJPanel = true;
        }

        }


    }

    GLCanvas realGLCanvas;
    GLJPanel realGLJPanel;
    static boolean enableGLCanvas = forceGLCanvas || noGLJPanel;

    /**
     * Change Global property forceGLCanvas
     * if no GLJPanel is available, GLCanvas is forced
     */
    public static boolean switchToGLCanvas(boolean onOrOff) {
	Debug.DEBUG("SwingScilabCanvasImpl", "switchToGLCanvas " + onOrOff);
	if(!onOrOff && noGLJPanel) {
	    InterpreterManagement.requestScilabExec(Messages.gettext("disp(\"WARNING: Despite of our previous warning, you choose to use Scilab with advanced graphics capabilities. Type \"\"help usecanvas\"\" for more information.\")"));
	}
	enableGLCanvas = onOrOff;
	return enableGLCanvas;
    }

    /**
     * Get Global property forceGLCanvas
     * if no GLJPanel is available, GLCanvas is forced
     */
    public static boolean isGLCanvasEnabled() {
	return enableGLCanvas;
    }

    public SwingScilabCanvasImpl(GLCapabilities cap) {
	if (enableGLCanvas) {
	    Debug.DEBUG(this.getClass().getSimpleName(), "Using GLCanvas for OpenGL implementation.");
	    realGLCanvas = new GLCanvas(cap);
	    realGLCanvas.addMouseMotionListener(new MouseMotionListener() {
		public void mouseDragged(MouseEvent arg0) {
		    arg0.setSource(realGLCanvas.getParent());
		    realGLCanvas.getParent().dispatchEvent(arg0);
		}
		public void mouseMoved(MouseEvent arg0) {
		    arg0.setSource(realGLCanvas.getParent());
		    realGLCanvas.getParent().dispatchEvent(arg0);
		}
	    });
	    realGLCanvas.addMouseListener(new MouseListener() {
		public void mouseClicked(MouseEvent arg0) {
		    arg0.setSource(realGLCanvas.getParent());
		    realGLCanvas.getParent().dispatchEvent(arg0);
		}

		public void mouseEntered(MouseEvent arg0) {
		    arg0.setSource(realGLCanvas.getParent());
		    realGLCanvas.getParent().dispatchEvent(arg0);
		}

		public void mouseExited(MouseEvent arg0) {
		    arg0.setSource(realGLCanvas.getParent());
		    realGLCanvas.getParent().dispatchEvent(arg0);
		}

		public void mousePressed(MouseEvent arg0) {
		    arg0.setSource(realGLCanvas.getParent());
		    realGLCanvas.getParent().dispatchEvent(arg0);
		}

		public void mouseReleased(MouseEvent arg0) {
		    arg0.setSource(realGLCanvas.getParent());
		    realGLCanvas.getParent().dispatchEvent(arg0);
		}
	    });
	}
	else {
	    Debug.DEBUG(this.getClass().getSimpleName(), "Using GLJPanel for OpenGL implementation.");
	    realGLJPanel = new GLJPanel(cap);
	}
    }

    public int getContextCreationFlags() {
	return getAsGL().getContextCreationFlags();
    }

    public void setContextCreationFlags(int flag) {
	getAsGL().setContextCreationFlags(flag);
    }
    
    public void destroy() {
	getAsGL().destroy();
    }

    public void invoke(boolean wait, GLRunnable glRunnable) {
	getAsGL().invoke(wait, glRunnable);
    }

    public GLAnimatorControl getAnimator() {
	return getAsGL().getAnimator();
    }

    public void setAnimator(GLAnimatorControl animator) {
	getAsGL().setAnimator(animator);
    }

    public void addGLEventListener(int index, GLEventListener listener) {
	getAsGL().addGLEventListener(index, listener);
    }

    public void setContext(GLContext context) {
	getAsGL().setContext(context);
    }

    public GLDrawableFactory getFactory() {
	return getAsGL().getFactory();
    }

    public long getHandle() {
	return getAsGL().getHandle();
    }

    public NativeSurface getNativeSurface() {
	return getAsGL().getNativeSurface();
    }

    public GLProfile getGLProfile() {
	return getAsGL().getGLProfile();
    }

    public boolean isRealized() {
	return getAsGL().isRealized();
    }

    /**
     * @return BLOUNO
     */
    private GLAutoDrawable getAsGL() {
    	/* Don't use enableGLCanvas. */
    	/* It might change but not current type of the canvas.*/
    	if (realGLCanvas != null) {
    		return realGLCanvas;
    	} else {
    		return realGLJPanel;
    	}
    }

    /**
     * @return BLOUNO
     */
    public Component getAsComponent() {
    	/* Don't use enableGLCanvas. */
    	/* It might change but not current type of the canvas.*/
    	if (realGLCanvas != null) {
    		return realGLCanvas;
    	} else {
    		return realGLJPanel;
    	}
    }

    public void addGLEventListener(GLEventListener arg0) {
	getAsGL().addGLEventListener(arg0);
    }

    public boolean getAutoSwapBufferMode() {
	return getAsGL().getAutoSwapBufferMode();
    }

    public GLContext getContext() {
	return getAsGL().getContext();
    }

    public GL getGL() {
	//Debug.DEBUG(this.getClass().getSimpleName(), "getGL");
	return getAsGL().getGL();
    }

    public void removeGLEventListener(GLEventListener arg0) {
	getAsGL().removeGLEventListener(arg0);
    }

    public void setAutoSwapBufferMode(boolean arg0) {
	getAsGL().setAutoSwapBufferMode(arg0);
    }

    public GL setGL(GL arg0) {
	return getAsGL().setGL(arg0);
    }

    public GLContext createContext(GLContext arg0) {
	return getAsGL().createContext(arg0);
    }

    public GLCapabilities getChosenGLCapabilities() {
	return (GLCapabilities) getAsGL().getChosenGLCapabilities();
    }

    public int getHeight() {
	return getAsGL().getHeight();
    }

    public int getWidth() {
	return getAsGL().getWidth();
    }

    public void setRealized(boolean arg0) {
	getAsGL().setRealized(arg0);
    }

    public void setSize(int arg0, int arg1) {
	getAsComponent().setSize(arg0, arg1);
    }

    public void swapBuffers() throws GLException {
	getAsGL().swapBuffers();
    }

    public void addComponentListener(ComponentListener arg0) {
	getAsComponent().addComponentListener(arg0);
    }

    public void addFocusListener(FocusListener arg0) {
	getAsComponent().addFocusListener(arg0);
    }

    public void addHierarchyBoundsListener(HierarchyBoundsListener arg0) {
	getAsComponent().addHierarchyBoundsListener(arg0);
    }

    public void addHierarchyListener(HierarchyListener arg0) {
	getAsComponent().addHierarchyListener(arg0);
    }

    public void addInputMethodListener(InputMethodListener arg0) {
	getAsComponent().addInputMethodListener(arg0);
    }

    public void addKeyListener(KeyListener arg0) {
	getAsComponent().addKeyListener(arg0);
    }

    public void addMouseListener(MouseListener arg0) {
	getAsComponent().addMouseListener(arg0);
    }

    public void addMouseMotionListener(MouseMotionListener arg0) {
	getAsComponent().addMouseMotionListener(arg0);
    }

    public void addMouseWheelListener(MouseWheelListener arg0) {
	getAsComponent().addMouseWheelListener(arg0);
    }

    public void addPropertyChangeListener(PropertyChangeListener arg0) {
	getAsComponent().addPropertyChangeListener(arg0);
    }

    public void addPropertyChangeListener(String arg0,
	    PropertyChangeListener arg1) {
	getAsComponent().addPropertyChangeListener(arg0, arg1);
    }

    public void removeComponentListener(ComponentListener arg0) {
	getAsComponent().removeComponentListener(arg0);
    }

    public void removeFocusListener(FocusListener arg0) {
	getAsComponent().removeFocusListener(arg0);
    }

    public void removeHierarchyBoundsListener(HierarchyBoundsListener arg0) {
	getAsComponent().removeHierarchyBoundsListener(arg0);
    }

    public void removeHierarchyListener(HierarchyListener arg0) {
	getAsComponent().removeHierarchyListener(arg0);
    }

    public void removeInputMethodListener(InputMethodListener arg0) {
	getAsComponent().removeInputMethodListener(arg0);
    }

    public void removeKeyListener(KeyListener arg0) {
	getAsComponent().removeKeyListener(arg0);
    }

    public void removeMouseListener(MouseListener arg0) {
	getAsComponent().removeMouseListener(arg0);
    }

    public void removeMouseMotionListener(MouseMotionListener arg0) {
	getAsComponent().removeMouseMotionListener(arg0);
    }

    public void removeMouseWheelListener(MouseWheelListener arg0) {
	getAsComponent().removeMouseWheelListener(arg0);
    }

    public void removePropertyChangeListener(PropertyChangeListener arg0) {
	getAsComponent().removePropertyChangeListener(arg0);
    }

    public void removePropertyChangeListener(String arg0,
	    PropertyChangeListener arg1) {
	getAsComponent().removePropertyChangeListener(arg0, arg1);
    }

    public boolean imageUpdate(Image arg0, int arg1, int arg2, int arg3,
	    int arg4, int arg5) {
	return getAsComponent().imageUpdate(arg0, arg1, arg2, arg3, arg4, arg5);
    }

    public Font getFont() {
	return getAsComponent().getFont();
    }

    public boolean postEvent(Event arg0) {
	Debug.DEBUG(this.getClass().getSimpleName(), "postEvent(Event arg0)");
	return getAsComponent().postEvent(arg0);
    }

    public void remove(MenuComponent arg0) {
	getAsComponent().remove(arg0);
    }

    public AccessibleContext getAccessibleContext() {
	return getAsComponent().getAccessibleContext();
    }

    public void setVisible(boolean arg0) {
	Debug.DEBUG(this.getClass().getSimpleName(), "setVisible : "+arg0);
	getAsComponent().setVisible(arg0);
    }

    public void doLayout() {
	Debug.DEBUG(this.getClass().getSimpleName(), "doLayout");
	getAsComponent().doLayout();
    }

    public int getX() {
	return getAsComponent().getX();
    }

    public int getY() {
	return getAsComponent().getY();
    }

    public void setFocusable(boolean arg0) {
	getAsComponent().setFocusable(arg0);
    }

    public void setSize(Dimension arg0) {
	getAsComponent().setSize(arg0);
    }

    public void setLocation(int arg0, int arg1) {
	getAsComponent().setLocation(arg0, arg1);
    }

    public void setBackground(Color arg0) {
    	
	getAsComponent().setBackground(arg0);
    }

    public void setPreferredSize(Dimension arg0) {
	getAsComponent().setPreferredSize(arg0);
    }

    public Dimension getPreferredSize() {
	return getAsComponent().getPreferredSize();
    }

    public void setCursor(Cursor arg0) {
	getAsComponent().setCursor(arg0);
    }

    public boolean isVisible() {
	return getAsComponent().isVisible();
    }

    public boolean isFocusable() {
	return getAsComponent().isFocusable();
    }


    public void setEnabled(boolean enable) {
	Debug.DEBUG(this.getClass().getSimpleName(), "setEnable : "+enable);
	getAsComponent().setEnabled(enable);
    }

    //
    // PAINT SECTION
    //
    // {
    //

    /**
     * Heavyweight component don't draw themselves automatically
     * So we need to call a function to force the redraw.
     * @param g graphics
     */
    public void paint(Graphics g) {
	//Debug.DEBUG(this.getClass().getSimpleName(), "paint");
	getAsComponent().paint(g);
    }

    public void display() {
	//Debug.DEBUG(this.getClass().getSimpleName(), "display");
	getAsGL().display();
    }

    public void repaint() {
	//Debug.DEBUG(this.getClass().getSimpleName(), "repaint");
	getAsGL().display();
	getAsComponent().repaint();
    }

    public void update(Graphics g) {
	//Debug.DEBUG(this.getClass().getSimpleName(), "update");
	getAsComponent().update(g);
    }
    //
    // }
    //
}
