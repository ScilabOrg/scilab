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

package org.scilab.modules.xcos.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Arrays;
import java.util.zip.GZIPInputStream;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;

/**
 * All the filetype recognized by Xcos.
 */
public enum XcosFileType {
	/**
	 * Represent the old Scicos text format.
	 */
	COSF("cosf", XcosMessages.FILE_COSF) {
		/**
		 * Export the typed file to the HDF5 format.
		 * @param arg0 The COSF formatted file
		 * @return The HDF5 formatted file
		 */
		@Override
		public File exportToHdf5(File arg0) {
			return loadScicosDiagram(arg0);
		}
	},
	/**
	 * Represent the old Scicos binary format.
	 */
	COS("cos", XcosMessages.FILE_COS) {
		/**
		 * Export the typed file to the HDF5 format.
		 * @param arg0 The COS formatted file
		 * @return The HDF5 formatted file
		 */
		@Override
		public File exportToHdf5(File arg0) {
			return loadScicosDiagram(arg0);
		}
	},
	/**
	 * Represent the Scilab I/O format.
	 */
	HDF5("h5", XcosMessages.FILE_HDF5) {
		/**
		 * Export the typed file to the HDF5 format. (does nothing there)
		 * @param arg0 The HDF5 formatted file
		 * @return The HDF5 formatted file
		 */
		@Override
		public File exportToHdf5(File arg0) {
			return arg0;
		}
	},
	/**
	 * Represent the Xcos XML format (compressed or not).
	 */
	XCOS("xcos", XcosMessages.FILE_XCOS),
	
	/**
	 * Any other format.
	 */
	UNKNOW("", "");
	
	
	private String extension;
	private String description;
	
	/**
	 * Default constructor
	 * @param extension file extension (without the dot)
	 * @param description file description
	 */
	XcosFileType(String extension, String description) {
		this.extension = extension;
		this.description = description;
	}
	
	/**
	 * @return the extension prefixed with a dot
	 */
	public String getDottedExtension() {
		return "." + extension;
	}
	
	/**
	 * @return the raw extension
	 */
	public String getExtension() {
		return extension;
	}
	
	/**
	 * @return the mask of this file
	 */
	public String getFileMask() {
		return "*." + getExtension();
	}
	
	/**
	 * @return the file description
	 */
	public String getDescription() {
		return description;
	}
	
	/**
	 * Find a filetype by the filename extension
	 * @param theFile Current file
	 * @return The determined filetype
	 */
	public static XcosFileType findFileType(File theFile) {
		int dotPos = theFile.getName().lastIndexOf('.');
		String extension = "";
		XcosFileType retValue = XcosFileType.UNKNOW;

		if (dotPos > 0 && dotPos <= theFile.getName().length() - 2) {
			extension = theFile.getName().substring(dotPos + 1);
		}
		
		for (XcosFileType currentFileType : XcosFileType.values()) {
			if (extension.compareToIgnoreCase(currentFileType.extension) == 0) {
				retValue = currentFileType;
				break;
			}
		}
		
		/* Validate xml header */
		if (retValue == XcosFileType.XCOS) {
			byte[] xmlMagic = "<?xml".getBytes();
			byte[] readMagic = new byte[xmlMagic.length];

			InputStream stream = null;
			try {
				
				final FileInputStream fStream = new FileInputStream(theFile);
				try {
					// try to get the compressed format
					stream = new GZIPInputStream(fStream);
				} catch (IOException e) {
					// Non compressed format
					fStream.close();
					stream = new FileInputStream(theFile);
				}
				
				int length;
				length = stream.read(readMagic);
				if (length != xmlMagic.length
						|| !Arrays.equals(xmlMagic, readMagic)) {
					retValue = XcosFileType.UNKNOW;
				}
			} catch (IOException e) {
				retValue = XcosFileType.UNKNOW;
			} finally {
				if (stream != null) {
					try {
						stream.close();
					} catch (IOException e) {
						LogFactory.getLog(XcosFileType.class).error(e);
					}
				}
			}
		}
		
		return retValue;
	}
	
	/** 
	 * @return the Xcos default filetype
	 */
	public static XcosFileType getDefault() {
		return XcosFileType.XCOS;
	}
	
	/** 
	 * @return the Scilab default filetype
	 */
	public static XcosFileType getScilabFileType() {
		return XcosFileType.HDF5;
	}
	
	/**
	 * Convert the file passed as an argument to Hdf5.
	 * @param file The file to convert
	 * @return The created file
	 */
	public File exportToHdf5(File file) {
	    throw new Error("Not implemented operation");
	}
	
	/**
	 * Get a valid file mask (useable by file selector)
	 * @return A valid file mask
	 */
	public static String[] getValidFileMask() {
	    String[] result = new String[XcosFileType.values().length - 1];
	    
	    for (int i = 0; i < result.length; i++) {
		result[i] = XcosFileType.values()[i].getFileMask();
	    }
	    
	    return result;
	}
	
	/**
	 * Get a valid file description (useable by file selector)
	 * @return A valid file mask
	 */
	public static String[] getValidFileDescription() {
	    String[] result = new String[XcosFileType.values().length - 1];
	    
	    for (int i = 0; i < result.length; i++) {
		result[i] = XcosFileType.values()[i].getDescription() + " (*." + XcosFileType.values()[i].getExtension() + ")";
	    }
	    
	    return result;
	}
	
	/**
	 * Convert a Scicos diagram (scs_m scilab script) to an hdf5 file
	 * @param filename The file to execute in scilab.
	 * @return The exported data in hdf5.
	 */
	private static File loadScicosDiagram(File filename) {
	    File tempOutput = null;
	    try {
		tempOutput = FileUtils.createTempFile();
		
		StringBuilder cmd = new StringBuilder();
		cmd.append("scs_m = importScicosDiagram(\"");
		cmd.append(filename.getAbsolutePath());
		cmd.append("\");");
		cmd.append("result = export_to_hdf5(\"");
		cmd.append(tempOutput.getAbsolutePath());
		cmd.append("\", \"scs_m\");");
		
		cmd.append("if result <> %t then deletefile(\"");
		cmd.append(tempOutput.getAbsolutePath());
		cmd.append("\"); end; ");
		
		try {
			ScilabInterpreterManagement.synchronousScilabExec(cmd.toString());
		} catch (InterpreterException e) {
			e.printStackTrace();
		}
	    } catch (IOException e) {
		e.printStackTrace();
	    }
	    
	    return tempOutput;
	}
}
