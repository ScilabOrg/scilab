/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io;

import java.util.ArrayList;
import java.util.List;

import ncsa.hdf.hdf5lib.exceptions.HDF5Exception;

import org.scilab.modules.hdf5.write.H5Write;
import org.scilab.modules.types.scilabTypes.ScilabBoolean;
import org.scilab.modules.types.scilabTypes.ScilabDouble;
import org.scilab.modules.types.scilabTypes.ScilabList;
import org.scilab.modules.types.scilabTypes.ScilabMList;
import org.scilab.modules.types.scilabTypes.ScilabString;
import org.scilab.modules.types.scilabTypes.ScilabTList;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.TextBlock;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.link.BasicLink;

/**
 * Class used to write a diagram contents into an HDF5 file
 */
public final class BlockWriter {
	/** Diagram MList header (scs_m) */
	private static final String[] DIAGRAM_FIELDS = {"diagram", "props", "objs", "version"};
	/** Diagram properties MList header (scs_m.props) */
	private static final String[] PROPS_FIELDS = {"params", "wpar", "title", "tol", "tf", "context", "void1", "options", "void2", "void3", "doc"};
	/** Diagram options MList header (scs_m.props.options) */
	private static final String[] OPTS_FIELDS = {"scsopt", "3D", "Background", "Link", "ID", "Cmap"};
	/**
	 * Window properties (scs_m.props.wpar).
	 * 
	 * This property has no impact among simulation
	 */
	private static final double[][] WPAR = {{600, 450, 0, 0, 600, 450}};
	
	// The window parameters and diagram options are not used in the simulation
	// thus we set it to default values.
	// As the values are scicos dependent we avoid using constant references.
	// CSOFF: MagicNumber
	private static final ScilabTList DIAGRAM_OPTIONS = new ScilabTList(
			OPTS_FIELDS) {
		{
			add(new ScilabList() { // 3D
				{
					add(new ScilabBoolean(true));
					add(new ScilabDouble(33));
				}
			});
			add(new ScilabDouble(new double[][] {{8, 1}})); // Background
			add(new ScilabDouble(new double[][] {{1, 5}})); // Link
			add(new ScilabList() { // ID
				{
					add(new ScilabDouble(new double[][] {{5, 1}}));
					add(new ScilabDouble(new double[][] {{4, 1}}));
				}
			});
			add(new ScilabDouble(new double[][] {{0.8, 0.8, 0.8}})); // Cmap
		}
	};
	// CSON: MagicNumber
	
    /**
     * This class is a static singleton thus constructor must not be used.
     */
    private BlockWriter() { }

    /**
     * Write a diagram to any file.
     * 
     * @param hdf5File file to create
     * @param diagram diagram to save
     * @return true if file created successfully
     */
    @Deprecated
    public static boolean writeDiagramToFile(String hdf5File, XcosDiagram diagram)	{

	boolean isSuccess = true;

	int fileId = H5Write.createFile(hdf5File);
	
	ScilabMList data = convertDiagramToMList(diagram);
	
	try {
	    H5Write.writeInDataSet(fileId, "scs_m", data);
	} catch (HDF5Exception e) {
	    e.printStackTrace();
	    isSuccess = false;
	}
	H5Write.closeFile(fileId);

	return isSuccess;
    }

    /**
     * Convert a diagram to a ScilabType.
     * 
     * @param diagram the diagram to be converted
     * @return the scilab formatted datas
     */
    @Deprecated
    public static ScilabMList convertDiagramToMList(XcosDiagram diagram) {
	ScilabMList data = new ScilabMList(DIAGRAM_FIELDS);
	data.add(getDiagramProps(diagram));
	data.add(getDiagramObjs(diagram));
	data.add(getDiagramVersion(diagram));
	
	return data;
    }


    /**
     * Create a Scilab TList from diagram properties
     * @param diagram the diagram
     * @return a TList
     */
    private static ScilabTList getDiagramProps(XcosDiagram diagram) {
	ScilabTList data = new ScilabTList(PROPS_FIELDS);
	data.add(new ScilabDouble(WPAR)); // wpar
	data.add(new ScilabString(diagram.getTitle())); // title
	data.add(new ScilabDouble(createTol(diagram))); // tol
	data.add(new ScilabDouble(diagram.getScicosParameters().getFinalIntegrationTime())); // tf
	data.add(new ScilabString(diagram.getScicosParameters().getContext())); // context
	data.add(new ScilabDouble()); // void1
	data.add(DIAGRAM_OPTIONS); // options
	data.add(new ScilabDouble()); // void2
	data.add(new ScilabDouble()); // void3
	data.add(new ScilabList()); // doc

	return data;
    }

    /**
     * Create tol matrix to be written
     * @param diagram the diagram
     * @return a matrix of values
     */
    private static double[][] createTol(XcosDiagram diagram) {
    	ScicosParameters parameters = diagram.getScicosParameters();
	double[][] tol = {{parameters.getIntegratorAbsoluteTolerance(), 
	    parameters.getIntegratorRelativeTolerance(),
	    parameters.getToleranceOnTime(),
	    parameters.getMaxIntegrationTimeInterval(),
	    parameters.getRealTimeScaling(),
	    parameters.getSolver(),
	    parameters.getMaximumStepSize()}};
	return tol;
    }

    /**
     * Create a Scilab TList containing all diagram objects 
     * @param diagram the diagram
     * @return the TList
     */
    @Deprecated
    private static ScilabList getDiagramObjs(XcosDiagram diagram) {
    	ScilabList data = new ScilabList();

    	int nbObjs = diagram.getModel().getChildCount(diagram.getDefaultParent());


    	List<BasicBlock> blockList = new ArrayList<BasicBlock>();
    	List<BasicLink> linkList = new ArrayList<BasicLink>();
    	for (int i = 0; i < nbObjs; ++i) {
    		Object currentObject = diagram.getModel().getChildAt(diagram.getDefaultParent(), i);
    		
    		
    		if (currentObject instanceof BasicBlock && !(currentObject instanceof TextBlock)) {
    			blockList.add((BasicBlock) currentObject);
    			//
    			// Look inside a Block to see if there is no "AutoLink"
    			// Jgraphx will store this link as block's child  
    			//
    			for (int j = 0; j < ((BasicBlock) currentObject).getChildCount(); ++j) {
    				if (((BasicBlock) currentObject).getChildAt(j) instanceof BasicLink) {
    					linkList.add((BasicLink) ((BasicBlock) currentObject).getChildAt(j));
    				}
    			}
    		} else if (currentObject instanceof BasicLink) {
    			linkList.add((BasicLink) currentObject);
    		}
    	}

    	// Go over all list to set ID
    	for (int i = 0; i < linkList.size(); ++i) {
    		linkList.get(i).setOrdering(i + blockList.size() + 1);
    	}

    	// Go over all blocks to dump it inside Scilab Structure
    	for (int i = 0; i < blockList.size(); ++i) {
    		blockList.get(i).setOrdering(i + 1);
    		data.add(BasicBlockInfo.getAsScilabObj(blockList.get(i)));
    	}

    	// Go over all link to dump it inside Scilab Structure
    	for (int i = 0; i < linkList.size(); ++i) {
    		data.add(linkList.get(i).getAsScilabObj());
    	}

    	return data;
    }

    /**
     * Create a Scilab String for diagram version
     * @param diagram the diagram
     * @return the Scilab String
     */
    private static ScilabString getDiagramVersion(XcosDiagram diagram) {
	return new ScilabString(diagram.getScicosParameters().getVersion());
    }


}
