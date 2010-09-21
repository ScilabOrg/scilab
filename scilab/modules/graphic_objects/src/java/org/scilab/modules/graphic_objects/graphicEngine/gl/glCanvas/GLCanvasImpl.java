/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.gl.glCanvas;

import org.scilab.modules.graphic_objects.util.MVC.IDataController;
import org.scilab.modules.graphic_objects.util.MVC.IDataEvent;
import org.scilab.modules.graphic_objects.util.MVC.IDataListener;
import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;
import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.IGraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.IDrawable;
import org.scilab.modules.graphic_objects.graphicEngine.maths.*;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.IRenderingZoneController;
import org.scilab.modules.graphic_objects.graphicEngine.canvas.ILocationInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.GLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.*;
import org.scilab.modules.graphic_objects.graphicEngine.canvas.ICanvas;
import org.scilab.modules.graphic_objects.graphicEngine.gl.synchronization.GLSynchronization;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.RenderingZoneController;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.RenderingZoneModelImpl;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.layout.FullLayoutPolicy;

import javax.media.opengl.*;
import javax.swing.event.EventListenerList;
import java.awt.event.MouseEvent;
import java.nio.FloatBuffer;
import java.util.EventListener;

/**
 * Author: Lando Pierre
 * Date: 18 mai 2010
 *
 * An implementation of ICanvas with a gl rendering.
 * A drawerManager is instantiate.
 * A full rendering zone is instantiate.
 */
public class GLCanvasImpl extends GLJPanel implements ICanvas<IGLDrawersManager>, IDataListener {
    private boolean needRedraw = true;

    private IDrawable[] drawableVector;
    private int drawableIndex = 0;

    private UniqueColor colorMaker;

    private final EventListenerList pickers = new EventListenerList(); 

    private IGLDrawersManager drawersManager = new GLDrawersManager();
    private final IGLRenderingInfo renderingInfo = new GLRenderingInfo(drawersManager);
    private IRenderingZoneController renderingZone;

    public GLCanvasImpl() {
        super(GLSynchronization.getCapabilities(), null, GLSynchronization.getContext());
        setRenderingZone(new RenderingZoneController(new RenderingZoneModelImpl()));
        getRenderingZone().setLayoutPolicy(new FullLayoutPolicy());

        setAutoSwapBufferMode(false);
        addGLEventListener(new GLCanvasEventListener());
    }

    
    public void update(IDataController sources, IDataEvent event) {
        display();
    }

    /**
     * Synchronized picker class.
     */
    private class GLLocationInfo implements EventListener, ILocationInfo {
        private boolean set = false;

        private IDrawable drawable = null;
        private IVector3f worldPosition = null;
        private IVector3f windowPosition = null;
        private IVector3f normalizedWindowPosition = null;
        private ISegment3f worldSegment = null;

        /**
         * Default constructor.
         * @param x the x coordinate of the location.
         * @param y the y coordinate of the location.
         */
        public GLLocationInfo(int x, int y) {
            windowPosition = new Vector3f(x, y, 0);
            normalizedWindowPosition = new Vector3f( (float) x/getWidth(), (float) (getHeight() - y - 1)/getHeight(), 0);
            if (x >= 0 && x < getWidth() && y>0 && y < getHeight()) {
                pickers.add(GLLocationInfo.class, this);
                needRedraw = false;
                display();
            } else {
                drawable = null;
                set = true;
            }
        }
        
        
        public IDrawable getDrawable() {
            checkSet();
            return drawable;
        }

        
        public IRenderingZoneController getRenderingZone() {
            return renderingZone;
        }

        
        public IVector3f getWindowPosition() {
            checkSet();
            if (windowPosition != null) {
                return new Vector3f(windowPosition);
            } else {
                return null;
            }
        }

        
        public IVector3f getNormalizedWindowPosition() {
            if (normalizedWindowPosition != null) {
                return normalizedWindowPosition;
            } else {
                return null;
            }
        }

        
        public IVector3f getWorldPosition() {
            checkSet();
            if (worldPosition != null) {
                return new Vector3f(worldPosition);
            } else {
                return null;
            }
        }

        
        public ISegment3f getWorldSegment() {
            return worldSegment;
        }

        
        public void forceEvaluation() {
            checkSet();
        }

        /**
         * Check information are set.
         */
        synchronized private void checkSet() {
            while(!set) {
                try {
                    wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

        synchronized public void perform(GL gl) {
            pickers.remove(GLLocationInfo.class, this);

            if (windowPosition != null) {
                int x = (int) windowPosition.getX();
                int ry = getHeight() - (int)windowPosition.getY() - 1;

                FloatBuffer buffer = FloatBuffer.allocate(16);
                gl.glReadBuffer(GL.GL_AUX0);
                gl.glReadPixels(x, ry, 1, 1, GL.GL_RGBA, GL.GL_FLOAT, buffer);
                buffer.rewind();

                int index = colorMaker.getIndex(buffer.get(), buffer.get(), buffer.get());
                drawable = drawableVector[index];

                buffer.rewind();
                gl.glReadBuffer(GL.GL_AUX0);
                gl.glReadPixels(x, ry, 1, 1, GL.GL_DEPTH_COMPONENT, GL.GL_FLOAT, buffer);
                buffer.rewind();
                float z = buffer.get();

                windowPosition.setZ(z);

                normalizedWindowPosition.setZ(0f);
                worldSegment = new Segment3f();
                worldSegment.setFirstPoint( MathUtil.unProject(renderingZone, normalizedWindowPosition) );
                normalizedWindowPosition.setZ(1f);
                worldSegment.setSecondPoint( MathUtil.unProject(renderingZone, normalizedWindowPosition) );

                normalizedWindowPosition.setZ(z);
                if (z != 1f) {
                    worldPosition = MathUtil.unProject(renderingZone, normalizedWindowPosition);
                } else {
                    worldPosition = null;
                }
            } else {
                drawable = null;
                normalizedWindowPosition = null;
                worldPosition = null;
            }

            set = true;
            notifyAll();
        }
    }

    /**
     * Implementation of GLEventListener.
     */
    private class GLCanvasEventListener implements GLEventListener {
        
        public void display(GLAutoDrawable glAutoDrawable) {
            GL gl = new DebugGL(glAutoDrawable.getGL());
            if (needRedraw) {
                long time = System.nanoTime();

                int size;
                if (renderingZone.getDataController() !=null) {
                    size = renderingZone.getDataController().getSize();
                } else {
                    size = 0;
                }
                drawableVector = new IDrawable[size+2]; // +1 for background, +1 for rendering zone.

                drawableVector[0] = null; // Set the background map to null.

                renderingInfo.setGL(gl);
                renderingInfo.setFrameWidth(getWidth());
                renderingInfo.setFrameHeight(getHeight());
                renderingInfo.setRenderingMode(IRenderingInfo.RenderingMode.NORMAL_MODE);
                drawableIndex = 0;

                renderAll(renderingZone, renderingInfo);

                swapBuffers();

                int[] oldDrawBuffer = new int[1];
                gl.glGetIntegerv(GL.GL_DRAW_BUFFER, oldDrawBuffer, 0);
                gl.glDrawBuffer(GL.GL_AUX0);

                renderingInfo.setRenderingMode(IRenderingInfo.RenderingMode.SELECT_MODE);
                drawableIndex = 0;

                gl.glDisable(GL.GL_BLEND);
                gl.glDisable(GL.GL_DITHER);
                gl.glDisable(GL.GL_FOG);
                gl.glDisable(GL.GL_LIGHTING);
                gl.glDisable(GL.GL_TEXTURE_1D);
                gl.glDisable(GL.GL_TEXTURE_2D);
                gl.glDisable(GL.GL_TEXTURE_3D);
                gl.glShadeModel(GL.GL_FLAT);

                renderAll(renderingZone, renderingInfo);

                gl.glDrawBuffer(oldDrawBuffer[0]);

                time = System.nanoTime() - time;
                //System.out.println("FPS: " + 1000000000 / time);
            }
            needRedraw = true;
            for (GLLocationInfo locationInfo : pickers.getListeners(GLLocationInfo.class)) {
                locationInfo.perform(gl);
            }
        }

        
        public void init(GLAutoDrawable glAutoDrawable) {
            colorMaker = new UniqueColor( new DebugGL(glAutoDrawable.getGL()) );
        }
        
        public void reshape(GLAutoDrawable glAutoDrawable, int x, int y, int width, int height) {
            needRedraw = true;
        }
        
        public void displayChanged(GLAutoDrawable glAutoDrawable, boolean modeChanged, boolean deviceChanged) {}
    }

    /**
     * This class make an unique color from an index.
     */
    private class UniqueColor {
        private final int redSize;
        private final int greenSize;
        private final int blueSize;

        public UniqueColor(GL gl) {
            int[] colorBits = new int[3];
            gl.glGetIntegerv (GL.GL_RED_BITS, colorBits, 0);
            gl.glGetIntegerv (GL.GL_GREEN_BITS, colorBits, 1);
            gl.glGetIntegerv (GL.GL_BLUE_BITS, colorBits, 2);

            redSize = 1 << colorBits[0];
            greenSize = 1 << colorBits[1];
            blueSize = 1 << colorBits[2];
        }

        private void setColor(GL gl, int index) {
            int i = index;
            float b = (i % blueSize) / (blueSize - 1f);
            i = i >> 8;
            float g = (i % greenSize) / (greenSize - 1f);
            i = i >> 8;
            float r = (i % redSize) / (redSize - 1f);

            gl.glColor3f(r, g, b);
            gl.glClearColor(r, g, b, 0);
        }

        private int getIndex(float r, float g, float b) {
            return (int) (((r*(redSize - 1f) + g) * (greenSize - 1f) + b) * (blueSize - 1f));
        }
    }

    
    public void setRenderingZone(IRenderingZoneController renderingZone) {
        if (this.renderingZone != null) {
            this.renderingZone.removeListener(this);
        }

        this.renderingZone = renderingZone;

        if (this.renderingZone != null) {
            this.renderingZone.addListener(this);
        }
    }

    
    public IRenderingZoneController getRenderingZone() {
        return renderingZone;
    }

    
    public void setDrawersManager(IGLDrawersManager drawerManager) {
        this.drawersManager = drawerManager;
    }

    
    public IGLDrawersManager getDrawersManager() {
        return drawersManager;
    }

    
    public ILocationInfo getLocationInfo(int x, int y) {
        return new GLLocationInfo(x, y);
    }

    
    public ILocationInfo getLocationInfo(MouseEvent e) {
        return getLocationInfo(e.getX(), e.getY());
    }

    /**
     * This function is used to render a specific drawable.
     * @param node to render.
     * @param renderingInfo rendering info.
     */
    private void renderNode(INodeController<IGraphicController> node, IGLRenderingInfo renderingInfo) {
        if (node!=null && node.isVisible()) {
            IGraphicController content = node.getContent();

            if (renderingInfo.getRenderingMode() == IRenderingInfo.RenderingMode.SELECT_MODE) {
                int index = ++drawableIndex;
                colorMaker.setColor(renderingInfo.getGL(), index);
                drawableVector[index] = node.getContent();
            }

            IGLDrawer drawer = null;
            if (content != null) {
                drawer = drawersManager.getSpecificDrawer(content);
            }

            if (drawer != null) {
                drawer.beginDraw(content, renderingInfo);
            }

            for(INodeController<IGraphicController> child : node.getChildren()) {
                renderNode(child, renderingInfo);
            }

            if (drawer != null) {
                drawer.endDraw(content, renderingInfo);
            }            
        }
    }

    private void renderAll(IRenderingZoneController renderingZone, IGLRenderingInfo renderingInfo) {
        if (renderingZone != null) {
            if (renderingInfo.getRenderingMode() == IRenderingInfo.RenderingMode.SELECT_MODE) {
                int index = ++drawableIndex;
                colorMaker.setColor(renderingInfo.getGL(), index);
                drawableVector[index] = renderingZone;
            }

            IGLDrawer drawer = drawersManager.getSpecificDrawer(renderingZone);
            if (drawer != null) {
                drawer.beginDraw(renderingZone, renderingInfo);
            }

            if (renderingZone.getDataController() != null) {
                renderNode(renderingZone.getDataController().getRootNode(), renderingInfo);
            }

            if (drawer != null) {
                drawer.endDraw(renderingZone, renderingInfo);
            }
        }
    }
}
