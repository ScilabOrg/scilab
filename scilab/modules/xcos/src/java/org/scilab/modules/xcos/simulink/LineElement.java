package org.scilab.modules.xcos.simulink;

import java.io.IOException;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.link.BasicLink;

import edu.tum.cs.simulink.model.SimulinkLine;

public class LineElement extends AbstractElement<BasicLink>{

	public void decode(SimulinkLine simulinkLine) {
		// TODO Auto-generated method stub
		LogFactory.getLog(LineElement.class).trace("Line: " + simulinkLine.toString());
	}

}
