/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 * Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.tests.modules.commons;

import org.scilab.modules.commons.ScilabConstants;
import org.testng.annotations.Test;

/**
 * Check the access of the {@link ScilabConstants} public fields using dynamic
 * invocation.
 */
public class Constants {
	
	@Test
	public void checkTmp() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		assert ScilabConstants.class.getDeclaredField("TMPDIR").get(null) != null;
	}
	
	@Test
	public void checkSci() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		assert ScilabConstants.class.getDeclaredField("SCI").get(null) != null;
	}
	
	@Test
	public void checkSciHome() throws SecurityException, NoSuchFieldException, IllegalArgumentException, IllegalAccessException {
		assert ScilabConstants.class.getDeclaredField("SCIHOME").get(null) != null;
	}
}
