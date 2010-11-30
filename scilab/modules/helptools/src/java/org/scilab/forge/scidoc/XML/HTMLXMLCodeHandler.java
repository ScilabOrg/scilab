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

package org.scilab.forge.scidoc.XML;

import java.io.IOException;

public class HTMLXMLCodeHandler extends AbstractXMLCodeHandler {

    private static final int BUFCAPACITY = 8192;
    private static HTMLXMLCodeHandler handler = new HTMLXMLCodeHandler();

    protected Appendable buffer;

    protected HTMLXMLCodeHandler() {
        buffer = new StringBuilder(BUFCAPACITY);
    }

    public static AbstractXMLCodeHandler getInstance() {
        ((StringBuilder) handler.buffer).setLength(0);
        return handler;
    }

    public static String replaceEntity(String str) {
        return str.replaceAll("&", "&amp;").replaceAll("<", "&#0060;").replaceAll(">", "&#0062;").replaceAll("\"", "&#0034;").replaceAll("\'", "&#0039;");
    }

    /**
     * {@inheritDoc}
     */
    public void handleDefault(String seq) throws IOException {
        buffer.append("<span class=\"default\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleEntity(String seq) throws IOException {
        buffer.append("<span class=\"entity\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleNothing(String seq) throws IOException {
        buffer.append(seq);
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenInstr(String seq) throws IOException {
        buffer.append("<span class=\"openinstr\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCloseInstr(String seq) throws IOException {
        buffer.append("<span class=\"closeinstr\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleInstrName(String seq) throws IOException {
        buffer.append("<span class=\"instrname\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleLow(String seq) throws IOException {
        buffer.append("<span class=\"lowtag\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenTagName(String seq) throws IOException {
        buffer.append("<span class=\"tagname\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleLowClose(String seq) throws IOException {
        buffer.append("<span class=\"lowclose\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleGreat(String seq) throws IOException {
        buffer.append("<span class=\"greattag\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenComment(String seq) throws IOException {
        buffer.append("<span class=\"opencomment\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenCdata(String seq) throws IOException {
        buffer.append("<span class=\"opencdata\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleAttributeName(String seq) throws IOException {
        buffer.append("<span class=\"attributename\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleEqual(String seq) throws IOException {
        buffer.append("<span class=\"equal\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleAttributeValue(String seq) throws IOException {
        buffer.append("<span class=\"attributevalue\">");
        buffer.append(replaceEntity(seq));
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleAutoClose(String seq) throws IOException {
        buffer.append("<span class=\"autoclose\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCommentEnd(String seq) throws IOException {
        buffer.append("<span class=\"commentend\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleComment(String seq) throws IOException {
        buffer.append("<span class=\"comment\">");
        buffer.append(replaceEntity(seq));
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCdataEnd(String seq) throws IOException {
        buffer.append("<span class=\"cdataend\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCdata(String seq) throws IOException {
        buffer.append("<span class=\"cdata\">");
        buffer.append(replaceEntity(seq));
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public String toString() {
        return buffer.toString();
    }
}
