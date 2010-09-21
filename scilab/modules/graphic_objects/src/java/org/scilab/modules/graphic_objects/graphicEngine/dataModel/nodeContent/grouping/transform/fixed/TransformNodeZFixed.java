package org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform.fixed;

import org.scilab.modules.graphic_objects.graphicEngine.dataModel.nodeContent.grouping.transform.ITransformNodeModel;
import org.scilab.modules.graphic_objects.graphicEngine.drawer.AbstractDrawableModel;
import org.scilab.modules.graphic_objects.graphicEngine.maths.IVector3f;
import org.scilab.modules.graphic_objects.graphicEngine.maths.Vector3f;

/**
 * Author: Lando Pierre
 * Date: june 29, 2010
 */
public class TransformNodeZFixed extends AbstractDrawableModel implements ITransformNodeModel {
    private final float[] matrix;
    private final float[] invMatrix;

    public TransformNodeZFixed(IVector3f givenZ) {
        IVector3f z = new Vector3f(givenZ);
        z.normalize();

        IVector3f x = new Vector3f(z.getY() - z.getZ(), z.getZ() - z.getX(), z.getX() - z.getY());
        x.normalize();

        IVector3f y = z.crossProduct(x).normalize();
        x = y.crossProduct(z).normalize();



        invMatrix = new float[]{
                x.getX(), y.getX(), z.getX(), 0,
                x.getY(), y.getY(), z.getY(), 0,
                x.getZ(), y.getZ(), z.getZ(), 0,
                0, 0, 0, 1,
        };

        matrix = new float[]{
                x.getX(), x.getY(), x.getZ(), 0,
                y.getX(), y.getY(), y.getZ(), 0,
                z.getX(), z.getY(), z.getZ(), 0,
                0, 0, 0, 1,
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

    
    public float[] getTransformMatrix() {
        return matrix;
    }

    
    public void setTranslation(IVector3f translation) {
        //TODO : remove.
    }

    
    public float[] getInverseMatrix() {
        return invMatrix;
    }

    
    public String getType() {
        return type;
    }
}
