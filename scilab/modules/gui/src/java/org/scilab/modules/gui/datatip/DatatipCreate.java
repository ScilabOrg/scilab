package org.scilab.modules.gui.datatip;

import org.scilab.modules.graphic_objects.graphicController.GraphicController;
import org.scilab.modules.graphic_objects.graphicObject.GraphicObject.Type;
import org.scilab.modules.graphic_objects.graphicObject.*;

import org.scilab.modules.renderer.CallRenderer;

import java.lang.String;

import org.scilab.modules.gui.editor.AxesHandler;

public class DatatipCreate {
	
	public static double[] pixelMouseCoordDouble = { 0.0 , 0.0 };
	public static double[] graphicCoord = { 0.0 , 0.0 };
	public static Integer[] datatipBounds = { 0 , 0 };
	public static Double[] datatipPosition = { 0.0 , 0.0 , 0.0 };
	public static String newDatatip;
	public static String axesUid;
	public static String[] datatipLabel;

	public static void createDatatip (String figureUid, Integer[] pixelMouseCoordInt)
	{
		axesUid = datatipAxesHandler (figureUid, pixelMouseCoordInt);
		pixelMouseCoordDouble = transformPixelCoordToDouble (pixelMouseCoordInt);
		graphicCoord = transformPixelCoordToGraphic (axesUid, pixelMouseCoordDouble);
		newDatatip = askToCreateObject ();
		datatipLabel = setDatatipLabel (graphicCoord);
		datatipBounds = getDatatipBounds (datatipLabel);
		datatipPosition = setDatatipPosition (graphicCoord);
		GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_TEXT_ARRAY_DIMENSIONS__, datatipBounds);
		GraphicController.getController().setGraphicObjectRelationship(axesUid, newDatatip);
		GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_TEXT_STRINGS__, datatipLabel);
		GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_BOX__, true);
		GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_POSITION__, datatipPosition);
		GraphicController.getController().setProperty(newDatatip, GraphicObjectProperties.__GO_LINE_MODE__, true);
		
	}

	public static String datatipAxesHandler (String figureUid, Integer[] pixelMouseCoordInt)
	{
		String axesUid = AxesHandler.clickedAxes(figureUid, pixelMouseCoordInt);
		return axesUid;
	}

	public static double[] transformPixelCoordToDouble (Integer[] pixelMouseCoordInt)
	{
		for (int i = 0 ; i < pixelMouseCoordInt.length ; i++)
		{	
			pixelMouseCoordDouble[i] = (double) pixelMouseCoordInt[i];
		}
		return pixelMouseCoordDouble;
	}

	public static double[] transformPixelCoordToGraphic (String axesUid, double[] pixelMouseCoordDouble)
	{
		graphicCoord = CallRenderer.get2dViewFromPixelCoordinates(axesUid, pixelMouseCoordDouble);
		return graphicCoord;
	}

	public static String askToCreateObject ()
	{
		newDatatip = GraphicController.getController().askObject(GraphicObject.getTypeFromName(GraphicObjectProperties.__GO_TEXT__));
		return newDatatip;
	}

	public static String[] setDatatipLabel (double[] graphicCoord)
	{
		String datatipLabelX = String.format("%.5g%n", graphicCoord[0]);
		String datatipLabelY = String.format("%.5g%n", graphicCoord[1]);
		String[] datatipLabel = { datatipLabelX , datatipLabelY };
		return datatipLabel;
	}

	public static Integer[] getDatatipBounds (String[] datatipLabel)
	{
		datatipBounds[0] = datatipLabel.length;
		datatipBounds[1] = 1;
		return datatipBounds;
	}

	public static Double[] setDatatipPosition (double[] graphicCoord)
	{
		datatipPosition[0] = graphicCoord[0];
		datatipPosition[1] = graphicCoord[1];
		datatipPosition[2] = 0.0;
		return datatipPosition;
	}
}
