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

package org.scilab.modules.xpad;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.FontMetrics;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.awt.Toolkit;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;

import java.util.Map;
import java.util.Stack;

import javax.swing.JPanel;
import javax.swing.border.Border;
import javax.swing.border.CompoundBorder;
import javax.swing.border.EmptyBorder;
import javax.swing.border.MatteBorder;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.text.Element;

import org.scilab.modules.xpad.utils.ConfigXpadManager;

/**
 * This class will display line numbers for a related text component. The text
 * component must use the same line height for each line. TextLineNumber
 * supports wrapped lines and will highlight the line number of the current
 * line in the text component.
 *
 * This class was designed to be used as a component added to the row header
 * of a JScrollPane.
 * @author Calixte DENIZET
 */
public class XpadLineNumberPanel extends JPanel implements CaretListener, DocumentListener {

    private static final int PANELGAPSIZE = 10; 
    private static final Border OUTER = new MatteBorder(0, 0, 0, 2, Color.GRAY);       
    private static final int HEIGHT = Integer.MAX_VALUE - 1000000;
    private static Map desktopFontHints;

    private ScilabEditorPane textPane;
    
    private int borderGap;
    private Color currentLineForeground;
    private boolean isHighlighted;
    //private Color alternColor = new Color(240, 240, 240);
    private Color currentColor = Color.GRAY;

    private int numbers;
    private int lastLine;
    
    private ScilabDocument doc;
    private FontMetrics metrics;
    private int ascent;
    private int availableWidth;
    
    private int[] lineNumber;
    private boolean whereami;

    /**
     *	Create a line number component for a text component.
     *  @param textPane the related text component
     */
    public XpadLineNumberPanel(ScilabEditorPane textPane) {
	this.textPane = textPane;
	this.doc = (ScilabDocument) textPane.getDocument();
	setOpaque(true);
	setFont(textPane.getFont());
	setBorderGap(PANELGAPSIZE);
	setCurrentLineForeground(Color.RED);
	updateFont(ConfigXpadManager.getFont());
	doc.addDocumentListener(this);
	textPane.addCaretListener(this);
    }

    /**
     * Set a line numbering compatible with the whereami function
     * @param b true to be compatible with whereami
     */
    public void setWhereamiLineNumbering(boolean b) {
	whereami = b;
    }

    /**
     * @return true if whereami-compatible
     */
    public boolean getWhereamiLineNumbering() {
	return whereami;
    }

    /**
     * Update the font used in this component
     * @param font the font to use
     */
    public void updateFont(Font font) {
	setFont(font);
	metrics = textPane.getFontMetrics(font);
	ascent = metrics.getAscent();
	updateWidth();
    }
    
    /**
     *  Gets the current line rendering Color
     *
     *  @return the Color used to render the current line number
     */
    public Color getCurrentLineForeground() {
	if (currentLineForeground == null) {
	    return getForeground();
	} else {
	    return currentLineForeground;
	}
    }
    
    /**
     *  The Color used to render the current line numbers. Default is Coolor.RED.
     *
     *  @param currentLineForeground  the Color used to render the current line
     */
    public void setCurrentLineForeground(Color currentLineForeground) {
	this.currentLineForeground = currentLineForeground;
    }
    
    /**
     * Update the width of this component in using the number of digits used
     */
    public void updateWidth() {
	++numbers;
	Insets insets = getInsets();
	int width = metrics.charWidth('0') * numbers;
	availableWidth = width;
	Dimension d = getPreferredSize();
	d.setSize(availableWidth, HEIGHT);
	availableWidth = width;
	setPreferredSize(d);
	setSize(d);
    }

    /**
     * Draw the line numbers
     * @param g the graphics where to paint
     */
    public void paintComponent(Graphics g) {
	super.paintComponent(g);
	
	if (desktopFontHints == null) {
	    desktopFontHints = (Map) (Toolkit.getDefaultToolkit().getDesktopProperty("awt.font.desktophints"));
	} else {
	    ((Graphics2D) g).addRenderingHints(desktopFontHints);
	}

	synchronized (doc) {
	    Element root = doc.getDefaultRootElement();
	    ScilabView view = doc.getView();
	    Rectangle clip = g.getClipBounds();
	    Point pt = new Point(0, clip.y);
	    int rowStartOffset = textPane.viewToModel(pt);
	    pt.y += clip.height;
	    int endOffset = textPane.viewToModel(pt);
	    int lineEnd = root.getElementIndex(endOffset);
	   
	    for (int line = root.getElementIndex(rowStartOffset); line <= lineEnd; line++) {
		String str;
		if (whereami && lineNumber != null) {
		    str = Integer.toString(lineNumber[line]);
		    //g.fillRect(0, view.getLineAllocation(line), availableWidth, metrics.getHeight());
		} else {
		    str = Integer.toString(line + 1);
		}

		if (line != lastLine) {
		    g.setColor(getForeground());
		} else {
		    g.setColor(getCurrentLineForeground());
		}

		int diff = (availableWidth - metrics.stringWidth(str)) / 2;
		if (diff <= 0) {
		    updateWidth();
		    diff = (availableWidth - metrics.stringWidth(str)) / 2;
		}

		g.drawString(str, diff, view.getLineAllocation(line) + ascent);
	    }
	}
    }
    
    /**
     * Update this component if the caret changed of line
     * @param e the event
     */
    public void caretUpdate(CaretEvent e) {
	Element root = doc.getDefaultRootElement();
	int currentLine = root.getElementIndex(textPane.getCaretPosition());

	if (lastLine != currentLine) {
	    lastLine = currentLine;
	    repaint();
	}
    }   

    /**
     * Useful method to determinate the number of the lines in being compatible
     * with the whereami function
     * @param p0 start position in the doc
     * @param p1 end position in the doc
     */
    private void updateLineNumber(int p0, int p1) {
	synchronized (doc) {
	    Stack<Integer> stk = new Stack();
	    Element root = doc.getDefaultRootElement();
	    int nlines = root.getElementCount();
	    lineNumber = new int[nlines + 1];
	    lineNumber[0] = 1;
	    int current = 1;
	    for (int i = 0; i < nlines; i++) {
		Element elem = root.getElement(i);
		if (elem instanceof ScilabDocument.ScilabLeafElement) {
		    int type = ((ScilabDocument.ScilabLeafElement) elem).getType();
		    switch (type) {
		    case ScilabDocument.ScilabLeafElement.NOTHING :
			lineNumber[i] = current++;		
			break;
		    case ScilabDocument.ScilabLeafElement.FUN :
			stk.push(new Integer(current));
			current = 2;
			lineNumber[i] = 1;
			break;
		    case ScilabDocument.ScilabLeafElement.ENDFUN :
			lineNumber[i] = current++;
			if (!stk.empty()) {
			    current = stk.pop().intValue() + lineNumber[i];
			}
			break;
		    default :
			break;
		    }
		} else {
		    lineNumber[i + 1] = lineNumber[i] + 1;
		}
	    }
	} 
    }

    /**
     * Nothing !
     * @param e the event
     */
    public void changedUpdate(DocumentEvent e) { }

    /**
     * Called when an insertion is made in the doc
     * @param e the event
     */
    public void insertUpdate(DocumentEvent e) {
	handleEvent(e.getOffset(), e.getLength());
    }
    
    /**
     * Called when a remove is made in the doc
     * @param e the event
     */
    public void removeUpdate(DocumentEvent e) {
	handleEvent(e.getOffset(), e.getLength());
    }
    
    /** 
     * Update the line numbering on a change in the document
     * @param offset offset where the event occured
     * @param length length of inserted or removed text
     */
    private void handleEvent(int offset, int length) {
	if (whereami) {
	    Element root = doc.getDefaultRootElement();
	    Element line = root.getElement(root.getElementIndex(offset));
	    if (line instanceof ScilabDocument.ScilabLeafElement) {
		((ScilabDocument.ScilabLeafElement) line).resetType();
		updateLineNumber(offset, offset + length);
	    }
	}
	repaint();
    }

    /**
     *  The border gap is used in calculating the left and right insets of the
     *  border. Default value is 5.
     *  @param borderGap  the gap in pixels
     */
    private void setBorderGap(int borderGap) {
	this.borderGap = borderGap;
	Border inner = new EmptyBorder(0, borderGap, 0, borderGap);
	setBorder(new CompoundBorder(OUTER, inner));
    }
    

}
