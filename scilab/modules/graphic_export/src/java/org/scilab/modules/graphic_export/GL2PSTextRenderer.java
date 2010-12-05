/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.graphic_export;

import java.awt.Font;

import javax.media.opengl.GL;
import org.scilab.modules.renderer.utils.textRendering.SciTextRenderer;
import org.scilab.modules.renderer.textDrawing.SpecialTextObjectGL;
import org.scilab.modules.renderer.textDrawing.TeXObjectGL;
import org.scilab.modules.renderer.textDrawing.MathMLObjectGL;

import java.io.ByteArrayOutputStream;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;

import org.apache.batik.svggen.SVGGraphics2D;
import org.apache.batik.svggen.SVGGraphics2DIOException;
import org.apache.batik.dom.GenericDOMImplementation;

import org.w3c.dom.DOMImplementation;

import com.sun.opengl.util.j2d.TextRenderer;


/**
 * GL2PSTextRenderer
 * @author Sylvestre Koumar
 *
 */
public class GL2PSTextRenderer extends SciTextRenderer {


        /**
         * Constructor from a Font to use.
         * @param fontSize font Size of the font
         * @param renderer mapped text renderer
         */
        public GL2PSTextRenderer(TextRenderer renderer, float fontSize) {
                super(renderer, fontSize);
        }

        /**
         * Display a string at the desired 3D location.
         * (x,y,z) is the baseline of the leftmost character.
         * @param gl current OpenGL pipeline
         * @param str string to draw
         * @param x X coordinate of the text
         * @param y Y coordinate of the text
         * @param z Z coordinate of the text
         * @param angle angle of the text to draw
         */
        public void draw3D(GL gl, String sstr, double x, double y, double z, double angle) {
                //Put the text on the figure
                String rendererType = getRendererType();
                GL2PS gl2ps = new GL2PS();
                String str = sstr;
                boolean mathml = false;
                if (str.length() > 0 && rendererType.equals("latex") && str.charAt(0) != '$') {
                    str = "$" + str + "$";
                } else if (str.length() > 0 && rendererType.equals("mathml") && str.charAt(0) == '<') {
                    mathml = true;
                }

                gl.glRasterPos3d(x, y, z);
                if (str.length() > 0 && (mathml || str.charAt(0) == '$')) {
                        SpecialTextObjectGL spe = getSpeRenderer().getContent(str);
                        if (spe == null) {
                            gl2ps.gl2psTextOpt(convertInSVGString(str), getFontPSName(getFont()),
                                                   (short) getFont().getSize(), GL2PS.GL2PS_TEXT_BL,
                                                   (float) Math.toDegrees(angle));
                        } else {
                            String svgCode;
                            if (str.charAt(0) == '<') {
                                    svgCode = new MathMLObjectSVG((MathMLObjectGL) spe).getCode();
                            } else {
                                    svgCode = new TeXObjectSVG((TeXObjectGL) spe).getCode();
                            }
                            /* the fontsize is set to 0 to include directly the svg code (see gl2ps.c) */
                            gl2ps.gl2psTextOpt(svgCode, getFontPSName(getFont()), (short) 0, GL2PS.GL2PS_SVG, (float) Math.toDegrees(angle));
                        }

                        return;
                }

                gl2ps.gl2psTextOpt(convertInSVGString(str), getFontPSName(getFont()),
                                   (short) getFont().getSize(), GL2PS.GL2PS_TEXT_BL,
                                   (float) Math.toDegrees(angle));
        }

        /**
         * Get the postscript name of a Java font.
         * This method should be used instead of the getPSName
         * of class Font. THe one provided by Java does not
         * work with GL2PS.
         * @param font AWT Font
         * @return postscript name of the font.
         */
        protected String getFontPSName(Font font) {

                /* A Postscript font is made as following */
                /* <fontFamily>-<option1><option2> */
                /* <fontFamily> name of the font family */
                /* <optionI> Bold, Italic or Oblique */

                String res = font.getFamily();
                /* add options at the end */
                if (font.isBold() && font.isItalic()) {
                        res += "-BoldItalic";
                } else if (font.isBold()) {
                        res += "-Bold";
                } else if (font.isItalic()) {
                        res += "-Italic";
                }

                return res;
        }

        /**
         * Convert a string into a another one where special
         * characters are replaced by HTML entities/
         * @param str the string to convert
         * @return the converted string.
         */
        private String convertInSVGString(String str) {
                /* Fix bug 6718 */
                DOMImplementation dom = GenericDOMImplementation.getDOMImplementation();
                SVGGraphics2D g2d = new SVGGraphics2D(dom.createDocument("", "svg", null));
                ByteArrayOutputStream buf = new ByteArrayOutputStream();

                g2d.drawString(str, 0, 0);

                try {
                    g2d.stream(new OutputStreamWriter(buf, "UTF-8"), true, true);
                } catch (SVGGraphics2DIOException e) {
                        System.err.println(e.toString());
                } catch (UnsupportedEncodingException e) {
                        System.err.println(e.toString());
                }

                String code = buf.toString();
                int pos = code.indexOf("</text");
                code = code.substring(code.lastIndexOf(">", pos) + 1, pos);
                g2d.dispose();

                return code;
        }
}
