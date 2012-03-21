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
import org.scilab.forge.scirenderer.data.AbstractDataProvider;
import org.scilab.forge.scirenderer.texture.Texture;
import org.scilab.forge.scirenderer.texture.TextureDataProvider;
import org.scilab.modules.graphic_objects.MainDataLoader;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicView.GraphicView;

import java.awt.Dimension;
import java.nio.Buffer;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.util.HashMap;

/**
 * @author Pierre Lando
 */
public class ScilabTextureManager {

    private final HashMap<String, Texture> hashMap = new HashMap<String, Texture>();
    private final Canvas canvas;

    public ScilabTextureManager(DrawerVisitor drawerVisitor) {
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
            texture.setMinifyingFilter(Texture.Filter.NEAREST);
            hashMap.put(identifier, texture);
        }
        return texture;
    }


    private class ScilabTextureDataProvider extends AbstractDataProvider<Texture> implements TextureDataProvider, GraphicView {
        private final String identifier;
        private Dimension dimension;
        private boolean isValid;

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
            dimension = new Dimension(width, height);
            fireUpdate();
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
        public ByteBuffer getData() {
            int bufferLength = dimension.width * dimension.height * 4;
            ByteBuffer buffer;
            try {
                buffer = ByteBuffer.allocate(bufferLength);
            } catch (OutOfMemoryError error) {
                // TODO: Scilab error.
                System.out.println("Java heap space to small, can't get " + bufferLength / (1024*1024) + "MBytes");
                return null;
            }
            MainDataLoader.fillTextureData(identifier, buffer, bufferLength);
            buffer.rewind();
            return buffer;
        }

        @Override
        public ByteBuffer getSubData(int x, int y, int width, int height) {
            int bufferLength = width * height * 4;
            ByteBuffer buffer;
            try {
                buffer = ByteBuffer.allocate(bufferLength);
            } catch (OutOfMemoryError error) {
                // TODO: Scilab error.
                System.out.println("Java heap space to small, can't get " + bufferLength / (1024*1024) + "MBytes");
                return null;
            }
            MainDataLoader.fillTextureData(identifier, buffer, bufferLength, x, y, width, height);
            buffer.rewind();
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
                // System.out.println("Prop: " + property);
                updateData();
            }
        }

        @Override
        public void deleteObject(String id) {
            if (isValid() && identifier.equals(id)) {
                isValid = false;
                // TODO: GraphicController.getController().unRegister(this);
            }
        }

        @Override
        public void createObject(String id) {
        }
    }
}
