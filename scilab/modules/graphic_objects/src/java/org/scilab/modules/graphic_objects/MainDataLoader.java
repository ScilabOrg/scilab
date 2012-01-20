/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2012 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_objects;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObjectProperties;

import org.scilab.modules.graphic_objects.DataLoader;
import org.scilab.modules.graphic_objects.JavaObjectLoader;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

/**
 * Class MainDataLoader
 * A utility class used to switch to the appropriate data loading class
 * depending on the type of the object whose data is loaded. A class is used
 * for purely Java-based objects and another one is used to load the data
 * of data model (i.e. C++-based) objects.
 * @author Manuel JULIACHS
 */
public class MainDataLoader {

        /**
         * The names of Java-based objects.
         */
        private static final Set<String> JAVA_OBJECTS = new HashSet<String>(Arrays.asList(
                GraphicObjectProperties.__GO_ARC__,
                GraphicObjectProperties.__GO_RECTANGLE__,
                GraphicObjectProperties.__GO_SEGS__
        ));

        /**
         * Returns the number of data elements for the given object.
         * @param the id of the given object.
         * @return the number of data elements.
         */
        public static int getDataSize(String id) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (JAVA_OBJECTS.contains(type)) {
                        return JavaObjectLoader.getDataSize(id);
                } else {
                        return DataLoader.getDataSize(id);
                }
        }

        /**
         * Fills the given buffer with vertex data from the given object.
         * @param the id of the given object.
         * @param the buffer to fill.
         * @param the number of coordinates taken by one element in the buffer.
         * @param the bit mask specifying which coordinates are filled (1 for X, 2 for Y, 4 for Z).
         * @param the conversion scale factor to apply to data.
         * @param the conversion translation value to apply to data.
         * @param the bit mask specifying whether logarithmic coordinates are used.
         */
        public static void fillVertices(String id, FloatBuffer buffer, int elementsSize,
                int coordinateMask, double[] scale, double[] translation, int logMask) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (JAVA_OBJECTS.contains(type)) {
                        JavaObjectLoader.fillVertices(id, buffer, elementsSize, coordinateMask, scale, translation, logMask);
                } else {
                        DataLoader.fillVertices(id, buffer, buffer.capacity(), elementsSize, coordinateMask, scale, translation, logMask);
                }
        }

        /**
         * Fills the given buffer with color data from the given object.
         * @param the id of the given object.
         * @param the buffer to fill.
         * @param the number of components taken by one element in the buffer (3 or 4).
         */
        public static void fillColors(String id, FloatBuffer buffer, int elementsSize) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (JAVA_OBJECTS.contains(type)) {
                        JavaObjectLoader.fillColors(id, buffer, elementsSize);
                } else {
                        DataLoader.fillColors(id, buffer, buffer.capacity(), elementsSize);
                }
        }

        /**
         * Returns the number of indices for the given object.
         * @param the id of the given object.
         * @return the object's number of indices.
         */
        public static int getIndicesSize(String id) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (JAVA_OBJECTS.contains(type)) {
                        return JavaObjectLoader.getIndicesSize(id);
                } else {
                        return DataLoader.getIndicesSize(id);
                }
        }

        /**
         * Fills the given buffer with indices data of the given object.
         * @param the id of the given object.
         * @param the buffer to fill.
         * @param the bit mask specifying whether logarithmic coordinates are used.
         * @return the number of indices actually written.
         */
        public static int fillIndices(String id, IntBuffer buffer, int logMask) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (JAVA_OBJECTS.contains(type)) {
                        return JavaObjectLoader.fillIndices(id, buffer, logMask);
                } else {
                        return DataLoader.fillIndices(id, buffer, buffer.capacity(), logMask);
                }
        }

        /**
         * Returns the number of wireframe indices of the given object.
         * @param the id of the given object.
         * @return the object's number of indices.
         */
        public static int getWireIndicesSize(String id) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (JAVA_OBJECTS.contains(type)) {
                        return JavaObjectLoader.getWireIndicesSize(id);
                } else {
                        return DataLoader.getWireIndicesSize(id);
                }
        }

        /**
         * Fills the given buffer with wireframe index data of the given object.
         * @param the id of the given object.
         * @param the buffer to fill.
         * @param the bit mask specifying whether logarithmic coordinates are used.
         * @return the number of indices actually written.
         */
        public static int fillWireIndices(String id, IntBuffer buffer, int logMask) {
                String type = (String) GraphicController.getController().getProperty(id, GraphicObjectProperties.__GO_TYPE__);

                if (JAVA_OBJECTS.contains(type)) {
                        return JavaObjectLoader.fillWireIndices(id, buffer, logMask);
                } else {
                        return DataLoader.fillWireIndices(id, buffer, buffer.capacity(), logMask);
                }
        }

        /**
         * Returns the number of mark indices of the given object.
         * To implement.
         * @param the id of the given object.
         * @return the number of mark indices.
         */
        public static int getMarkIndicesSize(String id)
        {
                return 0;
        }

        /**
         * Fills the given buffer with mark index data of the given object.
         * To implement.
         * @param the id of the given object.
         * @param the buffer to fill.
         * @return the number of indices actually written.
         */
        public static int fillMarkIndices(String id, IntBuffer buffer)
        {
                return 0;
        }
}
