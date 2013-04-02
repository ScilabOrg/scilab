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

package org.scilab.modules.xcos;

import java.io.File;
import java.io.IOException;
import java.nio.file.FileSystem;
import java.nio.file.FileSystems;
import java.nio.file.FileVisitResult;
import java.nio.file.FileVisitor;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.PathMatcher;
import java.nio.file.attribute.BasicFileAttributes;

import org.scilab.modules.commons.CommonFileUtils;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.io.XcosFileType;

/**
 *
 */
public final class XcosInternals {

    public static void xcosToZcos() throws IOException {
        final String cwd = CommonFileUtils.getCWD();
        final FileSystem fs = FileSystems.getDefault();

        Files.walkFileTree(fs.getPath(cwd), new XcosFileVisitor(fs));
    }

    private static final class XcosFileVisitor implements FileVisitor<Path> {
        final PathMatcher matcher;
        final Xcos instance;

        public XcosFileVisitor(FileSystem fs) {
            matcher = fs.getPathMatcher("glob:*.xcos");
            instance = Xcos.getInstance();
        }

        @Override
        public FileVisitResult preVisitDirectory(Path dir, BasicFileAttributes attrs) throws IOException {
            return FileVisitResult.CONTINUE;
        }

        @Override
        public FileVisitResult visitFile(Path file, BasicFileAttributes attrs) throws IOException {
            if (!attrs.isDirectory() && matcher.matches(file.getFileName())) {
                openAndSaveToZcos(file.toFile(), instance);
                Files.delete(file);
            }
            return FileVisitResult.CONTINUE;
        }

        @Override
        public FileVisitResult visitFileFailed(Path file, IOException exc) throws IOException {
            return FileVisitResult.CONTINUE;
        }

        @Override
        public FileVisitResult postVisitDirectory(Path dir, IOException exc) throws IOException {
            return FileVisitResult.CONTINUE;
        }

    }

    private static void openAndSaveToZcos(final File f, final Xcos instance) throws IOException {
        System.err.println(f);

        /*
         * Allocate and setup a new diagram
         */
        final XcosDiagram diag = new XcosDiagram();
        diag.installListeners();

        /*
         * Load the file and synchronize on the instance (as usual)
         */
        instance.setLastError(null);
        synchronized (instance) {
            diag.transformAndLoadFile(f.getCanonicalPath(), null);

            try {
                while (instance.getLastError() == null) {
                    instance.wait();
                }
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        /*
         * Rename then save the file
         */
        final StringBuilder fname = new StringBuilder(f.getCanonicalPath());
        fname.replace(fname.lastIndexOf(".xcos"), fname.length(), ".zcos");

        try {
            XcosFileType.ZCOS.save(fname.toString(), diag);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
