package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform;

import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;
import org.scilab.modules.graphic_objects.graphicEngine.maths.MathUtil;
import org.scilab.modules.graphic_objects.graphicEngine.maths.Vector3f;

/**
 * Author: Lando Pierre
 * Date: june 24, 2010
 */
public class TransformNodeImpl extends AbstractDrawableModel implements ITransformNodeModel {
    protected float[] matrix = new float[] {
            1,  0,  0,  0,
            0,  1,  0,  0,
            0,  0,  1,  0,
            0,  0,  0,  1
    };

    protected float[] invMatrix = new float[] {
            1,  0,  0,  0,
            0,  1,  0,  0,
            0,  0,  1,  0,
            0,  0,  0,  1
    };

    public TransformNodeImpl() {
        
    }

    public TransformNodeImpl(IVector3f translation, IVector3f scale) {
        if (scale == null) {
            scale = new Vector3f(1f, 1f,1f);
        }

        if (translation == null) {
            translation = new Vector3f(0f, 0f, 0f);
        }

        matrix = new float[]{
                scale.getX(), 0, 0, 0,
                0, scale.getY(), 0, 0,
                0, 0, scale.getZ(), 0,
                translation.getX(), translation.getY(), translation.getZ(), 1
        };

        invMatrix = new float[]{
                1f/scale.getX(), 0, 0, 0,
                0, 1f/scale.getY(), 0, 0,
                0, 0, 1f/scale.getZ(), 0,
                -translation.getX()/scale.getX(), -translation.getY()/scale.getY(), -translation.getZ()/scale.getZ(), 1
        };

        
        // TODO: unit test.
        /*
        float[] testMatrix = MathUtil.matrixMultiply(matrix, invMatrix);
        System.out.println(testMatrix[0] + "\t|" + testMatrix[4] + "\t|" + testMatrix[8] + "\t|" + testMatrix[12] + "\t|");
        System.out.println(testMatrix[1] + "\t|" + testMatrix[5] + "\t|" + testMatrix[9] + "\t|" + testMatrix[13] + "\t|");
        System.out.println(testMatrix[2] + "\t|" + testMatrix[6] + "\t|" + testMatrix[10] + "\t|" + testMatrix[14] + "\t|");
        System.out.println(testMatrix[3] + "\t|" + testMatrix[7] + "\t|" + testMatrix[11] + "\t|" + testMatrix[15] + "\t|");
        System.out.println();
                             */
    }

    
    public void setTranslation(IVector3f translation) {
        if (translation != null) {
            matrix[12] = translation.getX() * matrix[15];
            matrix[13] = translation.getY() * matrix[15];
            matrix[14] = translation.getZ() * matrix[15];

            float[] trInv = new float[] {
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    -translation.getX(), -translation.getY(), -translation.getZ(), 1
            };
            invMatrix = MathUtil.matrixMultiply(invMatrix.clone(), trInv);
        }
    }

    
    public float[] getInverseMatrix() {
        return invMatrix;
    }

    
    public float[] getTransformMatrix() {
        return matrix;
    }

    
    public String getType() {
        return type;
    }
}
