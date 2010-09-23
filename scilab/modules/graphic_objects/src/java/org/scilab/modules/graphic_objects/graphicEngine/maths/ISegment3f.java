package org.scilab.modules.graphic_objects.graphicEngine.maths;

/**
 * Author: Lando Pierre
 * Date: june 24, 2010
 *
 * Interface of a segment.
 * The segment is defined as two point.
 */
public interface ISegment3f {

    /**
     * Set the first point of this segment.
     * @param firstPoint the first point of this segment.
     */
    void setFirstPoint(IVector3f firstPoint);

    /**
     * return the first point of this segment.
     * @return the first point of this segment.
     */
    IVector3f getFirstPoint();

    /**
     * Set the second point of this segment.
     * @param secondPoint the second point of this segment.
     */
    void setSecondPoint(IVector3f secondPoint);

    /**
     * return the second point of this segment.
     * @return the second point of this segment.
     */
    IVector3f getSecondPoint();

    /**
     * @param value a scalar.
     * @return firstPoint * (1 - value) + secondPoint * value.
     */
    IVector3f getLinear(float value);

    /**
     * @param delta vector added to the segment.
     * @return this segment shifted by delta.
     */
    ISegment3f plus(IVector3f delta);

    /**
     * @param delta opposite vector added to the segment.
     * @return this segment shifted by -delta. 
     */
    ISegment3f minus(IVector3f delta);

    /**
     * @return secondPoint - firstPoint
     */
    IVector3f getDirection();
}
