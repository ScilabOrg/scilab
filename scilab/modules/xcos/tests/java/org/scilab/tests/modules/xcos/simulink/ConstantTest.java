/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Jerzy ZAGORSKI
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.tests.modules.xcos.simulink;

import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.XcosTab;
import org.scilab.modules.types.ScilabDouble;

import org.testng.annotations.*;


/**
 * Test of the {@link ConstantTest} block importation.
 */
public class ConstantTest {

	/**
	 * Initialization of diagram needed by tests
	 */
	@BeforeClass
	public void setUp() {
		/*
		 * should be some const value
		 */
		String XcosTest =  System.getenv("SCI") + "/modules/xcos/tests/java/org/scilab/tests/modules/xcos/simulink/";
		Xcos.simulinkImport(XcosTest + "ConstantTest.mdl");
		assert XcosTab.getAllDiagrams().size() == 1;
	}
	
	@Test
	public void checkSimulationFunctionName() {
		XcosDiagram xcosDiagram = XcosTab.getAllDiagrams()[0].getDiagrams().get(0);
		BasicBlock toVerify = (BasicBlock) xcosDiagram.getModel().getChildAt(xcosDiagram.getDefaultParent(), 0);
		
		assert toVerify.getSimulationFunctionName().equals("cstblk4");
	}
	@Test
	public void checkSimulationFunctionType() {
		XcosDiagram xcosDiagram = XcosTab.getAllDiagrams()[0].getDiagrams().get(0);
		BasicBlock toVerify = (BasicBlock) xcosDiagram.getModel().getChildAt(xcosDiagram.getDefaultParent(), 0);
		
		assert toVerify.getSimulationFunctionName() == 4;
	}
	@Test
	public void checkGraphics() {
		
	}
	@Test
	public void checkExprs() {
		
	}
	@Test
	public void checkState() {
		
	}
	@Test
	public void checkDState() {
		
	}
	@Test
	public void checkODState() {
		
	}
	@Test
	public void checkRealParameters() {
		XcosDiagram xcosDiagram = XcosTab.getAllDiagrams()[0].getDiagrams().get(0);
		BasicBlock toVerify = (BasicBlock) xcosDiagram.getModel().getChildAt(xcosDiagram.getDefaultParent(), 0);
		
		assert toVerify.getRealParameters() == ScilabDouble(1);
	}
	@Test
	public void checkIntegerParameters() {
		
	}
	@Test
	public void checkObjectParameters() {
		
	}
	@Test
	public void checkDependsOnT() {
		
	}
	@Test
	public void checkNbZeroCrossing() {
		
	}
	@Test
	public void checkNmode() {
		
	}
	@Test
	public void checkEquations() {
		
	}
	@Test
	public void checkInterfaceFunctionName() {
		XcosDiagram xcosDiagram = XcosTab.getAllDiagrams()[0].getDiagrams().get(0);
		BasicBlock toVerify = (BasicBlock) xcosDiagram.getModel().getChildAt(xcosDiagram.getDefaultParent(), 0);
		
		assert toVerify.getInterfaceFunctionName().equals("CONST_m");
	}
	
	/**
	 * Clean up after tests are launched, and test if Xcos diagrams were closed correctly
	 */
	@AfterClass
	public void cleanUp() {
		Xcos.closeSession();
	
		assert !PaletteManager.isVisible();
		assert XcosTab.getAllDiagrams().size() == 0;
	}
}
