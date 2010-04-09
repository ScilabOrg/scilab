package org.scilab.modules.graphic_objects.tests;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.GraphicObject.Type;
import org.scilab.modules.graphic_objects.*;
import java.rmi.server.UID;
import org.scilab.modules.graphic_objects.ClippableProperty.ClipStateType;

/**
 * Class Test
 * A simple test class
 * @author juliachs
 */
public class Test {

	public static void main(String[] args) {
		System.out.println("Test\n");

		GraphicController mycontroller = GraphicController.getController();

		ClippableProperty clipProp = new ClippableProperty();

		Arc myArc = new Arc();

		System.out.format("arc wh: %f %f\n", myArc.getHeight(), myArc
				.getWidth());
		System.out.format("arc startAngle/endAngle: %f %f\n",
				myArc.getHeight(), myArc.getWidth());

		myArc.setArcDrawingMethod(Arc.ArcDrawingMethod.NURBS);

		Arc.ArcDrawingMethod mymeth = myArc.getArcDrawingMethod();

		if (mymeth != Arc.ArcDrawingMethod.NURBS) {
			System.out.format("!= NURBS\n");
		} else {
			System.out.format("NURBS\n");
		}

		Double[] pt = new Double[3];
		pt[0] = new Double(12.0);
		pt[1] = new Double(5.6);
		pt[2] = new Double(3.4);

		myArc.setUpperLeftPoint(pt);

		// pt[0] = 1.0; pt[1] = 1.0; pt[2] = 0.0;
		// myArc.setUpperLeftPoint(pt);

		Double[] ptret = myArc.getUpperLeftPoint();
		System.out.format("ul point: %f %f %f\n", ptret[0].doubleValue(),
				ptret[1].doubleValue(), ptret[2].doubleValue());

		double[] clipBox = new double[4];
		clipBox[0] = 1.0;
		clipBox[1] = 1.0;
		clipBox[2] = 1.0;
		clipBox[3] = 1.0;

		clipProp.setClipState(ClippableProperty.ClipStateType.OFF);
		clipProp.setClipBox(clipBox);

		myArc.setClipProperty(clipProp);

		ClippableProperty clipProp2 = myArc.getClipProperty();

		double[] clipBox2 = clipProp2.getClipBox();
		System.out.format("clipBox2: %f %f %f %f\n", clipBox2[0], clipBox2[1],
				clipBox2[2], clipBox2[3]);

		System.out.format("myarc BG: %d\n", myArc.getBackground());

		System.out.println("child objects list: " + myArc.getChildren() + "\n");
		System.out.println("parent object: " + myArc.getParent() + "\n");
		System.out.println("is visible: " + myArc.isVisible() + "\n");
		System.out.println("user data: " + myArc.getUserData() + "\n");

		byte[] ud = new byte[3];
		ud[0] = 124;
		ud[1] = 67;
		ud[2] = 125;

		myArc.setUserData(ud);
		byte[] ud2 = myArc.getUserData();
		System.out.format("user data: %d %d %d\n", ud2[0], ud2[1], ud2[2]);

		Rectangle rect = new Rectangle();

		double[] pt2 = new double[2];
		pt2[0] = -1.0;
		pt2[1] = 1.0;

		rect.setUpperLeftPoint(pt2);
		rect.setWidth(1.0);
		rect.setHeight(1.0);

		/*
		 * ptret = rect.getUpperLeftPoint(); System.out.format("rect xy: %f
		 * %f\n", ptret[0], ptret[1]); System.out.format("rect wh: %f %f\n",
		 * rect.getWidth(), rect.getHeight());
		 */

		System.out.format("rect isvisible: %b\n", rect.isVisible());
		rect.setVisible(true);
		System.out.format("rect isvisible: %b\n", rect.isVisible());

		System.out.format("rect bg: %d\n", rect.getBackground());
		System.out.format("fill mode: %b\n", rect.getFillMode());

		Line newLine = new Line();
		rect.setLine(newLine);

		System.out.format("rect line fg: %d\n", rect.getLine().getColor());
		System.out.format("rect line mode: %b\n", rect.getLine().getMode());
		System.out.format("rect line thickness: %f\n", rect.getLine()
				.getThickness());
		rect.getLine().setThickness(1.0);
		System.out.format("rect line thickness after set: %f\n", rect.getLine()
				.getThickness());

		System.out.format("Before askObject\n");
		UID arcID = mycontroller.askObject(Type.ARC);
		UID axesID = mycontroller.askObject(Type.AXES);
		UID figureID = mycontroller.askObject(Type.FIGURE);
		UID rectID = mycontroller.askObject(Type.RECTANGLE);
		UID unknownID = mycontroller.askObject(Type.UNKNOWNOBJECT);

		mycontroller.setProperty(arcID, "ArcDrawingMethod",
				(Object) Arc.ArcDrawingMethod.NURBS);
		mymeth = (Arc.ArcDrawingMethod) mycontroller.getProperty(arcID,
				"ArcDrawingMethod");

		if (mymeth != Arc.ArcDrawingMethod.NURBS) {
			System.out.format("!= NURBS\n");
		} else {
			System.out.format("NURBS\n");
		}

		mycontroller.setProperty(arcID, "ArcDrawingMethod",
				(Object) Arc.ArcDrawingMethod.LINES);

		mycontroller.setProperty(arcID, "Width", (Object) new Double(8.567));
		Double doubleval;
		doubleval = (Double) mycontroller.getProperty(arcID, "Width");
		System.out.format("Arc width: %f\n", doubleval.doubleValue());
		mycontroller.setProperty(arcID, "Height", (Object) new Double(3.0293));
		doubleval = (Double) mycontroller.getProperty(arcID, "Height");
		System.out.format("Arc Height: %f\n", doubleval.doubleValue());

		mycontroller.setProperty(arcID, "StartAngle",
				(Object) new Double(8.567));
		doubleval = (Double) mycontroller.getProperty(arcID, "StartAngle");
		System.out.format("Arc StartAngle: %f\n", doubleval.doubleValue());

		mycontroller.setProperty(arcID, "EndAngle", (Object) new Double(8.567));
		doubleval = (Double) mycontroller.getProperty(arcID, "EndAngle");
		System.out.format("Arc EndAngle: %f\n", doubleval.doubleValue());

		/* tests sur setPropertyFast */
		mycontroller.setPropertyFast(arcID, "EndAngle", (Object) new Double(
				54.78));
		mycontroller.setPropertyFast(arcID, "StartAngle", (Object) new Double(
				4354.78));
		mycontroller.setPropertyFast(arcID, "Width", (Object) new Double(100.0));
		mycontroller.setPropertyFast(arcID, "Height",
				(Object) new Double(200.0));

		mycontroller.setPropertyFast(arcID, "NonExistingProperty",
				(Object) new Double(4354.78));

		doubleval = (Double) mycontroller.getPropertyFast(arcID, "StartAngle");
		System.out.println("StartAngle: " + doubleval + "\n");
		doubleval = (Double) mycontroller.getPropertyFast(arcID, "EndAngle");
		System.out.println("EndAngle: " + doubleval + "\n");
		doubleval = (Double) mycontroller.getPropertyFast(arcID, "Width");
		System.out.println("Width: " + doubleval + "\n");
		doubleval = (Double) mycontroller.getPropertyFast(arcID, "Height");
		System.out.println("Height: " + doubleval + "\n");
		// doubleval = (Double) mycontroller.getPropertyFast(arcID,
		// "UpperLeftPoint");
		// System.out.println("ca devrait etre null: " + doubleval + "\n");
		Object mymeth2 = mycontroller
				.getPropertyFast(arcID, "ArcDrawingMethod");

		if ((Arc.ArcDrawingMethod) mymeth2 == Arc.ArcDrawingMethod.NURBS) {
			System.out.println("NURBS\n");
		} else {
			System.out.println("LINES\n");
		}

		mycontroller.setPropertyFast(arcID, "ArcDrawingMethod",
				(Object) Arc.ArcDrawingMethod.NURBS);

		mymeth2 = mycontroller.getPropertyFast(arcID, "ArcDrawingMethod");

		if ((Arc.ArcDrawingMethod) mymeth2 == Arc.ArcDrawingMethod.NURBS) {
			System.out.println("NURBS\n");
		} else {
			System.out.println("LINES\n");
		}

		// Integer intval = (Integer) mycontroller.getPropertyFast(arcID,
		// "Background");
		Integer intval = (Integer) mycontroller.getPropertyFast(arcID,
				"Background");
		System.out.format("background value: %d\n", intval.intValue());
		Boolean boolval = (Boolean) mycontroller.getPropertyFast(arcID,
				"FillMode");
		System.out.format("fill mode: %b\n", boolval.booleanValue());

		mycontroller.setPropertyFast(arcID, "Background",
				(Object) new Integer(66));
		// System.out.format("fill mode: %b\n", boolval.booleanValue());

		intval = (Integer) mycontroller.getPropertyFast(arcID, "Background");
		System.out.format("background value: %d\n", intval.intValue());

		// Get the parent
		System.out.format("get parent\n");
		GraphicObject myobject = (GraphicObject) mycontroller.getPropertyFast(
				arcID, "Parent");

		if (myobject == null) {
			System.out.format("null parent object !\n");
		} else {
			System.out.format("parent object: %s\n", myobject.toString());
		}

		boolval = (Boolean) mycontroller.getPropertyFast(arcID, "Visible");
		System.out.format("visible value: %b\n", boolval.booleanValue());

		mycontroller.setPropertyFast(arcID, "Visible", true);

		boolval = (Boolean) mycontroller.getProperty(arcID, "Visible");
		System.out.format("visible value after setProperty: %b\n", boolval
				.booleanValue());

		Object myrect = mycontroller.getObjectFromId(rectID);
		System.out.format("myrect toString: %s\n", myrect.toString());

		mycontroller.setPropertyFast(arcID, "Parent", myrect);
		Object myparent = mycontroller.getPropertyFast(arcID, "Parent");
		System.out.format("myparent.toString: %s\n", myparent.toString());
		mycontroller.setPropertyFast(arcID, "Parent", null);
		myparent = mycontroller.getPropertyFast(arcID, "Parent");

		if (myparent == null) {
			System.out.println("null parent !\n");
		}

		clipProp2 = (ClippableProperty) mycontroller.getPropertyFast(arcID,
				"ClipProperty");

		if (clipProp2 == null) {
			System.out.format("clipProp2 null\n");
		}

		mycontroller.setProperty(arcID, "ClipProperty", clipProp);

		clipProp2 = (ClippableProperty) mycontroller.getPropertyFast(arcID,
				"ClipProperty");
		System.out.format("After set ClipProperty\n");

		if (clipProp2 == null) {
			System.out.format("clipProp2 null\n");
		} else {
			System.out.format("clipProp2 non null\n");
			double[] clipBoxRet = clipProp2.getClipBox();
			System.out.format("clipBoxRet length: %d\n", clipBoxRet.length);
			System.out.format("clipBoxRet: %f %f %f %f\n", clipBoxRet[0],
					clipBoxRet[1], clipBoxRet[2], clipBoxRet[3]);
		}

		clipBox[0] = clipBox[1] = clipBox[2] = clipBox[3] = 2.5;
		clipProp.setClipBox(clipBox);
		mycontroller.setPropertyFast(arcID, "ClipProperty", clipProp);

		clipProp2 = (ClippableProperty) mycontroller.getPropertyFast(arcID,
				"ClipProperty");
		System.out.format("After ClipProperty\n");
		double[] clipBoxRet = clipProp2.getClipBox();
		System.out.format("clipBoxRet length: %d\n", clipBoxRet.length);
		System.out.format("clipBoxRet: %f %f %f %f\n", clipBoxRet[0],
				clipBoxRet[1], clipBoxRet[2], clipBoxRet[3]);

		Double[] myClipBox = new Double[4];
		myClipBox[0] = new Double(12.4);
		myClipBox[1] = new Double(7.9);
		myClipBox[2] = new Double(-3.6);
		myClipBox[3] = new Double(0.8);
		System.out.format("myClipBox length: %d\n", myClipBox.length);
		System.out.format("myClipBox values: %f %f %f %f\n", myClipBox[0]
				.doubleValue(), myClipBox[1].doubleValue(), myClipBox[2]
				.doubleValue(), myClipBox[3].doubleValue());
		mycontroller.setPropertyFast(arcID, "ClipBox", myClipBox);
		Double[] myclipbox = (Double[]) mycontroller.getPropertyFast(arcID,
				"ClipBox");

		System.out.format("myclipbox values: %f %f %f %f\n", myclipbox[0]
				.doubleValue(), myclipbox[1].doubleValue(), myclipbox[2]
				.doubleValue(), myclipbox[3].doubleValue());

		mycontroller.setPropertyFast(arcID, "ClipState", ClipStateType.ON);

		ClipStateType currentClipState = (ClipStateType) mycontroller
				.getPropertyFast(arcID, "ClipState");

		if (currentClipState == ClipStateType.ON) {
			System.out.format("ClipState ON\n");
		}

		mycontroller.setPropertyFast(arcID, "ClipState", ClipStateType.CLIPGRF);
		currentClipState = (ClipStateType) mycontroller.getPropertyFast(arcID,
				"ClipState");
		if (currentClipState == ClipStateType.CLIPGRF) {
			System.out.format("ClipState CLIPGRF\n");
		}

		mycontroller.setPropertyFast(arcID, "ClipState", ClipStateType.OFF);
		currentClipState = (ClipStateType) mycontroller.getPropertyFast(arcID,
				"ClipState");
		if (currentClipState == ClipStateType.OFF) {
			System.out.format("ClipState OFF\n");
		}

		Integer mybg0 = (Integer) mycontroller.getPropertyFast(arcID,
				"Background");
		System.out.format("background: %d\n", mybg0.intValue());
		Boolean fillmode = (Boolean) mycontroller.getPropertyFast(arcID,
				"FillMode");
		mycontroller.setPropertyFast(arcID, "FillMode", new Boolean(true));
		fillmode = (Boolean) mycontroller.getPropertyFast(arcID, "FillMode");
		System.out.format("fillmode: %b\n", fillmode.booleanValue());

		ud[0] = 100;
		ud[1] = 66;
		ud[2] = 12;
		mycontroller.setPropertyFast(arcID, "UserData", (Object) ud);
		//
		byte[] myuserdata = (byte[]) mycontroller.getPropertyFast(arcID,
				"UserData");
		System.out.format("userdata size: %d\n", myuserdata.length);
		System.out.format("myuserdata: %d %d %d\n", myuserdata[0],
				myuserdata[1], myuserdata[2]);

		mycontroller.setPropertyFast(rectID, "Width", new Double(66.7));
		Double rectwidth = (Double) mycontroller.getProperty(rectID, "Width");
		System.out.format("rectwidth: %f\n", rectwidth.doubleValue());

		Boolean autoclear = (Boolean) mycontroller.getPropertyFast(axesID,
				"AutoClear");
		mycontroller.setPropertyFast(axesID, "AutoClear", new Boolean(true));
		autoclear = (Boolean) mycontroller.getPropertyFast(axesID, "AutoClear");
		System.out.format("autoclear axes: %b\n", autoclear);
		double[] newdb = new double[6];
		newdb[0] = 0.0;
		newdb[1] = 1.0;
		newdb[2] = 0.0;
		newdb[3] = 1.0;
		newdb[4] = -1.0;
		newdb[5] = 1.0;

		Box newbox = new Box();
		mycontroller.setPropertyFast(axesID, "Box", newbox);
		// mycontroller.setPropertyFast(axesID, "DataBounds", newdb);

		Box boxret = (Box) mycontroller.getPropertyFast(axesID, "Box");
		System.out.format("boxret: %s\n", boxret.toString());
		System.out.format("newbox: %s\n", newbox.toString());

		// newbox.setDataBounds(newdb);

		newdb[0] = 349.0;
		newdb[1] = -63.0;
		mycontroller.setPropertyFast(axesID, "DataBounds", newdb);
		double[] dbret = (double[]) mycontroller.getPropertyFast(axesID,
				"DataBounds");
		System.out.format("debret: %f %f %f %f %f %f\n", dbret[0], dbret[1],
				dbret[2], dbret[3], dbret[4], dbret[5]);

		Boolean figvisible = (Boolean) mycontroller.getPropertyFast(figureID,
				"Visible");
		System.out.format("fig visible: %b\n", figvisible.booleanValue());
		// figureID

		GraphicObject parent = (GraphicObject) mycontroller.getPropertyFast(
				figureID, "Parent");
		System.out.println("parent " + parent + "\n");
		mycontroller.setPropertyFast(figureID, "Visible", new Boolean(true));
		figvisible = (Boolean) mycontroller
				.getPropertyFast(figureID, "Visible");
		System.out.format("fig visible after set: %b\n", figvisible
				.booleanValue());
		mycontroller.setPropertyFast(figureID, "InfoMessage", new String(
				"This is an info message"));
		String inforet = (String) mycontroller.getPropertyFast(figureID,
				"InfoMessage");
		System.out.format("info message ret: %s\n", inforet);

		int[] position = { 12, 50 };
		mycontroller.setPropertyFast(figureID, "Position", position);
		int[] posret;
		posret = (int[]) mycontroller.getPropertyFast(figureID, "Position");
		System.out.format("posret: %d %d\n", posret[0], posret[1]);
		int[] figSize = { 600, 500 };
		mycontroller.setPropertyFast(figureID, "Size", figSize);
		posret = (int[]) mycontroller.getPropertyFast(figureID, "Size");
		System.out.format("size: %d %d\n", posret[0], posret[1]);

		mycontroller.setPropertyFast(figureID, "Name", new String(
				"I am a figure"));
		mycontroller.setPropertyFast(figureID, "Id", new Integer(12));
		String nameret = (String) mycontroller
				.getPropertyFast(figureID, "Name");
		nameret = null;
		String nameret2 = (String) mycontroller.getPropertyFast(figureID,
				"Name");
		int idret = (Integer) mycontroller.getPropertyFast(figureID, "Id");

		System.out.format("name ret: %s, id ret: %d\n", nameret2, idret);

		mycontroller.setProperty(figureID, "ImmediateDrawing",
				new Boolean(true));
		boolean immdret = (Boolean) mycontroller.getPropertyFast(figureID,
				"ImmediateDrawing");
		mycontroller.setPropertyFast(figureID, "Antialiasing", new Integer(16));
		int aaret = (Integer) mycontroller.getPropertyFast(figureID,
				"Antialiasing");
		System.out.format("immdret: %b, aaret: %d\n", immdret, aaret);

		System.exit(1);

		Line newline = new Line();
		mycontroller.setPropertyFast(arcID, "Line", newline);

		System.out.format("before getLT\n");
		Double myretdouble = (Double) mycontroller.getPropertyFast(arcID,
				"LineThickness");
		System.out.format("mydouble: %f\n", myretdouble.doubleValue());
		mycontroller.setPropertyFast(arcID, "LineThickness", new Double(678.4));
		System.out.format("mydouble: %f\n", myretdouble.doubleValue());
		myretdouble = (Double) mycontroller.getPropertyFast(arcID,
				"LineThickness");
		System.out.format("après setLT: %f\n", myretdouble.doubleValue());
		Integer mylinecolor = (Integer) mycontroller.getPropertyFast(arcID,
				"LineColor");
		System.out.format("myint: %d\n", mylinecolor.intValue());
		mycontroller.setPropertyFast(arcID, "LineColor", new Integer(18));
		mylinecolor = (Integer) mycontroller
				.getPropertyFast(arcID, "LineColor");
		System.out.format("line color after set: %d\n", mylinecolor.intValue());

		Mark newmark = new Mark();
		mycontroller.setPropertyFast(arcID, "Mark", newmark);

		Object myobj = mycontroller.getPropertyFast(arcID, "Mark");
		System.out.println("my mark ! " + myobj);
		mycontroller.setPropertyFast(arcID, "MarkBackground", new Integer(66));
		Integer markbg = (Integer) mycontroller.getPropertyFast(arcID,
				"MarkBackground");
		System.out.format("markbg: %d\n", markbg.intValue());
		mycontroller.setPropertyFast(arcID, "MarkForeground", new Integer(47));
		Integer markfg = (Integer) mycontroller.getPropertyFast(arcID,
				"MarkForeground");
		System.out.format("markfg: %d\n", markfg.intValue());

		mycontroller.setPropertyFast(arcID, "MarkStyle", new Integer(847));
		Integer markstyle = (Integer) mycontroller.getPropertyFast(arcID,
				"MarkStyle");
		System.out.format("mark style: %d\n", markstyle.intValue());

		Boolean markmode = (Boolean) mycontroller.getPropertyFast(arcID,
				"MarkMode");
		System.out.format("markmode: %b\n", markmode.booleanValue());
		mycontroller.setPropertyFast(arcID, "MarkMode", new Boolean(true));
		markmode = (Boolean) mycontroller.getPropertyFast(arcID, "MarkMode");
		System.out.format("markmode after set: %b\n", markmode.booleanValue());

		System.exit(1);

		/* sample performance evaluation loop */
		/* to be extended */
		System.out.format("Test\n");
		long t0 = System.currentTimeMillis();
		for (int i = 0; i < 1000000; i++) {
			int myval = (int) (Math.random() * 128.0);
			// double myval = Math.random() * 200.0;
			// System.out.format("myval: %d\n", myval);

			mycontroller.setPropertyFast(arcID, "Background",
					(Object) new Integer(myval));

			// mycontroller.setPropertyFast(arcID, "Visible", (Object) new
			// Boolean(true));
			// mycontroller.setPropertyFast(arcID, "Width", (Object) new
			// Double(myval));
			// mycontroller.setPropertyFast(arcID, "Width", (Object) new
			// Double(myval));

			Boolean retbool = (Boolean) mycontroller.getPropertyFast(arcID,
					"Visible");

			Integer mybg = (Integer) mycontroller.getPropertyFast(arcID,
					"Background");

			mycontroller.setPropertyFast2(arcID, "ClipBox", myClipBox);
			// System.out.format("retbool value: %b\n", retbool);

			Double[] myclipbox2 = (Double[]) mycontroller.getPropertyFast(
					arcID, "ClipBox");

			// System.out.format("BG value: %d %d\n", myval,
			// mybg.intValue());
		}

		long t1 = System.currentTimeMillis();
		System.out.format("Delta t (millisecs): %d\n", t1 - t0);

		System.out.format("End\n");
	}

}
