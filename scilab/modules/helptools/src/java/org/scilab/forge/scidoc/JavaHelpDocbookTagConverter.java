/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.forge.scidoc;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Stack;

import org.xml.sax.SAXException;

/**
 * Class to convert DocBook to JavaHelp
 * @author Calixte DENIZET
 */
public class JavaHelpDocbookTagConverter extends HTMLDocbookTagConverter {

    private static final String XMLSTRING = "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>\n";

    private StringBuilder buffer = new StringBuilder(8192);

    /**
     * Constructor
     * @param inName the name of the input stream
     * @param primConf the file containing the primitives of Scilab
     * @param macroConf the file containing the macros of Scilab
     * @param out the output stream
     */
    public JavaHelpDocbookTagConverter(String inName, String outName, String primConf, String macroConf, String template, String version, String imageDir, boolean checkLast) throws IOException, SAXException {
        super(inName, outName, primConf, macroConf, template, version, imageDir, checkLast);
    }

    /**
     * {@inheritDoc}
     */
    public void endDocument() throws SAXException {
        try {
            FileOutputStream outToc = new FileOutputStream("jhelptoc.xml");
            FileOutputStream outMap = new FileOutputStream("jhelpmap.jhm");
            FileOutputStream outSet = new FileOutputStream("jhelpset.hs");
            FileOutputStream outIndex = new FileOutputStream("jhelpidx.xml");
            OutputStreamWriter writerIndex = new OutputStreamWriter(outIndex);
            OutputStreamWriter writerSet = new OutputStreamWriter(outSet);
            OutputStreamWriter writerMap = new OutputStreamWriter(outMap);
            OutputStreamWriter writerToc = new OutputStreamWriter(outToc);
            writerMap.append(XMLSTRING);
            writerMap.append("<!DOCTYPE map PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp Map Version 1.0//EN\" \"http://java.sun.com/products/javahelp/map_1_0.dtd\">\n");
            writerMap.append(convertMapId());
            writerMap.flush();
            writerMap.close();
            outMap.flush();
            outMap.close();

            writerToc.append(XMLSTRING);
            writerToc.append("<!DOCTYPE toc PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp TOC Version 1.0//EN\" \"http://java.sun.com/products/javahelp/toc_1_0.dtd\">\n");
            writerToc.append(convertTocItem());
            writerToc.flush();
            writerToc.close();
            outToc.flush();
            outToc.close();

            writerSet.append(XMLSTRING);
            String str = "<!DOCTYPE helpset\n  PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp HelpSet Version 1.0//EN\" \"http://java.sun.com/products/javahelp/helpset_1_0.dtd\">\n<helpset version=\"1.0\">\n<title>TITLE</title>\n<maps>\n<homeID>top</homeID>\n<mapref location=\"jhelpmap.jhm\"/>\n</maps>\n<view>\n<name>TOC</name>\n<label>Table Of Contents</label>\n<type>javax.help.TOCView</type>\n<data>jhelptoc.xml</data>\n</view>\n<view>\n<name>Index</name>\n<label>Index</label>\n<type>javax.help.IndexView</type>\n<data>jhelpidx.xml</data>\n</view>\n<view>\n<name>Search</name>\n<label>Search</label>\n<type>javax.help.SearchView</type>\n<data engine=\"com.sun.java.help.search.DefaultSearchEngine\">JavaHelpSearch</data>\n</view>\n</helpset>".replaceFirst("TITLE", bookTitle);
            writerSet.append(str);
            writerSet.flush();
            writerSet.close();
            outSet.flush();
            outSet.close();

            writerIndex.append(XMLSTRING);
            writerIndex.append("<!DOCTYPE index PUBLIC \"-//Sun Microsystems Inc.//DTD JavaHelp Index Version 1.0//EN\" \"http://java.sun.com/products/javahelp/index_1_0.dtd\">\n<index version=\"1.0\"/>");
            writerIndex.flush();
            writerIndex.close();
            outIndex.flush();
            outIndex.close();
        } catch (IOException e) {
            exceptionOccured(e);
        }
    }

    private String convertMapId() {
        buffer.setLength(0);
        buffer.append("<map version=\"1.0\">\n<mapID target=\"index\" url=\"index.html\"/>\n");
        Iterator<String> iter = mapId.keySet().iterator();
        while (iter.hasNext()) {
            String id = iter.next();
            buffer.append("<mapID target=\"");
            buffer.append(id);
            buffer.append("\" url=\"");
            buffer.append(mapId.get(id));
            buffer.append("\"/>\n");
        }
        buffer.append("</map>");

        return buffer.toString();
    }

    private void convertTreeId(HTMLDocbookLinkResolver.TreeId leaf) {
        if (leaf.children != null) {
            for (HTMLDocbookLinkResolver.TreeId c : leaf.children) {
                buffer.append("<tocitem target=\"");
                buffer.append(c.id);
                buffer.append("\" text=\"");
                buffer.append(tocitem.get(c.id));
                if (c.children == null) {
                    buffer.append("\"/>\n");
                } else {
                    buffer.append("\">\n");
                    convertTreeId(c);
                    buffer.append("</tocitem>\n");
                }
            }
        }
    }

    private String convertTocItem() {
        buffer.setLength(0);
        buffer.append("<toc version=\"1.0\">\n<tocitem target=\"index\" text=\"" + bookTitle + "\">\n");
        convertTreeId(tree);
        buffer.append("</tocitem>\n</toc>");

        return buffer.toString();
    }
}
