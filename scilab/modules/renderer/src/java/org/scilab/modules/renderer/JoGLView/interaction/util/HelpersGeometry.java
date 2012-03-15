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

package org.scilab.modules.renderer.JoGLView.interaction.util;

import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.buffers.BuffersManager;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.shapes.geometry.DefaultGeometry;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.renderer.JoGLView.interaction.RubberBox;

import java.nio.FloatBuffer;

/**
 * @author Pierre Lando
 */
public class HelpersGeometry extends DefaultGeometry {
    /** The helper size */
    private static final double HELPERS_SIZE = .05;

    private final BuffersManager bufferManager;
    private final ElementsBuffer vertexBuffer;

    public HelpersGeometry(DrawingTools drawingTools) {
        bufferManager = drawingTools.getCanvas().getBuffersManager();
        vertexBuffer = bufferManager.createElementsBuffer();

        setLineDrawingMode(Geometry.LineDrawingMode.SEGMENTS);
        setFillDrawingMode(Geometry.FillDrawingMode.NONE);
        setVertices(vertexBuffer);
    }

    @Override
    public void finalize() throws Throwable {
        super.finalize();
        bufferManager.dispose(vertexBuffer);
    }

    public void updateVertex(Axes axes, PointAComputer pointAComputer, Vector3d secondPoint, RubberBox.Status status) {
        if ((pointAComputer != null) && (pointAComputer.getFirstAxisIndex() != -1)) {
            Double[] bounds = axes.getDisplayedBounds();
            if ((status == RubberBox.Status.WAIT_POINT_A) || (status == RubberBox.Status.WAIT_POINT_B)) {
                FloatBuffer data = FloatBuffer.allocate(2 * 2 * 2 * 4 * 4);
                for (int axesIndex = 0; axesIndex < 3; axesIndex++) {
                    if ((axesIndex + 2) % 3 != pointAComputer.getFirstAxisIndex()) {
                        for (int u = 0; u < 2; u++) {
                            for (int v = 0; v < 2; v++) {
                                double base[] = secondPoint.getData();
                                base[axesIndex] = bounds[axesIndex * 2 + u];
                                int nextAxesIndex = (axesIndex + 1) % 3;
                                base[nextAxesIndex] = bounds[nextAxesIndex * 2 + v];
                                float[] buffer = asFloatArray(base);
                                data.put(buffer);
                                buffer[axesIndex] = buffer[axesIndex] + (float) (HELPERS_SIZE * (bounds[axesIndex * 2 + (1 - u)] - bounds[axesIndex * 2 + u]));
                                data.put(buffer);
                                buffer[axesIndex] = bounds[axesIndex * 2 + u].floatValue();
                                data.put(buffer);
                                buffer[nextAxesIndex] = buffer[nextAxesIndex] + (float) (HELPERS_SIZE * (bounds[nextAxesIndex * 2 + (1 - v)] - bounds[nextAxesIndex * 2 + v]));
                                data.put(buffer);
                            }
                        }
                    }
                }
                getVertices().setData(data, 3);
            } else if ((status == RubberBox.Status.WAIT_POINT_C) || (status == RubberBox.Status.WAIT_POINT_D)) {
                FloatBuffer data = FloatBuffer.allocate(1 * 2 * 2 * 4 * 4);
                for (int axesIndex = 0; axesIndex < 3; axesIndex++) {
                    if ((axesIndex + 2) % 3 == pointAComputer.getFirstAxisIndex()) {
                        for (int u = 0; u < 2; u++) {
                            for (int v = 0; v < 2; v++) {
                                double base[] = secondPoint.getData();
                                base[axesIndex] = bounds[axesIndex * 2 + u];
                                int nextAxesIndex = (axesIndex + 1) % 3;
                                base[nextAxesIndex] = bounds[nextAxesIndex * 2 + v];
                                float[] buffer = asFloatArray(base);
                                data.put(buffer);
                                buffer[axesIndex] = buffer[axesIndex] + (float) (HELPERS_SIZE * (bounds[axesIndex * 2 + (1 - u)] - bounds[axesIndex * 2 + u]));
                                data.put(buffer);
                                buffer[axesIndex] = bounds[axesIndex * 2 + u].floatValue();
                                data.put(buffer);
                                buffer[nextAxesIndex] = buffer[nextAxesIndex] + (float) (HELPERS_SIZE * (bounds[nextAxesIndex * 2 + (1 - v)] - bounds[nextAxesIndex * 2 + v]));
                                data.put(buffer);
                            }
                        }
                    }
                }
                getVertices().setData(data, 3);
            } else {
                getVertices().setData(new Float[0], 4);
            }
        } else {
            getVertices().setData(new Float[0], 4);
        }
    }

    private float[] asFloatArray(double[] base) {
        float data[] = new float[base.length];
        for (int i = 0; i < base.length; i++) {
            data[i] = (float) base[i];
        }
        return data;
    }
}
