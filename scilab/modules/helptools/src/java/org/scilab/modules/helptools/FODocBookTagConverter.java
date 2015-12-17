package org.scilab.modules.helptools;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;
import java.util.Map.Entry;

import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.sax.SAXResult;
import javax.xml.transform.stream.StreamSource;

import org.apache.fop.apps.Fop;
import org.apache.fop.apps.FopFactory;
import org.apache.fop.apps.FormattingResults;
import org.apache.fop.apps.MimeConstants;
import org.scilab.forge.jlatexmath.fop.JLaTeXMathElementMapping;
import org.scilab.forge.jlatexmath.fop.JLaTeXMathXMLHandler;
import org.scilab.modules.helptools.image.Image;
import org.scilab.modules.helptools.image.ImageConverter;
import org.xml.sax.SAXException;

public class FODocBookTagConverter extends DocbookTagConverter {

    public static final String FO_DOCUMENT_HEADER = "<?xml version='1.0' encoding='UTF-8'?>\n"
            + "<fo:root xmlns:fo='http://www.w3.org/1999/XSL/Format' font-family='serif,Symbol,ZapfDingbats' font-size='10pt' text-align='justify' line-height='normal' font-selection-strategy='character-by-character' line-height-shift-adjustment='disregard-shifts' writing-mode='lr-tb' language='en'>\n"
            + "	<fo:layout-master-set>\n"
            + "		<fo:simple-page-master master-name='blank' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body display-align='center' margin-bottom='0.5in' margin-top='0.5in' margin-left='0in' margin-right='0in' region-name='blank-body'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-blank' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-blank' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-blank' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-blank' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='titlepage-first' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-first' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-first' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-first' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-first' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='titlepage-odd' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-odd' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-odd' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-odd' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-odd' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='titlepage-even' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-even' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-even' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-even' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-even' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='lot-first' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-first' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-first' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-first' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-first' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='lot-odd' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-odd' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-odd' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-odd' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-odd' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='lot-even' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-even' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-even' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-even' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-even' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='front-first' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-first' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-first' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-first' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-first' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='front-odd' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-odd' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-odd' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-odd' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-odd' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='front-even' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-even' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-even' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-even' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-even' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='body-first' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-first' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-first' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-first' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-first' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='body-odd' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-odd' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-odd' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-odd' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-odd' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='body-even' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-even' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-even' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-even' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-even' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='back-first' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-first' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-first' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-first' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-first' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='back-odd' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-odd' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-odd' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-odd' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-odd' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='back-even' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='1' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-even' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-even' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-even' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-even' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='index-first' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='2' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-first' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-first' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-first' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-first' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='index-odd' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='2' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-odd' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-odd' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-odd' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-odd' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n"
            + "		<fo:simple-page-master master-name='index-even' page-width='210mm' page-height='297mm' margin-top='0.5in' margin-bottom='0.5in' margin-left='1in' margin-right='1in'>\n"
            + "			<fo:region-body margin-bottom='0.5in' margin-top='0.5in' column-gap='12pt' column-count='2' margin-left='0in' margin-right='0in'/>\n"
            + "			<fo:region-before region-name='xsl-region-before-even' extent='0.4in' precedence='true' display-align='before'/>\n"
            + "			<fo:region-after region-name='xsl-region-after-even' extent='0.4in' precedence='true' display-align='after'/>\n"
            + "			<fo:region-start border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-outer-even' precedence='false' extent='0in'/>\n"
            + "			<fo:region-end border-width='0' padding='0' reference-orientation='90' region-name='xsl-region-inner-even' precedence='false' extent='0in'/>\n"
            + "		</fo:simple-page-master>\n" + "		<fo:page-sequence-master master-name='titlepage'>\n"
            + "			<fo:repeatable-page-master-alternatives>\n"
            + "				<fo:conditional-page-master-reference master-reference='blank' blank-or-not-blank='blank'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='titlepage-first' page-position='first'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='titlepage-odd' odd-or-even='odd'/>\n"
            + "				<fo:conditional-page-master-reference odd-or-even='even' master-reference='titlepage-odd'/>\n"
            + "			</fo:repeatable-page-master-alternatives>\n" + "		</fo:page-sequence-master>\n"
            + "		<fo:page-sequence-master master-name='lot'>\n"
            + "			<fo:repeatable-page-master-alternatives>\n"
            + "				<fo:conditional-page-master-reference master-reference='blank' blank-or-not-blank='blank'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='lot-first' page-position='first'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='lot-odd' odd-or-even='odd'/>\n"
            + "				<fo:conditional-page-master-reference odd-or-even='even' master-reference='lot-odd'/>\n"
            + "			</fo:repeatable-page-master-alternatives>\n" + "		</fo:page-sequence-master>\n"
            + "		<fo:page-sequence-master master-name='front'>\n"
            + "			<fo:repeatable-page-master-alternatives>\n"
            + "				<fo:conditional-page-master-reference master-reference='blank' blank-or-not-blank='blank'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='front-first' page-position='first'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='front-odd' odd-or-even='odd'/>\n"
            + "				<fo:conditional-page-master-reference odd-or-even='even' master-reference='front-odd'/>\n"
            + "			</fo:repeatable-page-master-alternatives>\n" + "		</fo:page-sequence-master>\n"
            + "		<fo:page-sequence-master master-name='body'>\n"
            + "			<fo:repeatable-page-master-alternatives>\n"
            + "				<fo:conditional-page-master-reference master-reference='blank' blank-or-not-blank='blank'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='body-first' page-position='first'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='body-odd' odd-or-even='odd'/>\n"
            + "				<fo:conditional-page-master-reference odd-or-even='even' master-reference='body-odd'/>\n"
            + "			</fo:repeatable-page-master-alternatives>\n" + "		</fo:page-sequence-master>\n"
            + "		<fo:page-sequence-master master-name='back'>\n"
            + "			<fo:repeatable-page-master-alternatives>\n"
            + "				<fo:conditional-page-master-reference master-reference='blank' blank-or-not-blank='blank'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='back-first' page-position='first'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='back-odd' odd-or-even='odd'/>\n"
            + "				<fo:conditional-page-master-reference odd-or-even='even' master-reference='back-odd'/>\n"
            + "			</fo:repeatable-page-master-alternatives>\n" + "		</fo:page-sequence-master>\n"
            + "		<fo:page-sequence-master master-name='index'>\n"
            + "			<fo:repeatable-page-master-alternatives>\n"
            + "				<fo:conditional-page-master-reference master-reference='blank' blank-or-not-blank='blank'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='index-first' page-position='first'/>\n"
            + "				<fo:conditional-page-master-reference master-reference='index-odd' odd-or-even='odd'/>\n"
            + "				<fo:conditional-page-master-reference odd-or-even='even' master-reference='index-odd'/>\n"
            + "			</fo:repeatable-page-master-alternatives>\n" + "		</fo:page-sequence-master>\n"
            + "	</fo:layout-master-set>\n"
            + "   <fo:page-sequence hyphenate='false' master-reference='body' language='en' format='1' initial-page-number='auto' force-page-count='no-force' hyphenation-character='-' hyphenation-push-character-count='2' hyphenation-remain-character-count='2'>\n"
            + "   <fo:flow flow-name='xsl-region-body' start-indent='4pc' end-indent='0pt'>\n";

    public static final String FO_DOUCMENT_FOOTER = "  </fo:flow>\n" + "  </fo:page-sequence>\n" + "</fo:root>";

    public static final String FO_BASIC_LINK = "fo:basic-link";
    public static final String FO_BLOCK = "fo:block";
    public static final String FO_EXTERNAL_GRAPHIC = "fo:external-graphic";
    public static final String FO_INLINE = "fo:inline";
    public static final String FO_TABLE = "fo:table";
    public static final String FO_TABLE_BODY = "fo:table-body";
    public static final String FO_TABLE_CELL = "fo:table-cell";
    public static final String FO_TABLE_ROW = "fo:table-row";

    public static final String REVHISTORY_COLDEF =
        "<fo:table-column column-number='1' column-width='25%' border-style='solid' border-width='1pt'/>\n" +
        "<fo:table-column column-number='2' column-width='75%' border-style='solid' border-width='1pt'/>\n";

    public static final Map<String, String> CHAPTER_TITLE_ATTRS = new HashMap<>();
    public static final Map<String, String> PART_TITLE_ATTRS = new HashMap<>();
    public static final Map<String, String> REFSECTION_TITLE_ATTRS = new HashMap<>();
    public static final Map<String, String> REFSYNOPSISDIV_TITLE_ATTRS = new HashMap<>();
    public static final Map<String, String> SECTION_TITLE_ATTRS = new HashMap<>();

    public static final Map<String, String> CAUTION_ATTRS = new HashMap<>();
    public static final Map<String, String> CAPTION_ATTRS = new HashMap<>();
    public static final Map<String, String> CODE_ATTRS = new HashMap<>();
    public static final Map<String, String> COMMAND_ATTRS = new HashMap<>();
    public static final Map<String, String> CONSTANT_ATTRS = new HashMap<>();
    public static final Map<String, String> EMPHASIS_ATTRS = new HashMap<>();
    public static final Map<String, String> FIRSTNAME_ATTRS = new HashMap<>();
    public static final Map<String, String> FUNCTION_ATTRS = new HashMap<>();
    public static final Map<String, String> IMPORTANT_ATTRS = new HashMap<>();
    public static final Map<String, String> INFORMALEQUATION_ATTRS = new HashMap<>();
    public static final Map<String, String> LITERAL_ATTRS = new HashMap<>();
    public static final Map<String, String> NOTE_ATTRS = new HashMap<>();
    public static final Map<String, String> OPTION_ATTRS = new HashMap<>();
    public static final Map<String, String> PARA_ATTRS = new HashMap<>();
    public static final Map<String, String> PROGRAMLISTING_ATTRS = new HashMap<>();
    public static final Map<String, String> PUBDATE_ATTRS = new HashMap<>();
    public static final Map<String, String> REFNAME_ATTRS = new HashMap<>();
    public static final Map<String, String> REFPURPOSE_ATTRS = new HashMap<>();
    public static final Map<String, String> REPLACEABLE_ATTRS = new HashMap<>();
    public static final Map<String, String> REVDESCRIPTION_ATTRS = new HashMap<>();
    public static final Map<String, String> REVHISTORY_ATTRS = new HashMap<>();
    public static final Map<String, String> REVISION_ATTRS = new HashMap<>();
    public static final Map<String, String> REVNUMBER_ATTRS = new HashMap<>();
    public static final Map<String, String> SCREEN_ATTRS = new HashMap<>();
    public static final Map<String, String> SUBSCRIPT_ATTRS = new HashMap<>();
    public static final Map<String, String> SUPERSCRIPT_ATTRS = new HashMap<>();
    public static final Map<String, String> SURNAME_ATTRS = new HashMap<>();
    public static final Map<String, String> SYNOPSIS_ATTRS = new HashMap<>();
    public static final Map<String, String> TERM_ATTRS = new HashMap<>();
    public static final Map<String, String> TIP_ATTRS = new HashMap<>();
    public static final Map<String, String> VARLIST_ATTRS = new HashMap<>();
    public static final Map<String, String> VARLISTENTRY_ATTRS = new HashMap<>();
    public static final Map<String, String> VARNAME_ATTRS = new HashMap<>();
    public static final Map<String, String> WARNING_ATTRS = new HashMap<>();

    public static final Map<String, String> ITEMIZEDLIST_LISTITEM_ATTRS = new HashMap<>();
    public static final Map<String, String> SIMPLELIST_LISTITEM_ATTRS = new HashMap<>();
    public static final Map<String, String> ORDEREDLIST_LISTITEM_ATTRS = new HashMap<>();
    public static final Map<String, String> VARLISTENTRY_LISTITEM_ATTRS = new HashMap<>();

    static {

        final Map<String, String> commonTitleAttrs = new HashMap<>();
        commonTitleAttrs.put("font-family", "sans-serif,Symbol,ZapfDingbats");
        commonTitleAttrs.put("font-size", "16pt");
        commonTitleAttrs.put("font-weight", "bold");
        commonTitleAttrs.put("keep-with-next.within-column", "always");
        commonTitleAttrs.put("space-after", "6pt");
        commonTitleAttrs.put("space-before", "24pt");
        CHAPTER_TITLE_ATTRS.putAll(commonTitleAttrs);
        PART_TITLE_ATTRS.putAll(commonTitleAttrs);
        REFSECTION_TITLE_ATTRS.putAll(commonTitleAttrs);
        REFSYNOPSISDIV_TITLE_ATTRS.putAll(commonTitleAttrs);
        SECTION_TITLE_ATTRS.putAll(commonTitleAttrs);

        final Map<String, String> commonVerbatimAttrs = new HashMap<>();
        commonVerbatimAttrs.put("background-color", "#E0E0E0");
        commonVerbatimAttrs.put("font-family", "monospace");
        commonVerbatimAttrs.put("hyphenate", "false");
        commonVerbatimAttrs.put("linefeed-treatment", "preserve");
        commonVerbatimAttrs.put("space-after", "6pt");
        commonVerbatimAttrs.put("space-before", "6pt");
        commonVerbatimAttrs.put("text-align", "start");
        commonVerbatimAttrs.put("white-space-collapse", "false");
        commonVerbatimAttrs.put("white-space-treatment", "preserve");
        commonVerbatimAttrs.put("wrap-option", "no-wrap");
        PROGRAMLISTING_ATTRS.putAll(commonVerbatimAttrs);
        SCREEN_ATTRS.putAll(commonVerbatimAttrs);
        SYNOPSIS_ATTRS.putAll(commonVerbatimAttrs);

        LITERAL_ATTRS.put("font-style", "italic");

        PARA_ATTRS.put("space-after", "6pt");

        REFNAME_ATTRS.put("font-family", "sans-serif,Symbol,ZapfDingbats");
        REFNAME_ATTRS.put("font-size", "24pt");
        REFNAME_ATTRS.put("font-weight", "bold");
        REFNAME_ATTRS.put("text-align", "center");

        REFPURPOSE_ATTRS.put("font-family", "sans-serif,Symbol,ZapfDingbats");
        REFPURPOSE_ATTRS.put("font-size", "16pt");
        REFPURPOSE_ATTRS.put("font-weight", "bold");
        REFPURPOSE_ATTRS.put("text-align", "center");
        REFPURPOSE_ATTRS.put("space-after", "36pt");

        SUBSCRIPT_ATTRS.put("font-size", "8pt");
        SUBSCRIPT_ATTRS.put("vertical-align", "sub");

        SUPERSCRIPT_ATTRS.put("font-size", "8pt");
        SUPERSCRIPT_ATTRS.put("vertical-align", "sub");

        TERM_ATTRS.put("keep-together.within-column", "always");
        TERM_ATTRS.put("keep-with-next.within-column", "always");
        TERM_ATTRS.put("space-after", "6pt");
        TERM_ATTRS.put("space-before", "6pt");

        VARLIST_ATTRS.put("space-after", "6pt");
        VARLIST_ATTRS.put("space-before", "6pt");

        VARLISTENTRY_ATTRS.put("keep-together.within-column", "always");
        VARLISTENTRY_ATTRS.put("keep-with-next.within-column", "always");
        VARLISTENTRY_ATTRS.put("space-before", "6pt");

        VARLISTENTRY_LISTITEM_ATTRS.put("margin-left", "0.25in");
    }

    private final String outputDirectory;
    private final String language;
    private final Backend format;
    private final String foFileName;
    private String buffer;

    public FODocBookTagConverter(SciDocMain sciDocMain, String inName, DocbookElement elem, ImageConverter imgConvert)
    throws IOException {
        super(inName, elem, imgConvert);
        this.outputDirectory = sciDocMain.getOutputDirectory();
        this.language = sciDocMain.getLanguage();
        this.format = sciDocMain.getFormat();
        this.foFileName = Helpers.getTemporaryNameFo(sciDocMain.getOutputDirectory());
    }

    public FODocBookTagConverter(SciDocMain sciDocMain, String in, ImageConverter imgConvert) throws IOException {
        super(in, imgConvert);
        this.outputDirectory = sciDocMain.getOutputDirectory();
        this.language = sciDocMain.getLanguage();
        this.format = sciDocMain.getFormat();
        this.foFileName = Helpers.getTemporaryNameFo(sciDocMain.getOutputDirectory());
    }

    @Override
    public void convert() throws SAXException, IOException {
        super.convert();
        String baseName = Helpers.getBaseName(language);
        String fileName = outputDirectory + "/" + baseName + "." + format.name().toLowerCase();

        createFOFile(foFileName, buffer);

        /* Only generate .fo */
        if (format == Backend.FO) {
            return;
        }

        /* Call FOP */
        try (OutputStream out = new BufferedOutputStream(new FileOutputStream(fileName));) {
            FopFactory fopFactory = FopFactory.newInstance(new File(System.getenv("SCI") + "/modules/helptools/etc/fopconf.xml"));
            fopFactory.addElementMapping(new JLaTeXMathElementMapping());
            fopFactory.getXMLHandlerRegistry().addXMLHandler(new JLaTeXMathXMLHandler());

            final Fop fop;
            switch (format) {
                case PS:
                    fop = fopFactory.newFop(MimeConstants.MIME_POSTSCRIPT, out);
                    break;
                case PDF:
                    fop = fopFactory.newFop(MimeConstants.MIME_PDF, out);
                    break;

                default:
                    throw new IOException(String.format("%s is not a supported format.\n", format));
            }

            TransformerFactory factory = TransformerFactory.newInstance();
            Transformer transformer = factory.newTransformer(); // identity transformer
            Source src = new StreamSource(new File(foFileName));
            Result res = new SAXResult(fop.getDefaultHandler());
            transformer.transform(src, res);

            FormattingResults foResults = fop.getResults();
            System.out.println("Generated " + foResults.getPageCount() + " pages in total.");
        } catch (TransformerException e) {
            System.out.println(e.getLocalizedMessage());
        }
    }

    public void createFOFile(final String fileName, final String contents) {
        try (FileWriter writer = new FileWriter(foFileName);) {
            writer.append(FO_DOCUMENT_HEADER);
            writer.append(contents);
            writer.append(FO_DOUCMENT_FOOTER);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private String generateAttributeList(Map<String, String> attrs) {
        if (attrs == null || attrs.size() == 0) {
            return "";
        }
        StringBuilder sb = new StringBuilder();
        if (!attrs.isEmpty()) {
            sb.append(' ');
        }
        for (Entry<String, String> e : attrs.entrySet()) {
            sb.append(e.getKey());
            sb.append("=\"");
            sb.append(e.getValue());
            sb.append("\" ");
        }
        return sb.toString();
    }

    private String generateXMLElement(String name, String content, Map<String, String> attrs,
                                      Map<String, String> extraAttrs) {
        StringBuilder sb = new StringBuilder();
        sb.append('<');
        sb.append(name);
        sb.append(generateAttributeList(attrs));
        sb.append(generateAttributeList(extraAttrs));
        if (content == null || content.trim().equals("")) {
            sb.append("/>");
        } else {
            sb.append('>');
            sb.append(content);
            sb.append("</");
            sb.append(name);
            sb.append('>');
        }
        return sb.toString();
    }

    private String generateXMLElement(String name, String content, Map<String, String> attrs) {
        return generateXMLElement(name, content, attrs, null);
    }

    private String generateXMLElement(String name, Map<String, String> attrs) {
        return generateXMLElement(name, null, attrs);
    }

    private String escapeXMLContent(String content) {
        return content.replaceAll("&", "&amp;").replaceAll("'", "&apos;").replaceAll("\"", "&quot;");
    }

    @Override
    public String generateImageCode(String code, String fileName, Map<String, String> attrs) {
        Map<String, String> outputAttributes = new HashMap<>();
        outputAttributes.put("src", "url(" + escapeXMLContent(fileName) + ")");
        return generateXMLElement(FO_EXTERNAL_GRAPHIC, outputAttributes);
    }

    @Override
    public String generateImageCode(Image img, String fileName, Map<String, String> attrs) {
        Map<String, String> outputAttributes = new HashMap<>();
        outputAttributes.put("src", "url(" + escapeXMLContent(fileName) + ")");
        return generateXMLElement(FO_EXTERNAL_GRAPHIC, outputAttributes);
    }

    @Override
    public String generateImageCode(String fileName, Map<String, String> attrs) {
        Map<String, String> outputAttributes = new HashMap<>();
        outputAttributes.put("src", "url(" + escapeXMLContent(fileName) + ")");
        return generateXMLElement(FO_EXTERNAL_GRAPHIC, outputAttributes);
    }

    @Override
    public String handleAnswer(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, null);
    }

    @Override
    public String handleBibliomixed(Map<String, String> attributes, String contents) throws SAXException {
        String id = attributes.get("id");
        return (id != null) ? generateXMLElement(FO_BASIC_LINK, contents, Collections.singletonMap("id", id))
               : generateXMLElement(FO_INLINE, contents, null);
    }

    @Override
    public String handleBibliomset(Map<String, String> attributes, String contents) throws SAXException {
        String id = attributes.get("id");
        return (id != null) ? generateXMLElement(FO_BASIC_LINK, contents, Collections.singletonMap("id", id))
               : generateXMLElement(FO_INLINE, contents, null);
    }

    @Override
    public String handleBook(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleCaption(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, CAPTION_ATTRS);
    }

    @Override
    public String handleCaution(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, CAUTION_ATTRS);
    }

    @Override
    public String handleChapter(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleCode(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, CODE_ATTRS);
    }

    @Override
    public String handleCommand(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, COMMAND_ATTRS);
    }

    @Override
    public String handleConstant(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, CONSTANT_ATTRS);
    }

    @Override
    public String handleEmphasis(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, EMPHASIS_ATTRS);
    }

    @Override
    public String handleFirstname(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, FIRSTNAME_ATTRS);
    }

    @Override
    public String handleFunction(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, FUNCTION_ATTRS);
    }

    @Override
    public String handleImagedata(Map<String, String> attributes, String contents) throws SAXException {
        Map<String, String> outputAttributes = new HashMap<>();
        outputAttributes.put("src", "url(" + escapeXMLContent(attributes.get("fileref")) + ")");
        return generateXMLElement(FO_EXTERNAL_GRAPHIC, outputAttributes);
    }

    @Override
    public String handleImageobject(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleImportant(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, IMPORTANT_ATTRS);
    }

    @Override
    public String handleInfo(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleInformalequation(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, INFORMALEQUATION_ATTRS);
    }

    @Override
    public String handleInformaltable(Map<String, String> attributes, String contents) throws SAXException {
        final String body = generateXMLElement(FO_TABLE_BODY, contents, null);
        return generateXMLElement(FO_TABLE, body, null);
    }

    @Override
    public String handleInlinemediaobject(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleItemizedlist(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleLatex(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleLink(Map<String, String> attributes, String contents) throws SAXException {
        Map<String, String> outputAttributes = new HashMap<>();
        outputAttributes.put("internal-destination", escapeXMLContent(attributes.get("linkend")));
        String link = generateXMLElement(FO_BASIC_LINK, contents, outputAttributes);
        if (getParentTagName().equals("member")) {
            return generateXMLElement(FO_BLOCK, link, null);
        } else {
            return link;
        }
    }

    @Override
    public String handleListitem(Map<String, String> attributes, String contents) throws SAXException {
        switch (getParentTagName()) {
            case "itemizedlist":
                generateXMLElement(FO_BLOCK, contents, ITEMIZEDLIST_LISTITEM_ATTRS);
                break;
            case "orderedlist":
                generateXMLElement(FO_BLOCK, contents, ORDEREDLIST_LISTITEM_ATTRS);
                break;
            case "simplelist":
                generateXMLElement(FO_BLOCK, contents, SIMPLELIST_LISTITEM_ATTRS);
                break;
            case "varlistentry":
                generateXMLElement(FO_BLOCK, contents, VARLISTENTRY_LISTITEM_ATTRS);
                break;
            default:
                break;
        }
        return "";
    }

    @Override
    public String handleLiteral(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, LITERAL_ATTRS);
    }

    @Override
    public String handleMediaobject(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleMember(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleNote(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, NOTE_ATTRS);
    }

    @Override
    public String handleOption(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, OPTION_ATTRS);
    }

    @Override
    public String handleOrderedlist(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handlePara(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, PARA_ATTRS);
    }

    @Override
    public String handlePart(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleProgramlisting(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, PROGRAMLISTING_ATTRS);
    }

    @Override
    public String handlePubdate(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, PUBDATE_ATTRS);
    }

    @Override
    public String handleQandaentry(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleQandaset(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleQuestion(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleRefentry(Map<String, String> attributes, String contents) throws SAXException {
        buffer = contents;
        return null;
    }

    @Override
    public String handleRefnamediv(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleRefname(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, REFNAME_ATTRS);
    }

    @Override
    public String handleRefpurpose(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, REFPURPOSE_ATTRS);
    }

    @Override
    public String handleRefsection(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleRefsynopsisdiv(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleReplaceable(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, REPLACEABLE_ATTRS);
    }

    @Override
    public String handleRevdescription(Map<String, String> attributes, String contents) throws SAXException {
        final String cellContent = generateXMLElement(FO_BLOCK, contents, null);
        return generateXMLElement(FO_TABLE_CELL, cellContent, REVDESCRIPTION_ATTRS);
    }

    @Override
    public String handleRevhistory(Map<String, String> attributes, String contents) throws SAXException {
        final String body = REVHISTORY_COLDEF + generateXMLElement(FO_TABLE_BODY, contents, null);
        return generateXMLElement(FO_TABLE, body, REVHISTORY_ATTRS);
    }

    @Override
    public String handleRevision(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_TABLE_ROW, contents, REVISION_ATTRS);
    }

    @Override
    public String handleRevnumber(Map<String, String> attributes, String contents) throws SAXException {
        final String cellContent = generateXMLElement(FO_BLOCK, contents, null);
        return generateXMLElement(FO_TABLE_CELL, cellContent, REVNUMBER_ATTRS);
    }

    @Override
    public String handleRevremark(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleScreen(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, SCREEN_ATTRS);
    }

    @Override
    public String handleScreenshot(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleSection(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleSimplelist(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleSubscript(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, SUBSCRIPT_ATTRS);
    }

    @Override
    public String handleSuperscript(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, SUPERSCRIPT_ATTRS);
    }

    @Override
    public String handleSurname(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, SURNAME_ATTRS);
    }

    @Override
    public String handleSynopsis(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, SYNOPSIS_ATTRS);
    }

    @Override
    public String handleTable(Map<String, String> attributes, String contents) throws SAXException {
        final String body = generateXMLElement(FO_TABLE_BODY, contents, null);
        return generateXMLElement(FO_TABLE, body, null);
    }

    @Override
    public String handleTbody(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleTd(Map<String, String> attributes, String contents) throws SAXException {
        final String cellContent = generateXMLElement(FO_BLOCK, contents, null);
        return generateXMLElement(FO_TABLE_CELL, cellContent, null);
    }

    @Override
    public String handleTerm(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, TERM_ATTRS);
    }

    @Override
    public String handleTh(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleTip(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, TIP_ATTRS);
    }

    @Override
    public String handleTitle(Map<String, String> attributes, String contents) throws SAXException {
        switch (getParentTagName()) {
            case "chapter":
                return generateXMLElement(FO_BLOCK, contents, CHAPTER_TITLE_ATTRS);
            case "part":
                return generateXMLElement(FO_BLOCK, contents, PART_TITLE_ATTRS);
            case "info":
                break;
            case "section":
                return generateXMLElement(FO_BLOCK, contents, SECTION_TITLE_ATTRS);
            case "refsection":
                return generateXMLElement(FO_BLOCK, contents, REFSECTION_TITLE_ATTRS);
            case "refsynopsisdiv":
                return generateXMLElement(FO_BLOCK, contents, REFSYNOPSISDIV_TITLE_ATTRS);
            default:
                break;
        }
        return null;
    }

    @Override
    public String handleTr(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_TABLE_ROW, contents, null);
    }

    @Override
    public String handleUlink(Map<String, String> attributes, String contents) throws SAXException {
        final Map<String, String> outputAttributes = Collections.singletonMap("external-destination",
                attributes.get("url"));
        return generateXMLElement(FO_BASIC_LINK, contents, outputAttributes);
    }

    @Override
    public String handleVariablelist(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleVarlistentry(Map<String, String> attributes, String contents) throws SAXException {
        return contents;
    }

    @Override
    public String handleVarname(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_INLINE, contents, VARNAME_ATTRS);
    }

    @Override
    public String handleWarning(Map<String, String> attributes, String contents) throws SAXException {
        return generateXMLElement(FO_BLOCK, contents, WARNING_ATTRS);
    }

    @Override
    public String handleXref(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleAnchor(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }

    @Override
    public String handleLiterallayout(Map<String, String> attributes, String contents) throws SAXException {
        // TODO Auto-generated method stub
        return null;
    }
}
