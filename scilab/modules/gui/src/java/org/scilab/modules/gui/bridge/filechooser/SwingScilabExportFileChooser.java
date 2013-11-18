/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre Koumar
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
package org.scilab.modules.gui.bridge.filechooser;

import java.awt.Component;
import java.awt.Window;
import java.io.File;
import java.util.ArrayList;

import javax.swing.BorderFactory;
import javax.swing.JFileChooser;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.SwingUtilities;
import javax.swing.border.TitledBorder;

import org.scilab.modules.graphic_export.FileExporter;
import org.scilab.modules.gui.SwingView;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.tab.SimpleTab;
import org.scilab.modules.gui.utils.ConfigManager;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.renderer.JoGLView.DrawerVisitor;

/**
 * This is the son of the usual Scilab file chooser,
 * it have been customized for the graphic export
 * @author Sylvestre Koumar
 *
 */
@SuppressWarnings(value = { "serial" })
public class SwingScilabExportFileChooser extends SwingScilabFileChooser {

    private static final int NB_FILE_MASKS = 12;

    private final String bmpDesc = Messages.gettext("Windows BMP image");
    private final String gifDesc = Messages.gettext("GIF image");
    private final String jpgDesc = Messages.gettext("JPEG image");
    private final String pngDesc = Messages.gettext("PNG image");
    private final String ppmDesc = Messages.gettext("PPM image");
    private final String emfDesc = Messages.gettext("Enhanced Metafile image (EMF)");
    private final String epsDesc = Messages.gettext("Encapsulated PostScript image (EPS)");
    private final String figDesc = Messages.gettext("FIG image");
    private final String pdfDesc = Messages.gettext("PDF image");
    private final String svgDesc = Messages.gettext("SVG image");
    private final String allFilesDesc = Messages.gettext("All files");

    private final String bmp = "bmp";
    private final String gif = "gif";
    private final String[] jpg = {"jpg", "jpeg"};
    private final String png = "png";
    private final String ppm = "ppm";
    private final String emf = "emf";
    private final String eps = "eps";
    private final String fig = "fig";
    private final String pdf = "pdf";
    private final String svg = "svg";
    private final String allFiles = "*";

    private String exportName;
    private String extensionSelected;
    private Integer figureUID;

    /**
     * Default constructor
     * @param figureId id of the exported figure
     */
    public SwingScilabExportFileChooser(Integer figureUID) {
        super();
        this.figureUID = figureUID;
        SwingScilabTab tab = (SwingScilabTab) SwingView.getFromId(figureUID);
        setParentFrame(tab.getParentWindow());
        exportCustomFileChooser(figureUID);
    }

    /**
     * We customize the file chooser for the graphic export
     * by adding format selection
     * @param figureId exported figure number
     */
    public void exportCustomFileChooser(Integer figureUID) {
        ArrayList<FileMask> v = new ArrayList<FileMask>(NB_FILE_MASKS);  /* The order does matter */
        v.add(new FileMask(png, pngDesc));
        v.add(new FileMask(jpg, jpgDesc));
        v.add(new FileMask(eps, epsDesc));
        v.add(new FileMask(pdf, pdfDesc));
        v.add(new FileMask(svg, svgDesc));
        v.add(new FileMask(emf, emfDesc));
        v.add(new FileMask(gif, gifDesc));
        v.add(new FileMask(bmp, bmpDesc));
        v.add(new FileMask(ppm, ppmDesc));
        v.add(new FileMask(fig, figDesc));
        v.add(new FileMask(allFiles, allFilesDesc)); // should always be at the last position

        super.setDialogTitle(Messages.gettext("Export"));
        super.setApproveButtonText(Messages.gettext("Export"));
        File exportFile = new File(Messages.gettext("Untitled-export"));
        super.setSelectedFile(exportFile);
        super.setAcceptAllFileFilterUsed(false);

        this.figureUID = figureUID;

        for (int i = 0; i < v.size(); i++) {
            FileMask fm = v.get(i);
            if (i == v.size() - 1) { /* Last case ... all files, remove the extension */
                fm.clearExtensions();
            }
            super.addChoosableFileFilter(fm);
        }

        //Preview panel
        JPanel panelPreview = new JPanel();

        //Title for preview panel
        TitledBorder titlePreview;
        titlePreview = BorderFactory.createTitledBorder(Messages.gettext("Preview"));
        panelPreview.setBorder(titlePreview);

        //add preview image
        panelPreview.add(new ImagePreview(this));

        //Main panel contains extensionTable panel & preview panel
        JPanel accessoryPanel = new JPanel();

        //accessoryPanel.add(scrollPane);
        accessoryPanel.add(panelPreview);
        accessoryPanel.setVisible(true);
        super.setAccessory(accessoryPanel);

        Component c = DrawerVisitor.getVisitor(figureUID).getComponent();
        Window parentWindow = (Window) SwingUtilities.getAncestorOfClass(Window.class, c);

        int selection = super.showSaveDialog(parentWindow);
        if (selection == JFileChooser.APPROVE_OPTION) {

            this.exportName = super.getSelectedFile().getAbsolutePath();

            //Test if there is a file with the same name
            if (new File(this.exportName).exists()) {
                int actionDialog = JOptionPane.showConfirmDialog(
                                       this, Messages.gettext("Replace existing file?"),
                                       Messages.gettext("File already exists"),
                                       JOptionPane.YES_NO_OPTION);

                if (actionDialog == JOptionPane.YES_OPTION) {

                } else {
                    return;
                }
            }

            /* Bug 3849 fix */
            ConfigManager.saveLastOpenedDirectory(new File(exportName).getParentFile().getPath());

            String extensionCombo = new String();
            try {
                // The try catch is necessary here when the user input the full
                // filename (foo.jpg) and press tab. It is going to update
                // the filter causing the following line to fail (cannot cast)
                // Therefor, we switch back to the allFiles (*) case.
                FileMask ft = (FileMask) super.getFileFilter();
                //get the extension from the Filter
                extensionCombo = ft.getExtensionFromFilter();
                if (extensionCombo == null) {
                    extensionCombo = allFiles;
                }
            } catch (java.lang.ClassCastException e) {
                extensionCombo = allFiles;
            }

            if (extensionCombo.equals(allFiles)) {
                exportManager();
            } else if (extensionCombo.equals(emf) || extensionCombo.equals(eps) || extensionCombo.equals(fig)
                       || extensionCombo.equals(pdf) || extensionCombo.equals(svg)) {
                vectorialExport(extensionCombo);

            } else {
                bitmapExport(extensionCombo);
            }
        } else {
            ;    // no file chosen
        }
    }

    /**
     * Return the file extension
     * @param fileName Name of the file
     * @return the extension
     */
    public String getExtension(String fileName) {
        if (fileName != null) {
            int i = fileName.lastIndexOf('.');
            if (i > 0 && i < fileName.length() - 1) {
                return fileName.substring(i + 1).toLowerCase();
            }
        }
        return null;
    }

    /**
     * Manage the export (bitmap/vectorial format) and export errors
     */
    public void exportManager() {
        //get the extension of the fileName chosen by the user (can be 'null')
        String userExtension = getExtension(this.exportName);

        if (userExtension == null) {
            //fileName without extension + "by extension (.*)" selected
            JOptionPane.showMessageDialog(
                this, Messages.gettext("Please specify a file format"),
                Messages.gettext("Error on export"), JOptionPane.ERROR_MESSAGE);
            return;
        } else if (userExtension.equals(bmp)) {
            bitmapExport(userExtension);

        } else if (userExtension.equals(gif)) {
            bitmapExport(userExtension);

        } else if (userExtension.equals(jpg[0]) || userExtension.equals(jpg[1])) {
            bitmapExport(userExtension);

        } else if (userExtension.equals(png)) {
            bitmapExport(userExtension);

        } else if (userExtension.equals(ppm)) {
            bitmapExport(userExtension);

        } else if (userExtension.equals(emf)) {
            vectorialExport(userExtension);

        } else if (userExtension.equals(eps)) {
            vectorialExport(userExtension);

        } else if (userExtension.equals(fig)) {
            vectorialExport(userExtension);

        } else if (userExtension.equals(pdf)) {
            vectorialExport(userExtension);

        } else if (userExtension.equals(svg)) {
            vectorialExport(userExtension);

        } else {
            //fileName with a wrong extension + "by extension (.*)" selected
            JOptionPane.showMessageDialog(
                this, Messages.gettext("Unrecognized extension '")
                + userExtension + Messages.gettext("'.\n Please specify a valid file format."),
                Messages.gettext("Error on export"), JOptionPane.ERROR_MESSAGE);
            return;
        }
    }

    /**
     * Export an bitmap file
     * @param userExtension extension caught by the user
     */
    public void bitmapExport(String userExtension) {
        ExportData exportData = new ExportData(figureUID, this.exportName, userExtension, null);

        String actualFilename = exportData.getExportName();
        if (this.getExtension(actualFilename) == null) {
            // Could not get the extension from the user input
            // take the one from the list
            actualFilename += "." + userExtension;
        }

        FileExporter.fileExport(figureUID, actualFilename, exportData.getExportExtension(), -1, 0);
    }

    /**
     * Export a vectorial file
     * @param userExtension extension caught by the user
     */
    public void vectorialExport(String userExtension) {
        Component c = DrawerVisitor.getVisitor(figureUID).getComponent();
        ExportData exportData = new ExportData(figureUID, this.exportName, userExtension, null);

        String actualFilename = exportData.getExportName();
        if (this.getExtension(actualFilename) == null) {
            // Could not get the extension from the user input
            // take the one from the list
            actualFilename += "." + userExtension;
        }

        FileExporter.fileExport(figureUID, actualFilename, exportData.getExportExtension(), -1, 0);
    }
}
