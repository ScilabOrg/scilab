package org.scilab.modules.xcos.io.spec;

import java.io.IOException;
import java.io.InputStream;
import java.io.StringWriter;
import java.util.logging.Logger;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMResult;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.scilab.modules.commons.xml.ScilabTransformerFactory;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.codec.XcosCodec;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;

public class ContentEntry implements Entry {
    private Document manifest;
    private XcosDiagram content;

    @Override
    public String getMediaType() {
        return "text/xml";
    }

    @Override
    public String getFullPath() {
        return "content.xml";
    }

    @Override
    public void setup(XcosPackage p) {
        manifest = p.getManifest();
        content = p.getContent();
    }

    @Override
    public void load(ZipEntry entry, InputStream stream) throws IOException {
        try {
            final XcosCodec codec = new XcosCodec();
            final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
            final Transformer aTransformer = tranFactory.newTransformer();

            final StreamSource src = new StreamSource(stream);
            final DOMResult result = new DOMResult();
            aTransformer.transform(src, result);

            codec.decode(result.getNode().getFirstChild(), content);
        } catch (TransformerConfigurationException e) {
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessageAndLocation());
        } catch (TransformerException e) {
            e.printStackTrace();
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessageAndLocation());
        }
    }

    @Override
    public void store(ZipOutputStream stream) throws IOException {
        /*
         * Append a ZipEntry
         */
        final ZipEntry entry = new ZipEntry(getFullPath());
        stream.putNextEntry(entry);

        /*
         * Store content
         */
        try {
            final XcosCodec codec = new XcosCodec();
            final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
            final Transformer aTransformer = tranFactory.newTransformer();

            final DOMSource src = new DOMSource(codec.encode(content));
            final StreamResult result = new StreamResult(stream);
            aTransformer.transform(src, result);

            /*
             * Add an entry to the manifest file
             */
            final Element e = manifest.createElement("manifest:file-entry");
            e.setAttribute("manifest:media-type", getMediaType());
            e.setAttribute("manifest:full-path", getFullPath());
            manifest.getFirstChild().appendChild(e);

        } catch (TransformerConfigurationException e) {
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessageAndLocation());
        } catch (TransformerException e) {
            Logger.getLogger(ContentEntry.class.getName()).severe(e.getMessageAndLocation());
        }
    }

    public static String printNode(Object o) {
        if (!(o instanceof Node)) {
            return o.toString();
        }

        try {
            final StringWriter str = new StringWriter();
            final TransformerFactory tranFactory = ScilabTransformerFactory.newInstance();
            final Transformer aTransformer = tranFactory.newTransformer();
            aTransformer.setOutputProperty(OutputKeys.INDENT, "yes");
            aTransformer.setOutputProperty("{http://xml.apache.org/xslt}indent-amount", "1");

            final DOMSource src = new DOMSource((Node) o);
            final StreamResult result = new StreamResult(str);
            aTransformer.transform(src, result);

            return str.toString();
        } catch (Exception e) {
            return e.toString();
        }
    }

}