/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.buffers.IndicesBuffer;
import org.scilab.modules.graphic_objects.MainDataLoader;
import org.scilab.modules.graphic_objects.ObjectRemovedException;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;
import org.scilab.modules.renderer.JoGLView.util.BufferAllocation;
import org.scilab.modules.renderer.JoGLView.util.OutOfMemory;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * @author Pierre Lando
 */
public class DataManager {

    /**
     * Set of properties that affect Fac3d data.
     */
    private static final Set<String> FAC3D_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__,
            GraphicObjectProperties.__GO_COLOR_FLAG__,
            GraphicObjectProperties.__GO_COLOR_MODE__,
            GraphicObjectProperties.__GO_DATA_MAPPING__
    ));

    /**
     * Set of properties that affect Fec data.
     */
    private static final Set<String> FEC_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__,
            GraphicObjectProperties.__GO_Z_BOUNDS__,
            GraphicObjectProperties.__GO_COLOR_RANGE__,
            GraphicObjectProperties.__GO_OUTSIDE_COLOR__
    ));

    /**
     * Set of properties that affect Grayplot data.
     */
    private static final Set<String> GRAYPLOT_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__,
            GraphicObjectProperties.__GO_DATA_MAPPING__
    ));

    /**
     * Set of properties that affect Matplot data.
     */
    private static final Set<String> MATPLOT_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__
    ));

    /**
     * Set of properties that affect polyline data.
     */
    private static final Set<String> POLYLINE_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__,
            GraphicObjectProperties.__GO_POLYLINE_STYLE__,
            GraphicObjectProperties.__GO_LINE_MODE__,
            GraphicObjectProperties.__GO_BAR_WIDTH__,
            GraphicObjectProperties.__GO_CLOSED__,
            GraphicObjectProperties.__GO_FILL_MODE__,
            GraphicObjectProperties.__GO_INTERP_COLOR_VECTOR__,
            GraphicObjectProperties.__GO_INTERP_COLOR_MODE__
    ));

    /**
     * Set of properties that affect Plot3d data.
     */
    private static final Set<String> PLOT3D_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_DATA_MODEL__
    ));

    /**
     * Set of properties that affect Arc data.
     */
    private static final Set<String> ARC_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_UPPER_LEFT_POINT__,
            GraphicObjectProperties.__GO_WIDTH__,
            GraphicObjectProperties.__GO_HEIGHT__,
            GraphicObjectProperties.__GO_START_ANGLE__,
            GraphicObjectProperties.__GO_END_ANGLE__
    ));

    /**
     * Set of properties that affect Champ data.
     */
    private static final Set<String> CHAMP_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_BASE_X__,
            GraphicObjectProperties.__GO_BASE_Y__,
            GraphicObjectProperties.__GO_BASE_Z__,
            GraphicObjectProperties.__GO_DIRECTION_X__,
            GraphicObjectProperties.__GO_DIRECTION_Y__,
            GraphicObjectProperties.__GO_DIRECTION_Z__,
            GraphicObjectProperties.__GO_COLORED__
    ));

    /**
     * Set of properties that affect Rectangle data.
     */
    private static final Set<String> RECTANGLE_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_UPPER_LEFT_POINT__,
            GraphicObjectProperties.__GO_WIDTH__,
            GraphicObjectProperties.__GO_HEIGHT__
    ));

    /**
     * Set of properties that affect Segs data.
     */
    private static final Set<String> SEGS_DATA_PROPERTIES = new HashSet<String>(Arrays.asList(
            GraphicObjectProperties.__GO_BASE__,
            GraphicObjectProperties.__GO_DIRECTION__,
            GraphicObjectProperties.__GO_SEGS_COLORS__
    ));

    private static final double[] DEFAULT_SCALE     = new double[] {1, 1, 1};
    private static final double[] DEFAULT_TRANSLATE = new double[] {0, 0, 0};

    /*
     * Bit mask specifying whether logarithmic coordinates are used.
     * Temporarily defined as a constant for now and set to 0 (linear x, y and z coordinates).
     * To do: pass it as an argument of fillVertexBuffer and fillWireIndexBuffer, when updating data.
     */
    private static final int DEFAULT_LOG_MASK = 0;


    private final Map<String, ElementsBuffer> vertexBufferMap = new HashMap<String, ElementsBuffer>();
    private final Map<String, ElementsBuffer> colorBufferMap = new ConcurrentHashMap<String, ElementsBuffer>();
    private final Map<String, ElementsBuffer> texturesCoordinatesBufferMap = new HashMap<String, ElementsBuffer>();
    private final Map<String, IndicesBuffer> indexBufferMap = new HashMap<String, IndicesBuffer>();
    private final Map<String, IndicesBuffer> wireIndexBufferMap = new HashMap<String, IndicesBuffer>();
    private final Canvas canvas;


    /**
     * Default constructor.
     * @param canvas the canvas where managed data live.
     */
    public DataManager(Canvas canvas) {
        this.canvas = canvas;
    }

    /**
     * Return the vertex buffer of the given object.
     * @param id the given object Id.
     * @return the vertex buffer of the given object.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public ElementsBuffer getVertexBuffer(String id) throws ObjectRemovedException, OutOfMemory {
        if (vertexBufferMap.containsKey(id)) {
            return vertexBufferMap.get(id);
        } else {
            ElementsBuffer vertexBuffer = canvas.getBuffersManager().createElementsBuffer();
            fillVertexBuffer(vertexBuffer, id);
            vertexBufferMap.put(id, vertexBuffer);
            return vertexBuffer;
        }
    }

    /**
     * Texture coordinates getter.
     * @param identifier the graphic object id.
     * @return the texture coordinates corresponding to the given graphic object.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public ElementsBuffer getTextureCoordinatesBuffer(String identifier) throws ObjectRemovedException, OutOfMemory {
        if (texturesCoordinatesBufferMap.containsKey(identifier)) {
            return texturesCoordinatesBufferMap.get(identifier);
        } else {
            ElementsBuffer texturesCoordinatesBuffer = canvas.getBuffersManager().createElementsBuffer();
            fillTextureCoordinatesBuffer(texturesCoordinatesBuffer, identifier);
            texturesCoordinatesBufferMap.put(identifier, texturesCoordinatesBuffer);
            return texturesCoordinatesBuffer;
        }
    }

    /**
     * Return the color buffer of the given object.
     * @param id the given object Id.
     * @return the color buffer of the given object.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public ElementsBuffer getColorBuffer(String id) throws ObjectRemovedException, OutOfMemory {
        if (colorBufferMap.containsKey(id)) {
            return colorBufferMap.get(id);
        } else {
            ElementsBuffer colorBuffer = canvas.getBuffersManager().createElementsBuffer();
            fillColorBuffer(colorBuffer, id);
            colorBufferMap.put(id, colorBuffer);
            return colorBuffer;
        }
    }

    /**
     * Return the index buffer of the given object.
     * @param id the given object Id.
     * @return the index buffer of the given object.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public IndicesBuffer getIndexBuffer(String id) throws ObjectRemovedException, OutOfMemory {
        if (indexBufferMap.containsKey(id)) {
            return indexBufferMap.get(id);
        } else {
            IndicesBuffer indexBuffer = canvas.getBuffersManager().createIndicesBuffer();
            fillIndexBuffer(indexBuffer, id);
            indexBufferMap.put(id, indexBuffer);
            return indexBuffer;
        }
    }

    /**
     * Return the wire index buffer of the given object.
     * @param id the given object Id.
     * @return the wire index buffer of the given object.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public IndicesBuffer getWireIndexBuffer(String id) throws ObjectRemovedException, OutOfMemory {
        if (wireIndexBufferMap.containsKey(id)) {
            return wireIndexBufferMap.get(id);
        } else {
            IndicesBuffer indexBuffer = canvas.getBuffersManager().createIndicesBuffer();
            fillWireIndexBuffer(indexBuffer, id);
            wireIndexBufferMap.put(id, indexBuffer);
            return indexBuffer;
        }
    }

    /**
     * Update texture coordinate buffer for the given object.
     * @param id given object id.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    public void updateTextureCoordinatesBuffer(String id) throws ObjectRemovedException, OutOfMemory {
        ElementsBuffer textureCoordinatesBuffer = texturesCoordinatesBufferMap.get(id);
        if (textureCoordinatesBuffer != null) {
            fillTextureCoordinatesBuffer(textureCoordinatesBuffer, id);
        }
    }

    /**
     * Update the data if needed.
     * @param id the modified object.
     * @param property the changed property.
     */
    public void update(String id, String property) throws OutOfMemory {
        String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

        try {
            if (vertexBufferMap.containsKey(id)) {
                if ((type.equals(GraphicObjectProperties.__GO_FAC3D__) && FAC3D_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_FEC__) && FEC_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_GRAYPLOT__) && GRAYPLOT_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_MATPLOT__) && MATPLOT_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_POLYLINE__) && POLYLINE_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_PLOT3D__) && PLOT3D_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_ARC__) && ARC_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_CHAMP__) && CHAMP_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_RECTANGLE__) && RECTANGLE_DATA_PROPERTIES.contains(property))
                        || (type.equals(GraphicObjectProperties.__GO_SEGS__) && SEGS_DATA_PROPERTIES.contains(property))) {
                    fillBuffers(id);
                }
            }
            if (property.equals(GraphicObjectProperties.__GO_X_AXIS_LOG_FLAG__)) {
                updateChildrenVertex(id, 0x01);
            }

            if (property.equals(GraphicObjectProperties.__GO_Y_AXIS_LOG_FLAG__)) {
                updateChildrenVertex(id, 0x02);
            }

            if (property.equals(GraphicObjectProperties.__GO_Z_AXIS_LOG_FLAG__)) {
                updateChildrenVertex(id, 0x04);
            }
        } catch (ObjectRemovedException e) {
            // Object has been removed before drawing : do nothing.
        }
    }

    /**
     * Update vertex buffer of the given object and is descendant.
     * @param id the id of the object.
     * @param coordinateMask the coordinateMask to use.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    private void updateChildrenVertex(String id, int coordinateMask) throws ObjectRemovedException {
        ElementsBuffer vertexBuffer = vertexBufferMap.get(id);
        if (vertexBuffer != null) {
            updateVertexBuffer(vertexBuffer, id, coordinateMask);
        }
        for (String childId : (String []) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_CHILDREN__)) {
            updateChildrenVertex(childId, coordinateMask);
        }
    }

    /**
     * Clear the buffer corresponding to the given object.
     * @param id object id.
     */
    public void dispose(String id) {
        if (vertexBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(vertexBufferMap.get(id));
            vertexBufferMap.remove(id);
        }

        if (colorBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(colorBufferMap.get(id));
            colorBufferMap.remove(id);
        }

        if (texturesCoordinatesBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(texturesCoordinatesBufferMap.get(id));
            texturesCoordinatesBufferMap.remove(id);
        }

        if (indexBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(indexBufferMap.get(id));
            indexBufferMap.remove(id);
        }

        if (wireIndexBufferMap.containsKey(id)) {
            canvas.getBuffersManager().dispose(wireIndexBufferMap.get(id));
            wireIndexBufferMap.remove(id);
        }
    }

    /**
     * Clears all the color buffers.
     */
    public void disposeAllColorBuffers() {
    	canvas.getBuffersManager().dispose(colorBufferMap.values());
        colorBufferMap.clear();
    }

    /**
     * Clears all the texture coordinates buffers.
     */
    public void disposeAllTextureCoordinatesBuffers() {
        canvas.getBuffersManager().dispose(texturesCoordinatesBufferMap.values());
        texturesCoordinatesBufferMap.clear();
    }

    /**
     * Fill the vertex, color, index and wire index buffers
     * of a given object.
     * @param id the object id.
     * @throws ObjectRemovedException if the object is now longer present.
     */
    private void fillBuffers(String id) throws ObjectRemovedException, OutOfMemory {
        ElementsBuffer vertexBuffer = vertexBufferMap.get(id);
        if (vertexBuffer != null) {
            fillVertexBuffer(vertexBuffer, id);
        }

        ElementsBuffer colorBuffer = colorBufferMap.get(id);
        if (colorBuffer != null) {
            fillColorBuffer(colorBuffer, id);
        }

        ElementsBuffer textureCoordinatesBuffer = texturesCoordinatesBufferMap.get(id);
        if (textureCoordinatesBuffer != null) {
            fillTextureCoordinatesBuffer(textureCoordinatesBuffer, id);
        }

        IndicesBuffer indexBuffer = indexBufferMap.get(id);
        if (indexBuffer != null) {
            fillIndexBuffer(indexBuffer, id);
        }

        IndicesBuffer wireIndexBuffer = wireIndexBufferMap.get(id);
        if (wireIndexBuffer != null) {
            fillWireIndexBuffer(wireIndexBuffer, id);
        }
    }

    private void fillVertexBuffer(ElementsBuffer vertexBuffer, String id) throws ObjectRemovedException, OutOfMemory {
        fillVertexBuffer(vertexBuffer, id, 0x01 | 0x02 | 0x04 | 0x08);
    }

    private void fillVertexBuffer(ElementsBuffer vertexBuffer, String id, int coordinateMask) throws ObjectRemovedException, OutOfMemory {
        int logMask = MainDataLoader.getLogMask(id);
        int length = MainDataLoader.getDataSize(id);
        FloatBuffer data = BufferAllocation.newFloatBuffer(length * 4);
        MainDataLoader.fillVertices(id, data, 4, coordinateMask, DEFAULT_SCALE, DEFAULT_TRANSLATE, logMask);
        vertexBuffer.setData(data, 4);
    }

    private void updateVertexBuffer(ElementsBuffer vertexBuffer, String id, int coordinateMask) throws ObjectRemovedException {
        int logMask = MainDataLoader.getLogMask(id);
        FloatBuffer data = vertexBuffer.getData();
        MainDataLoader.fillVertices(id, data, 4, coordinateMask, DEFAULT_SCALE, DEFAULT_TRANSLATE, logMask);
        vertexBuffer.setData(data, 4);
    }

    private void fillTextureCoordinatesBuffer(ElementsBuffer colorBuffer, String id) throws ObjectRemovedException, OutOfMemory {
        int length = MainDataLoader.getDataSize(id);
        FloatBuffer data = BufferAllocation.newFloatBuffer(length * 4);
        MainDataLoader.fillTextureCoordinates(id, data, length);
        colorBuffer.setData(data, 4);
    }

    private void fillColorBuffer(ElementsBuffer colorBuffer, String id) throws ObjectRemovedException, OutOfMemory {
            int length = MainDataLoader.getDataSize(id);
            FloatBuffer data = BufferAllocation.newFloatBuffer(length * 4);
            MainDataLoader.fillColors(id, data, 4);
            colorBuffer.setData(data, 4);
    }

    private void fillIndexBuffer(IndicesBuffer indexBuffer, String id) throws ObjectRemovedException, OutOfMemory {
        int length = MainDataLoader.getIndicesSize(id);
        IntBuffer data = BufferAllocation.newIntBuffer(length);

        int actualLength = 0;
        if (length != 0) {
            /* Do not call JNI when the buffer is empty */
            /* Because under Mac OS X, GetDirectBufferAddress returns a NULL pointer in this case */
            /* This generates an exception in DataLoader_wrap.c */
            actualLength = MainDataLoader.fillIndices(id, data, DEFAULT_LOG_MASK);
        }

        /* Set the buffer size to the actual number of indices */
        data.limit(actualLength);

        indexBuffer.setData(data);
    }

    private void fillWireIndexBuffer(IndicesBuffer indexBuffer, String id) throws ObjectRemovedException, OutOfMemory {
        int length = MainDataLoader.getWireIndicesSize(id);
        IntBuffer data = BufferAllocation.newIntBuffer(length);

        int actualLength = 0;
        if (length != 0) {
            /* Do not call JNI when the buffer is empty */
            /* Because under Mac OS X, GetDirectBufferAddress returns a NULL pointer in this case */
            /* This generates an exception in DataLoader_wrap.c */
            actualLength = MainDataLoader.fillWireIndices(id, data, DEFAULT_LOG_MASK);
        }

        /* Set the buffer size to the actual number of indices */
        data.limit(actualLength);

        indexBuffer.setData(data);
    }
}
