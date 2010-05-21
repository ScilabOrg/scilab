/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Manuel JULIACHS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.tests.modules.graphic_objects;

import org.testng.annotations.*;


import java.rmi.server.UID;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

import org.scilab.modules.graphic_objects.axes.Axes.GridPosition;
import org.scilab.modules.graphic_objects.axes.AxisProperty;
import org.scilab.modules.graphic_objects.axes.AxisProperty.AxisLocation;
import org.scilab.modules.graphic_objects.axes.Box;
import org.scilab.modules.graphic_objects.axes.Box.BoxType;
import org.scilab.modules.graphic_objects.axes.Camera;
import org.scilab.modules.graphic_objects.axes.Camera.ViewType;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.label.Label;
import org.scilab.modules.graphic_objects.textObject.FormattedText;
import org.scilab.modules.graphic_objects.textObject.Font;


/**
 * testAxesGetSetPerformance
 * @author Manuel JULIACHS
 */
public class testAxesGetSetPerformance {
	/**
	 * The number of iterations for the two test loops,
	 * over which performance is averaged
	 * Arbitrarily chosen for now
	 */
	public final static int NUMBER_ITERATIONS = 10000;

	/** The default number of ticks */
	public final static int DEFAULT_NUMBER_OF_TICKS = 20;

	/** Figure identifier */
	private UID figureID;

	/** The Axes identifier */
	private UID axesID;

	/** X-Axis label identifier */
	private UID xlabelID;

	/** Y-Axis label identifier */
	private UID ylabelID;

	/** Z-Axis label identifier */
	private UID zlabelID;

	/** Title label identifier */
	private UID titleID;

	/** Graphic controller */
	private GraphicController controller;

	/** X-Axis label */
	private	Label xlabel;

	/** Y-Axis label */
	private Label ylabel;

	/** Z-Axis label */
	private Label zlabel;

	/** title label */
	private Label title;

	/** visible */
	private Boolean visible;

	/** X,Y,Z axes properties array */
	private AxisProperty[] axesProperties;

	/** camera property */
	private Camera camera;

	/** box property */
	private Box box;

	/* Miscellaneous Axes properties */
	/** grid position */
	private GridPosition gridPosition;

	/** auto-clear */
	private Boolean autoClear;

	/** filled */
	private Boolean filled;
	
	/** margins */
	private Double[] margins;

	/** axes bounds */
	private Double[] axesBounds;


	@Test
	public void performanceTest() throws NullPointerException {
		double tstart;
		double tend;
		double reflectsetDeltat;
		double reflectgetDeltat;
		double fastsetDeltat;
		double fastgetDeltat;
		double setspeedratio;
		double getspeedratio;

		boolean propertyTestResult;

		controller = GraphicController.getController();

		assert controller != null;

		figureID = controller.askObject(Type.FIGURE);
		GraphicObject parentFigure = (GraphicObject) controller.getObjectFromId(figureID);

		if (parentFigure == null) {
			throw new NullPointerException();
		}

		axesID = controller.askObject(Type.AXES);
		GraphicObject axes = (GraphicObject) controller.getObjectFromId(axesID);

		if (axes == null) {
			throw new NullPointerException();
		}

		xlabelID = controller.askObject(Type.LABEL);
		xlabel = (Label) controller.getObjectFromId(xlabelID);

		if (xlabel == null) {
			throw new NullPointerException();
		}

		ylabelID = controller.askObject(Type.LABEL);
		ylabel = (Label) controller.getObjectFromId(ylabelID);

		if (ylabel == null) {
			throw new NullPointerException();
		}

		zlabelID = controller.askObject(Type.LABEL);
		zlabel = (Label) controller.getObjectFromId(zlabelID);

		if (zlabel == null) {
			throw new NullPointerException();
		}

		titleID = controller.askObject(Type.LABEL);
		title = (Label) controller.getObjectFromId(titleID);

		if (title == null) {
			throw new NullPointerException();
		}

		/* To begin with, test whether set/get operate correctly */
		/* Initialize the data to set reflect set/get methods */
		initializeData();

		System.out.format("Test reflect set/get axes properties\n");
		propertyTestResult = testReflectSetGetAxesProperties();

		assert propertyTestResult;

		/* Initialize the data a second time to test fast set/get methods */
		initializeData();

		System.out.format("Test fast set/get axes properties\n");
		propertyTestResult = testFastSetGetAxesProperties();

		assert propertyTestResult;


		/* set/get performance evaluation loops */

		/* Reflection-based set/getProperty test loops */
		System.out.format("Reflect set/get performance evaluation loop\n");
		tstart = System.currentTimeMillis();

		/* set loop */
		for (int i = 0; i < NUMBER_ITERATIONS; i++) {
			reflectSetIteration();
		}

		tend = System.currentTimeMillis();

		reflectsetDeltat = tend - tstart;

		tstart = System.currentTimeMillis();

		/* get loop */
		for (int i = 0; i < NUMBER_ITERATIONS; i++) {
			reflectGetIteration();
		}

		tend = System.currentTimeMillis();

		reflectgetDeltat = tend - tstart;

		/* Fast set/getProperty test loops */
		System.out.format("Fast set/get performance evaluation loop\n");
		tstart = System.currentTimeMillis();

		/* set loop */
		for (int i = 0; i < NUMBER_ITERATIONS; i++) {
			fastSetIteration();
		}

		tend = System.currentTimeMillis();
		fastsetDeltat = tend - tstart;

		tstart = System.currentTimeMillis();

		/* get loop */
		for (int i = 0; i < NUMBER_ITERATIONS; i++) {
			fastGetIteration();
		}

		tend = System.currentTimeMillis();
		fastgetDeltat = tend - tstart;

		System.out.println();

		System.out.format("Reflect set/get methods performance:\n");
		System.out.format("Total number of iterations: %d\n", NUMBER_ITERATIONS);
		System.out.format("Reflect set delta t: %f\n", reflectsetDeltat);
		System.out.format("Reflect get delta t: %f\n", reflectgetDeltat);
		System.out.format("M set Iterations/s: %f\n", 0.001 * NUMBER_ITERATIONS / reflectsetDeltat);
		System.out.format("M get Iterations/s: %f\n", 0.001 * NUMBER_ITERATIONS / reflectgetDeltat);

		System.out.println();

		System.out.format("Fast set/get methods performance:\n");
		System.out.format("Total number of iterations: %d\n", NUMBER_ITERATIONS);
		System.out.format("Fast set delta t: %f\n", fastsetDeltat);
		System.out.format("Fast get delta t: %f\n", fastgetDeltat);
		System.out.format("M set Iterations/s: %f\n", 0.001 * NUMBER_ITERATIONS / fastgetDeltat);
		System.out.format("M get Iterations/s: %f\n", 0.001 * NUMBER_ITERATIONS / fastsetDeltat);

		System.out.println();

		if (fastsetDeltat > 0.0) {
			setspeedratio = reflectsetDeltat / fastsetDeltat;
		} else {
			setspeedratio = reflectsetDeltat;
		}

		if (fastgetDeltat > 0.0) {
			getspeedratio = reflectgetDeltat / fastgetDeltat;
		} else {
			getspeedratio = reflectgetDeltat;
		}

		System.out.format("Fast / reflect performance ratios:\n");
		System.out.format("set speed ratio: %f\n", setspeedratio);
		System.out.format("get speed ratio: %f\n", getspeedratio);
	}

	/**
	 * Performs reflection-based axes properties set calls
	 */
	public void reflectSetIteration() {
		/* X-Axis properties */
		controller.setProperty(axesID, "XAxisVisible", axesProperties[0].getVisible());
		controller.setProperty(axesID, "XAxisReverse", axesProperties[0].getReverse());
		controller.setProperty(axesID, "XAxisGridColor", axesProperties[0].getGridColor());
		controller.setProperty(axesID, "XAxisLabel", xlabel);
		controller.setProperty(axesID, "XAxisLocation", axesProperties[0].getAxisLocation());
		controller.setProperty(axesID, "XAxisLogFlag", axesProperties[0].getLogFlag());
		controller.setProperty(axesID, "XAxisAutoTicks", axesProperties[0].getAutoTicks());
		controller.setProperty(axesID, "XAxisSubticks", axesProperties[0].getSubticks());
		controller.setProperty(axesID, "XAxisTicksLocations", axesProperties[0].getTicksLocations());
		controller.setProperty(axesID, "XAxisTicksLabels", axesProperties[0].getTicksLabels());

		/* Y-Axis properties */
		controller.setProperty(axesID, "YAxisVisible", axesProperties[1].getVisible());
		controller.setProperty(axesID, "YAxisReverse", axesProperties[1].getReverse());
		controller.setProperty(axesID, "YAxisGridColor", axesProperties[1].getGridColor());
		controller.setProperty(axesID, "YAxisLabel", ylabel);
		controller.setProperty(axesID, "YAxisLocation", axesProperties[1].getAxisLocation());
		controller.setProperty(axesID, "YAxisLogFlag", axesProperties[1].getLogFlag());
		controller.setProperty(axesID, "YAxisAutoTicks", axesProperties[1].getAutoTicks());
		controller.setProperty(axesID, "YAxisSubticks", axesProperties[1].getSubticks());
		controller.setProperty(axesID, "YAxisTicksLocations", axesProperties[1].getTicksLocations());
		controller.setProperty(axesID, "YAxisTicksLabels", axesProperties[1].getTicksLabels());

		/* Z-Axis properties */
		controller.setProperty(axesID, "ZAxisVisible", axesProperties[2].getVisible());
		controller.setProperty(axesID, "ZAxisReverse", axesProperties[2].getReverse());
		controller.setProperty(axesID, "ZAxisGridColor", axesProperties[2].getGridColor());
		controller.setProperty(axesID, "ZAxisLabel", zlabel);
		controller.setProperty(axesID, "ZAxisLocation", axesProperties[2].getAxisLocation());
		controller.setProperty(axesID, "ZAxisLogFlag", axesProperties[2].getLogFlag());
		controller.setProperty(axesID, "ZAxisAutoTicks", axesProperties[2].getAutoTicks());
		controller.setProperty(axesID, "ZAxisSubticks", axesProperties[2].getSubticks());
		controller.setProperty(axesID, "ZAxisTicksLocations", axesProperties[2].getTicksLocations());
		controller.setProperty(axesID, "ZAxisTicksLabels", axesProperties[2].getTicksLabels());

		/* Properties inherited from GraphicObject */
		controller.setProperty(axesID, "Visible", visible);

		/* Miscellaneous properties */
		controller.setProperty(axesID, "GridPosition", gridPosition);
		controller.setProperty(axesID, "Title", title);
		controller.setProperty(axesID, "AutoClear", autoClear);
		controller.setProperty(axesID, "Filled", filled);
		controller.setProperty(axesID, "Margins", margins);
		controller.setProperty(axesID, "AxesBounds", axesBounds);

		/* Camera properties */
		controller.setProperty(axesID, "View", camera.getView());   	
		controller.setProperty(axesID, "Isoview", camera.getIsoview());
		controller.setProperty(axesID, "CubeScaling", camera.getCubeScaling());
		controller.setProperty(axesID, "RotationAngles", camera.getRotationAngles());

		/* Box properties */
		controller.setProperty(axesID, "HiddenAxisColor", box.getHiddenAxisColor());
		controller.setProperty(axesID, "TightLimits", box.getTightLimits());
		controller.setProperty(axesID, "DataBounds", box.getDataBounds());
		controller.setProperty(axesID, "RealDataBounds", box.getRealDataBounds());
		controller.setProperty(axesID, "ZoomBox", box.getZoomBox());
		controller.setProperty(axesID, "AutoScale", box.getAutoScale());
	}

	/**
	 * Performs reflection-based axes properties get calls
	 */
	public void reflectGetIteration() {
		/* X-Axis properties */
		Boolean retAxisvisible = (Boolean) controller.getProperty(axesID, "XAxisVisible");
		Boolean retAxisreverse = (Boolean) controller.getProperty(axesID, "XAxisReverse");     	
		Integer retGridcolor = (Integer) controller.getProperty(axesID, "XAxisGridColor");   	
		Label retXlabel = (Label) controller.getProperty(axesID, "XAxisLabel");
		AxisLocation retAxislocation = (AxisLocation) controller.getProperty(axesID, "XAxisLocation");
		Boolean retLogflag = (Boolean) controller.getProperty(axesID, "XAxisLogFlag");
		Boolean retAutoticks = (Boolean) controller.getProperty(axesID, "XAxisAutoTicks");
		Integer retSubticks = (Integer) controller.getProperty(axesID, "XAxisSubticks");
		Double[] retTickslocations = (Double[]) controller.getProperty(axesID, "XAxisTicksLocations");
		ArrayList retTickslabels = (ArrayList) controller.getProperty(axesID, "XAxisTicksLabels");

		/* Y-Axis properties */
		retAxisvisible = (Boolean) controller.getProperty(axesID, "YAxisVisible");
		retAxisreverse = (Boolean) controller.getProperty(axesID, "YAxisReverse");
		retGridcolor = (Integer) controller.getProperty(axesID, "YAxisGridColor");
		Label retYlabel = (Label) controller.getProperty(axesID, "YAxisLabel");
		retAxislocation = (AxisLocation) controller.getProperty(axesID, "YAxisLocation");
		retLogflag = (Boolean) controller.getProperty(axesID, "YAxisLogFlag");
		retAutoticks = (Boolean) controller.getProperty(axesID, "YAxisAutoTicks");
		retSubticks = (Integer) controller.getProperty(axesID, "YAxisSubticks");
		retTickslocations = (Double[]) controller.getProperty(axesID, "YAxisTicksLocations");
		retTickslabels = (ArrayList) controller.getProperty(axesID, "YAxisTicksLabels");

		/* Z-Axis properties */
		retAxisvisible = (Boolean) controller.getProperty(axesID, "ZAxisVisible");
		retAxisreverse = (Boolean) controller.getProperty(axesID, "ZAxisReverse");
		retGridcolor = (Integer) controller.getProperty(axesID, "ZAxisGridColor");
		Label retZlabel = (Label) controller.getProperty(axesID, "ZAxisLabel");
		retAxislocation = (AxisLocation) controller.getProperty(axesID, "ZAxisLocation");
		retLogflag = (Boolean) controller.getProperty(axesID, "ZAxisLogFlag");
		retAutoticks = (Boolean) controller.getProperty(axesID, "ZAxisAutoTicks");
		retSubticks = (Integer) controller.getProperty(axesID, "ZAxisSubticks");
		retTickslocations = (Double[]) controller.getProperty(axesID, "ZAxisTicksLocations");
		retTickslabels = (ArrayList) controller.getProperty(axesID, "ZAxisTicksLabels");

		/* Properties inherited from GraphicObject */
		Boolean retVisible = (Boolean) controller.getProperty(axesID, "Visible");

		/* Miscellaneous properties */
		GridPosition gridpos = (GridPosition) controller.getProperty(axesID, "GridPosition");
		Label retTitle = (Label) controller.getProperty(axesID, "Title");
		Boolean retAutoclear = (Boolean) controller.getProperty(axesID, "AutoClear");
		Boolean retFilled = (Boolean) controller.getProperty(axesID, "Filled");
		Double[] retMargins = (Double[]) controller.getProperty(axesID, "Margins");
		controller.setProperty(axesID, "AxesBounds", axesBounds);

		/* Camera properties */
		ViewType retView = (ViewType) controller.getProperty(axesID, "View");   	
		Boolean retIsoview = (Boolean) controller.getProperty(axesID, "Isoview");
		Boolean retCubescaling = (Boolean) controller.getProperty(axesID, "CubeScaling");
		Double[] retRotangles = (Double[]) controller.getProperty(axesID, "RotationAngles");

		/* Box properties */
		Integer retHidaxiscolor = (Integer) controller.getProperty(axesID, "HiddenAxisColor");
		Boolean retTightlimits = (Boolean) controller.getProperty(axesID, "TightLimits");
		Double[] retDataBounds = (Double[]) controller.getProperty(axesID, "DataBounds");
		Double[] retReadDataBounds = (Double[]) controller.getProperty(axesID, "RealDataBounds");
		Double[] retZoomBox = (Double[]) controller.getProperty(axesID, "ZoomBox");
		Boolean retAutoScale = (Boolean) controller.getProperty(axesID, "AutoScale");
	}

	/**
	 * Performs fast axes properties set calls
	 */
	public void fastSetIteration() {

		/* X-Axis properties */
		controller.setPropertyFast(axesID, "XAxisVisible", axesProperties[0].getVisible());
		controller.setPropertyFast(axesID, "XAxisReverse", axesProperties[0].getReverse());
		controller.setPropertyFast(axesID, "XAxisGridColor", axesProperties[0].getGridColor());
		controller.setPropertyFast(axesID, "XAxisLabel", xlabel);
		controller.setPropertyFast(axesID, "XAxisLocation", axesProperties[0].getAxisLocation());
		controller.setPropertyFast(axesID, "XAxisLogFlag", axesProperties[0].getLogFlag());
		controller.setPropertyFast(axesID, "XAxisAutoTicks", axesProperties[0].getAutoTicks());
		controller.setPropertyFast(axesID, "XAxisSubticks", axesProperties[0].getSubticks());
		controller.setPropertyFast(axesID, "XAxisTicksLocations", axesProperties[0].getTicksLocations());
		controller.setPropertyFast(axesID, "XAxisTicksLabels", axesProperties[0].getTicksLabels());

		/* Y-Axis properties */
		controller.setPropertyFast(axesID, "YAxisVisible", axesProperties[1].getVisible());
		controller.setPropertyFast(axesID, "YAxisReverse", axesProperties[1].getReverse());
		controller.setPropertyFast(axesID, "YAxisGridColor", axesProperties[1].getGridColor());
		controller.setPropertyFast(axesID, "YAxisLabel", ylabel);
		controller.setPropertyFast(axesID, "YAxisLocation", axesProperties[1].getAxisLocation());
		controller.setPropertyFast(axesID, "YAxisLogFlag", axesProperties[1].getLogFlag());
		controller.setPropertyFast(axesID, "YAxisAutoTicks", axesProperties[1].getAutoTicks());
		controller.setPropertyFast(axesID, "YAxisSubticks", axesProperties[1].getSubticks());
		controller.setPropertyFast(axesID, "YAxisTicksLocations", axesProperties[1].getTicksLocations());
		controller.setPropertyFast(axesID, "YAxisTicksLabels", axesProperties[1].getTicksLabels());

		/* Z-Axis properties */
		controller.setPropertyFast(axesID, "ZAxisVisible", axesProperties[2].getVisible());
		controller.setPropertyFast(axesID, "ZAxisReverse", axesProperties[2].getReverse());
		controller.setPropertyFast(axesID, "ZAxisGridColor", axesProperties[2].getGridColor());
		controller.setPropertyFast(axesID, "ZAxisLabel", zlabel);
		controller.setPropertyFast(axesID, "ZAxisLocation", axesProperties[2].getAxisLocation());
		controller.setPropertyFast(axesID, "ZAxisLogFlag", axesProperties[2].getLogFlag());
		controller.setPropertyFast(axesID, "ZAxisAutoTicks", axesProperties[2].getAutoTicks());
		controller.setPropertyFast(axesID, "ZAxisSubticks", axesProperties[2].getSubticks());
		controller.setPropertyFast(axesID, "ZAxisTicksLocations", axesProperties[2].getTicksLocations());
		controller.setPropertyFast(axesID, "ZAxisTicksLabels", axesProperties[2].getTicksLabels());

		/* Properties inherited from GraphicObject */
		controller.setPropertyFast(axesID, "Visible", visible);

		/* Miscellaneous properties */
		controller.setPropertyFast(axesID, "GridPosition", gridPosition);
		controller.setPropertyFast(axesID, "Title", title);
		controller.setPropertyFast(axesID, "AutoClear", autoClear);
		controller.setPropertyFast(axesID, "Filled", filled);
		controller.setPropertyFast(axesID, "Margins", margins);
		controller.setPropertyFast(axesID, "AxesBounds", axesBounds);

		/* Camera properties */
		controller.setPropertyFast(axesID, "View", camera.getView());   	
		controller.setPropertyFast(axesID, "Isoview", camera.getIsoview());
		controller.setPropertyFast(axesID, "CubeScaling", camera.getCubeScaling());
		controller.setPropertyFast(axesID, "RotationAngles", camera.getRotationAngles());

		/* Box properties */
		controller.setPropertyFast(axesID, "HiddenAxisColor", box.getHiddenAxisColor());
		controller.setPropertyFast(axesID, "TightLimits", box.getTightLimits());
		controller.setPropertyFast(axesID, "DataBounds", box.getDataBounds());
		controller.setPropertyFast(axesID, "RealDataBounds", box.getRealDataBounds());
		controller.setPropertyFast(axesID, "ZoomBox", box.getZoomBox());
		controller.setPropertyFast(axesID, "AutoScale", box.getAutoScale());
	}

	/**
	 * Performs fast axes properties get calls 
	 */
	public void fastGetIteration() {
		/* X-Axis properties */
		Boolean axisvisible = (Boolean) controller.getPropertyFast(axesID, "XAxisVisible");
		Boolean axisreverse = (Boolean) controller.getPropertyFast(axesID, "XAxisReverse");     	
		Integer gridcolor = (Integer) controller.getPropertyFast(axesID, "XAxisGridColor");   	
		Label retXlabel = (Label) controller.getPropertyFast(axesID, "XAxisLabel");
		AxisLocation retAxislocation = (AxisLocation) controller.getPropertyFast(axesID, "XAxisLocation");
		Boolean logflag = (Boolean) controller.getPropertyFast(axesID, "XAxisLogFlag");
		Boolean autoticks = (Boolean) controller.getPropertyFast(axesID, "XAxisAutoTicks");
		Integer subticks = (Integer) controller.getPropertyFast(axesID, "XAxisSubticks");
		Double[] tickslocations = (Double[]) controller.getPropertyFast(axesID, "XAxisTicksLocations");
		ArrayList tickslabels = (ArrayList) controller.getPropertyFast(axesID, "XAxisTicksLabels");

		/* Y-Axis properties */
		axisvisible = (Boolean) controller.getPropertyFast(axesID, "YAxisVisible");
		axisreverse = (Boolean) controller.getPropertyFast(axesID, "YAxisReverse");
		gridcolor = (Integer) controller.getPropertyFast(axesID, "YAxisGridColor");
		Label retYlabel = (Label) controller.getPropertyFast(axesID, "YAxisLabel");
		retAxislocation = (AxisLocation) controller.getPropertyFast(axesID, "YAxisLocation");
		logflag = (Boolean) controller.getPropertyFast(axesID, "YAxisLogFlag");
		autoticks = (Boolean) controller.getPropertyFast(axesID, "YAxisAutoTicks");
		subticks = (Integer) controller.getPropertyFast(axesID, "YAxisSubticks");
		tickslocations = (Double[]) controller.getPropertyFast(axesID, "YAxisTicksLocations");
		tickslabels = (ArrayList) controller.getPropertyFast(axesID, "YAxisTicksLabels");

		/* Z-Axis properties */
		axisvisible = (Boolean) controller.getPropertyFast(axesID, "ZAxisVisible");
		axisreverse = (Boolean) controller.getPropertyFast(axesID, "ZAxisReverse");
		gridcolor = (Integer) controller.getPropertyFast(axesID, "ZAxisGridColor");
		Label retZlabel = (Label) controller.getPropertyFast(axesID, "ZAxisLabel");
		retAxislocation = (AxisLocation) controller.getPropertyFast(axesID, "ZAxisLocation");
		logflag = (Boolean) controller.getPropertyFast(axesID, "ZAxisLogFlag");
		autoticks = (Boolean) controller.getPropertyFast(axesID, "ZAxisAutoTicks");
		subticks = (Integer) controller.getPropertyFast(axesID, "ZAxisSubticks");
		tickslocations = (Double[]) controller.getPropertyFast(axesID, "ZAxisTicksLocations");
		tickslabels = (ArrayList) controller.getPropertyFast(axesID, "ZAxisTicksLabels");

		/* Properties inherited from GraphicObject */
		Boolean retVisible = (Boolean) controller.getPropertyFast(axesID, "Visible");

		/* Miscellaneous properties */
		GridPosition gridpos = (GridPosition) controller.getPropertyFast(axesID, "GridPosition");
		Label retTitle = (Label) controller.getPropertyFast(axesID, "Title");
		Boolean autoclear = (Boolean) controller.getPropertyFast(axesID, "AutoClear");
		Boolean retFilled = (Boolean) controller.getPropertyFast(axesID, "Filled");
		Double[] retMargins = (Double[]) controller.getPropertyFast(axesID, "Margins");
		controller.setProperty(axesID, "AxesBounds", axesBounds);

		/* Camera properties */
		ViewType retView = (ViewType) controller.getPropertyFast(axesID, "View");   	
		Boolean retIsoview = (Boolean) controller.getPropertyFast(axesID, "Isoview");
		Boolean retCubescaling = (Boolean) controller.getPropertyFast(axesID, "CubeScaling");
		Double[] retRotangles = (Double[]) controller.getPropertyFast(axesID, "RotationAngles");

		/* Box properties */
		Integer retHidaxiscolor = (Integer) controller.getPropertyFast(axesID, "HiddenAxisColor");
		Boolean retTightlimits = (Boolean) controller.getPropertyFast(axesID, "TightLimits");
		Double[] retDataBounds = (Double[]) controller.getPropertyFast(axesID, "DataBounds");
		Double[] retReadDataBounds = (Double[]) controller.getPropertyFast(axesID, "RealDataBounds");
		Double[] retZoomBox = (Double[]) controller.getPropertyFast(axesID, "ZoomBox");
		Boolean retAutoScale = (Boolean) controller.getPropertyFast(axesID, "AutoScale");
	}

	/**
	 * Utility method which initializes data relevant to the Axes properties 
	 * Must be called prior to any set/get calls and prior to the execution of
	 * the get/set performance evaluation loops
	 * Most properties are randomly chosen (for example the axes properties such as visible,
	 * reverse, etc.). However, some are non-randomly set, such as the margins, databounds, ticks locations
	 * and labels, though they could be either slightly jittered or also randomly chosen.
	 * The point of random initialization is to ensure that set and get work correctly, by comparing
	 * the results obtained by get calls to the initial random values which are then assigned by set calls
	 * (the former and the latter should of course be equal) 
	 */
	public void initializeData() {
		Random randomGen = new Random();

		/* GraphicObjects properties */
		visible = randomGen.nextBoolean();

		axesProperties = new AxisProperty[3];
		axesProperties[0] = new AxisProperty();
		axesProperties[1] = new AxisProperty();
		axesProperties[2] = new AxisProperty();

		axesProperties[0].setVisible(randomGen.nextBoolean());
		axesProperties[0].setReverse(randomGen.nextBoolean());
		axesProperties[0].setGridColor(randomGen.nextInt());
		axesProperties[0].setAxisLocation(AxisLocation.BOTTOM);
		axesProperties[0].setLogFlag(randomGen.nextBoolean());
		axesProperties[0].setAutoTicks(randomGen.nextBoolean());
		axesProperties[0].setSubticks(randomGen.nextInt());

		axesProperties[1].setVisible(randomGen.nextBoolean());
		axesProperties[1].setReverse(randomGen.nextBoolean());
		axesProperties[1].setGridColor(randomGen.nextInt());
		axesProperties[1].setAxisLocation(AxisLocation.RIGHT);
		axesProperties[1].setLogFlag(randomGen.nextBoolean());
		axesProperties[1].setAutoTicks(randomGen.nextBoolean());
		axesProperties[1].setSubticks(randomGen.nextInt());

		axesProperties[2].setVisible(randomGen.nextBoolean());
		axesProperties[2].setReverse(randomGen.nextBoolean());
		axesProperties[2].setGridColor(randomGen.nextInt());
		axesProperties[2].setAxisLocation(AxisLocation.ORIGIN);
		axesProperties[2].setLogFlag(randomGen.nextBoolean());
		axesProperties[2].setAutoTicks(randomGen.nextBoolean());
		axesProperties[2].setSubticks(randomGen.nextInt());

		/* ticks locations */
		Double[] xlocs = new Double[DEFAULT_NUMBER_OF_TICKS];
		Double[] ylocs = new Double[DEFAULT_NUMBER_OF_TICKS];
		Double[] zlocs = new Double[DEFAULT_NUMBER_OF_TICKS];

		for (int i = 0; i < DEFAULT_NUMBER_OF_TICKS; i++) {
			xlocs[i] = (double) i / (double) DEFAULT_NUMBER_OF_TICKS;
			ylocs[i] = (double) i / (double) DEFAULT_NUMBER_OF_TICKS;
			zlocs[i] = 2.0 * ((double) i) / (double) DEFAULT_NUMBER_OF_TICKS - 1.0;
		}

		ArrayList <FormattedText> xlabels = new ArrayList(DEFAULT_NUMBER_OF_TICKS);
		ArrayList <FormattedText> ylabels = new ArrayList(DEFAULT_NUMBER_OF_TICKS);
		ArrayList <FormattedText> zlabels = new ArrayList(DEFAULT_NUMBER_OF_TICKS);

		Font xFont = new Font();
		xFont.setColor(randomGen.nextInt());
		xFont.setSize(1.0);
		Font yFont = new Font();
		yFont.setColor(randomGen.nextInt());
		yFont.setSize(2.0);
		Font zFont = new Font();
		zFont.setColor(randomGen.nextInt());
		zFont.setSize(3.0);

		for(int i = 0; i < DEFAULT_NUMBER_OF_TICKS; i++) {
			FormattedText newtext = new FormattedText();
			Integer ind = new Integer(i);
			newtext.setText("Xtext" + ind.toString());
			newtext.setFont(xFont);
			xlabels.add(newtext);
			newtext = new FormattedText();
			newtext.setText("Ytext" + ind.toString());
			newtext.setFont(yFont);
			ylabels.add(newtext);
			newtext = new FormattedText();
			newtext.setText("Ztext" + ind.toString());
			newtext.setFont(zFont);
			zlabels.add(newtext);
		}

		axesProperties[0].setTicksLocations(xlocs);
		axesProperties[0].setTicksLabels(xlabels);
		axesProperties[1].setTicksLocations(ylocs);
		axesProperties[1].setTicksLabels(ylabels);
		axesProperties[2].setTicksLocations(zlocs);
		axesProperties[2].setTicksLabels(zlabels);
		
		/* Camera properties */
		camera = new Camera();

		Double[] rotationAngles = new Double[] {0.0, 270.0};

		camera.setView(ViewType.VIEW_2D);
		camera.setCubeScaling(randomGen.nextBoolean());
		camera.setIsoview(randomGen.nextBoolean());
		camera.setRotationAngles(rotationAngles);

		/* Box properties */
		box = new Box();

		Double[] dataBounds = new Double[] {0.0, 0.0, -1.0, 1.0, 1.0, 1.0};
		Double[] realDataBounds = new Double[] {0.0, 0.0, -1.0, 1.0, 1.0, 1.0};
		Double[] zoomBox = new Double[] {0.0, 0.0, -1.0, 1.0, 1.0, 1.0};

		box.setBox(BoxType.ON);
		box.setHiddenAxisColor(randomGen.nextInt());
		box.setTightLimits(randomGen.nextBoolean());
		box.setDataBounds(dataBounds);
		box.setRealDataBounds(realDataBounds);
		box.setZoomBox(zoomBox);
		box.setAutoScale(randomGen.nextBoolean());

		/* Miscellaneous Axes properties */
		gridPosition = GridPosition.BACKGROUND;
		autoClear = randomGen.nextBoolean();
		filled = randomGen.nextBoolean();

		margins = new Double[] {0.15, 0.15, 0.15, 0.15};
		axesBounds = new Double[] {0.0, 0.0, 1.0, 1.0};
	}

	/**
	 * Tests whether reflection-based set/getProperty methods work correctly
	 * @return the test result
	 */
	public boolean testReflectSetGetAxesProperties() {

		/* Properties inherited from GraphicObject */
		controller.setProperty(axesID, "Visible", visible);

		/*
		 * The Parent property set/get are not tested at the moment since there is a problem with it
		 * when reflection is used
		 */
		GraphicObject parentFigure = (GraphicObject) controller.getObjectFromId(figureID);

		controller.setPropertyFast(axesID, "Parent", (Object) parentFigure);

		GraphicObject retParent = (GraphicObject) controller.getProperty(axesID, "Parent");

		/* X-Axis properties */
		controller.setProperty(axesID, "XAxisVisible", axesProperties[0].getVisible());
		controller.setProperty(axesID, "XAxisReverse", axesProperties[0].getReverse());
		controller.setProperty(axesID, "XAxisGridColor", axesProperties[0].getGridColor());
		controller.setProperty(axesID, "XAxisLabel", xlabel);
		controller.setProperty(axesID, "XAxisLocation", axesProperties[0].getAxisLocation());
		controller.setProperty(axesID, "XAxisLogFlag", axesProperties[0].getLogFlag());
		controller.setProperty(axesID, "XAxisAutoTicks", axesProperties[0].getAutoTicks());
		controller.setProperty(axesID, "XAxisSubticks", axesProperties[0].getSubticks());
		controller.setProperty(axesID, "XAxisTicksLocations", axesProperties[0].getTicksLocations());
		controller.setProperty(axesID, "XAxisTicksLabels", axesProperties[0].getTicksLabels());

		/* Y-Axis properties */
		controller.setProperty(axesID, "YAxisVisible", axesProperties[1].getVisible());
		controller.setProperty(axesID, "YAxisReverse", axesProperties[1].getReverse());
		controller.setProperty(axesID, "YAxisGridColor", axesProperties[1].getGridColor());
		controller.setProperty(axesID, "YAxisLabel", ylabel);
		controller.setProperty(axesID, "YAxisLocation", axesProperties[1].getAxisLocation());
		controller.setProperty(axesID, "YAxisLogFlag", axesProperties[1].getLogFlag());
		controller.setProperty(axesID, "YAxisAutoTicks", axesProperties[1].getAutoTicks());
		controller.setProperty(axesID, "YAxisSubticks", axesProperties[1].getSubticks());
		controller.setProperty(axesID, "YAxisTicksLocations", axesProperties[1].getTicksLocations());
		controller.setProperty(axesID, "YAxisTicksLabels", axesProperties[1].getTicksLabels());

		/* Z-Axis properties */
		controller.setProperty(axesID, "ZAxisVisible", axesProperties[2].getVisible());
		controller.setProperty(axesID, "ZAxisReverse", axesProperties[2].getReverse());
		controller.setProperty(axesID, "ZAxisGridColor", axesProperties[2].getGridColor());
		controller.setProperty(axesID, "ZAxisLabel", zlabel);
		controller.setProperty(axesID, "ZAxisLocation", axesProperties[2].getAxisLocation());
		controller.setProperty(axesID, "ZAxisLogFlag", axesProperties[2].getLogFlag());
		controller.setProperty(axesID, "ZAxisAutoTicks", axesProperties[2].getAutoTicks());
		controller.setProperty(axesID, "ZAxisSubticks", axesProperties[2].getSubticks());
		controller.setProperty(axesID, "ZAxisTicksLocations", axesProperties[2].getTicksLocations());
		controller.setProperty(axesID, "ZAxisTicksLabels", axesProperties[2].getTicksLabels());

		/* Miscellaneous properties */
		controller.setProperty(axesID, "GridPosition", gridPosition);
		controller.setProperty(axesID, "Title", title);
		controller.setProperty(axesID, "AutoClear", autoClear);
		controller.setProperty(axesID, "Filled", filled);
		controller.setProperty(axesID, "Margins", margins);
		controller.setProperty(axesID, "AxesBounds", axesBounds);

		/* Camera properties */
		controller.setProperty(axesID, "View", camera.getView());
		controller.setProperty(axesID, "Isoview", camera.getIsoview());
		controller.setProperty(axesID, "CubeScaling", camera.getCubeScaling());
		controller.setProperty(axesID, "RotationAngles", camera.getRotationAngles());


		/* Box properties */
		controller.setProperty(axesID, "BoxType", box.getBox());
		controller.setProperty(axesID, "HiddenAxisColor", box.getHiddenAxisColor());
		controller.setProperty(axesID, "TightLimits", box.getTightLimits());
		controller.setProperty(axesID, "DataBounds", box.getDataBounds());
		controller.setProperty(axesID, "RealDataBounds", box.getRealDataBounds());
		controller.setProperty(axesID, "ZoomBox", box.getZoomBox());
		controller.setProperty(axesID, "AutoScale", box.getAutoScale());


		/* Get properties */

		/* GraphicObject-inherited properties */

		Boolean axesvisible  = (Boolean) controller.getProperty(axesID, "Visible");

		if (!axesvisible.equals(visible)) {
			return false;
		}

		GraphicObject parentObject = (GraphicObject) controller.getProperty(axesID, "Parent");

		if (!parentObject.equals(parentFigure)) {
			return false;
		}

		/* X-axis properties */
		Boolean retAxisvisible = (Boolean) controller.getProperty(axesID, "XAxisVisible");
		Boolean retAxisreverse = (Boolean) controller.getProperty(axesID, "XAxisReverse");     	
		Integer retGridcolor = (Integer) controller.getProperty(axesID, "XAxisGridColor");   	
		Label retXlabel = (Label) controller.getProperty(axesID, "XAxisLabel");
		AxisLocation retAxislocation = (AxisLocation) controller.getProperty(axesID, "XAxisLocation");
		Boolean retLogflag = (Boolean) controller.getProperty(axesID, "XAxisLogFlag");
		Boolean retAutoticks = (Boolean) controller.getProperty(axesID, "XAxisAutoTicks");
		Integer retSubticks = (Integer) controller.getProperty(axesID, "XAxisSubticks");
		Double[] retTickslocations = (Double[]) controller.getProperty(axesID, "XAxisTicksLocations");
		ArrayList retTickslabels = (ArrayList) controller.getProperty(axesID, "XAxisTicksLabels");

		if (!retAxisvisible.equals(axesProperties[0].getVisible())) {
			return false;
		}
		if (!retAxisreverse.equals(axesProperties[0].getReverse())) {
			return false;
		}
		if (!retGridcolor.equals(axesProperties[0].getGridColor())) {
			return false;
		}
		if (!retXlabel.equals(this.xlabel)) {
			return false;
		}
		if (!retAxislocation.equals(axesProperties[0].getAxisLocation())) {
			return false;
		}
		if (!retLogflag.equals(axesProperties[0].getLogFlag())) {
			return false;
		}
		if (!retAutoticks.equals(axesProperties[0].getAutoTicks())) {
			return false;
		}
		if (!retSubticks.equals(axesProperties[0].getSubticks())) {
			return false;
		}
		if (!Arrays.equals(retTickslocations, axesProperties[0].getTicksLocations())) {
			return false;
		}
		if (!compareFormattedTextLists(retTickslabels, axesProperties[0].getTicksLabels())) {
			return false;
		}

		/* Y-Axis properties */
		retAxisvisible = (Boolean) controller.getProperty(axesID, "YAxisVisible");
		retAxisreverse = (Boolean) controller.getProperty(axesID, "YAxisReverse");
		retGridcolor = (Integer) controller.getProperty(axesID, "YAxisGridColor");
		Label retYlabel = (Label) controller.getProperty(axesID, "YAxisLabel");
		retAxislocation = (AxisLocation) controller.getProperty(axesID, "YAxisLocation");
		retLogflag = (Boolean) controller.getProperty(axesID, "YAxisLogFlag");
		retAutoticks = (Boolean) controller.getProperty(axesID, "YAxisAutoTicks");
		retSubticks = (Integer) controller.getProperty(axesID, "YAxisSubticks");
		retTickslocations 	= (Double[]) controller.getProperty(axesID, "YAxisTicksLocations");
		retTickslabels = (ArrayList) controller.getProperty(axesID, "YAxisTicksLabels");

		if (!retAxisvisible.equals(axesProperties[1].getVisible())) {
			return false;
		}
		if (!retAxisreverse.equals(axesProperties[1].getReverse())) {
			return false;
		}
		if (!retGridcolor.equals(axesProperties[1].getGridColor())) {
			return false;
		}
		if (!retYlabel.equals(this.ylabel)) {
			return false;
		}
		if (!retAxislocation.equals(axesProperties[1].getAxisLocation())) {
			return false;
		}
		if (!retLogflag.equals(axesProperties[1].getLogFlag())) {
			return false;
		}
		if (!retAutoticks.equals(axesProperties[1].getAutoTicks())) {
			return false;
		}
		if (!retSubticks.equals(axesProperties[1].getSubticks())) {
			return false;
		}
		if (!Arrays.equals(retTickslocations, axesProperties[1].getTicksLocations())) {
			return false;
		}
		if (!compareFormattedTextLists(retTickslabels, axesProperties[1].getTicksLabels())) {
			return false;
		}

		/* Z-Axis properties */
		retAxisvisible = (Boolean) controller.getProperty(axesID, "ZAxisVisible");
		retAxisreverse = (Boolean) controller.getProperty(axesID, "ZAxisReverse");
		retGridcolor = (Integer) controller.getProperty(axesID, "ZAxisGridColor");
		Label retZlabel = (Label) controller.getProperty(axesID, "ZAxisLabel");
		retAxislocation = (AxisLocation) controller.getProperty(axesID, "ZAxisLocation");
		retLogflag = (Boolean) controller.getProperty(axesID, "ZAxisLogFlag");
		retAutoticks = (Boolean) controller.getProperty(axesID, "ZAxisAutoTicks");
		retSubticks = (Integer) controller.getProperty(axesID, "ZAxisSubticks");
		retTickslocations = (Double[]) controller.getProperty(axesID, "ZAxisTicksLocations");
		retTickslabels = (ArrayList) controller.getProperty(axesID, "ZAxisTicksLabels");


		if (!retAxisvisible.equals(axesProperties[2].getVisible())) {
			return false;
		}
		if (!retAxisreverse.equals(axesProperties[2].getReverse())) {
			return false;
		}
		if (!retGridcolor.equals(axesProperties[2].getGridColor())) {
			return false;
		}
		if (!retZlabel.equals(this.zlabel)) {
			return false;
		}
		if (!retAxislocation.equals(axesProperties[2].getAxisLocation())) {
			return false;
		}
		if (!retLogflag.equals(axesProperties[2].getLogFlag())) {
			return false;
		}
		if (!retAutoticks.equals(axesProperties[2].getAutoTicks())) {
			return false;
		}
		if (!retSubticks.equals(axesProperties[2].getSubticks())) {
			return false;
		}
		if (!Arrays.equals(retTickslocations, axesProperties[2].getTicksLocations())) {
			return false;
		}
		if (!compareFormattedTextLists(retTickslabels, axesProperties[2].getTicksLabels())) {
			return false;
		}


		/* Camera properties */
		ViewType retView = (ViewType) controller.getProperty(axesID, "View");
		Boolean retIsoview = (Boolean) controller.getProperty(axesID, "Isoview");
		Boolean retCubeScaling = (Boolean) controller.getProperty(axesID, "CubeScaling");
		Double[] retRotationAngles = (Double[]) controller.getProperty(axesID, "RotationAngles");

		if (!retView.equals(camera.getView())) {  
			return false;
		}
		if (!retIsoview.equals(camera.getIsoview())) {
			return false;
		}
		if (!retCubeScaling.equals(camera.getCubeScaling())) {  
			return false;
		}
		if (!Arrays.equals(retRotationAngles, camera.getRotationAngles()) || retRotationAngles.length != 2) {
			return false;
		}

		/* Box properties */
		BoxType retBoxType = (BoxType) controller.getProperty(axesID, "BoxType");
		Boolean retTightLimits = (Boolean) controller.getProperty(axesID, "TightLimits");
		Double[] retDataBounds = (Double[]) controller.getProperty(axesID, "DataBounds");
		Double[] retRealDataBounds = (Double[]) controller.getProperty(axesID, "RealDataBounds");
		Double[] retZoomBox = (Double[]) controller.getProperty(axesID, "ZoomBox");
		Boolean retAutoScale = (Boolean) controller.getProperty(axesID, "AutoScale");

		if (!retBoxType.equals(box.getBox())) {
			return false;
		}
		if (!retTightLimits.equals(box.getTightLimits())) {
			return false;
		}
		if (!Arrays.equals(retDataBounds, box.getDataBounds())) {
			return false;
		}
		if (!Arrays.equals(retRealDataBounds, box.getRealDataBounds())) {
			return false;
		}
		if (!Arrays.equals(retZoomBox, box.getZoomBox())) {
			return false;
		}
		if (!retAutoScale.equals(box.getAutoScale())) {
			return false;
		}

		/* Miscellaneous Axes properties */
		GridPosition retGridpos = (GridPosition) controller.getProperty(axesID, "GridPosition");
		Label retTitle = (Label) controller.getProperty(axesID, "Title");
		Boolean retAutoclear = (Boolean) controller.getProperty(axesID, "AutoClear");
		Boolean retFilled = (Boolean) controller.getProperty(axesID, "Filled");
		Double[] retMargins = (Double[]) controller.getProperty(axesID, "Margins");
		Double[] retAxesBounds = (Double[]) controller.getProperty(axesID, "AxesBounds");

		if (!retGridpos.equals(this.gridPosition)) {
			return false;
		}
		if (!retTitle.equals(this.title)) {
			return false;
		}
		if (!retAutoclear.equals(autoClear)) {
			return false;
		}
		if (!retFilled.equals(this.filled)) {
			return false;
		}
		if (!Arrays.equals(retMargins, margins)) {
			return false;
		}
		if (!Arrays.equals(retAxesBounds, axesBounds)) {
			return false;
		}

		return true;
	}

	/**
	 * Tests whether fast set/getProperty methods work correctly
	 * @return the test result
	 */
	public boolean testFastSetGetAxesProperties() {

		/* Properties inherited from GraphicObject */
		controller.setPropertyFast(axesID, "Visible", visible);

		GraphicObject parentFigure = (GraphicObject) controller.getObjectFromId(figureID);

		controller.setPropertyFast(axesID, "Parent", (Object) parentFigure);

		/* X-Axis properties */
		controller.setPropertyFast(axesID, "XAxisVisible", axesProperties[0].getVisible());
		controller.setPropertyFast(axesID, "XAxisReverse", axesProperties[0].getReverse());
		controller.setPropertyFast(axesID, "XAxisGridColor", axesProperties[0].getGridColor());
		controller.setPropertyFast(axesID, "XAxisLabel", xlabel);
		controller.setPropertyFast(axesID, "XAxisLocation", axesProperties[0].getAxisLocation());
		controller.setPropertyFast(axesID, "XAxisLogFlag", axesProperties[0].getLogFlag());
		controller.setPropertyFast(axesID, "XAxisAutoTicks", axesProperties[0].getAutoTicks());
		controller.setPropertyFast(axesID, "XAxisSubticks", axesProperties[0].getSubticks());
		controller.setPropertyFast(axesID, "XAxisTicksLocations", axesProperties[0].getTicksLocations()); 	
		controller.setPropertyFast(axesID, "XAxisTicksLabels", axesProperties[0].getTicksLabels());

    	
		/* Y-Axis properties */
		controller.setPropertyFast(axesID, "YAxisVisible", axesProperties[1].getVisible());
		controller.setPropertyFast(axesID, "YAxisReverse", axesProperties[1].getReverse());
		controller.setPropertyFast(axesID, "YAxisGridColor", axesProperties[1].getGridColor());
		controller.setPropertyFast(axesID, "YAxisLabel", ylabel);
		controller.setPropertyFast(axesID, "YAxisLocation", axesProperties[1].getAxisLocation());
		controller.setPropertyFast(axesID, "YAxisLogFlag", axesProperties[1].getLogFlag());
		controller.setPropertyFast(axesID, "YAxisAutoTicks", axesProperties[1].getAutoTicks());
		controller.setPropertyFast(axesID, "YAxisSubticks", axesProperties[1].getSubticks());
		controller.setPropertyFast(axesID, "YAxisTicksLocations", axesProperties[1].getTicksLocations());
		controller.setPropertyFast(axesID, "YAxisTicksLabels", axesProperties[1].getTicksLabels());

		/* Z-Axis properties */
		controller.setPropertyFast(axesID, "ZAxisVisible", axesProperties[2].getVisible());
		controller.setPropertyFast(axesID, "ZAxisReverse", axesProperties[2].getReverse());
		controller.setPropertyFast(axesID, "ZAxisGridColor", axesProperties[2].getGridColor());
		controller.setPropertyFast(axesID, "ZAxisLabel", zlabel);
		controller.setPropertyFast(axesID, "ZAxisLocation", axesProperties[2].getAxisLocation());
		controller.setPropertyFast(axesID, "ZAxisLogFlag", axesProperties[2].getLogFlag());
		controller.setPropertyFast(axesID, "ZAxisAutoTicks", axesProperties[2].getAutoTicks());
		controller.setPropertyFast(axesID, "ZAxisSubticks", axesProperties[2].getSubticks());
		controller.setPropertyFast(axesID, "ZAxisTicksLocations", axesProperties[2].getTicksLocations());
		controller.setPropertyFast(axesID, "ZAxisTicksLabels", axesProperties[2].getTicksLabels());

		/* Miscellaneous properties */
		controller.setPropertyFast(axesID, "GridPosition", gridPosition);
		controller.setPropertyFast(axesID, "Title", title);
		controller.setPropertyFast(axesID, "AutoClear", autoClear);
		controller.setPropertyFast(axesID, "Filled", filled);
		controller.setPropertyFast(axesID, "Margins", margins);
		controller.setPropertyFast(axesID, "AxesBounds", axesBounds);

		/* Camera properties */
		controller.setPropertyFast(axesID, "View", camera.getView());
		controller.setPropertyFast(axesID, "Isoview", camera.getIsoview());
		controller.setPropertyFast(axesID, "CubeScaling", camera.getCubeScaling());
		controller.setPropertyFast(axesID, "RotationAngles", camera.getRotationAngles());

		/* Box properties */
		controller.setPropertyFast(axesID, "BoxType", box.getBox());
		controller.setPropertyFast(axesID, "HiddenAxisColor", box.getHiddenAxisColor());
		controller.setPropertyFast(axesID, "TightLimits", box.getTightLimits());
		controller.setPropertyFast(axesID, "DataBounds", box.getDataBounds());
		controller.setPropertyFast(axesID, "RealDataBounds", box.getRealDataBounds());
		controller.setPropertyFast(axesID, "ZoomBox", box.getZoomBox());
		controller.setPropertyFast(axesID, "AutoScale", box.getAutoScale());


		/* Get properties */

		/* GraphicObject-inherited properties */

		Boolean axesvisible  = (Boolean) controller.getPropertyFast(axesID, "Visible");

		if (!axesvisible.equals(visible)) {
			return false;
		}

		GraphicObject parentObject = (GraphicObject) controller.getPropertyFast(axesID, "Parent");

		if (!parentObject.equals(parentFigure)) {
			return false;
		}

		/* X-axis properties */
		Boolean retAxisvisible = (Boolean) controller.getPropertyFast(axesID, "XAxisVisible");
		Boolean retAxisreverse = (Boolean) controller.getPropertyFast(axesID, "XAxisReverse");     	
		Integer retGridcolor = (Integer) controller.getPropertyFast(axesID, "XAxisGridColor");   	
		Label retXlabel = (Label) controller.getPropertyFast(axesID, "XAxisLabel");
		AxisLocation retAxislocation = (AxisLocation) controller.getPropertyFast(axesID, "XAxisLocation");
		Boolean retLogflag = (Boolean) controller.getPropertyFast(axesID, "XAxisLogFlag");
		Boolean retAutoticks = (Boolean) controller.getPropertyFast(axesID, "XAxisAutoTicks");
		Integer retSubticks = (Integer) controller.getPropertyFast(axesID, "XAxisSubticks");
		Double[] retTickslocations = (Double[]) controller.getPropertyFast(axesID, "XAxisTicksLocations");
		ArrayList retTickslabels = (ArrayList) controller.getPropertyFast(axesID, "XAxisTicksLabels");

		/* X-axis properties */
		if (!retAxisvisible.equals(axesProperties[0].getVisible())) {
			return false;
		}
		if (!retAxisreverse.equals(axesProperties[0].getReverse())) {
			return false;
		}
		if (!retGridcolor.equals(axesProperties[0].getGridColor())) {
			return false;
		}
		if (!retXlabel.equals(this.xlabel)) {
			return false;
		}
		if (!retAxislocation.equals(axesProperties[0].getAxisLocation())) {
			return false;
		}
		if (!retLogflag.equals(axesProperties[0].getLogFlag())) {
			return false;
		}
		if (!retAutoticks.equals(axesProperties[0].getAutoTicks())) {
			return false;
		}
		if (!retSubticks.equals(axesProperties[0].getSubticks())) {
			return false;
		}
		if (!Arrays.equals(retTickslocations, axesProperties[0].getTicksLocations())) {
			return false;
		}
		if (!compareFormattedTextLists(retTickslabels, axesProperties[0].getTicksLabels())) {
			return false;
		}

		/* Y-Axis properties */
		retAxisvisible = (Boolean) controller.getPropertyFast(axesID, "YAxisVisible");
		retAxisreverse = (Boolean) controller.getPropertyFast(axesID, "YAxisReverse");
		retGridcolor = (Integer) controller.getPropertyFast(axesID, "YAxisGridColor");
		Label retYlabel = (Label) controller.getPropertyFast(axesID, "YAxisLabel");
		retAxislocation = (AxisLocation) controller.getPropertyFast(axesID, "YAxisLocation");
		retLogflag = (Boolean) controller.getPropertyFast(axesID, "YAxisLogFlag");
		retAutoticks = (Boolean) controller.getPropertyFast(axesID, "YAxisAutoTicks");
		retSubticks = (Integer) controller.getPropertyFast(axesID, "YAxisSubticks");
		retTickslocations 	= (Double[]) controller.getPropertyFast(axesID, "YAxisTicksLocations");
		retTickslabels = (ArrayList) controller.getPropertyFast(axesID, "YAxisTicksLabels");

		/* Y-axis properties */
		if (!retAxisvisible.equals(axesProperties[1].getVisible())) {
			return false;
		}
		if (!retAxisreverse.equals(axesProperties[1].getReverse())) {
			return false;
		}
		if (!retGridcolor.equals(axesProperties[1].getGridColor())) {
			return false;
		}
		if (!retYlabel.equals(ylabel)) {
			return false;
		}
		if (!retAxislocation.equals(axesProperties[1].getAxisLocation())) {
			return false;
		}
		if (!retLogflag.equals(axesProperties[1].getLogFlag())) {
			return false;
		}
		if (!retAutoticks.equals(axesProperties[1].getAutoTicks())) {
			return false;
		}
		if (!retSubticks.equals(axesProperties[1].getSubticks())) {
			return false;
		}
		if (!Arrays.equals(retTickslocations, axesProperties[1].getTicksLocations())) {
			return false;
		}
		if (!compareFormattedTextLists(retTickslabels, axesProperties[1].getTicksLabels())) {
			return false;
		}

		/* Z-Axis properties */
		retAxisvisible = (Boolean) controller.getPropertyFast(axesID, "ZAxisVisible");
		retAxisreverse = (Boolean) controller.getPropertyFast(axesID, "ZAxisReverse");
		retGridcolor = (Integer) controller.getPropertyFast(axesID, "ZAxisGridColor");
		Label retZlabel = (Label) controller.getPropertyFast(axesID, "ZAxisLabel");
		retAxislocation = (AxisLocation) controller.getPropertyFast(axesID, "ZAxisLocation");
		retLogflag = (Boolean) controller.getPropertyFast(axesID, "ZAxisLogFlag");
		retAutoticks = (Boolean) controller.getPropertyFast(axesID, "ZAxisAutoTicks");
		retSubticks = (Integer) controller.getPropertyFast(axesID, "ZAxisSubticks");
		retTickslocations = (Double[]) controller.getPropertyFast(axesID, "ZAxisTicksLocations");
		retTickslabels = (ArrayList) controller.getPropertyFast(axesID, "ZAxisTicksLabels");


		if (!retAxisvisible.equals(axesProperties[2].getVisible())) {
			return false;
		}
		if (!retAxisreverse.equals(axesProperties[2].getReverse())) {
			return false;
		}
		if (!retGridcolor.equals(axesProperties[2].getGridColor())) {
			return false;
		}
		if (!retZlabel.equals(zlabel)) {
			return false;
		}
		if (!retAxislocation.equals(axesProperties[2].getAxisLocation())) {
			return false;
		}
		if (!retLogflag.equals(axesProperties[2].getLogFlag())) {
			return false;
		}
		if (!retAutoticks.equals(axesProperties[2].getAutoTicks())) {
			return false;
		}
		if (!retSubticks.equals(axesProperties[2].getSubticks())) {
			return false;
		}
		if (!Arrays.equals(retTickslocations, axesProperties[2].getTicksLocations())) {
			return false;
		}
		if (!compareFormattedTextLists(retTickslabels, axesProperties[2].getTicksLabels())) {
			return false;
		}

		/* Camera properties */
		ViewType retView = (ViewType) controller.getProperty(axesID, "View");
		Boolean retIsoview = (Boolean) controller.getProperty(axesID, "Isoview");
		Boolean retCubeScaling = (Boolean) controller.getProperty(axesID, "CubeScaling");
		Double[] retRotationAngles = (Double[]) controller.getProperty(axesID, "RotationAngles");

		if (!retView.equals(camera.getView())) {  
			return false;
		}
		if (!retIsoview.equals(camera.getIsoview())) {
			return false;
		}
		if (!retCubeScaling.equals(camera.getCubeScaling())) {  
			return false;
		}
		if (!Arrays.equals(retRotationAngles, camera.getRotationAngles()) || retRotationAngles.length != 2) {
			return false;
		}

		/* Box properties */
		BoxType retBoxType = (BoxType) controller.getPropertyFast(axesID, "BoxType");
		Boolean retTightLimits = (Boolean) controller.getPropertyFast(axesID, "TightLimits");
		Double[] retDataBounds = (Double[]) controller.getPropertyFast(axesID, "DataBounds");
		Double[] retRealDataBounds = (Double[]) controller.getPropertyFast(axesID, "RealDataBounds");
		Double[] retZoomBox = (Double[]) controller.getPropertyFast(axesID, "ZoomBox");
		Boolean retAutoScale = (Boolean) controller.getPropertyFast(axesID, "AutoScale");

		if (!retBoxType.equals(box.getBox())) {
			return false;
		}
		if (!retTightLimits.equals(box.getTightLimits())) {
			return false;
		}
		if (!Arrays.equals(retDataBounds, box.getDataBounds())) {
			return false;
		}
		if (!Arrays.equals(retRealDataBounds, box.getRealDataBounds())) {
			return false;
		}
		if (!Arrays.equals(retZoomBox, box.getZoomBox())) {
			return false;
		}
		if (!retAutoScale.equals(box.getAutoScale())) {
			return false;
		}

		/* Miscellaneous Axes properties */
		GridPosition retGridpos = (GridPosition) controller.getPropertyFast(axesID, "GridPosition");
		Label retTitle = (Label) controller.getPropertyFast(axesID, "Title");
		Boolean retAutoclear = (Boolean) controller.getPropertyFast(axesID, "AutoClear");
		Boolean retFilled = (Boolean) controller.getPropertyFast(axesID, "Filled");
		Double[] retMargins = (Double[]) controller.getPropertyFast(axesID, "Margins");
		Double[] retAxesBounds = (Double[]) controller.getPropertyFast(axesID, "AxesBounds");

		if (!retGridpos.equals(this.gridPosition)) {
			return false;
		}
		if (!retTitle.equals(title)) {
			return false;
		}
		if (!retAutoclear.equals(autoClear)) {
			return false;
		}
		if (!retFilled.equals(this.filled)) {
			return false;
		}
		if (!Arrays.equals(retMargins, margins)) {
			return false;
		}
		if (!Arrays.equals(retAxesBounds, axesBounds)) {
			return false;
		}

		return true;
	}

	/**
	 * A simple utility method which compares two FormattedText ArrayLists
	 * @param list1 the first list
	 * @param list2 the second list
	 * @return true if the two lists are equal, else false  
	 */
	public boolean compareFormattedTextLists(ArrayList <FormattedText> list1, ArrayList <FormattedText> list2) {

		if (list1.size() != list2.size()) {
			return false;
		}

		for (int i = 0; i < list1.size(); i++) {
			String s1 = new String(((FormattedText) list1.get(i)).getText()); 
			String s2 = new String(((FormattedText) list2.get(i)).getText());

			if (!s1.equals(s2)) {
				return false;
			}

			Font f1 = list1.get(i).getFont();
			Font f2 = list2.get(i).getFont();

			if (!f1.equals(f2)) {
				return false;
			}
		}

		return true;
	}
}
