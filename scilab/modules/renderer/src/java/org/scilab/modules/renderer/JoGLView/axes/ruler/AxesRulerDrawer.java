/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.renderer.JoGLView.axes.ruler;

import org.scilab.forge.scirenderer.Canvas;
import org.scilab.forge.scirenderer.DrawingTools;
import org.scilab.forge.scirenderer.buffers.ElementsBuffer;
import org.scilab.forge.scirenderer.ruler.DefaultRulerModel;
import org.scilab.forge.scirenderer.ruler.RulerDrawer;
import org.scilab.forge.scirenderer.ruler.RulerDrawingResult;
import org.scilab.forge.scirenderer.ruler.RulerModel;
import org.scilab.forge.scirenderer.shapes.appearance.Appearance;
import org.scilab.forge.scirenderer.shapes.geometry.Geometry;
import org.scilab.forge.scirenderer.shapes.geometry.GeometryImpl;
import org.scilab.forge.scirenderer.tranformations.DegenerateMatrixException;
import org.scilab.forge.scirenderer.tranformations.Transformation;
import org.scilab.forge.scirenderer.tranformations.TransformationFactory;
import org.scilab.forge.scirenderer.tranformations.Vector3d;
import org.scilab.modules.graphic_objects.axes.Axes;
import org.scilab.modules.graphic_objects.axes.AxisProperty;
import org.scilab.modules.graphic_objects.axes.Camera;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.renderer.JoGLView.axes.AxesDrawer;
import org.scilab.modules.renderer.JoGLView.label.AxisLabelPositioner;
import org.scilab.modules.renderer.JoGLView.util.ColorFactory;

import java.nio.FloatBuffer;

/**
 * @author Pierre Lando
 */
public class AxesRulerDrawer {

    private static final double LINEAR_MINIMAL_SUB_TICKS_DISTANCE = 8;
    private static final double LOG_MINIMAL_SUB_TICKS_DISTANCE = 2;

    private static final short GRID_LINE_PATTERN = (short) 0xF0F0;
    private static final double TICKS_SIZE = .1;

    private final RulerDrawerManager rulerDrawerManager;

    public AxesRulerDrawer(Canvas canvas) {
        this.rulerDrawerManager = new RulerDrawerManager(canvas.getSpriteManager());
    }

    /**
     * Draw the ruler.
     * @param axes the current {@see Axes}
     * @param axesDrawer the drawer used to draw the current {@see Axes}
     * @param colorMap current {@see ColorMap}
     * @param drawingTools the used {@see DrawingTools}
     */
    public void drawRuler(Axes axes, AxesDrawer axesDrawer, ColorMap colorMap, DrawingTools drawingTools) {
        Double[] bounds = axes.computeDisplayedBounds();
        double[] matrix = drawingTools.getTransformationManager().getModelViewStack().peek().getMatrix();

        RulerDrawingResult rulerDrawingResult;
        double[] values;

        RulerDrawer[] rulerDrawers = rulerDrawerManager.get(axes);
        DefaultRulerModel rulerModel = new DefaultRulerModel();

        Transformation canvasProjection = drawingTools.getTransformationManager().getCanvasProjection();

        Vector3d xAxisPosition = computeXAxisPosition(matrix, axes.getAxes()[0].getAxisLocation());
        Vector3d yAxisPosition = computeYAxisPosition(matrix, axes.getAxes()[1].getAxisLocation());

        Vector3d px = canvasProjection.projectDirection(new Vector3d(1, 0, 0)).setZ(0);
        Vector3d py = canvasProjection.projectDirection(new Vector3d(0, 1, 0)).setZ(0);
        Vector3d pz = canvasProjection.projectDirection(new Vector3d(0, 0, 1)).setZ(0);

        Vector3d xTicksDirection, yTicksDirection;
        if (py.getNorm2() > pz.getNorm2()) {
            xTicksDirection = new Vector3d(0, TICKS_SIZE * xAxisPosition.getY(), 0);
        } else {
            xTicksDirection = new Vector3d(0, 0, TICKS_SIZE * xAxisPosition.getZ());
        }

        if (px.getNorm2() > pz.getNorm2()) {
            yTicksDirection = new Vector3d(TICKS_SIZE * yAxisPosition.getX(), 0, 0);
        } else {
            yTicksDirection = new Vector3d(0, 0, TICKS_SIZE * yAxisPosition.getZ());
        }

        int gridPosition;
        if (axes.getGridPositionAsEnum().equals(Axes.GridPosition.FOREGROUND)) {
            gridPosition = 1;
        } else {
            gridPosition = -1;
        }

        // Draw X ruler

        ElementsBuffer vertexBuffer = drawingTools.getCanvas().getBuffersManager().createElementsBuffer();

        Appearance gridAppearance = new Appearance();
        gridAppearance.setLinePattern(GRID_LINE_PATTERN);

        rulerModel.setTicksDirection(xTicksDirection);
        rulerModel.setFirstPoint(xAxisPosition.setX(-1));
        rulerModel.setSecondPoint(xAxisPosition.setX(1));

        setRulerBounds(axes.getXAxis(), rulerModel, bounds[0], bounds[1]);

        rulerModel.setLogarithmic(axes.getXAxis().getLogFlag());
        rulerModel.setMinimalSubTicksDistance(axes.getXAxis().getLogFlag() ? LOG_MINIMAL_SUB_TICKS_DISTANCE : LINEAR_MINIMAL_SUB_TICKS_DISTANCE);

        if (!axes.getAxes()[0].getAutoTicks()) {
            rulerModel.setUserGraduation(new UserDefineGraduation(axes.getXAxis(), bounds[0], bounds[1]));
            rulerModel.setAutoTicks(false);
        } else {
            rulerModel.setAutoTicks(true);
        }

        double distanceRatio;
        AxisLabelPositioner xAxisLabelPositioner = axesDrawer.getXAxisLabelPositioner();
        xAxisLabelPositioner.setLabelPosition(xAxisPosition);

        if (axes.getXAxisVisible()) {
            rulerDrawingResult = rulerDrawers[0].draw(drawingTools, rulerModel);
            values = rulerDrawingResult.getTicksValues();
            if (axes.getXAxisAutoTicks()) {
                // TODO
                axes.setXAxisTicksLocations(toDoubleArray(values));
                axes.setXAxisTicksLabels(toStringArray(values));
                axes.setXAxisSubticks(rulerDrawingResult.getSubTicksDensity() - 1);
            }

            distanceRatio  = rulerDrawingResult.getMaxDistToTicksDirNorm();

            xAxisLabelPositioner.setTicksDirection(xTicksDirection);
            xAxisLabelPositioner.setDistanceRatio(distanceRatio);
            xAxisLabelPositioner.setProjectedTicksDirection(rulerDrawingResult.getNormalizedTicksDirection().setZ(0));

            if (axes.getXAxisGridColor() != -1) {
                FloatBuffer vertexData;
                if(axes.getXAxisLogFlag()) {
                    vertexData = getXGridData(rulerDrawingResult.getSubTicksValues(), rulerModel);
                } else {
                    vertexData = getXGridData(values, rulerModel);
                }
                vertexBuffer.setData(vertexData, 4);

                Transformation mirror;
                try {
                    mirror = TransformationFactory.getScaleTransformation(
                            1,
                            matrix[6] < 0 ? gridPosition : -gridPosition,
                            matrix[10] < 0 ? gridPosition : -gridPosition
                    );
                } catch (DegenerateMatrixException ignored) {
                    // Should never happens as long as gridPosition the value 1 or -1
                    mirror = TransformationFactory.getIdentity();
                }

                gridAppearance.setLineColor(ColorFactory.createColor(colorMap, axes.getXAxisGridColor()));
                drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(mirror);
                drawingTools.draw(new GeometryImpl(Geometry.DrawingMode.SEGMENTS, vertexBuffer), gridAppearance);
                drawingTools.getTransformationManager().getModelViewStack().pop();
            }

        }

        // Draw Y ruler
        rulerModel.setTicksDirection(yTicksDirection);
        rulerModel.setFirstPoint(yAxisPosition.setY(-1));
        rulerModel.setSecondPoint(yAxisPosition.setY(1));

        setRulerBounds(axes.getYAxis(), rulerModel, bounds[2], bounds[3]);
        rulerModel.setLogarithmic(axes.getYAxis().getLogFlag());
        rulerModel.setMinimalSubTicksDistance(axes.getYAxis().getLogFlag() ? LOG_MINIMAL_SUB_TICKS_DISTANCE : LINEAR_MINIMAL_SUB_TICKS_DISTANCE);

        if (!axes.getAxes()[1].getAutoTicks()) {
            rulerModel.setUserGraduation(new UserDefineGraduation(axes.getXAxis(), bounds[2], bounds[3]));
            rulerModel.setAutoTicks(false);
        } else {
            rulerModel.setAutoTicks(true);
        }

        AxisLabelPositioner yAxisLabelPositioner = axesDrawer.getYAxisLabelPositioner();
        yAxisLabelPositioner.setLabelPosition(yAxisPosition);

        if (axes.getYAxisVisible()) {
            rulerDrawingResult = rulerDrawers[1].draw(drawingTools, rulerModel);
            values = rulerDrawingResult.getTicksValues();
            if (axes.getYAxisAutoTicks()) {
                // TODO
                axes.setYAxisTicksLocations(toDoubleArray(values));
                axes.setYAxisTicksLabels(toStringArray(values));
                axes.setYAxisSubticks(rulerDrawingResult.getSubTicksDensity() - 1);
            }

            distanceRatio = rulerDrawingResult.getMaxDistToTicksDirNorm();

            yAxisLabelPositioner.setTicksDirection(yTicksDirection);
            yAxisLabelPositioner.setDistanceRatio(distanceRatio);
            yAxisLabelPositioner.setProjectedTicksDirection(rulerDrawingResult.getNormalizedTicksDirection().setZ(0));

            if (axes.getYAxisGridColor() != -1) {
                FloatBuffer vertexData;
                if(axes.getYAxisLogFlag()) {
                    vertexData = getYGridData(rulerDrawingResult.getSubTicksValues(), rulerModel);
                } else {
                    vertexData = getYGridData(values, rulerModel);
                }
                vertexBuffer.setData(vertexData, 4);

                Transformation mirror;
                try {
                    mirror = TransformationFactory.getScaleTransformation(
                            matrix[2] < 0 ? gridPosition : -gridPosition,
                            1,
                            matrix[10] < 0 ? gridPosition : -gridPosition
                    );
                } catch (DegenerateMatrixException ignored) {
                    // Should never happens as long as gridPosition the value 1 or -1
                    mirror = TransformationFactory.getIdentity();
                }

                gridAppearance.setLineColor(ColorFactory.createColor(colorMap, axes.getYAxisGridColor()));
                drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(mirror);
                drawingTools.draw(new GeometryImpl(Geometry.DrawingMode.SEGMENTS, vertexBuffer), gridAppearance);
                drawingTools.getTransformationManager().getModelViewStack().pop();
            }
        }

        // Draw Z ruler
        if (axes.getViewAsEnum() == Camera.ViewType.VIEW_3D && axes.getRotationAngles()[1] != 90.0) {
            double txs, tys, xs, ys;

            if (Math.abs(matrix[2]) < Math.abs(matrix[6])) {
                xs = Math.signum(matrix[2]);
                ys = -Math.signum(matrix[6]);
                txs = xs;
                tys = 0;
            } else {
                xs = -Math.signum(matrix[2]);
                ys = Math.signum(matrix[6]);
                txs = 0;
                tys = ys;
            }

            rulerModel.setFirstPoint(new Vector3d(xs, ys, -1));
            rulerModel.setSecondPoint(new Vector3d(xs, ys, 1));
            rulerModel.setTicksDirection(new Vector3d(TICKS_SIZE * txs, TICKS_SIZE * tys, 0));


            setRulerBounds(axes.getZAxis(), rulerModel, bounds[4], bounds[5]);
            rulerModel.setLogarithmic(axes.getZAxis().getLogFlag());
            rulerModel.setMinimalSubTicksDistance(axes.getZAxis().getLogFlag() ? LOG_MINIMAL_SUB_TICKS_DISTANCE : LINEAR_MINIMAL_SUB_TICKS_DISTANCE);

            if (!axes.getAxes()[2].getAutoTicks()) {
                rulerModel.setUserGraduation(new UserDefineGraduation(axes.getXAxis(), bounds[4], bounds[5]));
                rulerModel.setAutoTicks(false);
            } else {
                rulerModel.setAutoTicks(true);
            }

            AxisLabelPositioner zAxisLabelPositioner = axesDrawer.getZAxisLabelPositioner();
            zAxisLabelPositioner.setLabelPosition(new Vector3d(xs, ys, 0));

            if (axes.getZAxisVisible()) {
                rulerDrawingResult = rulerDrawers[2].draw(drawingTools, rulerModel);
                values = rulerDrawingResult.getTicksValues();
                if (axes.getZAxisAutoTicks()) {
                    // TODO
                    axes.setZAxisTicksLocations(toDoubleArray(values));
                    axes.setZAxisTicksLabels(toStringArray(values));
                    axes.setZAxisSubticks(rulerDrawingResult.getSubTicksDensity() - 1);
                }

                distanceRatio = rulerDrawingResult.getMaxDistToTicksDirNorm();

                zAxisLabelPositioner.setTicksDirection(new Vector3d(TICKS_SIZE * txs, TICKS_SIZE * tys, 0.0));
                zAxisLabelPositioner.setDistanceRatio(distanceRatio);
                zAxisLabelPositioner.setProjectedTicksDirection(rulerDrawingResult.getNormalizedTicksDirection().setZ(0));

                if (axes.getZAxisGridColor() != -1 || !axes.getZAxisVisible()) {
                    FloatBuffer vertexData;
                    if(axes.getZAxisLogFlag()) {
                        vertexData = getZGridData(rulerDrawingResult.getSubTicksValues(), rulerModel);
                    } else {
                        vertexData = getZGridData(values, rulerModel);
                    }
                    vertexBuffer.setData(vertexData, 4);

                    Transformation mirror;
                    try {
                        mirror = TransformationFactory.getScaleTransformation(
                                matrix[2] < 0 ? gridPosition : -gridPosition,
                                matrix[6] < 0 ? gridPosition : -gridPosition,
                                1
                        );
                    } catch (DegenerateMatrixException ignored) {
                        // Should never happens as long as gridPosition the value 1 or -1
                        mirror = TransformationFactory.getIdentity();
                    }

                    gridAppearance.setLineColor(ColorFactory.createColor(colorMap, axes.getZAxisGridColor()));
                    drawingTools.getTransformationManager().getModelViewStack().pushRightMultiply(mirror);
                    drawingTools.draw(new GeometryImpl(Geometry.DrawingMode.SEGMENTS, vertexBuffer), gridAppearance);
                    drawingTools.getTransformationManager().getModelViewStack().pop();
                }
            }
        }

        drawingTools.getCanvas().getBuffersManager().dispose(vertexBuffer);
    }

    private void setRulerBounds(AxisProperty axis, DefaultRulerModel rulerModel, double axisMin, double axisMax) {
        double min, max;
        if (axis.getReverse()) {
            min = axisMin;
            max = axisMax;
        } else {
            min = axisMax;
            max = axisMin;
        }

        if (axis.getLogFlag()) {
            min = Math.pow(10, min);
            max = Math.pow(10, max);
        }
        rulerModel.setValues(min, max);
    }

    private Vector3d computeXAxisPosition(double[] matrix, AxisProperty.AxisLocation axisLocation) {
        double y, z;
        if (axisLocation.equals(AxisProperty.AxisLocation.BOTTOM)) {
            z = -Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
            y = -Math.signum(matrix[6]) * z * Math.signum(matrix[10]);
            if (y == 0) {
                y = +1;
            }
        } else if (axisLocation.equals(AxisProperty.AxisLocation.MIDDLE)) {
            z = -Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
            y = -Math.signum(matrix[6]) * z * Math.signum(matrix[10]);
        } else {
            z = Math.signum(matrix[9]);  // First : switch Z such that Y was maximal.
            y = -Math.signum(matrix[6]) * z * Math.signum(matrix[10]);
            if (y == 0) {
                y = -1;
            }
        } // TODO : center, origin case ?!
        return new Vector3d(0, y, z);
    }

    private Vector3d computeYAxisPosition(double[] matrix, AxisProperty.AxisLocation axisLocation) {
        double x, z;
        if (axisLocation.equals(AxisProperty.AxisLocation.LEFT)) {
            z = -Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
            x = -Math.signum(matrix[2]) * z * Math.signum(matrix[10]);
            if (x == 0) {
                x = +1;
            }
        } else if (axisLocation.equals(AxisProperty.AxisLocation.MIDDLE)) {
            z = -Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
            x = -Math.signum(matrix[2]) * z * Math.signum(matrix[10]);
        } else {
            z = Math.signum(matrix[9]);  // First : switch Z such that Y was minimal.
            x = -Math.signum(matrix[2]) * z * Math.signum(matrix[10]); // Then switch X such that Z max but not in the middle.
            if (x == 0) {
                x = -1;
            }
        } // TODO : center, origin case ?!
        return new Vector3d(x, 0, z);
    }

    private String[] toStringArray(double[] values) {
        String[] r = new String[values.length];
        for (int i = 0; i < values.length; i++) {
            r[i] = String.valueOf(values[i]);
        }
        return r;
    }

    private Double[] toDoubleArray(double[] values) {
        Double[] r = new Double[values.length];
        for (int i = 0; i < values.length; i++) {
            r[i] = values[i];
        }
        return r;
    }

    /**
     * Build X grid data.
     * @param values X values where grid is drawn.
     * @param rulerModel used rulerModel to compute grid world position.
     * @return X grid data.
     */
    private FloatBuffer getXGridData(double[] values, RulerModel rulerModel) {
        FloatBuffer vertexData = FloatBuffer.allocate(values.length * 16);
        int limit = 0;
        for (double value : values) {
            float p = (float) rulerModel.getPosition(value).getX();
            if ((p != -1) && (p!=1)) {
                vertexData.put(p); vertexData.put(+1); vertexData.put(+1); vertexData.put(1);
                vertexData.put(p); vertexData.put(-1); vertexData.put(+1); vertexData.put(1);
                vertexData.put(p); vertexData.put(+1); vertexData.put(+1); vertexData.put(1);
                vertexData.put(p); vertexData.put(+1); vertexData.put(-1); vertexData.put(1);
                limit += 16;
            }
        }
        vertexData.limit(limit);
        return vertexData;
    }

    /**
     * Build Y grid data.
     * @param values Y values where grid is drawn.
     * @param rulerModel used rulerModel to compute grid world position.
     * @return Y grid data.
     */
    private FloatBuffer getYGridData(double[] values, RulerModel rulerModel) {
        FloatBuffer vertexData = FloatBuffer.allocate(values.length * 16);
        int limit = 0;
        for (double value : values) {
            float p = (float) rulerModel.getPosition(value).getY();
            if ((p != -1) && (p!=1)) {
                vertexData.put(+1); vertexData.put(p); vertexData.put(+1); vertexData.put(1);
                vertexData.put(-1); vertexData.put(p); vertexData.put(+1); vertexData.put(1);
                vertexData.put(+1); vertexData.put(p); vertexData.put(+1); vertexData.put(1);
                vertexData.put(+1); vertexData.put(p); vertexData.put(-1); vertexData.put(1);
                limit += 16;
            }
        }
        vertexData.limit(limit);
        return vertexData;
    }

    /**
     * Build Z grid data.
     * @param values Z values where grid is drawn.
     * @param rulerModel used rulerModel to compute grid world position.
     * @return Z grid data.
     */
    private FloatBuffer getZGridData(double[] values, RulerModel rulerModel) {
        FloatBuffer vertexData = FloatBuffer.allocate(values.length * 16);
        int limit = 0;
        for (double value : values) {
            float p = (float) rulerModel.getPosition(value).getZ();
            if ((p != -1) && (p!=1)) {
                vertexData.put(+1); vertexData.put(+1); vertexData.put(p); vertexData.put(1);
                vertexData.put(-1); vertexData.put(+1); vertexData.put(p); vertexData.put(1);
                vertexData.put(+1); vertexData.put(+1); vertexData.put(p); vertexData.put(1);
                vertexData.put(+1); vertexData.put(-1); vertexData.put(p); vertexData.put(1);
                limit += 16;
            }
        }
        vertexData.limit(limit);
        return vertexData;
    }

    public void disposeAll() {
        this.rulerDrawerManager.disposeAll();
    }

    public void update(String id, String property) {
        this.rulerDrawerManager.update(id, property);
    }

    public void dispose(String id) {
        this.rulerDrawerManager.dispose(id);
    }
}