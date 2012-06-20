/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons;

import javax.xml.xpath.XPath;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpressionException;
import javax.xml.xpath.XPathFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;
import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;

public class ScilabGeneralPrefs implements XConfigurationListener {

    private static final String LAYOUT_PATH = "//general/desktop-layout/body/layouts";
    private static final ScilabGeneralPrefs instance = new ScilabGeneralPrefs();

    private ScilabGeneralPrefs() { }

    public static ScilabGeneralPrefs getInstance() {
        return instance;
    }

    public static void setJavaHeap(int heapSize) {
        String path = System.getenv("SCI") + "/etc/jvm_options.xml";
        Document doc = ScilabXMLUtilities.readDocument(path);
        XPathFactory xpathFactory = XPathFactory.newInstance();
        XPath xp = xpathFactory.newXPath();
        try {
            NodeList nodes = (NodeList) xp.compile("//option[contains(@value,'-Xmx')]/@value").evaluate(doc, XPathConstants.NODESET);
            if (nodes != null && nodes.getLength() > 0) {
                Node node = nodes.item(0);
                String oldValue = node.getNodeValue();
                String value = "-Xmx" + Integer.toString(heapSize) + "m";
                if (!oldValue.equals(value)) {
                    node.setNodeValue(value);
                    ScilabXMLUtilities.writeDocument(doc, path);
                }
            }
        } catch (XPathExpressionException e) { }
    }

    public void configurationChanged(XConfigurationEvent e) {
        if (e.getModifiedPaths().contains(JHM_PATH)) {
            Document doc = XConfiguration.getXConfigurationDocument();
            JavaHeapMemory jhm = XConfiguration.get(JavaHeapMemory.class, doc, JHM_PATH)[0];
            if (jhm.javaHeap > 0) {
                setJavaHeap(jhm.javaHeap);
            }
        }
    }

    @XConfAttribute
    private static class JavaHeapMemory {

        public int javaHeap;

        private JavaHeapMemory() { }

        @XConfAttribute(attributes = {"heap-size"})
        private void set(int javaHeap) {
            this.javaHeap = javaHeap;
        }
    }
}