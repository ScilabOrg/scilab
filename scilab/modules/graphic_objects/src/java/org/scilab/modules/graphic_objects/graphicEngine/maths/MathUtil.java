/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.graphicEngine.maths;

import org.scilab.modules.graphic_objects.util.MVC.tree.node.INodeController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.IGraphicController;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform.ITransformNodeModel;
import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.navigation.ICameraModel;
import org.scilab.modules.graphic_objects.graphicEngine.renderingZone.IRenderingZoneController;

/**
 * Author: Lando Pierre
 * Date: 20 mai 2010
 *
 *
 * A class full of very usefull maths function.
 */
public class MathUtil {

    /**
     * @param renderingZone a given rendering zone.
     * @param windowPosition a given vector corresponding to a normalized (ie: in [0, 1]^3) window position.
     * @return the world coordinate corresponding to the given window position in the given rendering zone.  
     */
    public static IVector3f unProject(IRenderingZoneController renderingZone, IVector3f windowPosition) {
        IVector3f v = new Vector3f(
                2f*windowPosition.getX() - 1f,
                2f*windowPosition.getY() - 1f,
                (2f*windowPosition.getZ()) - 1f
        );
        return matrixMultiply(getUnProjectMatrix(renderingZone), v);
    }

    /**
     * @param renderingZone the given rendering zone.
     * @return the projection matrix corresponding to the given renderingZone.
     */
    public static float[] getProjectionMatrix(IRenderingZoneController renderingZone) {
        if (renderingZone == null) {
            return null;
        } else if (renderingZone.getCamera() == null) {
            return null;
        } else {
            ICameraModel camera = renderingZone.getCamera();
            float realAspectRation = renderingZone.getLayoutPolicy().getAspectRatio();

            float matrix1[] = camera.getOrientation().getRotationMatrix();
            IVector3f v= camera.getOrientation().conjugate(camera.getPosition());

            matrix1[12] = -v.getX();
            matrix1[13] = -v.getY();
            matrix1[14] = -v.getZ();


            float delta = camera.getNearPlane()* (float) Math.tan(Math.toRadians(camera.getFieldOfView()/2f));
            float dx;
            float dy;
            if(realAspectRation < camera.getAspectRatio()) {
                dx = delta * camera.getAspectRatio();
                dy = delta * camera.getAspectRatio() / realAspectRation;
            } else {
                dx = delta * realAspectRation;
                dy = delta;
            }

            float zNear = camera.getNearPlane();
            float zFar = camera.getFarPlane();

            float matrix2[] = new float[]{
                    zNear/dx,	0,			0,	0,
                    0,			zNear/dy,	0,	0,
                    0,			0,			(zNear+zFar)/(zNear-zFar), -1,
                    0,			0,			(2*zNear*zFar)/(zNear-zFar),	0
                    };

            return matrixMultiply(matrix1, matrix2);
        }
    }

    /**
     * @param renderingZone the given rendering zone.
     * @return the unproject matrix corresponding to the given rendering zone.
     */
    public static float[] getUnProjectMatrix(IRenderingZoneController renderingZone) {
        if (renderingZone == null) {
            return null;
        } else {
            ICameraModel camera = renderingZone.getCamera();
            if (camera != null) {
                float realAspectRation = renderingZone.getLayoutPolicy().getAspectRatio();
                float matrix1[] = camera.getOrientation().getUnRotateMatrix();
                IVector3f v= camera.getPosition();

                matrix1[12] = v.getX();
                matrix1[13] = v.getY();
                matrix1[14] = v.getZ();


                float delta = camera.getNearPlane()* (float) Math.tan(Math.toRadians(camera.getFieldOfView()/2f));
                float dx;
                float dy;
                if(realAspectRation < camera.getAspectRatio()) {
                    dx = delta * camera.getAspectRatio();
                    dy = delta * camera.getAspectRatio() / realAspectRation;
                } else {
                    dx = delta * realAspectRation;
                    dy = delta;
                }

                float zNear = camera.getNearPlane();
                float zFar = camera.getFarPlane();

                float matrix2[] = new float[]{
                        dx/zNear,	0,			0,	0,
                        0,			dy/zNear,	0,	0,
                        0,			0,          0, (zNear-zFar) / (2*zNear*zFar),
                        0,			0,         -1, (zNear+zFar) / (2*zNear*zFar)
                        };

                return matrixMultiply(matrix2, matrix1);
            } else {
                return null;
            }
        }
    }

    /**
     * @param matrix1 first matrix.
     * @param matrix2 second matrix.
     * @return the product of the two given matrix.
     */
    public static float[] matrixMultiply(float[] matrix1, float[] matrix2) {
        float returnedMatrix[] = new float[16];

        for (int l = 0 ; l < 4 ; l++) {
            for (int c = 0 ; c < 4 ; c++) {
                float r=0;
                for (int i = 0 ; i<4 ; i++) {
                    r+= matrix1[4*l+i] * matrix2[4*i+c]; 
                }
                returnedMatrix[4*l+c] = r;
            }
        }
        return returnedMatrix;
    }

    /**
     * @param matrix a given matrix.
     * @param vector a given vector (x, y, z).
     * @return the vector result of matrix times vector (x, y, z, 1).
     */
    public static IVector3f matrixMultiply(float[] matrix, IVector3f vector) {
        if (matrix == null || vector == null || matrix.length != 16) {
            return null;
        } else {
            return new Vector3f(
                matrix[0] * vector.getX() + matrix[4] * vector.getY() + matrix[8] * vector.getZ() + matrix[12],
                matrix[1] * vector.getX() + matrix[5] * vector.getY() + matrix[9] * vector.getZ() + matrix[13],
                matrix[2] * vector.getX() + matrix[6] * vector.getY() + matrix[10] * vector.getZ() + matrix[14],
                matrix[3] * vector.getX() + matrix[7] * vector.getY() + matrix[11] * vector.getZ() + matrix[15]
            );
        }
    }

    /**
     * See http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm
     * @param firstSegment first given segment.
     * @param secondSegment second given segment.
     * @return shortest segment between firstSegment and secondSegment; 
     */
    public static ISegment3f getNearest(ISegment3f firstSegment, ISegment3f secondSegment) {
        try {
            float [] coord = getNearestCoord(firstSegment, secondSegment);

            return new Segment3f(firstSegment.getLinear(coord[0]), secondSegment.getLinear(coord[1]));
        } catch (NullPointerException e) {
            return null;
        }
    }

    /**
     * See http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm
     * @param firstSegment first given segment.
     * @param secondSegment second given segment.
     * @return coord (in respective segment) of the shortest segment between firstSegment and secondSegment;
     */
    public static float[] getNearestCoord(ISegment3f firstSegment, ISegment3f secondSegment) {
        try {
            IVector3f u = firstSegment.getDirection();
            IVector3f v = secondSegment.getDirection();
            IVector3f w0 = firstSegment.getFirstPoint().minus(secondSegment.getFirstPoint());

            float a = u.dot(u);
            float b = u.dot(v);
            float c = v.dot(v);
            float d = u.dot(w0);
            float e = v.dot(w0);

            float div = a*c - b*b;

            // Parallel lines.
            if (Math.abs(div) <= Float.MIN_VALUE*1024) {
                return null;
            }

            float sc = (b*e - c*d) / div;
            float tc = (a*e - b*d) / div;

            return new float[] {sc, tc};
        } catch (NullPointerException e) {
            return null;
        }
    }


    /**
     * @param node a node controller from a graphic tree.
     * @param position a position relative to the given node.
     * @return the world position corresponding to the given position.
     */
    public static IVector3f toWorldCoordinate(INodeController<IGraphicController> node, IVector3f position) {
        // TODO !!
        System.out.println("WARNING WARNING WARNING : 'toWorldCoordinate' have never been tested !!!");
        IVector3f newPosition;
        if (node.getContent() instanceof ITransformNodeModel) {
            ITransformNodeModel transformNode = (ITransformNodeModel) node.getContent();
            newPosition = matrixMultiply(transformNode.getTransformMatrix(), position);
        } else {
            newPosition = position;
        }

        if (node.getParentNode() == null) {
            return newPosition;
        } else {
            return toWorldCoordinate(node.getParentNode(), newPosition);
        }
    }

    /**
     * @param node a node controller from a graphic tree.
     * @param position a world relative position.
     * @return the position relative to the given node and corresponding to the given world position.
     */
    public static IVector3f toLocalCoordinate(INodeController<IGraphicController> node, IVector3f position) {
        IVector3f parentPosition;

        if (node.getParentNode() == null) {
            parentPosition = position;
        } else {
            parentPosition = toLocalCoordinate(node.getParentNode(), position);
        }

        if (node.getContent() instanceof ITransformNodeModel) {
            ITransformNodeModel transformNode = (ITransformNodeModel) node.getContent();
            return matrixMultiply(transformNode.getInverseMatrix(), parentPosition);
        } else {
            return parentPosition;
        }
    }

    /**
     * @param node a node controller from a graphic tree.
     * @param segment a segment with coordinate relative to the given node.
     * @return a segment with world coordinate corresponding to the given segment.
     */
    public static ISegment3f toWorldCoordinate(INodeController<IGraphicController> node, ISegment3f segment) {
        return new Segment3f(toWorldCoordinate(node, segment.getFirstPoint()), toWorldCoordinate(node, segment.getSecondPoint()));
    }

    /**
     * @param node a node controller from a graphic tree.
     * @param segment a segment with world coordinate.
     * @return a segment with coordinate relative to the give node and corresponding to the given segment.
     */
    public static ISegment3f toLocalCoordinate(INodeController<IGraphicController> node, ISegment3f segment) {
        return new Segment3f(toLocalCoordinate(node, segment.getFirstPoint()), toLocalCoordinate(node, segment.getSecondPoint()));
    }

    /**
     * This method compute the intersection between the line path through the given segment and the given plane.
     * @param segment given segment.
     * @param plane given plane.
     * @return the intersection between the line path through the given segment and the given plane.
     */
    public static IVector3f getInteraction(ISegment3f segment, IPlane3f plane) {
        float s = plane.getScalar();
        IVector3f n = plane.getNormal();
        IVector3f u = segment.getDirection();

        float si = (s - n.dot(segment.getFirstPoint())) / (n.dot(u));

        return segment.getLinear(si);
    }
}
