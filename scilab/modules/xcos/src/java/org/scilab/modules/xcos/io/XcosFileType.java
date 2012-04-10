/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.logging.Logger;

import javax.swing.filechooser.FileFilter;
import javax.swing.filechooser.FileNameExtensionFilter;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.stream.StreamSource;

import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement;
import org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.InterpreterException;
import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.codec.XcosCodec;
import org.scilab.modules.xcos.io.scicos.H5RWHandler;
import org.scilab.modules.xcos.io.scicos.ScilabDirectHandler;
import org.scilab.modules.xcos.io.spec.XcosPackage;
import org.scilab.modules.xcos.utils.XcosMessages;
import org.xml.sax.SAXException;

/**
 * All the filetype recognized by Xcos.
 */
public enum XcosFileType {
    /**
     * Represent the Xcos (a la ODT) format.
     */
    XCOS_COMPRESSED("xcos", XcosMessages.FILE_XCOS_COMPRESSED) {
        @Override
        public void load(String file, XcosDiagram into) throws TransformerException, IOException, SAXException, ParserConfigurationException {
            XcosPackage p = new XcosPackage(new File(file));
            p.setContent(into);

            p.load();
        }
    },
    /**
     * Represent the Xcos XML flat format.
     */
    XCOS("xcos", XcosMessages.FILE_XCOS) {
        @Override
        public void load(String file, XcosDiagram into) throws TransformerException {
            final XcosCodec codec = new XcosCodec();
            final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
            final Transformer aTransformer = tranFactory.newTransformer();

            final StreamSource src = new StreamSource(file);
            final DOMResult result = new DOMResult();
            aTransformer.transform(src, result);

            codec.decode(result.getNode().getFirstChild(), into);
        }
    },
    /**
     * Represent the old Scicos text format.
     */
    COSF("cosf", XcosMessages.FILE_COSF) {
        @Override
        public void load(String file, XcosDiagram into) throws Exception {
            loadScicosDiagram(file, into);
        }
    },
    /**
     * Represent the old Scicos binary format.
     */
    COS("cos", XcosMessages.FILE_COS) {

        @Override
        public void load(String file, XcosDiagram into) throws Exception {
            loadScicosDiagram(file, into);
        }
    },
    /**
     * Represent the Scilab I/O format.
     */
    HDF5("h5", XcosMessages.FILE_HDF5) {
        @Override
        public void load(String file, XcosDiagram into) {
            new H5RWHandler(file).readDiagram(into);
        }
    };

    private static final String BEFORE_EXT = " (*.";
    private static final String AFTER_EXT = ")";

    private String extension;
    private String description;

    /**
     * Default constructor
     *
     * @param extension
     *            file extension (without the dot)
     * @param description
     *            file description
     */
    XcosFileType(String extension, String description) {
        this.extension = extension;
        this.description = description + BEFORE_EXT + extension + AFTER_EXT;
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
     *
     * @param theFile
     *            Current file
     * @return The determined filetype
     */
    public static XcosFileType findFileType(String theFile) {
        int dotPos = theFile.lastIndexOf('.');
        String extension = "";
        XcosFileType retValue = null;

        if (dotPos > 0 && dotPos <= theFile.length() - 2) {
            extension = theFile.substring(dotPos + 1);
        }

        for (XcosFileType currentFileType : XcosFileType.values()) {
            if (extension.compareToIgnoreCase(currentFileType.extension) == 0) {
                retValue = currentFileType;
                break;
            }
        }

        /* Validate xml header */
        if (retValue == XCOS || retValue == XCOS_COMPRESSED) {
            retValue = checkHeader(theFile);
        }

        return retValue;
    }

    /**
     * Check the Xcos file header
     *
     * @param theFile
     *            the file to check
     * @return the found file type
     */
    private static XcosFileType checkHeader(String theFile) {
        XcosFileType retValue = null;

        final byte[] xmlMagic = "<?xml".getBytes();
        final byte[] readMagic = new byte[xmlMagic.length];

        /*
         * Check if the file is an xml one
         */
        FileInputStream stream = null;
        try {
            stream = new FileInputStream(theFile);
            int length;
            length = stream.read(readMagic);
            if (length == xmlMagic.length && Arrays.equals(xmlMagic, readMagic)) {
                retValue = XCOS;
            }
        } catch (IOException e) {
            retValue = null;
        } finally {
            if (stream != null) {
                try {
                    stream.close();
                } catch (IOException e) {
                    Logger.getLogger(XcosFileType.class.getName()).severe(e.toString());
                }
            }
        }

        /*
         * Check if the file is a valid zip file
         */
        if (retValue == null) {
            try {
                new XcosPackage(new File(theFile)).checkHeader();
                retValue = XCOS_COMPRESSED;
            } catch (IOException e) {
            } catch (ParserConfigurationException e) {
            } catch (TransformerException e) {
            }
        }

        return retValue;
    }

    /**
     * @return the Xcos default filetype
     */
    public static XcosFileType getDefault() {
        return XcosFileType.XCOS_COMPRESSED;
    }

    /**
     * Load a file into an XcosDiagram instance
     *
     * @param file
     *            the file to load
     * @param into
     *            the diagram instance to fill
     * @throws Exception
     *             in case of problem
     */
    public abstract void load(final String file, final XcosDiagram into) throws Exception;

    /**
     * @return the valid file filters
     */
    public static FileFilter[] getValidFilters() {
        final FileFilter[] filters = new FileFilter[values().length];

        for (int i = 0; i < filters.length; i++) {
            final XcosFileType type = values()[i];
            filters[i] = new FileNameExtensionFilter(type.getDescription(), type.getExtension());
        }
        return filters;
    }

    /**
     * Get a valid file mask (useable by file selector)
     *
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
     *
     * @return A valid file mask
     */
    public static String[] getValidFileDescription() {
        String[] result = new String[XcosFileType.values().length - 1];

        for (int i = 0; i < result.length; i++) {
            result[i] = XcosFileType.values()[i].getDescription() + BEFORE_EXT + XcosFileType.values()[i].getExtension() + AFTER_EXT;
        }

        return result;
    }

    /**
     * Load a Scicos diagram file int a diagram
     */
    private static void loadScicosDiagram(final String filename, final XcosDiagram into) {
        final StringBuilder cmd = new StringBuilder();
        cmd.append(ScilabDirectHandler.SCS_M);
        cmd.append(" = importScicosDiagram(\"");
        cmd.append(filename);
        cmd.append("\");");

        try {
            ScilabInterpreterManagement.synchronousScilabExec(cmd.toString());
        } catch (InterpreterException e) {
            e.printStackTrace();
        }

        new ScilabDirectHandler().readDiagram(into);
    }
}
