package org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.Geometry;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.ShapeComponent.geometry.cylinder.ICylinderModel;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glCore.IGLRenderingInfo;
import org.scilab.modules.graphic_objects.graphicEngine.gl.glDrawer.IGLDrawer;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;
import javax.media.opengl.glu.GLUquadric;

/**
 * Author: Lando Pierre
 * Date: june 21, 2010
 *
 * OpenGl drawer of ICylinderModel
 */
public class CylinderGLDrawer implements IGLDrawer<ICylinderModel> {
    /**
     * Singleton of this class.
     */
    private static final CylinderGLDrawer singleton = new CylinderGLDrawer();

    /**
     * Make constructor private for singleton.
     */
    private CylinderGLDrawer() {
    }

    public static CylinderGLDrawer getDrawer() {
        return singleton;
    }

    
    public String getType() {
        return ICylinderModel.type;
    }

    
    public void beginDraw(ICylinderModel cylinder, IGLRenderingInfo renderingInfo) {
        if (cylinder.getVisible()) {
            GL gl = renderingInfo.getGL();
            GLU glu = new GLU();
            GLUquadric quad = glu.gluNewQuadric();

            gl.glMatrixMode(GL.GL_MODELVIEW);
            gl.glPushMatrix();
            
            switch(cylinder.getOrigin()) {
                case BOTTOM:
                    break;
                case CENTER:
                    gl.glTranslatef(0, 0, -cylinder.getHeight()/2);
                    break;
                case TOP:
                    gl.glTranslatef(0, 0, -cylinder.getHeight());
                    break;
            }

            if (cylinder.getSideVisible()) {
                glu.gluCylinder(quad, cylinder.getBottomRadius(), cylinder.getTopRadius(), cylinder.getHeight(), 64, 4);
            }

            if (cylinder.getBottomVisible() && cylinder.getBottomRadius() > 0f) {
                glu.gluDisk(quad, 0, cylinder.getBottomRadius(), 64, 4);
            }

            if (cylinder.getTopVisible() && cylinder.getTopRadius() > 0f) {
                gl.glPushMatrix();
                gl.glTranslatef(0, 0, cylinder.getHeight());
                glu.gluDisk(quad, 0, cylinder.getTopRadius(), 64, 4);
                gl.glPopMatrix();
            }

            gl.glPopMatrix();
            glu.gluDeleteQuadric(quad);
        }
    }

    
    public void endDraw(ICylinderModel shape, IGLRenderingInfo renderingInfo) {
    }
}
