/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Chenfeng ZHU
 *
 */
package org.scilab.modules.xcos.utils;

import java.util.ArrayList;
import java.util.List;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxPoint;

/**
 * Provide methods to calculate the route.
 *
 */
public class XcosRoute {

    /**
     * The error which can be accepted as it is aligned.
     */
    public final static double ALIGN_ERROR = 5;

    /**
     * The error which can be accepted as it is aligned strictly.
     */
    public final static double ALIGN_STRICT_ERROR = 0.01;

    /**
     *
     */
    public final static double BEAUTY_DISTANCE = 10;

    /**
     * Check whether the two points are aligned(vertical or horizontal) or not
     * (Considering the acceptable error - <code>XcosRoute.ALIGN_ERROR</code>).
     *
     * @param point1
     *            the first point
     * @param point2
     *            the second point
     * @return <b>true</b> if two points are aligned.
     */
    public static boolean isAligned(mxPoint point1, mxPoint point2) {
        double x1 = point1.getX();
        double y1 = point1.getY();
        double x2 = point2.getX();
        double y2 = point2.getY();
        return isAligned(x1, y1, x2, y2);
    }

    /**
     * Check whether the two points are aligned(vertical or horizontal) or not
     * (Considering the acceptable error - <code>XcosRoute.ALIGN_ERROR</code>).
     *
     * @param x1
     *            the x-coordinate of the first point
     * @param y1
     *            the y-coordinate of the first point
     * @param x2
     *            the x-coordinate of the second point
     * @param y2
     *            the y-coordinate of the second point
     * @return <b>true</b> if two points are aligned.
     */
    public static boolean isAligned(double x1, double y1, double x2, double y2) {
        double error = XcosRoute.ALIGN_ERROR;
        if (Math.abs(x2 - x1) < error) {
            return true;
        }
        if (Math.abs(y2 - y1) < error) {
            return true;
        }
        return false;
    }

    /**
     * Check whether the two points are strictly aligned(vertical or horizontal)
     * or not. (The accepted error is <code>XcosRoute.ALIGN_STRICT_ERROR</code>)
     *
     * @param x1
     *            the x-coordinate of the first point
     * @param y1
     *            the y-coordinate of the first point
     * @param x2
     *            the x-coordinate of the second point
     * @param y2
     *            the y-coordinate of the second point
     * @return <b>true</b> if two points are aligned.
     */
    public static boolean isStrictlyAligned(double x1, double y1, double x2, double y2) {
        double error = XcosRoute.ALIGN_STRICT_ERROR;
        if (Math.abs(x2 - x1) < error) {
            return true;
        }
        if (Math.abs(y2 - y1) < error) {
            return true;
        }
        return false;
    }

    /**
     * Check whether there are blocks between two points.
     *
     * @param x1
     *            the x-coordinate of the first point of the line
     * @param y1
     *            the y-coordinate of the first point of the line
     * @param x2
     *            the x-coordinate of the second point of the line
     * @param y2
     *            the y-coordinate of the second point of the line
     * @param allCells
     * @return <b>true</b> if there is at least one blocks in the line.
     */
    public static boolean checkObstacle(double x1, double y1, double x2, double y2,
                                        Object[] allCells) {
        for (Object o : allCells) {
            if (o instanceof mxCell) {
                mxCell c = (mxCell) o;
                if (c.isEdge()) {
                    if (checkLinesCoincide(x1, y1, x2, y2, c)) {
                        ;// return true;
                    }
                } else {
                    mxPoint interction = c.getGeometry().intersectLine(x1, y1, x2, y2);
                    if (interction != null) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    /**
     * Check whether two lines coincide or not. The lines are vertical or
     * horizontal. <br/>
     * <b>NOTE:</b> This method is used to check coincidence, NOT intersection!
     *
     * @param x1
     *            the x-coordinate of the first point of the first line
     * @param y1
     *            the y-coordinate of the first point of the first line
     * @param x2
     *            the x-coordinate of the second point of the first line
     * @param y2
     *            the y-coordinate of the second point of the first line
     * @param edge
     *            the second line
     * @return <b>true</b> if two lines coincide completely or partly.
     */
    private static boolean checkLinesCoincide(double x1, double y1, double x2, double y2,
            mxCell edge) {
        if (edge.isVertex()) {
            return false;
        }
        // mxICell source = line.getSource();
        // mxICell target = line.getTarget();
        List<mxPoint> listPoints = edge.getGeometry().getPoints();
        if (listPoints == null || listPoints.size() == 0) {
            // if this, the edge is straight or vertical or horizontal style.
            // source.getGeometry().getCenterX();
            // target.getGeometry().getCenterY();
        } else if (listPoints.size() == 1) {
        } else {
            for (int i = 1; i < listPoints.size(); i++) {
                mxPoint point3 = listPoints.get(i - 1);
                mxPoint point4 = listPoints.get(i);
                double x3 = point3.getX();
                double y3 = point3.getY();
                double x4 = point4.getX();
                double y4 = point4.getY();
                if (x1 == x2) {
                    if (x3 != x1 || x4 != x1) {
                        return false;
                    }
                }
                if (y1 == y2) {
                    if (y3 != y1 || y4 != y1) {
                        return false;
                    }
                }
                // unfinished;
            }
        }
        return false;
    }

    /**
     * In the method, only 4 turning points at most are supported.
     *
     * @param p1
     *            the source point
     * @param p2
     *            the target point
     * @param allCells
     *            all the possible
     * @return
     */
    public static List<mxPoint> getSimpleRoute(mxPoint p1, mxPoint p2, Object[] allCells) {
        // point1 and point2 are not in the vertical or horizontal line.
        List<mxPoint> listSimpleRoute = new ArrayList<mxPoint>(0);
        List<Double> listX = new ArrayList<Double>(0);
        List<Double> listY = new ArrayList<Double>(0);
        double distance = XcosRoute.BEAUTY_DISTANCE;
        double x1 = p1.getX();
        double y1 = p1.getY();
        double x2 = p2.getX();
        double y2 = p2.getY();
        // simplest situation
        if (!checkObstacle(x1, y1, x2, y1, allCells)
                && !checkObstacle(x2, y1, x2, y2, allCells)) {
            listSimpleRoute.add(p1);
            listSimpleRoute.add(new mxPoint(x2, y1));
            listSimpleRoute.add(p2);
            return listSimpleRoute;
        } else if (!checkObstacle(x1, y1, x1, y2, allCells)
                   && !checkObstacle(x1, y2, x2, y2, allCells)) {
            listSimpleRoute.add(p1);
            listSimpleRoute.add(new mxPoint(x1, y2));
            listSimpleRoute.add(p2);
            return listSimpleRoute;
        }
        // check the nodes in x-coordinate
        double xmax = Math.max(x1 + distance, x2 + distance);
        double xmin = Math.min(x1 - distance, x2 - distance);
        for (double xi = xmin; xi <= xmax; xi++) {
            if (!checkObstacle(x1, y1, xi, y1, allCells)
                    && !checkObstacle(xi, y1, xi, y2, allCells)
                    && !checkObstacle(xi, y2, x2, y2, allCells)) {
                listX.add(xi);
            }
        }
        if (listX.size() > 0) {
            double x = choosePoint(listX);
            listSimpleRoute.add(p1);
            listSimpleRoute.add(new mxPoint(x, y1));
            listSimpleRoute.add(new mxPoint(x, y2));
            listSimpleRoute.add(p2);
            return listSimpleRoute;
        }
        // check the nodes in y-coordinate
        double ymax = Math.max(y1 + distance, y2 + distance);
        double ymin = Math.min(y1 - distance, y2 - distance);
        for (double yi = ymin; yi <= ymax; yi++) {
            if (!checkObstacle(x1, y1, x1, yi, allCells)
                    && !checkObstacle(x1, yi, x2, yi, allCells)
                    && !checkObstacle(x2, yi, x2, y2, allCells)) {
                listY.add(yi);
            }
        }
        if (listY.size() > 0) {
            double y = choosePoint(listY);
            if (y > 0) {
                listSimpleRoute.add(p1);
                listSimpleRoute.add(new mxPoint(x1, y));
                listSimpleRoute.add(new mxPoint(x2, y));
                listSimpleRoute.add(p2);
                return listSimpleRoute;
            }
        }
        // listRoute.add(p1);
        // listRoute.add(p2);
        return listSimpleRoute;
    }

    /**
     * Choose a better point (which is the average number in the widest range in
     * a certain density) from the list which contains discrete numbers.
     *
     * @param list
     * @return
     */
    private static double choosePoint(List<Double> list) {
        if (list == null || list.size() == 0) {
            return 0;
        }
        double start = list.get(0);
        double start_temp = list.get(0);
        double end = list.get(0);
        double end_temp = list.get(0);
        int counter = 1;
        for (int i = 1; i < list.size(); i++) {
            if (Math.abs(list.get(i) - list.get(i - 1)) <= 1.1) {
                end_temp = list.get(i);
                counter++;
            } else {
                if (counter == 1) {
                    start_temp = list.get(i);
                    continue;
                }
                if (Math.abs(end_temp - start_temp) > Math.abs(end - start)) {
                    start = start_temp;
                    end = end_temp;
                    start_temp = list.get(i);
                    end_temp = list.get(i);
                    counter = 1;
                }
            }
        }
        if (Math.abs(end_temp - start_temp) > Math.abs(end - start)) {
            start = start_temp;
            end = end_temp;
        }
        // if it is too narrow, give up.
        // if (Math.abs(end - start) < BEAUTY_DISTANCE) {
        // return -1;
        // }
        return (start + end) / 2;
    }

    private List<mxPoint> listRoute = new ArrayList<mxPoint>(0);
    private int bound = 1000;
    private int turning_times = 5; // the most turning times.
    private int vTurning = bound / turning_times;

    public List<mxPoint> getRoute(mxPoint current, mxPoint target, mxPoint last,
                                  Object[] allCells) {
        listRoute.clear();
        this.pathValue(current, target, last, allCells);
        return listRoute;
    }

    /**
     * Automatically find the optimal path. But it will out of Stack.
     *
     * @param current
     * @param target
     * @param last
     * @param allCells
     * @return
     */
    protected int pathValue(mxPoint current, mxPoint target, mxPoint last, Object[] allCells) {
        double step = BEAUTY_DISTANCE;
        int value = (int) step;
        int vEast = 0; // value in east direction
        int vSouth = 0;// value in south direction
        int vWest = 0;// value in west direction
        int vNorth = 0;// value in north direction
        mxPoint next = new mxPoint(current);
        double x1 = current.getX();
        double y1 = current.getY();
        double x2 = target.getX();
        double y2 = target.getY();
        double x3 = last.getX();
        double y3 = last.getY();
        double dx = current.getX() - last.getX();
        double dy = current.getY() - last.getY();
        // if it goes out of bounds, it will be a dead route.
        if (x1 > bound || y1 > bound || x1 < 0 || y1 < 0) {
            return Integer.MAX_VALUE;
        }
        // if there is a block, it will be a dead route.
        if (checkObstacle(x3, x3, x1, y1, allCells)) {
            return Integer.MAX_VALUE;
        }
        // if it can "see" the target, get this point.
        if (x1 == x2 || y1 == y2) {
            if (checkObstacle(x1, y1, x2, y2, allCells)) {
                listRoute.add(current);
                return 0;
            }
        }
        // in case that last and current is the same point.
        if (x1 != x2 && y1 != y2) {
            if (Math.abs(x1 - x2) == (Math.abs(x1 - x3) + Math.abs(x3 - x2))) {
                if (!checkObstacle(x2, y1, x2, y2, allCells)) {
                    listRoute.add(new mxPoint(x2, y1));
                    return 0;
                }
            }
            if (Math.abs(y1 - y2) == (Math.abs(y1 - y3) + Math.abs(y3 - y2))) {
                if (!checkObstacle(x1, y2, x2, y2, allCells)) {
                    listRoute.add(new mxPoint(x1, y2));
                    return 0;
                }
            }
        }
        // it will never go back. And it takes more effort to turn.
        if (dx >= 0 && dy == 0) { // EAST→
            vWest = Integer.MAX_VALUE;
            vSouth = vTurning;
            vNorth = vTurning;
        } else if (dx == 0 && dy > 0) { // SOUTH↓
            vNorth = Integer.MAX_VALUE;
            vEast = vTurning;
            vWest = vTurning;
        } else if (dx < 0 && dy == 0) { // WEST←
            vEast = Integer.MAX_VALUE;
            vSouth = vTurning;
            vNorth = vTurning;
        } else if (dx == 0 && dy < 0) { // NORHT↑
            vSouth = Integer.MAX_VALUE;
            vEast = vTurning;
            vWest = vTurning;
        }
        // calculate the value getting to next point.
        if (vEast < Integer.MAX_VALUE) {
            vEast += pathValue(new mxPoint(x1 + step, y1), target, current, allCells);
        }
        if (vSouth < Integer.MAX_VALUE) {
            vSouth += pathValue(new mxPoint(x1, y1 + step), target, current, allCells);
        }
        if (vWest < Integer.MAX_VALUE) {
            vWest += pathValue(new mxPoint(x1 - step, y1), target, current, allCells);
        }
        if (vNorth < Integer.MAX_VALUE) {
            vNorth += pathValue(new mxPoint(x1, y1 - step), target, current, allCells);
        }
        // get the minimum value
        if (vEast <= vSouth && vEast <= vWest && vEast <= vNorth) {
            next.setX(x1 + step);
            value = vEast;
        } else if (vSouth <= vEast && vSouth <= vWest && vSouth <= vNorth) {
            next.setY(y1 + step);
            value = vSouth;
        } else if (vWest <= vEast && vWest <= vSouth && vWest <= vNorth) {
            next.setX(x1 - step);
            value = vWest;
        } else if (vNorth <= vEast && vNorth <= vSouth && vNorth <= vWest) {
            next.setY(y1 - step);
            value = vNorth;
        }
        listRoute.add(next);
        return value;
    }

}
