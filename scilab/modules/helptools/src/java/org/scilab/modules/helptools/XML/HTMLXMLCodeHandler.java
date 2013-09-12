/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.helptools.XML;

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
        buffer.append("<span class=\"xmldefault\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleEntity(String seq) throws IOException {
        buffer.append("<span class=\"xmlentity\">");
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
        buffer.append("<span class=\"xmlopeninstr\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCloseInstr(String seq) throws IOException {
        buffer.append("<span class=\"xmlcloseinstr\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleInstrName(String seq) throws IOException {
        buffer.append("<span class=\"xmlinstrname\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleLow(String seq) throws IOException {
        buffer.append("<span class=\"xmllowtag\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenTagName(String seq) throws IOException {
        buffer.append("<span class=\"xmltagname\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleLowClose(String seq) throws IOException {
        buffer.append("<span class=\"xmllowclose\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleGreat(String seq) throws IOException {
        buffer.append("<span class=\"xmlgreattag\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenComment(String seq) throws IOException {
        buffer.append("<span class=\"xmlopencomment\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleOpenCdata(String seq) throws IOException {
        buffer.append("<span class=\"xmlopencdata\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleAttributeName(String seq) throws IOException {
        buffer.append("<span class=\"xmlattributename\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleEqual(String seq) throws IOException {
        buffer.append("<span class=\"xmlequal\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleAttributeValue(String seq) throws IOException {
        buffer.append("<span class=\"xmlattributevalue\">");
        buffer.append(replaceEntity(seq));
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleAutoClose(String seq) throws IOException {
        buffer.append("<span class=\"xmlautoclose\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCommentEnd(String seq) throws IOException {
        buffer.append("<span class=\"xmlcommentend\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleComment(String seq) throws IOException {
        buffer.append("<span class=\"xmlcomment\">");
        buffer.append(replaceEntity(seq));
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCdataEnd(String seq) throws IOException {
        buffer.append("<span class=\"xmlcdataend\">");
        buffer.append(seq);
        buffer.append("</span>");
    }

    /**
     * {@inheritDoc}
     */
    public void handleCdata(String seq) throws IOException {
        buffer.append("<span class=\"xmlcdata\">");
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
