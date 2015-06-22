/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Chenfeng ZHU
 *
 */
package org.scilab.modules.xcos.utils;

import java.util.ArrayList;
import java.util.List;

import org.scilab.modules.xcos.port.Orientation;

import com.mxgraph.model.mxCell;
import com.mxgraph.util.mxPoint;

/**
 * Provide methods to calculate the route.
 *
 */
public abstract class XcosRoute {

    /**
     * The error which can be accepted as it is aligned.
     */
    public final static double ALIGN_ERROR = 5;

    /**
     * The error which can be accepted as it is aligned strictly.
     */
    public final static double ALIGN_STRICT_ERROR = 0.01;

    /**
     * The distance for a point away to the port.
     */
    public final static double BEAUTY_AWAY_DISTANCE = 15;
    public final static double BEAUTY_AWAY_REVISION = 10;

    /**
     * Define a normal half size of block to avoid.
     */
    public final static double NORMAL_BLOCK_SIZE = 40;

    /**
     * Times trying to find a complex route.
     */
    public final static int TRY_TIMES = 3;

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
                        return true;
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
     *
     * @param x
     *            the x-coordinate of the point
     * @param y
     *            the y-coordinate of the point
     * @param allCells
     * @return <b>true</b> if one point is in at least one line.
     */
    public static boolean checkPointInLines(double x, double y, Object[] allCells) {
        for (Object o : allCells) {
            if (o instanceof mxCell) {
                mxCell edge = (mxCell) o;
                if (edge.isEdge()) {
                    List<mxPoint> listPoints = edge.getGeometry().getPoints();
                    if (listPoints == null || listPoints.size() <= 1) {
                    } else {
                        for (int i = 1; i < listPoints.size(); i++) {
                            mxPoint point1 = listPoints.get(i - 1);
                            mxPoint point2 = listPoints.get(i);
                            double x1 = point1.getX();
                            double y1 = point1.getY();
                            double x2 = point2.getX();
                            double y2 = point2.getY();
                            if (x1 == x2 && x != x1) {
                                return false;
                            }
                            if (y1 == y2 && y != y1) {
                                return false;
                            }
                            if (pointInLine(x, y, x1, y1, x2, y2)) {
                                return true;
                            }
                        }
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
            // if the edge is straight or vertical or horizontal style, there is
            // no way to check.
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
                if (linesCoincide(x1, y1, x2, y2, x3, y3, x4, y4)) {
                    return true;
                }
            }
        }
        return false;
    }

    /**
     * Check whether two lines coincide or not.
     *
     * @param x1
     *            the x-coordinate of the first point of the first line
     * @param y1
     *            the y-coordinate of the first point of the first line
     * @param x2
     *            the x-coordinate of the second point of the first line
     * @param y2
     *            the y-coordinate of the second point of the first line
     * @param x3
     *            the x-coordinate of the first point of the second line
     * @param y3
     *            the y-coordinate of the first point of the second line
     * @param x4
     *            the x-coordinate of the second point of the second line
     * @param y4
     *            the y-coordinate of the second point of the second line
     * @return <b>true</b> if two lines coincide.
     */
    private static boolean linesCoincide(double x1, double y1, double x2, double y2,
                                         double x3, double y3, double x4, double y4) {
        // the first line is inside the second line.
        if (pointInLine(x1, y1, x3, y3, x4, y4) && pointInLine(x2, y2, x3, y3, x4, y4)) {
            return true;
        }
        // the second line is inside the first line.
        if (pointInLine(x3, y3, x1, y1, x2, y2) && pointInLine(x4, y4, x1, y1, x2, y2)) {
            return true;
        }
        double i = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        // two lines are parallel.
        if (i == 0) {
            if (pointInLine(x1, y1, x3, y3, x4, y4) || pointInLine(x2, y2, x3, y3, x4, y4)
                    || pointInLine(x3, y3, x1, y1, x2, y2)
                    || pointInLine(x4, y4, x1, y1, x2, y2)) {
                return true;
            }
        }
        return false;
    }

    /**
     * Check whether the point is in the line segment or not.
     *
     * @param x1
     *            the x-coordinate of the point
     * @param y1
     *            the y-coordinate of the point
     * @param x2
     *            the x-coordinate of the first point of the line
     * @param y2
     *            the y-coordinate of the first point of the line
     * @param x3
     *            the x-coordinate of the second point of the line
     * @param y3
     *            the y-coordinate of the second point of the line
     * @return <b>true</b> if the point is in the line segment.
     */
    private static boolean pointInLine(double x1, double y1, double x2, double y2, double x3,
                                       double y3) {
        double l12 = Math.sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
        double l23 = Math.sqrt((x3 - x2) * (x3 - x2) + (y3 - y2) * (y3 - y2));
        double l13 = Math.sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
        if (l23 == (l12 + l13)) {
            return true;
        }
        return false;
    }

    /**
     * In the method, only 4 turning points at most are supported.
     *
     * @param p1
     *            the point away from the first port
     * @param p2
     *            the point away from the second port
     * @param allCells
     *            all the possible
     * @return
     */
    public static List<mxPoint> getSimpleRoute(mxPoint p1, mxPoint p2, Object[] allCells) {
        return getSimpleRoute(p1, null, p2, null, allCells);
    }

    /**
     * In the method, only 4 turning points at most are supported.
     *
     * @param p1
     *            the point away from the first port
     * @param o1
     *            the orientation of the first port
     * @param p2
     *            the point away from the second port
     * @param o2
     *            the orientation of the second port
     * @param allCells
     *            all the possible
     * @return
     */
    public static List<mxPoint> getSimpleRoute(mxPoint p1, Orientation o1, mxPoint p2,
            Orientation o2, Object[] allCells) {
        // point1 and point2 are not in the vertical or horizontal line.
        List<mxPoint> listSimpleRoute = new ArrayList<mxPoint>(0);
        List<Double> listX = new ArrayList<Double>(0);
        List<Double> listY = new ArrayList<Double>(0);
        double distance = XcosRoute.NORMAL_BLOCK_SIZE;
        double x1 = p1.getX();
        double y1 = p1.getY();
        double x2 = p2.getX();
        double y2 = p2.getY();
        // simplest situation
        if (!checkObstacle(x1, y1, x2, y1, allCells)
                && !checkObstacle(x2, y1, x2, y2, allCells)) {
            if (o1 != Orientation.EAST && o1 != Orientation.WEST) {
                listSimpleRoute.add(p1);
            }
            listSimpleRoute.add(new mxPoint(x2, y1));
            if (o2 != Orientation.NORTH && o2 != Orientation.SOUTH) {
                listSimpleRoute.add(p2);
            }
            return listSimpleRoute;
        } else if (!checkObstacle(x1, y1, x1, y2, allCells)
                   && !checkObstacle(x1, y2, x2, y2, allCells)) {
            if (o1 != Orientation.NORTH && o1 != Orientation.SOUTH) {
                listSimpleRoute.add(p1);
            }
            listSimpleRoute.add(new mxPoint(x1, y2));
            if (o2 != Orientation.EAST && o2 != Orientation.WEST) {
                listSimpleRoute.add(p2);
            }
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
            if (o1 != Orientation.EAST && o1 != Orientation.WEST) {
                listSimpleRoute.add(p1);
            }
            listSimpleRoute.add(new mxPoint(x, y1));
            listSimpleRoute.add(new mxPoint(x, y2));
            if (o2 != Orientation.EAST && o2 != Orientation.WEST) {
                listSimpleRoute.add(p2);
            }
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
            if (o1 != Orientation.NORTH && o1 != Orientation.SOUTH) {
                listSimpleRoute.add(p1);
            }
            listSimpleRoute.add(new mxPoint(x1, y));
            listSimpleRoute.add(new mxPoint(x2, y));
            if (o2 != Orientation.NORTH && o2 != Orientation.SOUTH) {
                listSimpleRoute.add(p2);
            }
            return listSimpleRoute;
        }
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
        return (start + end) / 2;
    }

    /**
     * Based on getSimpleRoute().
     *
     * @param p1
     *            the point away from the first port
     * @param o1
     *            the orientation of the first port
     * @param p2
     *            the point away from the second port
     * @param o2
     *            the orientation of the second port
     * @param allCells
     *            all the possible obstacles
     * @param times
     * @return
     */
    public static List<mxPoint> getComplexRoute(mxPoint p1, Orientation o1, mxPoint p2,
            Orientation o2, Object[] allCells, int times) {
        if (times <= 0) {
            return null;
        }
        double newPosition = NORMAL_BLOCK_SIZE;
        List<mxPoint> listComplexRoute = new ArrayList<mxPoint>(0);
        List<mxPoint> listTmp = new ArrayList<mxPoint>(0);
        listComplexRoute.add(p1);
        List<mxPoint> listNewP1 = new ArrayList<mxPoint>(0);
        if (o1 != Orientation.EAST) {
            mxPoint np1 = new mxPoint(p1.getX() - newPosition, p1.getY());
            if (!checkObstacle(p1.getX(), p1.getY(), np1.getX(), np1.getY(), allCells)) {
                listTmp = getSimpleRoute(np1, Orientation.WEST, p2, o2, allCells);
                if (listTmp != null && listTmp.size() > 0) {
                    listComplexRoute.addAll(listTmp);
                    return listComplexRoute;
                }
                listNewP1.add(np1);
            }
        }
        if (o1 != Orientation.WEST) {
            mxPoint np1 = new mxPoint(p1.getX() + newPosition, p1.getY());
            if (!checkObstacle(p1.getX(), p1.getY(), np1.getX(), np1.getY(), allCells)) {
                listTmp = getSimpleRoute(np1, Orientation.EAST, p2, o2, allCells);
                if (listTmp != null && listTmp.size() > 0) {
                    listComplexRoute.addAll(listTmp);
                    return listComplexRoute;
                }
                listNewP1.add(np1);
            }
        }
        if (o1 != Orientation.SOUTH) {
            mxPoint np1 = new mxPoint(p1.getX(), p1.getY() - newPosition);
            if (!checkObstacle(p1.getX(), p1.getY(), np1.getX(), np1.getY(), allCells)) {
                listTmp = getSimpleRoute(np1, Orientation.NORTH, p2, o2, allCells);
                if (listTmp != null && listTmp.size() > 0) {
                    listComplexRoute.addAll(listTmp);
                    return listComplexRoute;
                }
                listNewP1.add(np1);
            }
        }
        if (o1 != Orientation.NORTH) {
            mxPoint np1 = new mxPoint(p1.getX(), p1.getY() + newPosition);
            if (!checkObstacle(p1.getX(), p1.getY(), np1.getX(), np1.getY(), allCells)) {
                listTmp = getSimpleRoute(np1, Orientation.SOUTH, p2, o2, allCells);
                if (listTmp != null && listTmp.size() > 0) {
                    listComplexRoute.addAll(listTmp);
                    return listComplexRoute;
                }
                listNewP1.add(np1);
            }
        }
        for (mxPoint np1 : listNewP1) {
            listTmp = getComplexRoute(np1, null, p2, o2, allCells, times - 1);
            if (listTmp != null && listTmp.size() > 1) {
                listComplexRoute.addAll(listTmp);
                return listComplexRoute;
            }
        }
        listComplexRoute.clear();
        return listComplexRoute;
    }

}
