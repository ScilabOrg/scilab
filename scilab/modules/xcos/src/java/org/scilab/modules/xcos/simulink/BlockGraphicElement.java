package org.scilab.modules.xcos.simulink;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.utils.BlockPositioning;

import edu.tum.cs.simulink.model.SimulinkBlock;

public class BlockGraphicElement{
	
	private static final Log LOG = LogFactory.getLog(BlockGraphicElement.class);
	
	public BasicBlock decode(SimulinkBlock from ,BasicBlock into) {
		
		validate();
		
		/*
		 * fill the data
		 */
		fillDimension(from, into);
		fillOrigin(from, into);
		fillFlipAndRotation(from, into);
		
		return into;
	}

	private void fillFlipAndRotation(SimulinkBlock from, BasicBlock into) {
		// TODO: Check if compatibility pattern needed
		// TODO: Add Flip handling
		
		String rotation = from.getParameter("BlockRotation");
		int theta = Integer.parseInt(rotation);
		theta = BlockPositioning.roundAngle(theta);
		
		into.setAngle(theta);
	}

	private void fillOrigin(SimulinkBlock from, BasicBlock into) {
		// TODO: Check if compatibility pattern needed
		String[] position = from.getParameter("Position").replaceAll("\\W", " ").trim().split("\\s+");
		if (LOG.isTraceEnabled()) {
			LOG.trace(Integer.toString(position.length));
			for(int i = 0; i < position.length ; i++){
				LOG.trace(position[i]);
			}
		}
		double x = Double.parseDouble(position[0]);
		double y = Double.parseDouble(position[1]);
		
		into.getGeometry().setX(x);
		into.getGeometry().setY(y);
	}

	private void fillDimension(SimulinkBlock from, BasicBlock into) {
		// TODO: Check if compatibility pattern needed
		String[] position = from.getParameter("Position").replaceAll("\\W", " ").trim().split("\\s+");
		double w = Double.parseDouble(position[2]) - Double.parseDouble(position[0]);
		double h = Double.parseDouble(position[3]) - Double.parseDouble(position[1]);
		
		into.getGeometry().setWidth(w);
		into.getGeometry().setHeight(h);
	}

	private void validate() {
		// TODO Auto-generated method stub
		
	}
}
