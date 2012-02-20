/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView;

import com.sun.opengl.util.BufferUtil;
import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.texture.AbstractDataProvider;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDataProvider;
import org.scilab.modules.graphic_objects.MainDataLoader;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;

import java.awt.Dimension;
import java.nio.Buffer;
import java.nio.FloatBuffer;
import java.util.HashMap;

/**
 * @author Pierre Lando
 */
public class TextureManager {

    private final HashMap<String, Texture> hashMap = new HashMap<String, Texture>();
    private final Canvas canvas;
    
    public TextureManager(DrawerVisitor drawerVisitor) {
        this.canvas = drawerVisitor.getCanvas();
    }

    /**
     * {@link Texture} getter.
     * @param identifier object identifier.
     * @return the {@link Texture} used to drawn the object who have the given identifier.
     */
    public Texture getTexture(String identifier) {
        Texture texture = hashMap.get(identifier);
        if (texture == null) {
            texture = canvas.getTextureManager().createTexture();
            texture.setDataProvider(new ScilabTextureDataProvider(identifier));
            texture.setMagnificationFilter(Texture.Filter.NEAREST);
            texture.setMinifyingFilter(Texture.Filter.LINEAR);
            hashMap.put(identifier, texture);
        }
        return texture;
    }


    private class ScilabTextureDataProvider extends AbstractDataProvider<Texture> implements TextureDataProvider, GraphicView {
        private final String identifier;
        private Dimension dimension;
        private boolean isValid;
        private FloatBuffer buffer;

        public ScilabTextureDataProvider(String identifier) {
            this.identifier = identifier;
            this.isValid = (identifier != null);

            if (isValid()) {
                updateData();
            }
            GraphicController.getController().register(this);
        }

        private void updateData() {
            int width = MainDataLoader.getTextureWidth(identifier);
            int height = MainDataLoader.getTextureHeight(identifier);
            int bufferLength = width * height * 4;
            dimension = new Dimension(width, height);
            buffer = BufferUtil.newFloatBuffer(bufferLength);
            buffer.rewind();
            MainDataLoader.fillTextureData(identifier, buffer, bufferLength);
            buffer.rewind();
        }

        @Override
        public Dimension getTextureSize() {
            if (isValid()) {
                return new Dimension(dimension);
            } else {
                return null;
            }
        }

        @Override
        public Buffer getData() {
            return buffer;
        }

        @Override
        public boolean isValid() {
            return isValid;
        }

        @Override
        public void updateObject(String id, String property) {
            if (isValid() && identifier.equals(id)) {
                // TODO check Property.
                System.out.println("Prop: " + property);
                updateData();
            }
        }

        @Override
        public void deleteObject(String id) {
            if (isValid() && identifier.equals(id)) {
                buffer = null;
                isValid = false;
                // TODO: GraphicController.getController().unRegister(this);
            }
        }

        @Override
        public void createObject(String id) {
        }
    }
}
