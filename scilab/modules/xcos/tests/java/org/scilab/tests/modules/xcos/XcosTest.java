/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.tests.modules.xcos;

import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.testng.annotations.Test;

/**
 * Test of the {@link XcosTest} class.
 */
public class XcosTest {

	/**
	 * Be careful when modifying the tradename and version. 
	 */
	@Test
	public void checkVersion() {
		assert Xcos.TRADENAME.compareTo("Xcos") == 0;
		assert Xcos.VERSION.compareTo("1.0") == 0;
	}
	
	@Test
	public void launchWithoutFilename() {
		Xcos.xcos();
		
		assert PaletteManager.isVisible();
		assert XcosTab.getAllDiagrams().size() == 1;
		
		Xcos.closeSession();
		
		assert !PaletteManager.isVisible();
		assert XcosTab.getAllDiagrams().size() == 0;
	}
	
}
