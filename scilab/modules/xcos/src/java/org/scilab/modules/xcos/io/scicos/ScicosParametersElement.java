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

import java.beans.PropertyVetoException;
import java.util.List;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabTList;
import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;

/**
 * Default element used to handle Scicos simulation parameters 
 */
public class ScicosParametersElement extends AbstractElement<ScicosParameters> {
	private static final List<String> DATA_FIELD_NAMES = asList(
			"params", "wpar", "title", "tol", "tf",
			"context", "void1", "options", "void2", "void3", "doc");
	private static final int TOL_INDEX = 3;
	private static final int TF_INDEX = 4;
	private static final int CONTEXT_INDEX = 5;
	
	/**
	 * Mutable field which contains the current working data. 
	 * 
	 * This field must be modified on each read/write call.
	 */
	private ScilabTList data;

	/**
	 * Default constructor
	 */
	public ScicosParametersElement() { }

	/**
	 * Decode the element into the instance
	 * 
	 * @param element the element to be decoded
	 * @param into the Xcos associated instance
	 * @return the modified into parameters
	 * @throws ScicosFormatException on decode error
	 * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.scilabTypes.ScilabType, java.lang.Object)
	 */
	@Override
	public ScicosParameters decode(ScilabType element, ScicosParameters into)
			throws ScicosFormatException {
		data = (ScilabTList) element;
		
		// Validate the fields
		validate();
		
		/*
		 * fill data
		 */
		
		fillWithThirdFields(into);
		
		try {
			into.setFinalIntegrationTime(((ScilabDouble) data.get(TF_INDEX)).getRealPart()[0][0]);
		} catch (PropertyVetoException e) {
			LogFactory.getLog(ScicosParametersElement.class).error(e);
		}
		
		fillContext(into);
		
		return into;
	}

	/**
	 * Validate the current data.
	 * 
	 * This method doesn't pass the metrics because it
	 * perform many test. Therefore all these tests are trivial and the
	 * conditioned action only throw an exception.
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
		 * Checking the TList header
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
		
		/*
		 * Checking the data
		 */
		
		// wpar
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException();
		}
		
		// title
		field++;
		if (!(data.get(field) instanceof ScilabString)) {
			throw new WrongTypeException();
		}
		
		// tol
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException();
		}
		
		// tf
		field++;
		if (!(data.get(field) instanceof ScilabDouble)) {
			throw new WrongTypeException();
		}
		
		// context
		field++;
		if (!(data.get(field) instanceof ScilabString)
				&& !isEmptyField(data.get(field))) {
			throw new WrongTypeException();
		}
		
		// void1
		field++;
		if (!isEmptyField(data.get(field))) {
			throw new WrongTypeException();
		}

		// options
		field++;
		if (!(data.get(field) instanceof ScilabTList)) {
			throw new WrongTypeException();
		}

		// void2
		field++;
		if (!isEmptyField(data.get(field))) {
			throw new WrongTypeException();
		}

		// void3
		field++;
		if (!isEmptyField(data.get(field))) {
			throw new WrongTypeException();
		}

		// doc
		field++;
		if (!(data.get(field) instanceof ScilabList)
				&& !isEmptyField(data.get(field))) {
			throw new WrongTypeException();
		}
	}
	// CSON: CyclomaticComplexity
	// CSON: NPathComplexity
	
	/**
	 * Fill the object with the data third field
	 * @param into the current object to put data into.
	 */
	private void fillWithThirdFields(ScicosParameters into) {
		final boolean isColumnDominant = 
			data.get(TOL_INDEX).getHeight() >= data.get(TOL_INDEX).getWidth();
		final double[][] realPart = ((ScilabDouble) data.get(TOL_INDEX)).getRealPart();
		
		// global index used to get the data
		int[] indexes = {0, 0};
		
		try {
			into.setIntegratorAbsoluteTolerance(realPart[indexes[0]][indexes[1]]);
		} catch (PropertyVetoException e) {
			LogFactory.getLog(ScicosParametersElement.class).error(e);
		}
		
		incrementIndexes(indexes, isColumnDominant);
		
		try {
			into.setIntegratorRelativeTolerance(realPart[indexes[0]][indexes[1]]);
		} catch (PropertyVetoException e) {
			LogFactory.getLog(ScicosParametersElement.class).error(e);
		}
		
		incrementIndexes(indexes, isColumnDominant);
		
		try {
			into.setToleranceOnTime(realPart[indexes[0]][indexes[1]]);
		} catch (PropertyVetoException e) {
			LogFactory.getLog(ScicosParametersElement.class).error(e);
		}
		
		incrementIndexes(indexes, isColumnDominant);
		
		try {
			into.setMaxIntegrationTimeInterval(realPart[indexes[0]][indexes[1]]);
		} catch (PropertyVetoException e) {
			LogFactory.getLog(ScicosParametersElement.class).error(e);
		}
		
		incrementIndexes(indexes, isColumnDominant);
		
		try {
			into.setRealTimeScaling(realPart[indexes[0]][indexes[1]]);
		} catch (PropertyVetoException e) {
			LogFactory.getLog(ScicosParametersElement.class).error(e);
		}
		
		incrementIndexes(indexes, isColumnDominant);
		
		try {
			into.setSolver(realPart[indexes[0]][indexes[1]]);
		} catch (PropertyVetoException e) {
			LogFactory.getLog(ScicosParametersElement.class).error(e);
		}
		
		incrementIndexes(indexes, isColumnDominant);
		
		// Some times the maximum step size may not exist. Catch it.
		try {
			into.setMaximumStepSize(realPart[indexes[0]][indexes[1]]);
		} catch (PropertyVetoException e1) {
			LogFactory.getLog(ScicosParametersElement.class).error(e1);
		} catch (ArrayIndexOutOfBoundsException e) {
			// do nothing as the maximum step size will keep its default value.
			LogFactory.getLog(ScicosParametersElement.class).info(e);
		}
	}
	
	/**
	 * Fill the object with the context (5th) field
	 * @param into the current object to put data into.
	 */
	private void fillContext(ScicosParameters into) {
		final boolean isColumnDominant = 
			data.get(CONTEXT_INDEX).getHeight() >= data.get(CONTEXT_INDEX).getWidth();
		final String[][] str = ((ScilabString) data.get(CONTEXT_INDEX))
				.getData();
		final int length = data.get(CONTEXT_INDEX).getHeight()
				+ data.get(CONTEXT_INDEX).getWidth() - 1;
		
		String[] context = new String[length];
		
		int[] indexes = {0, 0};
		for (int i = 0; i < length; i++) {
			context[i] = str[indexes[0]][indexes[1]];
			incrementIndexes(indexes, isColumnDominant);
		}
		
		try {
			into.setContext(context);
		} catch (PropertyVetoException e) {
			LogFactory.getLog(ScicosParametersElement.class).error(e);
		}
	}
	
	/**
	 * Test if the element can be decoded with this instance implementation.
	 * 
	 * @param element the element 
	 * @return true when the implementation can be used to decode the element.
	 * @see org.scilab.modules.xcos.io.scicos.Element#canDecode(org.scilab.modules.types.scilabTypes.ScilabType)
	 */
	@Override
	public boolean canDecode(ScilabType element) {
		data = (ScilabTList) element;
		
		final String type = ((ScilabString) data.get(0)).getData()[0][0];
		return type.equals(DATA_FIELD_NAMES.get(0));
	}
	
	/**
	 * Encode the instance into the element
	 * 
	 * @param from the source instance
	 * @param element the previously allocated element.
	 * @return the element parameter
	 * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object, org.scilab.modules.types.scilabTypes.ScilabType)
	 */
	@Override
	public ScilabType encode(ScicosParameters from, ScilabType element) {
		if (element == null) {
			throw new IllegalArgumentException();
		}
		
		data = (ScilabTList) element;
		
		/*
		 * fill the tol field
		 */
		double[][] tolField = {
				{from.getIntegratorAbsoluteTolerance()},
				{from.getIntegratorRelativeTolerance()},
				{from.getToleranceOnTime()},
				{from.getMaxIntegrationTimeInterval()},
				{from.getRealTimeScaling()},
				{from.getSolver()},
				{from.getMaximumStepSize()}
		};
		ScilabDouble scilabTolField = new ScilabDouble(tolField);
		data.set(TOL_INDEX, scilabTolField);
		
		/*
		 * fill the tf field
		 */
		data.set(TF_INDEX, new ScilabDouble(from.getFinalIntegrationTime()));
		
		/*
		 * fill the context
		 */
		data.set(CONTEXT_INDEX, new ScilabString(from.getContext()));
		
		return data;
	}
}
