package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.appearance;

import org.scilab.modules.graphic_objects.graphicEngine.core.IRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.appearance.solidcolor.ISolidColorModel;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;

import javax.media.opengl.GL;

/**
 * Author: Lando Pierre
 * Date: june 18, 2010
 *
 * OpenGl drawer of ISolidColorModel 
 */
public class SolidColorGLDrawer implements IGLDrawer<ISolidColorModel> {
    /**
     * Singleton of this class.
     */
    private static final SolidColorGLDrawer singleton = new SolidColorGLDrawer();

    /**
     * Make constructor private for singleton.
     */
    private SolidColorGLDrawer() {
    }

    public static SolidColorGLDrawer getDrawer() {
        return singleton;
    }

    
    public String getType() {
        return ISolidColorModel.type;
    }

    
    public void beginDraw(ISolidColorModel color, IGLRenderingInfo renderingInfo) {
        if (renderingInfo.getRenderingMode() == IRenderingInfo.RenderingMode.NORMAL_MODE) {
            GL gl = renderingInfo.getGL();
            gl.glPushAttrib(GL.GL_COLOR_BUFFER_BIT);
            gl.glColor4f(color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

            if (color.getAlpha() != 1f) {
                gl.glEnable(GL.GL_BLEND);
                gl.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_ALPHA);
            }
        }
    }

    
    public void endDraw(ISolidColorModel color, IGLRenderingInfo renderingInfo) {
        if (renderingInfo.getRenderingMode() == IRenderingInfo.RenderingMode.NORMAL_MODE) {
            GL gl = renderingInfo.getGL();
            gl.glPopAttrib();
        }
    }
}
