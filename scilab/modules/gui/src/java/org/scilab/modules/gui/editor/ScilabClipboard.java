/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Pedro Arthur dos S. Souza
 * Copyright (C) 2012 - Caio Lucas dos S. Souza
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.editor;


import org.scilab.modules.gui.editor.PolylineHandler;

/**
* Implements clipboard between figures for polylines.
*
* @author Caio Souza <caioc2bolado@gmail.com>
* @author Pedro Souza <bygrandao@gmail.com>
*
* @since 2012-06-01
*/

public class ScilabClipboard {

	static ScilabClipboard instance = null;
	String polylineUid = null;
	Integer copiedColor = 0;
	boolean needDup = false;


	public static ScilabClipboard getInstance() {
		if (instance == null) {
			instance = new ScilabClipboard();
		}
		return instance;
	}

	/**
	* Copy a polyline to the clipboad.
	* @param polyline Polyline unique identifier.
	*/
	public void copy(String polyline) {
		polylineUid = polyline;
		needDup = true;
	}

	/**
	* Paste the copied polyline to the given axes.
	* @param figure Figure unique identifier.
	* @param position Vector with mouse position x and y.
	*/
	public void paste(String figure, Integer[] position) {
		String polyline = polylineUid;
		String axesFrom = (new ObjectSearcher()).searchParent(polyline, "Axes");

		if (axesFrom == null) {
			System.out.println("Null axesFrom!");
			return;
		}

		if (needDup == true ) {
			polyline = PolylineHandler.getInstance().duplicate(polylineUid);
		}
		else {
			PolylineHandler.getInstance().cut(polyline);
		}

		PolylineHandler.getInstance().setColor(polyline, copiedColor);
		
		String axesTo = AxesHandler.clickedAxes(figure, position);
		if (axesTo != null) {
			AxesHandler.axesBound(axesFrom, axesTo);
			AxesHandler.setAxesVisible(axesTo);
			PolylineHandler.getInstance().insert(axesTo, polyline);
			polylineUid = null;
		}
		else {
			axesTo = AxesHandler.duplicateAxes(figure, axesFrom);
			if (axesTo != null) {
				AxesHandler.axesBound(axesFrom, axesTo);
				PolylineHandler.getInstance().insert(axesTo, polyline);
				polylineUid = null;
			}
			System.out.println("Failed duplicating Axes");
		}
	}

	/**
	* Cut a polyline to the clipboad.
	* @param polyline Polyline unique identifier.
	*/
	public void cut(String polyline) {
		polylineUid = polyline;
		needDup = false;
	}

	/**
	* Check if there is any polyline copied/cuted to be pasted.
	* @return True if can be pasted, false otherwise.
	*/
	public boolean canPaste() {
		if (polylineUid != null) {
			return true;
		}
		return false;
	}

	/**
	* Used to store the color of the copied polyline.
	* @param color Polyline color.
	*/
	public void setCopiedColor(Integer color) {
		copiedColor = color;
	}

}
