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

package org.scilab.modules.commons;

import java.io.File;
import java.io.IOException;
import java.net.URI;

/**
 * Implement extra methods from fileio
 */
public final class ScilabFile extends File {
	/**
	 * Convert a file to a shortname
	 * @param f the file
	 * @return the shortname
	 */
	public static String convertToShortName(final String f) {
		String ret;
		
		try {
			String path = f;
			int[] status = {1};
			
			path = ScilabCommons.getshortpathname(path, status);
			if (status[0] != 0) {
				throw new IOException("getshortpathname: error");
			}
			
			ret = path;
		} catch (IOException e) {
			ret = null;
		}
		
		return ret;
	}
	
	/**
	 * {@inheritDoc}
	 */
	public ScilabFile(File parent, String child) {
		super(parent, child);
	}

	/**
	 * {@inheritDoc}
	 */
	public ScilabFile(String parent, String child) {
		super(parent, child);
	}

	/**
	 * {@inheritDoc}
	 */
	public ScilabFile(String pathname) {
		super(pathname);
	}

	/**
	 * {@inheritDoc}
	 */
	public ScilabFile(URI uri) {
		super(uri);
	}
}
