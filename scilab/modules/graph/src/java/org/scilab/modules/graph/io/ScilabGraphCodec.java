/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2009 - DIGITEO - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graph.io;

import java.awt.Color;
import java.util.Map;

import org.scilab.modules.graph.ScilabGraph;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxObjectCodec;

/**
 * Default codec for the graph component
 */
public class ScilabGraphCodec extends mxObjectCodec {
	private static final String BACKGROUND = "background";

	/**
	 * Default constructor
	 * @param template the object template
	 */
	public ScilabGraphCodec(Object template) {
		super(template);
	}
	
	/**
	 * The constructor used on for configuration
	 * @param template Prototypical instance of the object to be encoded/decoded.
	 * @param exclude Optional array of fieldnames to be ignored.
	 * @param idrefs Optional array of fieldnames to be converted to/from references.
	 * @param mapping Optional mapping from field- to attributenames.
	 */
	public ScilabGraphCodec(Object template, String[] exclude, String[] idrefs,
			Map<String, String> mapping)
	{
		super(template, exclude, idrefs, mapping);
	}

	/**
	 * Things to do before encoding
	 * @param enc Codec that controls the encoding process.
	 * @param obj Object to be encoded.
	 * @param node XML node to encode the object into.
	 * @return Returns the object to be encoded by the default encoding.
	 * @see com.mxgraph.io.mxObjectCodec#beforeEncode(com.mxgraph.io.mxCodec, java.lang.Object, org.w3c.dom.Node)
	 */
	public Object beforeEncode(mxCodec enc, Object obj, Node node) {
		((Element) node).setAttribute(BACKGROUND,
				String.valueOf(((ScilabGraph) obj).getAsComponent().getBackground().getRGB()));
		return super.beforeEncode(enc, obj, node);
	}

	/**
	 * Apply compatibility pattern to the decoded object
	 * @param dec Codec that controls the decoding process.
	 * @param node XML node to decode the object from.
	 * @param obj Object decoded.
	 * @return The Object transformed 
	 * @see org.scilab.modules.xcos.io.XcosObjectCodec#afterDecode(com.mxgraph.io.mxCodec, org.w3c.dom.Node, java.lang.Object)
	 */
	public Object afterDecode(mxCodec dec, Node node, Object obj) {
		
		((ScilabGraph) obj).getAsComponent().setBackground((new Color(
				Integer.parseInt((((Element) node).getAttribute(BACKGROUND))))));
		return super.afterDecode(dec, node, obj);
	}
	
	
}
