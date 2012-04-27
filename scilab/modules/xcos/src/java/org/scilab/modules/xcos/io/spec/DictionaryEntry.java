/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.io.spec;

import java.io.BufferedOutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.logging.Logger;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import org.scilab.modules.types.ScilabList;
import org.scilab.modules.xcos.graph.XcosDiagram;

import org.w3c.dom.Document;
import org.w3c.dom.Element;

public class DictionaryEntry implements Entry {

    private static final Logger LOG = Logger.getLogger(DictionaryEntry.class.getName());

    private Document manifest;
    private XcosDiagram content;
    private ScilabList dict;

    public DictionaryEntry() { }

    public DictionaryEntry(ScilabList dict) {
        this.dict = dict;
    }

    public ScilabList getDictionary() {
        return dict;
    }

    @Override
    public String getMediaType() {
        return "bin/ser";
    }

    @Override
    public String getFullPath() {
        return "dictionary/dictionary.ser";
    }

    @Override
    public void setup(XcosPackage p) {
        manifest = p.getManifest();
        content = p.getContent();
    }

    @Override
    public void load(ZipEntry entry, InputStream stream) throws IOException {
        ObjectInputStream ois = null;
        try {
            ois = new ObjectInputStream(stream);
            dict = (ScilabList) ois.readObject();
        } catch (IOException e) {
            Logger.getLogger(DictionaryEntry.class.getName()).severe(e.getMessage());
        } catch (ClassNotFoundException e) {
	    Logger.getLogger(DictionaryEntry.class.getName()).severe(e.getMessage());
	} finally {
	    ois.close();
	}
    }

    @Override
    public void store(ZipOutputStream stream) {
        /*
         * Append a ZipEntry
         */
        final ZipEntry entry = new ZipEntry(getFullPath());      
        ObjectOutputStream oos = null;
        try {
	    stream.putNextEntry(entry);

            oos = new ObjectOutputStream(new BufferedOutputStream(stream));
            oos.writeObject(dict);
            oos.flush();

            /*
             * Add an entry to the manifest file
             */
            final Element e = manifest.createElement("manifest:file-entry");
            e.setAttribute("manifest:media-type", getMediaType());
            e.setAttribute("manifest:full-path", getFullPath());
            manifest.getFirstChild().appendChild(e);
        } catch (IOException e) {
	    Logger.getLogger(DictionaryEntry.class.getName()).severe(e.getMessage());
        }
    }
}
