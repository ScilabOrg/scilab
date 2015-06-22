/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Chenfeng ZHU
 *
 */
package org.scilab.modules.xcos.utils;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;

import com.mxgraph.model.mxCell;
import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxPoint;
import com.mxgraph.util.mxUtils;

/**
 * Provide methods to calculate the route.
 *
 */
public abstract class XcosRouteUtils {

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
    public final static double BEAUTY_AWAY_DISTANCE = 30;
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
        double error = XcosRouteUtils.ALIGN_ERROR;
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
        double error = XcosRouteUtils.ALIGN_STRICT_ERROR;
        if (Math.abs(x2 - x1) < error) {
            return true;
        }
        if (Math.abs(y2 - y1) < error) {
            return true;
        }
        return false;
    }

    /**
     * Get all points of one link including the source port and target port.
     *
     * @param link
     * @return
     */
    protected static List<mxPoint> getLinkPoints(BasicLink link) {
        List<mxPoint> list = new ArrayList<mxPoint>(0);
        mxICell source = link.getSource();
        mxGeometry sourceGeo = source.getGeometry();
        double sourceX = sourceGeo.getCenterX();
        double sourceY = sourceGeo.getCenterY();
        mxICell sourceParent = source.getParent();
        mxGeometry srcParGeo = sourceParent.getGeometry();
        if (srcParGeo == null) {
            srcParGeo = new mxGeometry(0, 0, 0, 0);
        }
        mxPoint offset = sourceGeo.getOffset();
        if (offset == null) {
            offset = new mxPoint(0, 0);
        }
        if (sourceGeo.isRelative()) {
            sourceX = srcParGeo.getX() + sourceGeo.getX() * srcParGeo.getWidth()
                      + offset.getX();
            sourceY = srcParGeo.getY() + sourceGeo.getY() * srcParGeo.getHeight()
                      + offset.getY();
        } else {
            sourceX = srcParGeo.getX() + sourceGeo.getX() + offset.getX();
            sourceY = srcParGeo.getY() + sourceGeo.getY() + offset.getY();
        }
        list.add(new mxPoint(sourceX, sourceY));
        if (link.getGeometry().getPoints() != null) {
            list.addAll(link.getGeometry().getPoints());
        }
        mxICell target = link.getTarget();
        mxGeometry targetGeo = target.getGeometry();
        double targetX = targetGeo.getCenterX();
        double targetY = targetGeo.getCenterY();
        mxICell targetParent = target.getParent();
        mxGeometry tgtParGeo = targetParent.getGeometry();
        if (tgtParGeo == null) {
            tgtParGeo = new mxGeometry(0, 0, 0, 0);
        }
        offset = targetGeo.getOffset();
        if (offset == null) {
            offset = new mxPoint(0, 0);
        }
        if (targetGeo.isRelative()) {
            targetX = tgtParGeo.getX() + targetGeo.getX() * tgtParGeo.getWidth()
                      + offset.getX();
            targetY = tgtParGeo.getY() + targetGeo.getY() * tgtParGeo.getHeight()
                      + offset.getY();
        } else {
            targetX = tgtParGeo.getX() + targetGeo.getX() + offset.getX();
            targetY = tgtParGeo.getY() + targetGeo.getY() + offset.getY();
        }
        list.add(new mxPoint(targetX, targetY));
        return list;
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
                if (c instanceof BasicLink) {
                    BasicLink link = (BasicLink) c;
                    if (checkLinesIntersection(x1, y1, x2, y2, link)) {
                        return true;
                    }
                    if (checkLinesCoincide(x1, y1, x2, y2, link)) {
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
     * Check whether a point is in one of the lines.
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
                                continue;
                            }
                            if (y1 == y2 && y != y1) {
                                continue;
                            }
                            if (pointInLineSegment(x, y, x1, y1, x2, y2)) {
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
     * Check whether a point is in one of the blocks.
     *
     * @param x
     *            the x-coordinate of the point
     * @param y
     *            the y-coordinate of the point
     * @param allCells
     * @return <b>true</b> if one point is in one block.
     */
    public static boolean checkPointInBlocks(double x, double y, Object[] allCells) {
        for (Object o : allCells) {
            if (o instanceof mxCell) {
                mxCell block = (mxCell) o;
                if (!block.isEdge()) {
                    double blockx = block.getGeometry().getX();
                    double blocky = block.getGeometry().getY();
                    double width = block.getGeometry().getWidth();
                    double height = block.getGeometry().getHeight();
                    if (x >= blockx && x <= (blockx + width) && y >= blocky
                            && y < (blocky + height)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    /**
     * Check whether two lines have intersection or not.
     *
     * @param x1
     *            the x-coordinate of the first point of the first line
     * @param y1
     *            the y-coordinate of the first point of the first line
     * @param x2
     *            the x-coordinate of the second point of the first line
     * @param y2
     *            the y-coordinate of the second point of the first line
     * @param link
     *            the second link
     * @return <b>true</b> if two lines have intersection(s).
     */
    public static boolean checkLinesIntersection(double x1, double y1, double x2, double y2,
            BasicLink link) {
        List<mxPoint> listPoints = getLinkPoints(link);
        if (listPoints == null || listPoints.size() <= 1) {
        } else {
            for (int i = 1; i < listPoints.size(); i++) {
                mxPoint point3 = listPoints.get(i - 1);
                mxPoint point4 = listPoints.get(i);
                double x3 = point3.getX();
                double y3 = point3.getY();
                double x4 = point4.getX();
                double y4 = point4.getY();
                mxPoint point = mxUtils.intersection(x1, y1, x2, y2, x3, y3, x4, y4);
                if (point != null) {
                    return true;
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
     * @param link
     *            the second line
     * @return <b>true</b> if two lines coincide completely or partly.
     */
    private static boolean checkLinesCoincide(double x1, double y1, double x2, double y2,
            BasicLink link) {
        // mxICell source = line.getSource();
        // mxICell target = line.getTarget();
        List<mxPoint> listPoints = link.getGeometry().getPoints();
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
        if (pointInLineSegment(x1, y1, x3, y3, x4, y4)
                && pointInLineSegment(x2, y2, x3, y3, x4, y4)) {
            return true;
        }
        // the second line is inside the first line.
        if (pointInLineSegment(x3, y3, x1, y1, x2, y2)
                && pointInLineSegment(x4, y4, x1, y1, x2, y2)) {
            return true;
        }
        double i = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
        // two lines are parallel.
        if (i == 0) {
            if (pointInLineSegment(x1, y1, x3, y3, x4, y4)
                    || pointInLineSegment(x2, y2, x3, y3, x4, y4)
                    || pointInLineSegment(x3, y3, x1, y1, x2, y2)
                    || pointInLineSegment(x4, y4, x1, y1, x2, y2)) {
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
    private static boolean pointInLineSegment(double x1, double y1, double x2, double y2,
            double x3, double y3) {
        if (((x3 - x2) * (y1 - y2) == (x1 - x2) * (y3 - y2))
                && (x1 >= Math.min(x2, x3) && x1 <= Math.max(x2, x3))
                && (y1 >= Math.min(y2, y3) && y1 <= Math.max(y2, y3))) {
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
        double distance = XcosRouteUtils.NORMAL_BLOCK_SIZE;
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
            double x = choosePoint(listX, x1, x2);
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
            double y = choosePoint(listY, y1, y2);
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
    private static double choosePoint(List<Double> list, double p1, double p2) {
        if (list == null || list.size() == 0) {
            return 0;
        }
        Collections.sort(list);
        double nMax = Math.max(p1, p2);
        double nMin = Math.min(p1, p2);
        double start = list.get(0);
        double start_temp = list.get(0);
        double end = list.get(0);
        double end_temp = list.get(0);
        double mid_temp = (end_temp + start_temp) / 2;
        double mid = (end + start) / 2;
        boolean restart = true;
        for (int i = 1; i < list.size(); i++) {
            if (Math.abs(list.get(i) - list.get(i - 1)) <= 1.1) {
                end_temp = list.get(i);
                restart = false;
            } else {
                if (restart) {
                    start_temp = list.get(i);
                    continue;
                }
                mid_temp = (end_temp + start_temp) / 2;
                mid = (end + start) / 2;
                if ((mid_temp < nMin || mid_temp > nMax) && (mid < nMax && mid > nMin)) {
                    // if the new one is out of two points and the previous one
                    // is inside,
                    start_temp = list.get(i);
                    end_temp = list.get(i);
                    restart = true;
                } else if ((Math.abs(end_temp - start_temp) > Math.abs(end - start))
                           || (mid_temp < nMax && mid_temp > nMin) && (mid < nMin || mid > nMax)) {
                    // if the new one in between two points and the previous one
                    // is out of them, or if the new one is longer than the
                    // previous one,
                    start = start_temp;
                    end = end_temp;
                    start_temp = list.get(i);
                    end_temp = list.get(i);
                    restart = true;
                }
            }
        }
        mid_temp = (end_temp + start_temp) / 2;
        mid = (end + start) / 2;
        if ((mid_temp < nMin || mid_temp > nMax) && (mid < nMax && mid > nMin)) {
        } else if ((Math.abs(end_temp - start_temp) > Math.abs(end - start))
                   || ((mid_temp < nMax && mid_temp > nMin) && (mid < nMin || mid > nMax))) {
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

    /**
     * Get the Orientation of a port to its parent.
     *
     * @param port
     * @return
     */
    protected Orientation getPortOrientation(BasicPort port) {
        if (port.getParent() == null) {
            return Orientation.EAST;
        }
        // the coordinate (x,y) for the port.
        mxGeometry portGeo = port.getGeometry();
        double portX = portGeo.getCenterY();
        double portY = portGeo.getCenterY();
        // the coordinate (x,y) and the width-height for the parent block
        mxICell parent = port.getParent();
        mxGeometry parentGeo = parent.getGeometry();
        double blockW = parentGeo.getWidth();
        double blockH = parentGeo.getHeight();
        if (portGeo.isRelative()) {
            portX *= blockW;
            portY *= blockH;
        }
        // calculate relative coordinate based on the center of parent block.
        portX -= blockW / 2;
        portY -= blockH / 2;
        Orientation orientation = Orientation.EAST;
        if ((portX) >= blockW * Math.abs(portY) / blockH) { // x>=w*|y|/h
            orientation = Orientation.EAST;
        } else if (portY >= blockH * Math.abs(portX) / blockW) { // y>=h*|x|/w
            orientation = Orientation.SOUTH;
        } else if (portX <= -blockW * Math.abs(portY) / blockH) { // x<=-w*|y|/h
            orientation = Orientation.WEST;
        } else if (portY <= -blockH * Math.abs(portX) / blockW) { // y<=-h*|x|/w
            orientation = Orientation.NORTH;
        }
        return orientation;
    }

}
