/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.scicos;

import java.util.Map;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.ui_data.UI_data;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.scicos.ScicosFormatException.VersionMismatchException;

public class ScicosTextHandler implements Handler {
	private final String strVarName;

	/**
	 * Create a unique string var name
	 * @param o the object to emit a var name for
	 * @return a unique temporary var name
	 */
	public static String createVarName(final Object o) {
		final StringBuilder str = new StringBuilder();
		
		str.append("scs_m");
		str.append(o.hashCode());
		
		return str.toString();
	}
	
	
	public ScicosTextHandler(final String strVarName) {
		this.strVarName = strVarName;
	}

	@Override
	public BasicBlock readBlock() throws ScicosFormatException {
		return null;
	}

	@Override
	public BasicBlock readBlock(BasicBlock into) throws ScicosFormatException {
		return null;
	}

	@Override
	public Map<String, String> readContext() {
		return null;
	}

	@Override
	public XcosDiagram readDiagram() throws VersionMismatchException {
		return null;
	}

	@Override
	public XcosDiagram readDiagram(XcosDiagram instance) {
		return null;
	}

	@Override
	public void writeBlock(BasicBlock block) {
		final BlockElement element = new BlockElement();
		final ScilabType data = element.encode(block, null);

		UI_data.putScilabVariable(strVarName,
				new String[] { data.toString() }, 1, 1);
	}

	@Override
	public void writeContext(String[] context) {
		UI_data.putScilabVariable(strVarName,
				context, 1, 1);
	}

	@Override
	public void writeDiagram(XcosDiagram diagram) {
		final DiagramElement element = new DiagramElement();
		final ScilabType data = element.encode(diagram, null);

		UI_data.putScilabVariable(strVarName,
				new String[] { data.toString() }, 1, 1);
	}
}
