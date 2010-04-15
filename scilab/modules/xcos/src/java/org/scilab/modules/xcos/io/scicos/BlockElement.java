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

import java.util.List;

import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabMList;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BlockFactory;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongElementException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongStructureException;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.WrongTypeException;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

/**
 * Perform a block transformation between Scicos and Xcos.
 */
//CSOFF: ClassDataAbstractionCoupling
public class BlockElement extends AbstractElement<BasicBlock> {
	private static final List<String> DATA_FIELD_NAMES = asList(
			"Block", "graphics", "model", "gui", "doc");
	private static final int INTERFUNCTION_INDEX = 3;
	
	/** Mutable field to easily get the data through methods */ 
	private ScilabMList data;

	/** Element used to decode/encode Scicos model part into a BasicBlock*/
	private BlockModelElement modelElement = new BlockModelElement();
	
	/** Element used to decode/encode Scicos model part into a BasicBlock*/
	private BlockGraphicElement graphicElement = new BlockGraphicElement();
	
	/*
	 * Decoder state 
	 */
	
	/**
	 * Current block ordering, the ordering change on each {@link BlockElement}
	 * instance so be careful when allocated a new {@link BlockElement}.
	 */
	private int ordering;
	
	/**
	 * The minimal X axis value, used to center block as Scicos did.
	 */
	private double minX = Double.MAX_VALUE;
	
	/**
	 * The minimal Y axis value, used to center block as Scicos did.
	 */
	private double minY = Double.MAX_VALUE; 

	/**
	 * Default constructor.
	 * 
	 * The state change on each {@link BlockElement} instance so be careful
	 * when allocated a new {@link BlockElement}.
	 */
	public BlockElement() { }
	
	/**
	 * @return the minX
	 */
	public double getMinX() {
		return minX;
	}
	
	/**
	 * @return the minY
	 */
	public double getMinY() {
		return minY;
	}
	
	/**
	 * Decode the element into the block.
	 * 
	 * @param element The current Scilab data
	 * @param into the target, if null a new instance is allocated and returned.
	 * @return the decoded block.
	 * @throws ScicosFormatException when e decoding error occurred.
	 * @see org.scilab.modules.xcos.io.scicos.Element#decode(org.scilab.modules.types.scilabTypes.ScilabType, java.lang.Object)
	 */
	@Override
	public BasicBlock decode(ScilabType element, BasicBlock into) throws ScicosFormatException {
		data = (ScilabMList) element;
		BasicBlock block = into;
		
		validate();
		
		/*
		 * Instantiate the block if it doesn't exist
		 */
		final String interfunction = ((ScilabString) data
				.get(INTERFUNCTION_INDEX)).getData()[0][0];
		if (block == null) {
			block = BlockFactory.createBlock(interfunction);
		}
		
		/*
		 * Fill block with the data structure
		 */
		int field = 1;
		graphicElement.decode(data.get(field), block);
		
		field++;
		modelElement.decode(data.get(field), block);
		
		field++;
		block.setInterfaceFunctionName(interfunction);
		
		field++;
		fillDocStructure(data.get(field), block);
		
		/*
		 * Allocate and setup ports
		 */
		InputPortElement inElement = new InputPortElement(data);
		final int numberOfInputPorts = inElement.getNumberOfInputPort();
		for (int i = 0; i < numberOfInputPorts; i++) {
			block.addPort(inElement.decode(data, null));
		}
		
		OutputPortElement outElement = new OutputPortElement(data);
		final int numberOfOutputPorts = outElement.getNumberOfOutputPort();
		for (int i = 0; i < numberOfOutputPorts; i++) {
			block.addPort(outElement.decode(data, null));
		}
		
		/*
		 * Set state dependent informations.
		 */
		block.setOrdering(ordering);
		ordering++;
		
		minX = Math.min(minX, block.getGeometry().getX());
		minY = Math.min(minX, block.getGeometry().getY());
		
		return block;
	}
	
	/**
	 * Use the Scicos documentation structure to get the previous Xcos IDs.  
	 * 
	 * @param scilabType the scicos documentation field.
	 * @param into the target instance.
	 */
	private void fillDocStructure(ScilabType scilabType, BasicBlock into) {
		/*
		 * The double type is used as the default one, generate on empty field.
		 */
		if (scilabType instanceof ScilabDouble) {
			into.generateId();
			return;
		}
		
		/*
		 * Classical behavior
		 */
		ScilabList list = (ScilabList) scilabType;
		
		if (list.size() > 0) {
			if (list.get(0) instanceof ScilabString) {
				ScilabString uid = (ScilabString) list.get(0);
				into.setId(uid.getData()[0][0]);
			}
		} else {
			into.generateId();
		}
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
		
		/*
		 * Checking the data
		 */
		
		// the second field must contain list of all graphic property (how the
		// block will be displayed )
		field++;
		if (!(data.get(field) instanceof ScilabMList)) {
			throw new WrongTypeException();
		}
		
		// the third field must contains all the informations needed to compile
		// the block
		field++;
		if (!(data.get(field) instanceof ScilabMList)) {
			throw new WrongTypeException();
		}
		
		// the fourth field must contains all the informations needed to represent
		// the block
		field++;
		if (!(data.get(field) instanceof ScilabString)) {
			throw new WrongTypeException();
		}
		
		// the last field must contain a list of nothing aka scicos doc
		field++;
		if (!(data.get(field) instanceof ScilabList)
				&& !isEmptyField(data.get(field))) {
			throw new WrongTypeException();
		}
	}
	// CSON: CyclomaticComplexity
	// CSON: NPathComplexity

	/**
	 * Test if the current instance can be used to decode the element
	 * 
	 * @param element the current element
	 * @return true, if the element can be decoded, false otherwise
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
	 * @param from the source instance
	 * @param element the previously allocated element.
	 * @return the element parameter
	 * @see org.scilab.modules.xcos.io.scicos.Element#encode(java.lang.Object, org.scilab.modules.types.scilabTypes.ScilabType)
	 */
	@Override
	public ScilabType encode(BasicBlock from, ScilabType element) {
		data = (ScilabMList) element;
		
		int field = 0;
		ScilabType base;
		
		if (data == null) {
			data = allocateElement();
			setupPortSize(from);
		}
		
		field++;
		base = data.get(field);
		base = graphicElement.encode(from, null);
		data.set(field, base);
		
		field++;
		base = data.get(field);
		base = modelElement.encode(from, null);
		data.set(field, base);
		
		field++;
		base = new ScilabString(from.getInterfaceFunctionName());
		data.set(field, base);
		
		field++;
		base = data.get(field);
		((ScilabList) base).add(new ScilabString(from.getId()));
		
		/*
		 * Encoding the InputPorts and OutputPorts using there own elements
		 */
		final InputPortElement inElement = new InputPortElement(data);
		final OutputPortElement outElement = new OutputPortElement(data);
		final int numberOfPorts = from.getChildCount();
		
		for (int i = 0; i < numberOfPorts; i++) {
			final Object instance = from.getChildAt(i);
			
			if (instance instanceof InputPort) {
				inElement.encode((InputPort) instance, data);
			} else if (instance instanceof OutputPort) {
				outElement.encode((OutputPort) instance, data);
			}
		}
		
		return data;
	}

	/**
	 * Set the the port size per type.
	 * 
	 * @param from the source block
	 */
	private void setupPortSize(BasicBlock from) {
		// Getting children size per type.
		int in = BasicBlockInfo.getAllTypedPorts(from, false, InputPort.class).size();
		int out = BasicBlockInfo.getAllTypedPorts(from, false, OutputPort.class).size();
		int ein = BasicBlockInfo.getAllTypedPorts(from, false, ControlPort.class).size();
		int eout = BasicBlockInfo.getAllTypedPorts(from, false, CommandPort.class).size();
		
		// Setup the graphics and model ports size
		graphicElement.setPortsSize(in, out, ein, eout);
		modelElement.setPortsSize(in, out, ein, eout);
	}

	/**
	 * Allocate a new element
	 * @return the new element
	 */
	private ScilabMList allocateElement() {
		ScilabMList element = new ScilabMList(DATA_FIELD_NAMES.toArray(new String[0]));
		element.add(new ScilabMList()); // graphics
		element.add(new ScilabMList()); // model
		element.add(new ScilabString()); // gui
		element.add(new ScilabList()); // doc
		return element;
	}
}
//CSON: ClassDataAbstractionCoupling
