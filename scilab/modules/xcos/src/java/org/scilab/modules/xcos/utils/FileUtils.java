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

package org.scilab.modules.xcos.utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.StringReader;
import java.nio.channels.FileChannel;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.jvm.utils.ScilabConstants;
import org.w3c.dom.Document;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;

import com.mxgraph.io.mxCodec;
import com.mxgraph.util.mxUtils;
import com.mxgraph.view.mxStylesheet;

/**
 * Contains useful method for managing files.
 */
public final class FileUtils {
	
	/**
	 * The style configuration file name.
	 */
	public static final String STYLE_FILENAME = "Xcos-style.xml";
	
	private static final Log LOG = LogFactory.getLog(FileUtils.class); 
	
	/**
	 * Default constructor
	 */
	private FileUtils() {
	}

	/**
	 * Copy in to out
	 * 
	 * @param in
	 *            the input file
	 * @param out
	 *            the output file
	 * @throws IOException
	 *             when an errors has occured
	 */
	public static void copy(File in, File out) throws IOException {
		FileChannel inChannel = new FileInputStream(in).getChannel();
		FileChannel outChannel = new FileOutputStream(out).getChannel();
		try {
			inChannel.transferTo(0, inChannel.size(), outChannel);
		} catch (IOException e) {
			LogFactory.getLog(FileUtils.class).warn(e);
			throw e;
		} finally {
			if (inChannel != null) {
				inChannel.close();
			}
			if (outChannel != null) {
				outChannel.close();
			}
		}
	}

	/**
	 * Force the copy from in to out
	 * 
	 * @param in
	 *            the input file
	 * @param out
	 *            the output file
	 */
	public static void forceCopy(File in, File out) {
		FileChannel inChannel = null;
		FileChannel outChannel = null;
		
		if (!out.exists()) {
			try {
				out.createNewFile();
			} catch (IOException e) {
				LogFactory.getLog(FileUtils.class).warn(e);
			}
		}
		
		try {
			inChannel = new FileInputStream(in).getChannel();
			outChannel = new FileOutputStream(out).getChannel();
			inChannel.transferTo(0, inChannel.size(), outChannel);
		} catch (IOException e) {
			LogFactory.getLog(FileUtils.class).warn(e);
		} finally {
			if (inChannel != null) {
				try {
					inChannel.close();
				} catch (IOException e) {
					LogFactory.getLog(FileUtils.class).warn(e);
				}
			}
			if (outChannel != null) {
				try {
					outChannel.close();
				} catch (IOException e) {
					LogFactory.getLog(FileUtils.class).warn(e);
				}
			}
		}
	}
	
	/**
	 * Create a temporary file and return it
	 * @return a new unique temporary file.
	 * @throws IOException when an error occurs
	 */
	public static File createTempFile() throws IOException {
		return File.createTempFile(XcosFileType.XCOS.getExtension(),
				XcosFileType.HDF5.getDottedExtension());
	}
	
	/**
	 * Delete the file and log an error message if unable to do so.
	 * @param f the file to delete.
	 */
	public static void delete(File f) {
		if (!f.delete()) {
			LogFactory.getLog(FileUtils.class).error(XcosMessages.UNABLE_TO_DELETE + f);
		}
	}
	
	/**
	 * Decode the style into the passed stylesheet.
	 * 
	 * @param styleSheet the current stylesheet
	 * @throws IOException on I/O errors
	 */
	public static void decodeStyle(final mxStylesheet styleSheet)
			throws IOException {
		/*
		 * Initialize constants
		 */
		final String homePath = ScilabConstants.SCIHOME.getAbsolutePath();
		final File userStyleSheet = new File(homePath + '/' + STYLE_FILENAME);
		
		/*
		 * Copy the base stylesheet into the user dir when it doesn't exist.
		 */
		if (!userStyleSheet.exists()) {
			final String sciPath = ScilabConstants.SCI.getAbsolutePath();

			File baseStyleSheet = new File(sciPath + "/modules/xcos/etc/" + STYLE_FILENAME);
			FileUtils.forceCopy(baseStyleSheet, userStyleSheet);
		}
		
		/*
		 * Load the stylesheet
		 */
		final String sciURL = ScilabConstants.SCI.toURI().toURL().toString();
		final String homeURL = ScilabConstants.SCIHOME.toURI().toURL().toString();
		
		String xml = mxUtils.readFile(userStyleSheet.getAbsolutePath());
		xml = xml.replaceAll("\\$SCILAB", sciURL);
		xml = xml.replaceAll("\\$SCIHOME", homeURL);
		
		DocumentBuilderFactory docBuilderFactory = DocumentBuilderFactory.newInstance();
		DocumentBuilder docBuilder;
		Document document = null;

	    try {
			docBuilder = docBuilderFactory.newDocumentBuilder();
			document = docBuilder.parse(new InputSource(new StringReader(xml)));
			new mxCodec().decode(document.getDocumentElement(), styleSheet);
		} catch (ParserConfigurationException e) {
			LOG.error(e);
		} catch (SAXException e) {
			LOG.error(e);
		}
	}
	
    /**
     * Load an Xcos file
     * 
     * @param xcosFile xcos file
     * @return opened document
     */
	public static Document loadXcosDocument(String xcosFile) {
		DocumentBuilderFactory docBuilderFactory = DocumentBuilderFactory
				.newInstance();
		DocumentBuilder docBuilder;
		try {
			docBuilder = docBuilderFactory.newDocumentBuilder();
			return docBuilder.parse(new InputSource(new FileInputStream(
					xcosFile)));
		} catch (ParserConfigurationException e) {
			LOG.error(e);
			return null;
		} catch (SAXException e) {
			LOG.error(e);
			return null;
		} catch (IOException e) {
			LOG.error(e);
			return null;
		}
	}
}
