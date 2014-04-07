/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

package org.scilab.forge.scirenderer.implementation.g2d;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.SciRendererException;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.lightning.LightManager;
import org.scilab.forge.scirenderer.renderer.Renderer;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.appearance.Color;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.texture.AnchorPosition;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.tranformations.TransformationManager;
import org.scilab.forge.scirenderer.tranformations.TransformationManagerImpl;
import org.scilab.forge.scirenderer.tranformations.TransformationManagerListener;
import org.scilab.forge.scirenderer.tranformations.Vector3d;

import org.scilab.forge.scirenderer.implementation.g2d.G2DCanvas;
import org.scilab.forge.scirenderer.implementation.g2d.clipping.G2DClippingManager;
import org.scilab.forge.scirenderer.implementation.g2d.motor.Motor3D;
import org.scilab.forge.scirenderer.implementation.g2d.lighting.G2DLightManager;

/**
 *
 * JoGl implementation of the DrawingTools.
 *
 * @author Calixte DENIZET
 */
public class G2DDrawingTools implements DrawingTools {

    private final TransformationManager transformationManager;
    private final G2DLightManager lightManager;
    private final G2DClippingManager clippingManager;
    private final G2DCanvas g2dCanvas;

    /**
     * Default constructor.
     * @param canvas the canvas where this drawing tools live.
     */
    G2DDrawingTools(G2DCanvas canvas) {
        this.transformationManager = new TransformationManagerImpl(canvas);
        this.lightManager = new G2DLightManager(this);
        this.clippingManager = new G2DClippingManager(this);
        this.g2dCanvas = canvas;

        transformationManager.addListener(new TransformationManagerListener() {
            @Override
            public void transformationChanged(TransformationManager transformationManager) {
                if (transformationManager.isUsingSceneCoordinate()) {
                    g2dCanvas.getMotor3D().setTransformation(transformationManager.getG2DProjection(), transformationManager.getG2DSingleProjection());
                } else {
                    g2dCanvas.getMotor3D().setTransformation(transformationManager.getG2DWindowProjection(), transformationManager.getG2DSingleProjection());
                }
            }
        });
    }

    public Motor3D getMotor3D() {
        return g2dCanvas.getMotor3D();
    }

    @Override
    public G2DCanvas getCanvas() {
        return g2dCanvas;
    }

    @Override
    public TransformationManager getTransformationManager() {
        return transformationManager;
    }

    @Override
    public LightManager getLightManager() {
        return lightManager;
    }

    @Override
    public G2DClippingManager getClippingManager() {
        return clippingManager;
    }

    @Override
    public void clear(Color color) {
        g2dCanvas.getMotor3D().reset(color);
    }

    @Override
    public void clear(java.awt.Color color) {
        g2dCanvas.getMotor3D().reset(color);
    }

    @Override
    public void clearDepthBuffer() {
        g2dCanvas.getMotor3D().clearDepth();
    }

    @Override
    public void draw(Renderer renderer) {
        g2dCanvas.getRendererManager().draw(this, renderer);
    }

    @Override
    public void draw(Geometry geometry) throws SciRendererException {
        g2dCanvas.getMotor3D().draw(this, geometry, Appearance.getDefault());
    }

    @Override
    public void draw(Geometry geometry, Appearance appearance) throws SciRendererException {
        g2dCanvas.getMotor3D().draw(this, geometry, appearance);
    }

    @Override
    public void draw(Texture texture) throws SciRendererException {
        g2dCanvas.getTextureManager().draw(this, texture);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions) {
        g2dCanvas.getMotor3D().draw(this, texture, anchor, positions, 0, 1, 0);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, ElementsBuffer positions, int offset, int stride, double rotationAngle) {
        g2dCanvas.getMotor3D().draw(this, texture, anchor, positions, offset, stride, rotationAngle);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, Vector3d position) {
        g2dCanvas.getMotor3D().draw(this, texture, anchor, position, 0);
    }

    @Override
    public void draw(Texture texture, AnchorPosition anchor, Vector3d position, double rotationAngle) {
        g2dCanvas.getMotor3D().draw(this, texture, anchor, position, rotationAngle);
    }
}
