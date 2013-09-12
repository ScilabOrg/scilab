/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.graph.io;

import java.util.Map;
import java.util.logging.Logger;

import org.scilab.modules.types.ScilabBoolean;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;

import com.mxgraph.io.mxCodec;
import com.mxgraph.io.mxCodecRegistry;

/**
 * Define serialization for a {@link ScilabBoolean} instance.
 */
public class ScilabBooleanCodec extends ScilabObjectCodec {

    private static final String VALUE = "value";

    /**
     * Default constructor
     *
     * @param template
     *            Prototypical instance of the object to be encoded/decoded.
     * @param exclude
     *            Optional array of fieldnames to be ignored.
     * @param idrefs
     *            Optional array of fieldnames to be converted to/from
     *            references.
     * @param mapping
     *            Optional mapping from field- to attributenames.
     */
    public ScilabBooleanCodec(Object template, String[] exclude, String[] idrefs, Map<String, String> mapping) {
        super(template, exclude, idrefs, mapping);

    }

    /**
     * Encodes the specified object and returns a node representing then given
     * object. Calls beforeEncode after creating the node and afterEncode with
     * the resulting node after processing.
     *
     * @param enc
     *            Codec that controls the encoding process.
     * @param obj
     *            Object to be encoded.
     * @return Returns the resulting XML node that represents the given object.
     */
    @Override
    public Node encode(mxCodec enc, Object obj) {
        String name = mxCodecRegistry.getName(obj);
        Node node = enc.getDocument().createElement(name);

        ScilabBoolean scilabBoolean = (ScilabBoolean) obj;

        if (binary) {
            int pos = binaryObjects.size();
            binaryObjects.add(scilabBoolean);
            mxCodec.setAttribute(node, BINARY, "true");
            mxCodec.setAttribute(node, POSITION, pos);

            return node;
        }

        mxCodec.setAttribute(node, WIDTH, scilabBoolean.getWidth());
        mxCodec.setAttribute(node, HEIGHT, scilabBoolean.getHeight());

        for (int i = 0; i < scilabBoolean.getHeight(); ++i) {
            for (int j = 0; j < scilabBoolean.getWidth(); ++j) {
                Node data = enc.getDocument().createElement(DATA);
                mxCodec.setAttribute(data, LINE, i);
                mxCodec.setAttribute(data, COLUMN, j);
                mxCodec.setAttribute(data, VALUE, scilabBoolean.getData()[i][j]);
                node.appendChild(data);
            }
        }
        return node;
    }

    /**
     * Parses the given node into the object or returns a new object
     * representing the given node.
     *
     * @param dec
     *            Codec that controls the encoding process.
     * @param node
     *            XML node to be decoded.
     * @param into
     *            Optional object to encode the node into.
     * @return Returns the resulting object that represents the given XML node
     *         or the object given to the method as the into parameter.
     */
    @Override
    public Object decode(mxCodec dec, Node node, Object into) {
        ScilabBoolean obj = null;

        try {
            if (node.getNodeType() != Node.ELEMENT_NODE) {
                throw new UnrecognizeFormatException();
            }

            // attrs = {"as", "height", "width", "binary", "position"}
            final NamedNodeMap attrs = node.getAttributes();
            if (attrs == null) {
                throw new UnrecognizeFormatException();
            }

            if (getBooleanAttribute(attrs, BINARY)) {
                return binaryObjects.get(getIntegerAttribute(attrs, POSITION));
            }

            obj = (ScilabBoolean) cloneTemplate(node);

            final int height = getHeight(attrs);
            final int width = getWidth(attrs);

            if (height * width == 0) {
                return obj;
            }

            final boolean[][] data = new boolean[height][width];
            fillData(node, data);

            obj.setData(data);

        } catch (UnrecognizeFormatException e) {
            Logger.getLogger(ScilabStringCodec.class.getName()).severe(e.toString());
        } catch (NumberFormatException e) {
            Logger.getLogger(ScilabStringCodec.class.getName()).severe(e.toString());
        }

        return obj;
    }

    /**
     * Fill the data from the node.
     *
     * @param node
     *            the ScilabBoolean node
     * @param data
     *            the allocated data
     * @throws UnrecognizeFormatException
     *             when we are unable to decode the node.
     */
    private void fillData(Node node, final boolean[][] data) throws UnrecognizeFormatException {
        for (Node n = node.getFirstChild(); n != null; n = n.getNextSibling()) {
            if (n.getNodeType() != Node.ELEMENT_NODE) {
                continue;
            }

            final NamedNodeMap dataAttrs = n.getAttributes();
            if (dataAttrs == null) {
                throw new UnrecognizeFormatException();
            }

            final int column = getColumnIndex(dataAttrs);
            final int line = getLineIndex(dataAttrs);

            final Node v = dataAttrs.getNamedItem(VALUE);
            if (v == null) {
                throw new UnrecognizeFormatException();
            }
            data[line][column] = Boolean.parseBoolean(v.getNodeValue());
        }
    }
}
