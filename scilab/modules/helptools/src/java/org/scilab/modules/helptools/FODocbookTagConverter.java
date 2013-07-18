/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.helptools;

import java.io.IOException;
import java.util.Map;

import org.scilab.modules.helptools.image.Image;
import org.scilab.modules.helptools.image.ImageConverter;
import org.xml.sax.SAXException;

public class FODocbookTagConverter extends DocbookTagConverter implements TemplateFiller {

    public FODocbookTagConverter(String inName, SciDocMain sciDocMain, final ImageConverter imgConvert) throws IOException {
        super(inName, imgConvert);
        // TODO Auto-generated constructor stub
    }

    @Override
    public void registerAllExternalXMLHandlers() {
        // TODO Auto-generated method stub

    }

    @Override
    public String generateImageCode(String code, String fileName, Map<String, String> attrs) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String generateImageCode(Image img, String fileName, Map<String, String> attrs) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String generateImageCode(String fileName, Map<String, String> attrs) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makePrevious(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makePath(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makeTop(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makeNext(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makeTocList(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makeLastModified(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makeSubtitle(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makeTitle(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makeGenerationDate(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makeVersion(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String makeStart(String id) {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleAnswer(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleBibliomixed(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleBibliomset(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleBook(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleCaption(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleCaution(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleChapter(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleCode(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleCommand(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleConstant(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleEmphasis(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleFirstname(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleFunction(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleImagedata(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleImageobject(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleImportant(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleInfo(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleInformalequation(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleInformaltable(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleInlinemediaobject(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleItemizedlist(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleLatex(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleLink(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleListitem(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleLiteral(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleMediaobject(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleMember(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleNote(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleOption(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleOrderedlist(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handlePara(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handlePart(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleProgramlisting(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handlePubdate(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleQandaentry(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleQandaset(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleQuestion(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRefentry(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRefnamediv(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRefname(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRefpurpose(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRefsection(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRefsynopsisdiv(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleReplaceable(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRevdescription(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRevhistory(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRevision(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRevnumber(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleRevremark(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleScreen(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleScreenshot(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleSection(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleSimplelist(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleSubscript(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleSuperscript(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleSurname(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleSynopsis(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleTable(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleTbody(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleTd(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleTerm(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleTh(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleTip(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleTitle(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleTr(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleUlink(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleVariablelist(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleVarlistentry(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleVarname(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleWarning(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleXref(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }
}
