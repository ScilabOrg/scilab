/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.javasci;

import java.io.File;

import org.scilab.modules.types.scilabTypes.ScilabType;
import org.scilab.modules.types.scilabTypes.ScilabTypeEnum;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabBoolean;
import org.scilab.modules.types.scilabTypes.ScilabInteger;
import org.scilab.modules.javasci.Call_Scilab;
import org.scilab.modules.javasci.JavasciException.InitializationException;

public class Scilab {
    private String SCI = null;
    

	/**
	 * Creator of the Scilab Javasci object. 
	 * Under GNU/Linux / Mac OS X, try to detect Scilab base path
	 * if the property SCI is set, use it
	 * if not, try with the global variable SCI
	 * if not, throws a new exception
	 * Under Windows, use also the registery
	 * @param SCI provide the path to Scilab data
	 */
    public Scilab() throws InitializationException {
		// Auto detect 
		String detectedSCI;
        try {
			detectedSCI = System.getProperty("SCI");
			if (detectedSCI == null || detectedSCI.length() == 0) {
				detectedSCI = System.getenv("SCI");
				if (detectedSCI == null || detectedSCI.length() == 0) {
					throw new InitializationException("Auto detection of SCI failed.\nSCI empty.");
				}
			}
			this.initScilab(detectedSCI);

        } catch (Exception e) {
			throw new InitializationException("Auto detection of SCI failed.\nCould not retrieve the variable SCI.", e);
        }
		// @TODO manage windows through the registery
    }

	/**
	 * Creator of the Scilab Javasci object. 
	 * @param SCI provide the path to Scilab data
	 */
    public Scilab(String SCI) throws InitializationException {
		this.initScilab(SCI);
    }

	private void initScilab(String SCI) throws InitializationException {
		File f = new File(SCI);
		if (!f.isDirectory()) { 
			throw new InitializationException("Could not find directory " + f.getAbsolutePath());
		}

		this.SCI = SCI;
	}

    /**
     * Open a connection to the Scilab engine
     * This function is based on Call_ScilabOpen from call_scilab
	 * Note: For now, only one instance of Scilab can be launched
	 * A second launch will return FALSE
     * @return if the operation is successful
     */
    public boolean open() throws InitializationException {
//        System.out.println("SCI : " + SCI);
		int res = Call_Scilab.Call_ScilabOpen(this.SCI, null, null);
		switch (res) {
			case -1: 
				/* @TODO : update this exception for a new one (already running) */
				System.err.println("Javasci already running");
				throw new InitializationException("Javasci already running");
			case -2:
				/* Should not occurd (processed before) */
				throw new InitializationException("Could not find SCI");
			case -3:
				throw new InitializationException("No existing directory");
		}
		return true;
		//        return Call_Scilab.Call_ScilabOpen(this.SCI, null, null) == 0;
    }

    /**
     * Open a connection to the Scilab engine and run the command job
     * This function is based on Call_ScilabOpen from call_scilab
	 * Note: For now, only one instance of Scilab can be launched
	 * A second launch will return FALSE
     * @param job The job to run on startup
     * @return if the operation is successful
     */
    public boolean open(String job) throws InitializationException {
        if (!this.open()) {
			return false;
		}

        return this.exec(job);
    }

    /**
     * Open a connection to the Scilab engine and run commands job
     * This function is based on Call_ScilabOpen from call_scilab
	 * Note: For now, only one instance of Scilab can be launched
	 * A second launch will return FALSE
     * @param jobs The serie of jobs to run on startup
     * @return if the operation is successful
     */
    public boolean open(String jobs[]) throws InitializationException {
        if (!this.open()) {
			return false;
		}

        return this.exec(jobs);
    }


    /**
     * Open a connection to the Scilab engine and run thefile scriptFilename
     * This function is based on Call_ScilabOpen from call_scilab
	 * Note: For now, only one instance of Scilab can be launched
	 * A second launch will return FALSE
     * @param job The script to execute on startup
     * @return if the operation is successful
     */
    public boolean open(File scriptFilename) throws InitializationException {
        if (!this.open()) {
			return false;
		}

        return this.exec(scriptFilename);
    }


    /**
     * Execute a single command in Scilab
     * This function is based on SendScilabJob from call_scilab
     * @param job the job to execute
     * @return if the operation is successful
     */
    public boolean exec(String job) {
        return (Call_Scilab.SendScilabJob(job) == 0);
    }


    /**
     * Execute several commands in Scilab
     * This function is based on SendScilabJob from call_scilab
     * @param job the serie of job to execute
     * @return if the operation is successful
     */
    public boolean exec(String jobs[]) {
        return (Call_Scilab.SendScilabJobs(jobs, jobs.length) == 0);
    }


    /**
     * Execute a Scilab script .sce/.sci
     * This function is based on SendScilabJob from call_scilab
     * @param job the script to execute
     * @return if the operation is successful
     */
    public boolean exec(File scriptFilename) {
		// @TODO: rajouter le check du fichier et lancer une nosuch file exception
        return this.exec("exec('" + scriptFilename + "');");
    }


    /**
     * Detect if a variable (varname) exists in Scilab
     * @param varname the variable to check
     * @return if the variable exists or not
     */
    public boolean isExistingVariable(String varname) {
        return true;
    }


    /**
     * Shutdown Scilab
     * This function is based on TerminateScilab from call_scilab
     * @return if the operation is successful
     */
    public boolean close() {
        return Call_Scilab.TerminateScilab(null);
    }


    /**
     * Return the last error code
     * @return the error code
     * @todo Check what is actually this number
     */
    public int getLastErrorCode() {
        return Call_Scilab.GetLastErrorCode();
    }


    /**
     * Return the last error message
     * @return the error message itself
     */
    String getLastErrorMessage() {
        return "getLastErrorMessage";
    }


    /**
     * Detect if a Scilab graphic window is still opened
     * This function is based on ScilabHaveAGraph from call_scilab
     * @return if the graphic is open or not
     */
    public boolean isGraphicOpened() {
        return false;
    }


    /**
     * Return the code type of a variable varname
     * The int refers to a Java enum from ScilabType.types
     * @param varname the name of the variable
     * @return the type of the variable
     * @todo check the enum here
    */
    public ScilabTypeEnum getVariableType(String varName) {
        ScilabTypeEnum variableType = null;
        try {
            variableType = Call_Scilab.getVariableType(varName);
        } catch (IllegalArgumentException e) {
            System.err.println("Could not find variable type: " + e.getLocalizedMessage());
        }

//        ScilabTypeEnum typeEnum = ScilabTypeEnum.swigToEnum(variableType);
        return variableType;
        
//        return typeEnum;

//        int varType = 
//        System.out.println(ScilabTypeEnum(varType));

    }




    /**
     * Returns a variable named varname
     * Throws an exception if the datatype is not managed or if the variable is not available
     * @param varname the name of the variable
     * @return return the variable 
    */
    public ScilabType get(String varname) {
		ScilabTypeEnum sciType = this.getVariableType(varname);
		switch (sciType) {
			case sci_matrix:
				return new ScilabDouble(Call_Scilab.getDouble(varname));
			case sci_boolean:
				return new ScilabBoolean(Call_Scilab.getBoolean(varname));
			default:
		//		throw new UnsupportedTypeException();
		}
		return null;
    }


    /**
     * Send to Scilab a variable theVariable named varname
     * Throws an exception if the datatype is not managed or if the variable is not available
     * @param varname the name of the variable
     * @param varname the variable itself
     * @return if the operation is successful
    */
    public boolean put(String varname, ScilabType theVariable) {
        if (theVariable instanceof ScilabDouble) {
            ScilabDouble sciDouble = (ScilabDouble)theVariable;
            if (sciDouble.isReal()) {
                Call_Scilab.putDouble(varname, sciDouble.getRealPart());
            } else {
				//                Call_Scilab.putDoubleComplex(varname,sciDouble.getRealPart(), sciDouble.getImaginaryPart());
            }
		}
		if (theVariable instanceof ScilabInteger) {
//			Call_Scilab.putInteger((ScilabInteger)theVariable.getRealPart());
		}
		if (theVariable instanceof ScilabBoolean) {
            ScilabBoolean sciBoolean = (ScilabBoolean)theVariable;
            Call_Scilab.putBoolean(varname, sciBoolean.getData());
        }
		//TODO: a remplacer par la bonne exception return new UnsupportedTypeException();
		//		throw new UnsupportedTypeException();
		return true;
    }

}
