package org.scilab.modules.xcos.simulink;

import org.scilab.modules.xcos.block.BasicBlock;

import edu.tum.cs.simulink.model.SimulinkBlock;

public class BlockSpecificElement {

	public BasicBlock decode(SimulinkBlock from ,BasicBlock into) {
		
		validate();
		
		/*
		 * fill the data
		 */
		//findCorrespondingParameter(string simulinkParameter);
		
		return into;
	}

	private void validate() {
		// TODO Auto-generated method stub
		
	}
}
