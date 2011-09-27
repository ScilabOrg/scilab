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

package org.scilab.modules.xcos.codegeneration;

import javax.xml.bind.annotation.XmlRegistry;

/**
 * This object contains factory methods for each Java content interface and Java
 * element interface generated in the
 * org.scilab.modules.xcos.configuration.model package.
 * <p>
 * An ObjectFactory allows you to programatically construct new instances of the
 * Java representation for XML content. The Java representation of XML content
 * can consist of schema derived interfaces and classes representing the binding
 * of schema type definitions, element declarations and model groups. Factory
 * methods for each of these are provided in this class.
 * 
 */
@XmlRegistry
public class ObjectFactory {

	/**
	 * Create a new ObjectFactory that can be used to create new instances of
	 * schema derived classes for package:
	 * org.scilab.modules.xcos.codegeneration
	 * 
	 */
	public ObjectFactory() {
	}

	/**
	 * Create an instance of {@link createCodegenerationTarget }
	 * 
	 * @return a new instance
	 */
	public CodegenerationTarget createCodegenerationTarget() {
		return new CodegenerationTarget();
	}
	
	/**
	 * Create an instance of {@link Options }
	 * 
	 * @return a new instance
	 */
	public Options createOptions() {
		return new Options();
	}
	
	/**
	 * Create an instance of {@link Option }
	 * @return a new instance
	 */
	public Option createOption() {
		return new Option();
	}
}
