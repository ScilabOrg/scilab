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
import com.mxgraph.view.mxCellState;

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
            graph.setCellStyle(null, links);
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
                List<mxPoint> list = new ArrayList<mxPoint>(0);
                list.addAll(listRoute);
                mxGeometry geometry = new mxGeometry();
                geometry.setPoints(list);
                ((mxGraphModel) (graph.getModel())).setGeometry(cell, geometry);
                listRoute.clear();
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
    protected boolean computeRoute(BasicLink link, Object[] allCells, XcosDiagram graph) {
        listRoute.clear();
        mxICell sourceCell = link.getSource();
        mxICell targetCell = link.getTarget();
        // if the link is not connected with BasicPort.
        if (!(sourceCell instanceof BasicPort) || !(targetCell instanceof BasicPort)) {
            // if ((!(sourceCell instanceof BasicPort) && !(sourceCell
            // instanceof SplitBlock))
            // || (!(targetCell instanceof BasicPort) && !(targetCell instanceof
            // SplitBlock))) {
            return false;
        }
        double srcx = 0;
        double srcy = 0;
        double tgtx = 0;
        double tgty = 0;
        mxPoint sourcePoint = new mxPoint(srcx, srcy);
        mxPoint targetPoint = new mxPoint(tgtx, tgty);
        // if source is a port, get a new start point.
        if (sourceCell instanceof BasicPort) {
            mxCellState state = graph.getView().getState(sourceCell);
            if (state != null) {
                srcx = state.getCenterX();
                srcy = state.getCenterY();
                BasicPort sourcePort = (BasicPort) sourceCell;
                sourcePoint = this.getPointAwayPort(sourcePort, graph);
            }
        }
        // if source is a SplitBlock
        if (sourceCell.getParent() instanceof SplitBlock) {
            srcx = sourceCell.getParent().getGeometry().getCenterX();
            srcy = sourceCell.getParent().getGeometry().getCenterY();
            sourcePoint.setX(srcx);
            sourcePoint.setY(srcy);
        }
        // if target is a port, get a new end point.
        if (targetCell instanceof BasicPort) {
            mxCellState state = graph.getView().getState(targetCell);
            if (state != null) {
                tgtx = state.getCenterX();
                tgty = state.getCenterY();
                BasicPort targetPort = (BasicPort) targetCell;
                targetPoint = this.getPointAwayPort(targetPort, graph);
            }
        }
        // if target is a SplitBlock
        if (targetCell.getParent() instanceof SplitBlock) {
            tgtx = targetCell.getParent().getGeometry().getCenterX();
            tgty = targetCell.getParent().getGeometry().getCenterY();
            targetPoint.setX(tgtx);
            targetPoint.setY(tgty);
        }
        // if two ports are aligned and there are no blocks between them,
        // use straight route.
        if ((XcosRoute.isStrictlyAligned(srcx, srcy, tgtx, tgty))
                && !XcosRoute.checkObstacle(srcx, srcy, tgtx, tgty, allCells)) {
            return true;
        }
        List<mxPoint> list = XcosRoute.getSimpleRoute(sourcePoint, targetPoint, allCells);
        if (list != null && list.size() > 0) {
            listRoute.addAll(list);
            return true;
        }
        listRoute.add(sourcePoint);
        listRoute.add(targetPoint);
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
    protected mxPoint getPointAwayPort(BasicPort port, XcosDiagram graph) {
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
    protected Orientation getPortRelativeOrientation(BasicPort port, XcosDiagram graph) {
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
     * Remove the selves from the array of all. Remove all SplitBlock.
     *
     * @param all
     * @param self
     * @return a new array of all objects excluding selves
     */
    protected Object[] getAllOtherCells(Object[] all, Object... self) {
        List<Object> listme = Arrays.asList(self);
        List<Object> listnew = new ArrayList<Object>(0);
        for (Object o : all) {
            if (!listme.contains(o) && !(o instanceof SplitBlock)) { //
                //                if (o instanceof SplitBlock) {
                //                    System.out.println("SplitBlock: " + o);
                //                }
                listnew.add(o);
            }
        }
        Object[] newAll = listnew.toArray();
        return newAll;
    }

}
