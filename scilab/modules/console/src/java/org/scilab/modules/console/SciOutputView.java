/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.console;

import java.awt.Dimension;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DropTarget;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.io.Writer;
import java.util.LinkedList;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

import javax.swing.BorderFactory;
import javax.swing.JEditorPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;
import javax.swing.SwingUtilities;
import javax.swing.text.BadLocationException;
import javax.swing.text.JTextComponent;
import javax.swing.text.PlainDocument;
import javax.swing.text.StyleContext;
import javax.swing.text.StyledDocument;

import org.scilab.modules.commons.gui.ScilabCaret;

import com.artenum.rosetta.interfaces.ui.OutputView;
import com.artenum.rosetta.util.BufferedWriter;
import com.artenum.rosetta.util.StringConstants;

/**
 * Scilab Console UI which contains the previous commands and their outputs
 *
 * @author Vincent COUVERT
 */
public class SciOutputView extends JEditorPane implements OutputView {
    private static final long serialVersionUID = 1L;

    private static final int TOP_BORDER = 0;

    private static final int BOTTOM_BORDER = 0;

    private static final int LEFT_BORDER = 0;

    private static final int RIGHT_BORDER = 0;

    private static final int BUFFER_SIZE = 10;

    private String activeStyle;

    private String lastAppendedStyle;

    private BlockingQueue<StringBuffer> bufferQueue;

    private LinkedList<String> styleQueue;

    private StringBuffer currentWorkingBuffer;

    private SciConsole console;

    private Thread thread;

    private int insertPosition;

    private int maxNumberOfLines;

    /**
     * Constructor
     */
    public SciOutputView() {
        super();

        /* A PlainDocument contains only "box" for lines not for all characters (as in a StyledDocument)
           so there are less boxes to explore in a PlainDocument... */
        setDocument(new PlainDocument());
        setMaxSize(10000);
        setBorder(BorderFactory.createEmptyBorder(TOP_BORDER, LEFT_BORDER, BOTTOM_BORDER, RIGHT_BORDER));

        // Enabled Drag&Drop with this component
        this.setDragEnabled(true);
        this.setDoubleBuffered(true);

        activeStyle = StyleContext.DEFAULT_STYLE;
        bufferQueue = new ArrayBlockingQueue<StringBuffer>(BUFFER_SIZE);
        styleQueue = new LinkedList<String>();
        
        /**
         * Default caret for output view (to handle paste actions using middle button)
         * @author Vincent COUVERT
         */
        final class FixedCaret extends ScilabCaret {

			private static final long serialVersionUID = 8230195712653828841L;

			/**
             * Constructor
             */
            private FixedCaret() {
                super(SciOutputView.this);
            }

            /**
             * Manages mouse clicks
             * @param e the event
             * @see javax.swing.text.DefaultCaret#mouseClicked(java.awt.event.MouseEvent)
             */
            public void mouseClicked(MouseEvent e) {
                if (SwingUtilities.isMiddleMouseButton(e) && e.getClickCount() == 1) {
                    /*** PASTE USING MIDDLE BUTTON ***/
                    JTextComponent c = (JTextComponent) e.getSource();
                    if (c != null) {
                        Toolkit tk = c.getToolkit();
                        Clipboard buffer = tk.getSystemSelection();
                        if (buffer != null) {
                            Transferable trans = buffer.getContents(null);
                            if (trans.isDataFlavorSupported(DataFlavor.stringFlavor)) {
                                try {
                                    String pastedText = (String) trans.getTransferData(DataFlavor.stringFlavor);
                                    ((JTextPane) getConsole().getConfiguration().getInputCommandView()).replaceSelection(pastedText);
                                } catch (UnsupportedFlavorException e1) {
                                    e1.printStackTrace();
                                } catch (IOException e1) {
                                    e1.printStackTrace();
                                }
                            }
                        }
                    }
                } else if (SwingUtilities.isLeftMouseButton(e) && e.getClickCount() == 1) {
                    /*** SEND THE FOCUS TO THE INPUT COMMAND VIEW ***/
                    ((JTextPane) getConsole().getConfiguration().getInputCommandView()).requestFocus();
                    ((JTextPane) getConsole().getConfiguration().getInputCommandView()).getCaret().setVisible(true);
                } else {
                    /*** DELEGATE TO THE SYSTEM ***/
                    super.mouseClicked(e);
                }
            }
        }

        // Set the caret
        setCaret(new FixedCaret());
        // Selection is forced to be visible because the component is not editable
        getCaret().setSelectionVisible(true);
    }

    /**
     * @param styledDocument
     */
    public void setStyledDocument(StyledDocument styledDocument) { }

    /**
     * Display a buffer entry in the console
     * @param buff the string  to write
     * @param style the style to use to format the string
     */
    private void displayLineBuffer(String buff, String style) {
        int sDocLength = getDocument().getLength();

        if (buff.equals("\r")) {
            /* If \r sent by mprintf then display nothing but prepare next display */
            /* Insertion will be done just after last NEW_LINE */
            try {
                String outputTxt = getDocument().getText(0, sDocLength);
                insertPosition = outputTxt.lastIndexOf(StringConstants.NEW_LINE) + 1;
            } catch (BadLocationException e) {
                e.printStackTrace();
            }
            return;
        } else {
            /* Change position for insertion if a previous \r still influence display */
            if ((insertPosition != 0) && (insertPosition < sDocLength)) {
                sDocLength = insertPosition;
                try {
                    /* Remove chars to be replaced */
                    if (insertPosition + buff.length() <= getDocument().getLength()) {
                        getDocument().remove(insertPosition, buff.length());
                    } else {
                        /* Remove end of line */
                        getDocument().remove(insertPosition, getDocument().getLength() - insertPosition);
                    }
                } catch (BadLocationException e) {
                    e.printStackTrace();
                }
            } else {
                /* Reinit insertPosition: 0 is equivalent to insertPosition value ignored */
                insertPosition = 0;
            }
        }

        try {
            getDocument().insertString(sDocLength, buff, null);
            /* Move insertPosition to the end of last inserted data */
            if (insertPosition != 0) {
                insertPosition += buff.length();
            }
        } catch (BadLocationException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

        int count = getDocument().getDefaultRootElement().getElementCount();
        if (count > 1.5 * maxNumberOfLines) {
            /* A removal is costly: array copy and with a gap buffer that leads to two array copies (when remove is followed by an insert).
               So the idea is to minimize the number of removal: a removal only when 0.5*maxNumberOfLines useless lines are entered.
            */
            try {
                getDocument().remove(0, getDocument().getDefaultRootElement().getElement(count - maxNumberOfLines - 1).getEndOffset());
            } catch (BadLocationException e) {
                e.printStackTrace();
            }
        }

        /* Special case for Scilab when clc or tohome have been used */
        String[] lines = buff.split(StringConstants.NEW_LINE);

        /* Change the size of the input command view if necessary */
        /* - if the console size has been forced to a value */
        /* - if a carriage return has been appended */
        if (console != null && console.getInputCommandViewSizeForced() && lines.length > 0) {
            JEditorPane outputView = ((JEditorPane) console.getConfiguration().getOutputView());

            // Get JScrollPane viewport size to adapt input command
            // view size
            JScrollPane jSP = console.getJScrollPane();
            Dimension jSPExtSize = jSP.getViewport().getExtentSize();

            /* Height of a text line in the ouput view */
            int charHeight = outputView.getFontMetrics(outputView.getFont()).getHeight();
            JPanel promptView = ((JPanel) console.getConfiguration().getPromptView());

            /* Input command view dimensions */
            JTextPane inputCmdView = ((JTextPane) console.getConfiguration().getInputCommandView());
            int height = inputCmdView.getPreferredSize().height;
            int width = inputCmdView.getPreferredSize().width - jSPExtSize.width;

            int promptViewHeight = promptView.getPreferredSize().height;

            /* New dimension for the input command view */
            /*
             * -1 because last EOL removed in
             * SwingScilabConsole.readline
             */
            int newHeight = height - (lines.length - 1) * charHeight;
            Dimension newDim = null;

            if (newHeight > promptViewHeight) {
                /*
                 * If the input command view is bigger than the
                 * promptUI
                 */
                /*
                 * It's height is descreased according to line
                 * number of lines added to output view
                 */
                newDim = new Dimension(width, newHeight);
            } else {
                /*
                 * If the input command view is smaller than the
                 * promptUI
                 */
                /* It's height adapted to the promptUI height */
                newDim = new Dimension(width, promptViewHeight);
                console.setInputCommandViewSizeForced(false);
            }
            /* Change the input command view size */
            ((JTextPane) console.getConfiguration().getInputCommandView()).setPreferredSize(newDim);
            ((JTextPane) console.getConfiguration().getInputCommandView()).invalidate();
            ((JTextPane) console.getConfiguration().getInputCommandView()).doLayout();
        }
        /* Update scroll only if console has been set */
        /* TODO : Must not do this each time... consume pretty much computing resources */
        if (console != null) {
            console.updateScrollPosition();
        }
    }

    /**
     * Adds text to the output view and change the size of others components if
     * necessary
     *
     * @param content
     *            text to add
     */
    public void append(String content) {
        //append(content, activeStyle);
        displayLineBuffer(content, activeStyle);
    }

    /**
     * Adds text to the output view and change the size of others components if
     * necessary
     *
     * @param content
     *            text to add
     * @param styleName
     *            style to set for content
     */
    public void append(String content, String styleName) {
        if (styleName.equals(lastAppendedStyle) && bufferQueue.size() > 1) {
            currentWorkingBuffer.append(content);
        } else {
            lastAppendedStyle = styleName;
            styleQueue.add(lastAppendedStyle);
            try {
                currentWorkingBuffer = new StringBuffer(content);
                bufferQueue.put(currentWorkingBuffer);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        if (!thread.isAlive()) {
            thread.run();
        }
    }

    /**
     * Gets the error writer
     *
     * @return the error writer
     * @see com.artenum.rosetta.interfaces.ui.OutputView#getErrorWriter()
     */
    public Writer getErrorWriter() {
        return new BufferedWriter(StyleContext.DEFAULT_STYLE, bufferQueue, styleQueue);
    }

    /**
     * Gets the writer
     *
     * @return the writer
     * @see com.artenum.rosetta.interfaces.ui.OutputView#getWriter()
     */
    public Writer getWriter() {
        return new BufferedWriter(StyleContext.DEFAULT_STYLE, bufferQueue, styleQueue);
    }

    /**
     * Resets the output view (remove text)
     *
     * @see com.artenum.rosetta.interfaces.ui.OutputView#reset()
     */
    public void reset() {
        setText("");
        setCaretPosition(0);
    }

    /**
     * Move the caret to the beginning of the styled document
     *
     * @see com.artenum.rosetta.interfaces.ui.OutputView#setCaretPositionToBeginning()
     */
    public void setCaretPositionToBeginning() {
        insertPosition = 0;
        setCaretPosition(0);
    }

    /**
     * Move the caret to the end of the styled document
     *
     * @see com.artenum.rosetta.interfaces.ui.OutputView#setCaretPositionToEnd()
     */
    public void setCaretPositionToEnd() {
        insertPosition = 0;
        setCaretPosition(getDocument().getLength());
    }

    /**
     * Set the style for current text
     *
     * @param styleName
     *            the style to set
     * @see com.artenum.rosetta.interfaces.ui.OutputView#setStyleName(java.lang.String)
     */
    public void setStyleName(String styleName) {
        activeStyle = styleName;
    }

    /**
     * Sets the console object containing this output view
     *
     * @param c
     *            the console associated
     */
    public void setConsole(SciConsole c) {
        console = c;

        // Drag n' Drop handling
        this.setDropTarget(new DropTarget(this,
                                          DnDConstants.ACTION_COPY_OR_MOVE, new SciDropTargetListener(console)));

        // Commented because now done by the caret class
        //FocusMouseListener focusGrabber = new FocusMouseListener(console);
        //this.addMouseListener(focusGrabber);
    }

    /**
     * Gets the console object containing this output view
     *
     * @return the console associated
     */
    public SciConsole getConsole() {
        return console;
    }

    /**
     * Get the current thread used to display
     * @return the thread
     */
    public Thread getThread() {
        return thread;
    }

    /**
     * Set the maximum number of lines to keep before deleting the older one
     * @param number the maximum
     */
    public void setMaxSize(int number) {
        maxNumberOfLines = Math.max(1, number);
    }

}
