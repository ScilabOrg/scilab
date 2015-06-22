/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Chenfeng ZHU
 */
package org.scilab.modules.xcos.link.actions;

import java.awt.event.ActionEvent;
import java.awt.event.KeyEvent;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.scilab.modules.graph.ScilabComponent;
import org.scilab.modules.graph.ScilabGraph;
import org.scilab.modules.gui.menuitem.MenuItem;
import org.scilab.modules.xcos.block.SplitBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.port.BasicPort;
import org.scilab.modules.xcos.port.Orientation;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.scilab.modules.xcos.utils.XcosRoute;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;
import com.mxgraph.util.mxPoint;

/**
 * Implement the set link optimal action.
 */
@SuppressWarnings(value = { "serial" })
public class StyleOptimalAction extends StyleAction {
    /** Name of the action */
    public static final String NAME = XcosMessages.LINK_STYLE_OPTIMAL;
    /** Icon name of the action */
    public static final String SMALL_ICON = "";
    /** Mnemonic key of the action */
    public static final int MNEMONIC_KEY = KeyEvent.VK_O;

    private List<mxPoint> listRoute = new ArrayList<mxPoint>(0);

    public StyleOptimalAction(ScilabGraph scilabGraph) {
        super(scilabGraph);
    }

    /**
     * @param scilabGraph
     * @return menu item
     */
    public static MenuItem createMenu(ScilabGraph scilabGraph) {
        return createMenu(scilabGraph, StyleOptimalAction.class);
    }

    /**
     * @param e
     * @see org.scilab.modules.xcos.link.actions.StyleAction#actionPerformed(java.awt.event.ActionEvent)
     */
    @Override
    public void actionPerformed(ActionEvent e) {
        final XcosDiagram graph = (XcosDiagram) getGraph(e);

        // action disabled when the cell is edited
        final ScilabComponent comp = ((ScilabComponent) graph.getAsComponent());
        if (comp.isEditing()) {
            return;
        }

        final Object[] links = getLinks();

        graph.getModel().beginUpdate();
        try {
            this.updateLinkOptimal(graph, links);
        } finally {
            graph.getModel().endUpdate();
        }
    }

    /**
     * Update the style of links one by one.
     *
     * @param graph
     * @param links
     */
    public void updateLinkOptimal(XcosDiagram graph, Object[] links) {
        Object[] allChildCells = graph.getChildCells(graph.getDefaultParent());
        for (Object o : links) {
            if (o instanceof BasicLink) {
                BasicLink link = (BasicLink) o;
                this.updateRoute(link, allChildCells, graph);
            }
        }
    }

    /**
     * Update the Edge.
     *
     * @param cell
     * @param graph
     */
    protected void updateRoute(BasicLink cell, Object[] allCells, XcosDiagram graph) {
        mxICell sourceCell = cell.getSource();
        mxICell targetCell = cell.getTarget();
        Object[] allOtherCells = getAllOtherCells(allCells, cell, sourceCell, targetCell);
        if (sourceCell != null && targetCell != null) {
            boolean isGetRoute = this.computeRoute(cell, allOtherCells, graph);
            if (isGetRoute) {
                mxGeometry geometry = new mxGeometry();
                geometry.setPoints(listRoute);
                ((mxGraphModel) (graph.getModel())).setGeometry(cell, geometry);
            } else {
                // if it cannot get the route, keep the same or change it to
                // straight or give a pop windows to inform user.
            }
        }
    }

    /**
     * Get the turning points for the optimal route. If the straight route is
     * the optimal route, return null.
     *
     * @param link
     * @param allCells
     * @return list of turning points
     */
    public boolean computeRoute(BasicLink link, Object[] allCells, XcosDiagram graph) {
        listRoute.clear();
        mxICell sourceCell = link.getSource();
        mxICell targetCell = link.getTarget();
        // for now, it is only support for BasicPort
        if (!(sourceCell instanceof BasicPort) || !(targetCell instanceof BasicPort)) {
            return false;
        }
        if ((sourceCell instanceof SplitBlock) || (targetCell instanceof SplitBlock)) {
            // it will provide another method to decide whether to move the
            // split node or not.
        }
        BasicPort sourcePort = (BasicPort) sourceCell;
        BasicPort targetPort = (BasicPort) targetCell;
        double srcx = graph.getView().getState(sourcePort).getCenterX();
        double srcy = graph.getView().getState(sourcePort).getCenterY();
        double tgtx = graph.getView().getState(targetPort).getCenterX();
        double tgty = graph.getView().getState(targetPort).getCenterY();
        // if two ports are not oblique and not in the same direction,
        // use straight route.
        if ((!XcosRoute.checkOblique(srcx, srcy, tgtx, tgty))
                && !XcosRoute.checkObstacle(srcx, srcy, tgtx, tgty, allCells)) {
            return true;
        }
        mxPoint sourcePoint1 = this.getPointAwayPort(sourcePort, graph);
        mxPoint targetPoint1 = this.getPointAwayPort(targetPort, graph);
        XcosRoute route = new XcosRoute();
        List<mxPoint> list = route.getSimpleRoute(sourcePoint1, targetPoint1, allCells);
        if (list != null && list.size() > 0) {
            listRoute.addAll(list);
            return true;
        }
        // list = route.getRoute(sourcePoint1, new mxPoint(
        // (srcx + sourcePoint1.getX()) / 2, (srcy + sourcePoint1.getY()) / 2),
        // targetPoint1, allCells);
        // Collections.reverse(listRoute);
        // listRoute.addAll(list);
        // return true;
        listRoute.add(sourcePoint1);
        listRoute.add(targetPoint1);
        return false;
    }

    /**
     * According to the relative position (orientation) of the port, get a point
     * which is XcosRoute.BEAUTY_DISTANCE away from the port and out of block.
     *
     * @param port
     * @param graph
     * @return
     */
    public mxPoint getPointAwayPort(BasicPort port, XcosDiagram graph) {
        double portx = graph.getView().getState(port).getCenterX();
        double porty = graph.getView().getState(port).getCenterY();
        mxPoint point = new mxPoint(portx, porty);
        double distance = XcosRoute.BEAUTY_DISTANCE;
        switch (getPortRelativeOrientation(port, graph)) {
                // switch (port.getOrientation()) {
            case EAST:
                point.setX(point.getX() + distance);
                break;
            case SOUTH:
                point.setY(point.getY() + distance);
                break;
            case WEST:
                point.setX(point.getX() - distance);
                break;
            case NORTH:
                point.setY(point.getY() - distance);
                break;
        }
        return point;
    }

    /**
     * As BasicPort.getOrientation is the default orientation, the Orientation
     * is not correct when the block is mirrored or flipped. This method could
     * get the current Orientation of the port.
     *
     * @param port
     * @return
     */
    public Orientation getPortRelativeOrientation(BasicPort port, XcosDiagram graph) {
        // the coordinate (x,y) for the port.
        double portx = graph.getView().getState(port).getCenterX();
        double porty = graph.getView().getState(port).getCenterY();
        // the coordinate (x,y) and the width-height for the parent block
        mxICell parent = port.getParent();
        double blockx = graph.getView().getState(parent).getCenterX();
        double blocky = graph.getView().getState(parent).getCenterY();
        double blockw = parent.getGeometry().getWidth();
        double blockh = parent.getGeometry().getHeight();
        // calculate relative coordinate based on the center of parent block.
        portx -= blockx;
        porty -= blocky;
        Orientation orientation = port.getOrientation();
        if ((portx) >= blockw * Math.abs(porty) / blockh) { // x>=w*|y|/h
            orientation = Orientation.EAST;
        } else if (porty >= blockh * Math.abs(portx) / blockw) { // y>=h*|x|/w
            orientation = Orientation.SOUTH;
        } else if (portx <= -blockw * Math.abs(porty) / blockh) { // x<=-w*|y|/h
            orientation = Orientation.WEST;
        } else if (porty <= -blockh * Math.abs(portx) / blockw) { // y<=-h*|x|/w
            orientation = Orientation.NORTH;
        }
        return orientation;
    }

    /**
     * Remove the selves from the array of all.
     *
     * @param all
     * @param self
     * @return a new array of all objects excluding selves
     */
    public Object[] getAllOtherCells(Object[] all, Object... self) {
        List<Object> listme = Arrays.asList(self);
        List<Object> listnew = new ArrayList<Object>(0);
        for (Object o : all) {
            if (!listme.contains(o)) {
                listnew.add(o);
            }
        }
        Object[] newAll = listnew.toArray();
        return newAll;
    }

}
