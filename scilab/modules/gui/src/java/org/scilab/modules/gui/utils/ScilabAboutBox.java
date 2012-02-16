/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009-2012 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Image;
import java.awt.Toolkit;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;

import javax.swing.Icon;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTextPane;

import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.localization.Messages;

/**
 * Scilab about box
 * @author Vincent COUVERT
 */
public class ScilabAboutBox {

    public static final String SCIDIR = System.getenv("SCI");
    public static final String IMAGEPATH = SCIDIR + "/modules/gui/images/icons/aboutscilab.png";
    private static Icon scilabIcon = new ImageIcon(ScilabSwingUtilities.findIcon("scilab", "256x256"));
    private static Image imageForIcon = ((ImageIcon) scilabIcon).getImage();

    /**
     * Constructor
     */
    protected ScilabAboutBox() {
        throw new UnsupportedOperationException(); /* Prevents calls from subclass */
    }

    /**
     * Display the about box
     */
    public static void displayAndWait() {

        String filename = SCIDIR + "/ACKNOWLEDGEMENTS"; // Source version
        if (!new File(filename).exists()) {
            filename = SCIDIR + "/../../ACKNOWLEDGEMENTS"; // Linux binary version
        }

        createAboutBox(Messages.gettext("About Scilab..."), filename, Messages.gettext("Acknowledgements"));

    }

    /**
     * Create the about box
     * @param aboutTitle title for the Window
     * @param ackFile path to acknowledgements file
     * @param ackTitle Title for acknowledgements button and window
     */
    public static void createAboutBox(String aboutTitle, String ackFile, String ackTitle) {
        final JFrame ackBox = new JFrame();
        ackBox.setIconImage(imageForIcon);
        final JFrame aboutBox = new JFrame();
        aboutBox.setIconImage(imageForIcon);
        aboutBox.setTitle(Messages.gettext(aboutTitle));

        aboutBox.setLayout(null);
        aboutBox.setAlwaysOnTop(true);
        aboutBox.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
        aboutBox.setLocationRelativeTo(null);

        ScilabSwingUtilities.closeOnEscape(aboutBox);
        ScilabSwingUtilities.closeOnEscape(ackBox);

        ImagePanel text = new ImagePanel();
        text.setOpaque(false);
        text.setEditable(false);
        text.setVisible(true);

        aboutBox.setContentPane(text);
        aboutBox.setResizable(false);

        JButton close = new JButton(Messages.gettext("Close"));
        close.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent event) {
                aboutBox.dispose();
                ackBox.dispose();
            }
        });
        aboutBox.add(close, 0);
        close.setSize(80, 20);
        close.setLocation((int) text.getPreferredSize().getWidth() - 90, (int) text.getPreferredSize().getHeight() - 30);
        Font closeFont = close.getFont();
        close.setFont(closeFont.deriveFont(closeFont.getSize2D() * 0.7f));

        if (ackFile != null && ackTitle != null) {
            JButton acknowledgements = new JButton(ackTitle);
            final String filename = ackFile;
            final String finalAckTitle = ackTitle;
            acknowledgements.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent event) {

                    StringBuilder contents = new StringBuilder();
                    try {
                        BufferedReader input = new BufferedReader(new InputStreamReader(new FileInputStream(filename), "UTF8"));
                        try {
                            String line = null;
                            while ((line = input.readLine()) != null) {
                                contents.append(line);
                                contents.append(System.getProperty("line.separator"));
                            }
                        } finally {
                            input.close();
                        }
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }

                    ackBox.setTitle(finalAckTitle);

                    ackBox.setLayout(null);
                    ackBox.setAlwaysOnTop(true);
                    ackBox.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
                    ackBox.setLocationRelativeTo(null);
                    ackBox.setSize(600, 400);

                    JTextPane text = new JTextPane();
                    text.setText(contents.toString());
                    text.setCaretPosition(0);

                    JScrollPane pane = new JScrollPane(text);
                    pane.setSize(600, 400);
                    ackBox.setContentPane(pane);

                    text.setEditable(false);

                    if (ScilabConsole.isExistingConsole()) {
                        ackBox.setLocationRelativeTo((Component) ScilabConsole.getConsole().getAsSimpleConsole());
                    } else {
                        ackBox.setLocation((int) Toolkit.getDefaultToolkit().getScreenSize().getWidth() / 2 - (aboutBox.getWidth() / 2),
                                           (int) Toolkit.getDefaultToolkit().getScreenSize().getHeight() / 2 - (aboutBox.getHeight() / 2));
                    }
                    ackBox.setVisible(true);
                }
            });
            aboutBox.add(acknowledgements, 0);
            acknowledgements.setSize(130, 20);
            acknowledgements.setLocation((int) text.getPreferredSize().getWidth() - 90 - 140, (int) text.getPreferredSize().getHeight() - 30);
            Font ackFont = acknowledgements.getFont();
            acknowledgements.setFont(ackFont.deriveFont(ackFont.getSize2D() * 0.7f));
        }

        if (ScilabConsole.isExistingConsole()) {
            aboutBox.setLocationRelativeTo((Component) ScilabConsole.getConsole().getAsSimpleConsole());
        } else {
            aboutBox.setLocation((int) Toolkit.getDefaultToolkit().getScreenSize().getWidth() / 2 - (aboutBox.getWidth() / 2),
                                 (int) Toolkit.getDefaultToolkit().getScreenSize().getHeight() / 2 - (aboutBox.getHeight() / 2));
        }

        aboutBox.pack();
        aboutBox.setVisible(true);
    }

}

final class ImagePanel extends JTextPane {
    private ImageIcon background = new ImageIcon(ScilabAboutBox.IMAGEPATH);

    @Override
    public Dimension getPreferredSize() {
        return new Dimension(background.getIconWidth(), background.getIconHeight());
    }

    @Override
    public Dimension getMaximumSize() {
        return getPreferredSize();
    }

    @Override
    public Dimension getMinimumSize() {
        return getPreferredSize();
    }

    @Override
    public void paintComponent(Graphics g) {
        g.drawImage(background.getImage(), 0, 0, (int) getSize().getWidth(), (int) getSize().getHeight(), this);
        super.paintComponent(g);

    }
}
