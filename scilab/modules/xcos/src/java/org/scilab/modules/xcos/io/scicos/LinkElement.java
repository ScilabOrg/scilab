/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import static java.util.Arrays.asList;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabMList;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.link.BasicLink;
import org.scilab.modules.xcos.link.LinkPortMap;
import org.scilab.modules.xcos.port.BasicPort;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.util.mxPoint;

/**
 * Perform a link transformation between Scicos and Xcos.
 */
// CSOFF: ClassDataAbstractionCoupling
public class LinkElement extends AbstractElement<BasicLink> {
	private static final List<String> DATA_FIELD_NAMES = asList("Link", "xx",
			"yy", "id", "thick", "ct", "from", "to");
	
	private static final int XX_INDEX = 1;
	private static final int YY_INDEX = 2;
	private static final int CT_INDEX = 5;
	private static final int FROM_INDEX = 6;
	private static final int TO_INDEX = 7;

	/** Mutable field to easily get the data through methods */
	private ScilabMList data;
	private BasicPort start;
	private BasicPort end;

	/** The already decoded block map */
	private final Map<Integer, BasicBlock> blocks;

	/**
	 * Default constructor
	 * 
	 * @param blocks
	 *            the already decoded blocks
	 */
	public LinkElement(Map<Integer, BasicBlock> blocks) {
		this.blocks = blocks;
	}

	/**
	 * Decode the element into the link.
	 * 
	 * @param element
	 *            The current Scilab data
	 * @param into
	 *            the target, if null a new instance is allocated and returned.
	 * @return the decoded block.
	 * @throws ScicosFormatException
	 *             when e decoding error occurred.
	 * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.scilabTypes.ScilabType,
	 *      java.lang.Object)
	 */
	@Override
	public BasicLink decode(ScilabType element, BasicLink into)
			throws ScicosFormatException {
		data = (ScilabMList) element;
		BasicLink link = into;

		validate();

		if (into == null) {
			link = allocateLink();
		}

		searchForPorts(link);
		List<mxPoint> points = getPoints();

		/*
		 * Fill the data
		 */
		link.setSource(start);
		link.setTarget(end);
		
		mxGeometry geom = link.getGeometry();
		if (geom == null) {
			geom = new mxGeometry();
		}
		geom.setPoints(points);
		link.setGeometry(geom);

		return link;
	}

	/**
	 * @return a new typed link
	 */
	private BasicLink allocateLink() {
		BasicLink link = null;
		final int type = (int) ((ScilabDouble) data.get(CT_INDEX))
				.getRealPart()[0][1];

		try {
			link = LinkPortMap.getLinkClass(type).newInstance();
		} catch (InstantiationException e) {
			LogFactory.getLog(LinkElement.class).error(e);
		} catch (IllegalAccessException e) {
			LogFactory.getLog(LinkElement.class).error(e);
		}

		return link;
	}

	/**
	 * @return the link points
	 */
	private List<mxPoint> getPoints() {
		List<mxPoint> points = new ArrayList<mxPoint>();

		final double[][] xAxis = ((ScilabDouble) data.get(XX_INDEX))
				.getRealPart();
		final double[][] yAxis = ((ScilabDouble) data.get(YY_INDEX))
				.getRealPart();
		final boolean isColumnDominant = data.get(XX_INDEX).getHeight() >= data
				.get(XX_INDEX).getWidth();

		/*
		 * The first, last and common indexes.
		 */
		final int min = 1;
		final int max;
		final int[] indexes = {0, 0};
		if (isColumnDominant) {
			max = data.get(1).getHeight() - 1;
			indexes[0] = min;
		} else {
			max = data.get(1).getWidth() - 1;
			indexes[1] = min;
		}

		/*
		 * Loop all over the points
		 */
		for (int i = min; i < max; i++) {
			double x = xAxis[indexes[0]][indexes[1]];
			double y = -yAxis[indexes[0]][indexes[1]];

			// Center links
			x = x + ((start.getGeometry().getX() + end.getGeometry().getX()) / 2.0);
			y = y + ((start.getGeometry().getY() + end.getGeometry().getY()) / 2.0);

			// offset the axis
			y = y + start.getGeometry().getHeight();
			
			points.add(new mxPoint(x, y));

			incrementIndexes(indexes, isColumnDominant);
		}

		return points;
	}

	/**
	 * Fill the {@link #start} and {@link #end} parameters.
	 * 
	 * @param link
	 *            the link instance
	 */
	private void searchForPorts(BasicLink link) {
		final ScilabDouble from = (ScilabDouble) data.get(FROM_INDEX);
		final ScilabDouble to = (ScilabDouble) data.get(TO_INDEX);
		
		final double[][] fromReal = from.getRealPart();
		final double[][] toReal = to.getRealPart();
		
		final int[] indexes = {0, 0};
		final boolean isColumnDominant = from.getHeight() >= from.getWidth();
		
		final int startBlockIndex = (int) fromReal[indexes[0]][indexes[1]];
		final int endBlockIndex = (int) toReal[indexes[0]][indexes[1]];

		final BasicBlock startBlock = blocks.get(startBlockIndex - 1);
		final BasicBlock endBlock = blocks.get(endBlockIndex - 1);
		
		incrementIndexes(indexes, isColumnDominant);
		
		final int startPortIndex = (int) fromReal[indexes[0]][indexes[1]];
		final int endPortIndex = (int) toReal[indexes[0]][indexes[1]];

		incrementIndexes(indexes, isColumnDominant);
		
		final boolean startPortIsStart;
		if (canGet(from, indexes)) {
			startPortIsStart = fromReal[indexes[0]][indexes[1]] == 0.0;
		} else {
			startPortIsStart = true;
		}
		
		final boolean endPortIsStart;
		if (canGet(to, indexes)) {
			endPortIsStart = toReal[indexes[0]][indexes[1]] == 0.0;
		} else {
			endPortIsStart = false;
		}
		
		final Class< ? extends BasicPort> startKlass = LinkPortMap.getPortClass(
				link.getClass(), startPortIsStart);
		final Class< ? extends BasicPort> endKlass = LinkPortMap.getPortClass(
				link.getClass(), endPortIsStart);

		start = BasicBlockInfo.getAllTypedPorts(startBlock, false, startKlass)
				.get(startPortIndex - 1);
		end = BasicBlockInfo.getAllTypedPorts(endBlock, false, endKlass)
				.get(endPortIndex - 1);
	}

	/**
	 * Validate the current data.
	 * 
	 * This method doesn't pass the metrics because it perform many test.
	 * Therefore all these tests are trivial and the conditioned action only
	 * throw an exception.
	 * 
	 * @throws ScicosFormatException
	 *             when there is a validation error.
	 */
	// CSOFF: CyclomaticComplexity
	// CSOFF: NPathComplexity
	private void validate() throws ScicosFormatException {
		if (!canDecode(data)) {
			throw new WrongElementException();
		}

		int field = 0;

		// we test if the structure as enough field
		if (data.size() != DATA_FIELD_NAMES.size()) {
			throw new WrongStructureException();
		}

		/*
		 * Checking the MList header
		 */

		// Check the first field
		if (!(data.get(field) instanceof ScilabString)) {
			throw new WrongTypeException();
		}
		final String[] header = ((ScilabString) data.get(field)).getData()[0];

		// Checking for the field names
		if (header.length != DATA_FIELD_NAMES.size()) {
			throw new WrongStructureException();
		}
		for (int i = 0; i < header.length; i++) {
			if (!header[i].equals(DATA_FIELD_NAMES.get(i))) {
				throw new WrongStructureException();
			}
		}

		// xx
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException();
		}

		// yy
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException();
		}

		// id
		field++;
		if (!(data.get(field) instanceof ScilabString)) {
			throw new WrongTypeException();
		}

		// thick
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException();
		}

		// ct
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException();
		}

		// from
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException();
		}

		// to
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException();
		}
	}

	// CSON: CyclomaticComplexity
	// CSON: NPathComplexity

	/**
	 * Test if the current implementation can be used to decode the element.
	 * 
	 * @param element
	 *            the element to test
	 * @return true, when the current implementation is the right one
	 * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.scilabTypes.ScilabType)
	 */
	@Override
	public boolean canDecode(ScilabType element) {
		data = (ScilabMList) element;

		final String type = ((ScilabString) data.get(0)).getData()[0][0];
		return type.equals(DATA_FIELD_NAMES.get(0));
	}

	/**
	 * Encode the instance into the element
	 * 
	 * @param from
	 *            the source instance
	 * @param element
	 *            the previously allocated element.
	 * @return the element parameter
	 * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object,
	 *      org.scilab.modules.types.scilabTypes.ScilabType)
	 */
	@Override
	public ScilabType encode(BasicLink from, ScilabType element) {
		if (element == null) {
			data = new ScilabMList(DATA_FIELD_NAMES.toArray(new String[0]));
		} else {
			data = (ScilabMList) element;
		}

		start = (BasicPort) from.getSource();
		end = (BasicPort) from.getTarget();

		final mxGeometry srcGeom = start.getGeometry();
		final mxGeometry endGeom = end.getGeometry();
		
		// xx and yy
		encodePoints(from, srcGeom, endGeom);

		data.add(new ScilabString("drawlink")); // id

		double[][] thick = {{0, 0}};
		data.add(new ScilabDouble(thick)); // thick

		data.add(new ScilabDouble(from.getColorAndType())); // ct

		double fromBlockID = ((BasicBlock) start.getParent()).getOrdering();
		double fromPortID = start.getOrdering();
		double fromType = LinkPortMap.isStart(start, end);

		double[][] fromData = {{fromBlockID, fromPortID, fromType}};
		data.add(new ScilabDouble(fromData)); // from

		double toBlockID = ((BasicBlock) end.getParent()).getOrdering();
		double toPortID = end.getOrdering();
		double toType = LinkPortMap.isStart(end, start);
		
		double[][] toData = {{toBlockID, toPortID, toType}};
		data.add(new ScilabDouble(toData)); // to

		return data;
	}

	/**
	 * Encode the link points
	 * @param from the source instance
	 * @param srcGeom the source geometry
	 * @param endGeom the target geometry
	 */
	private void encodePoints(BasicLink from, final mxGeometry srcGeom,
			final mxGeometry endGeom) {
		final int ptCount = from.getPointCount();
		final List<mxPoint> lnkPoints = from.getGeometry().getPoints();
		
		double[][] xx = new double[2 + ptCount][1];
		double[][] yy = new double[2 + ptCount][1];
		
		/*
		 * Start point
		 */
		xx[0][0] = srcGeom.getCenterX()	+ start.getParent().getGeometry().getX();
		yy[0][0] = -(srcGeom.getCenterY()
				+ start.getParent().getGeometry().getY() - start.getParent()
				.getGeometry().getHeight());
		
		/*
		 * Control points
		 */
		for (int i = 0; i < ptCount; i++) {
			xx[1 + i][0] = ((mxPoint) lnkPoints.get(i)).getX();
			yy[1 + i][0] = -((mxPoint) lnkPoints.get(i)).getY();
		}
		
		/*
		 * End point
		 */
		xx[1 + ptCount][0] = endGeom.getCenterX() + end.getParent().getGeometry().getX();
		yy[1 + ptCount][0] = -(endGeom.getCenterY()
				+ end.getParent().getGeometry().getY() - end.getParent()
				.getGeometry().getHeight());
		
		data.add(new ScilabDouble(xx));
		data.add(new ScilabDouble(yy));
	}
}
// CSON: ClassDataAbstractionCoupling
