package org.scilab.modules.xcos.simulink;

import java.io.IOException;
import java.util.Iterator;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.port.BasicPort;

import edu.tum.cs.simulink.model.SimulinkBlock;
import edu.tum.cs.simulink.model.SimulinkInPort;
import edu.tum.cs.simulink.model.SimulinkLine;

public class InputPortElement {

	private static final Log LOG = LogFactory.getLog(InputPortElement.class);
	
	public InputPortElement(SimulinkBlock base) {
		// TODO Auto-generated constructor stub
	}

	public BasicPort decode(SimulinkInPort simulinkInPort, Object object) {
		// TODO Auto-generated method stub
		if (LOG.isTraceEnabled()) {
			LogFactory.getLog(InputPortElement.class).trace("InPort" + simulinkInPort.toString());
		}
		/*
		 * Set in line
		 */
		
		LineElement lineElement = new LineElement();
		lineElement.decode(simulinkInPort.getLine());
		
		return null;
	}
}
