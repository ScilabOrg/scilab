package org.scilab.modules.graphic_objects.graphicEngine.maths;

/**
 * Author: Lando Pierre
 * Date: june 24, 2010
 *
 * Basic implementation of ISegment3f.
 */
public class Segment3f implements ISegment3f {
    private IVector3f firstPoint = null;
    private IVector3f secondPoint = null;

    public Segment3f() {
    }

    public Segment3f(IVector3f firstPoint, IVector3f secondPoint) {
        setFirstPoint(new Vector3f(firstPoint));
        setSecondPoint(new Vector3f(secondPoint));
    }

    public Segment3f(ISegment3f segment) {
        this(segment.getFirstPoint(), segment.getSecondPoint());
    }

    
    public void setFirstPoint(IVector3f firstPoint) {
        this.firstPoint = firstPoint;
    }

    
    public IVector3f getFirstPoint() {
        return new Vector3f(firstPoint);
    }

    
    public void setSecondPoint(IVector3f secondPoint) {
        this.secondPoint = secondPoint;
    }

    
    public IVector3f getSecondPoint() {
        return new Vector3f(secondPoint);
    }

    
    public IVector3f getLinear(float value) {
        return firstPoint.times(1f - value).plus(secondPoint.times(value));
    }

    
    public ISegment3f plus(IVector3f delta) {
        return new Segment3f(firstPoint.plus(delta), secondPoint.plus(delta));
    }

    
    public ISegment3f minus(IVector3f delta) {
        return new Segment3f(firstPoint.minus(delta), secondPoint.minus(delta));
    }

    
    public IVector3f getDirection() {
        return secondPoint.minus(firstPoint);
    }

    
    public String toString() {
        return "A: " + firstPoint + "    B: " + secondPoint;
    }
}
