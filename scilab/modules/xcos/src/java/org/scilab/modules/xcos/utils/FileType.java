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

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.net.URLEncoder;
import java.util.Arrays;

import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;

import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosCodec;

import com.mxgraph.io.mxCodec;
import com.mxgraph.swing.mxGraphComponent;
import com.mxgraph.util.mxCellRenderer;
import com.mxgraph.util.mxUtils;
import com.mxgraph.util.png.mxPngEncodeParam;
import com.mxgraph.util.png.mxPngImageEncoder;

/**
 * All the filetype recognized by Xcos.
 */
public enum FileType {
	/**
	 * Represent the Xcos XML format.
	 */
	XCOS("xcos", XcosMessages.FILE_XCOS) {
		/**
		 * {@inheritDoc}
		 */
		@Override
		public void save(XcosDiagram diagram, File file) throws IOException {
			final XcosCodec codec = new XcosCodec();
			final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
			
			try {
				final Transformer aTransformer = tranFactory.newTransformer();
				
				final DOMSource src = new DOMSource(codec.encode(diagram));
				final StreamResult result = new StreamResult(file);
				aTransformer.transform(src, result);
			} catch (TransformerException e) {
				throw new IOException(e);
			}
		}
	},
	/**
	 * Represent the Xcos PNG+XML format.
	 */
	PNG("png", XcosMessages.FILE_PNG) {
		/**
		 *{@inheritDoc}
		 */
		@Override
		public void save(XcosDiagram diagram, File file) throws IOException {
			final mxGraphComponent comp = diagram.getAsComponent();
			
			// Creates the image for the PNG file
			final BufferedImage image = mxCellRenderer.createBufferedImage(diagram,
					null, 1, comp.getBackground(), comp.isAntiAlias(), null,
					comp.getCanvas());
			
			// Creates the URL-encoded XML data
			mxCodec codec = new mxCodec();
			String xml = URLEncoder.encode(
					mxUtils.getXml(codec.encode(diagram.getModel())), "UTF-8");
			mxPngEncodeParam param = mxPngEncodeParam
					.getDefaultEncodeParam(image);
			param.setCompressedText(new String[] {"mxGraphModel", xml});
			
			// Saves as a PNG file
			FileOutputStream outputStream = new FileOutputStream(file);
			mxPngImageEncoder encoder = new mxPngImageEncoder(outputStream,
					param);
			if (image != null) {
				encoder.encode(image);
			} else {
				throw new IOException("Unable to encode data.");
			}
		}
	},
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
	FileType(String extension, String description) {
		this.extension = extension;
		this.description = description + " (*." + extension + ")";
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
	public static FileType findFileType(File theFile) {
		int dotPos = theFile.getName().lastIndexOf('.');
		String extension = "";
		FileType retValue = null;

		if (dotPos > 0 && dotPos <= theFile.getName().length() - 2) {
			extension = theFile.getName().substring(dotPos + 1);
		}
		
		for (FileType currentFileType : FileType.values()) {
			if (extension.compareToIgnoreCase(currentFileType.extension) == 0) {
				retValue = currentFileType;
				break;
			}
		}
		
		/* Validate xml header */
		if (retValue == FileType.XCOS) {
			byte[] xmlMagic = "<?xml".getBytes();
			byte[] readMagic = new byte[xmlMagic.length];

			FileInputStream stream = null;
			try {
				
				stream = new FileInputStream(theFile);
				
				int length;
				length = stream.read(readMagic);
				if (length != xmlMagic.length
						|| !Arrays.equals(xmlMagic, readMagic)) {
					retValue = null;
				}
			} catch (IOException e) {
				retValue = null;
			} finally {
				if (stream != null) {
					try {
						stream.close();
					} catch (IOException e) {
						LogFactory.getLog(FileType.class).error(e);
					}
				}
			}
		}
		
		return retValue;
	}
	
	/** 
	 * @return the Xcos default filetype
	 */
	@Deprecated
	public static FileType getDefault() {
		return FileType.XCOS;
	}
	
	/**
	 * @return all the save and load file types
	 */
	public static FileType[] getValidFileType() {
		return new FileType[] {
				FileType.XCOS, FileType.PNG
		};
	}
	
	/** 
	 * @return the Scilab default filetype
	 */
	public static FileType getScilabFileType() {
		return FileType.HDF5;
	}
	
	/**
	 * Convert the file passed as an argument to Hdf5.
	 * @param file The file to convert
	 * @return The created file
	 */
	public File exportToHdf5(File file) {
	    throw new Error();
	}
	
	/**
	 * Save a diagram to a file for the specific type
	 * @param diagram the diagram to save
	 * @param file the file to save to
	 * @throws IOException on problem
	 */
	public void save(XcosDiagram diagram, File file) throws IOException {
		throw new Error();
	}
	
	/**
	 * @return the valid file filters
	 */
	public static FileFilter[] getValidFilters() {
		final FileFilter[] filters = new FileFilter[values().length];
		
		for (int i = 0; i < filters.length; i++) {
			final FileType type = values()[i];
			filters[i] = new FileNameExtensionFilter(
					type.getDescription(), 
					type.getExtension());
		}
		return filters;
	}
	
	/**
	 * Get a valid file mask (useable by file selector)
	 * @return A valid file mask
	 */
	public static String[] getValidFileMask() {
	    String[] result = new String[FileType.values().length - 1];
	    
	    for (int i = 0; i < result.length; i++) {
		result[i] = FileType.values()[i].getFileMask();
	    }
	    
	    return result;
	}
	
	/**
	 * Get a valid file description (useable by file selector)
	 * @return A valid file mask
	 */
	public static String[] getValidFileDescription() {
	    String[] result = new String[FileType.values().length - 1];
	    
	    for (int i = 0; i < result.length; i++) {
		result[i] = FileType.values()[i].getDescription() + " (*." + FileType.values()[i].getExtension() + ")";
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
