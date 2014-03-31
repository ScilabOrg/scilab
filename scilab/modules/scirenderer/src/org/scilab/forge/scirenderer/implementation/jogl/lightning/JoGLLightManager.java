/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2011 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.jogl.lightning;

import org.scilab.forge.scirenderer.implementation.jogl.JoGLDrawingTools;
import org.scilab.forge.scirenderer.implementation.jogl.utils.GLShortCuts;
import org.scilab.forge.scirenderer.lightning.Light;
import org.scilab.forge.scirenderer.lightning.LightManager;
import org.scilab.forge.scirenderer.shapes.appearance.Material;

import javax.media.opengl.GL2;

/**
 * JoGL implementation of {@link LightManager}
 *
 * @author Pierre Lando
 */
public class JoGLLightManager implements LightManager {

    /**
     * The drawing tools.
     */
    private final JoGLDrawingTools drawingTools;

    /**
     * The lights.
     */
    private final JoGLLight[] lights;

    /**
     * The current lightning status.
     */
    private boolean isLightningEnable = DEFAULT_LIGHTNING_STATUS;

    /**
     * Default constructor.
     * @param drawingTools the drawing tools.
     */
    public JoGLLightManager(JoGLDrawingTools drawingTools) {
        this.drawingTools = drawingTools;
        lights = new JoGLLight[getLightNumber()];
    }

    @Override
    public int getLightNumber() {
        return drawingTools.getGLCapacity().getLightNumber();
    }

    @Override
    public Light getLight(int i) {
        if (i < 0 || i >= getLightNumber()) {
            return null;
        } else {
            if (lights[i] == null) {
                lights[i] = new JoGLLight(drawingTools.getGl(), i);
            }
            return lights[i];
        }
    }

    @Override
    public void setLightningEnable(boolean isLightningEnable) {
        this.isLightningEnable = isLightningEnable;
        GLShortCuts.setEnable(drawingTools.getGl().getGL2(), GL2.GL_LIGHTING, isLightningEnable);
    }

    @Override
    public boolean isLightningEnable() {
        return isLightningEnable;
    }

    @Override
    public void setMaterial(Material material) {
        if (material != null) {
            GLShortCuts.setEnable(drawingTools.getGl().getGL2(), GL2.GL_COLOR_MATERIAL, material.isColorMaterialEnable());
            float[] black = new float[] {0.0f, 0.0f, 0.0f, 1.0f};
            drawingTools.getGl().glLightModelfv(GL2.GL_LIGHT_MODEL_AMBIENT, black, 0);
            drawingTools.getGl().glLightModeli(GL2.GL_LIGHT_MODEL_LOCAL_VIEWER, GL2.GL_FALSE);
            drawingTools.getGl().glLightModeli(GL2.GL_LIGHT_MODEL_TWO_SIDE, GL2.GL_TRUE);

            if (material.isColorMaterialEnable()) {
                drawingTools.getGl().glColorMaterial(GL2.GL_FRONT_AND_BACK, GL2.GL_AMBIENT_AND_DIFFUSE);
            } else {
                drawingTools.getGl().glMaterialfv(GL2.GL_FRONT_AND_BACK, GL2.GL_AMBIENT, material.getAmbientColor().getComponents(null), 0);
                drawingTools.getGl().glMaterialfv(GL2.GL_FRONT_AND_BACK, GL2.GL_DIFFUSE, material.getDiffuseColor().getComponents(null), 0);
            }
            drawingTools.getGl().glMaterialfv(GL2.GL_FRONT_AND_BACK, GL2.GL_SPECULAR, material.getSpecularColor().getComponents(null), 0);
            drawingTools.getGl().glMaterialf(GL2.GL_FRONT_AND_BACK, GL2.GL_SHININESS, material.getShininess());
            float[] f = {0.0f, 0.0f, 0.0f, 0.0f};
            drawingTools.getGl().glMaterialfv(GL2.GL_FRONT_AND_BACK, GL2.GL_EMISSION, f, 0);
        }
    }
    /**
     * Reload light.
     */
    public void reload() {
        GL2 gl = drawingTools.getGl().getGL2();
        GLShortCuts.setEnable(gl, GL2.GL_LIGHTING, isLightningEnable);
        for (JoGLLight light : lights) {
            if (light != null && light.isEnable()) {
                light.reload(gl);
            }
        }
    }
}
