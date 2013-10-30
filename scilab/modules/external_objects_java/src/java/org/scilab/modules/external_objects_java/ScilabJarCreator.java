/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.external_objects_java;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.BufferedInputStream;
import java.io.IOException;
import java.util.List;
import java.util.ArrayList;
import java.util.jar.JarEntry;
import java.util.jar.JarOutputStream;
import java.util.jar.Manifest;


/**
 * Class to create JAR files (for JIMS)
 */
public class ScilabJarCreator {
    /**
     * Create a Jar archive from a set of files
     * @param jarFilePath the destination jar path
     * @param filePaths the set of gile paths to be jar-ed
     */

    public static int createJarArchive(String jarFilePath, String[] filePaths) throws ScilabJavaException {
        JarOutputStream jarOutputStream = null;
        try {
            List<String> expandedFilePaths = expandPaths(filePaths);
            String commonPath = getCommonPath(expandedFilePaths);
            //System.out.println("common path: " + commonPath);

            // TODO: if jar exists ?
            FileOutputStream stream = new FileOutputStream(jarFilePath);
            jarOutputStream = new JarOutputStream(stream, new Manifest());

            for (String filePath : expandedFilePaths) {
                File file = new File(filePath);

                String pathInJar;
                if (!commonPath.isEmpty()) {
                    pathInJar = getRelativePath(commonPath, filePath);
                } else {
                    pathInJar = file.getPath();
                }

                addFileToJarArchive(file, pathInJar, jarOutputStream);
            }

            closeJarArchive(jarOutputStream);
            jarOutputStream = null;
            return 0;
        } catch (Exception e) {
            closeJarArchive(jarOutputStream);
            deleteJarArchive(jarFilePath);
            throw new ScilabJavaException(String.format("Cannot create jar archive %s : %s\n", jarFilePath, e.getLocalizedMessage()));
        }
    }

    /**
     * Recursively list all chidren file paths contained in a list of paths
     * @param paths the paths to expand
     */
    private static List<String> expandPaths(String... paths) {
        ArrayList<String> expandedPaths = new ArrayList<String>();
        for (String filePath : paths) {
            File file = new File(filePath);
            if (file.isDirectory()) {
                for (File childFile: file.listFiles()) {
                    expandedPaths.addAll(expandPaths(childFile.getPath()));
                }
            } else {
                expandedPaths.add(filePath);
            }
        }
        return expandedPaths;
    }

    /**
     * Adds a file to an opened Jar archive
     * @param file the file to add
     * @param pathInJar the path of file in jar
     * @param jar the opened Jar in which the file is added
     */
    private static void addFileToJarArchive(File file, String pathInJar, JarOutputStream jar) throws IOException {
        //System.out.println("Add file: " + file.getPath() + ", pathInJar: " + pathInJar);

        BufferedInputStream in = null;
        try {
            if (!file.exists()) {
                // TODO throw error
                return;
            }

            if (pathInJar.isEmpty()) {
                // TODO throw error
                return;
            }
            pathInJar.replace("\\", "/");

            JarEntry entry = new JarEntry(pathInJar);
            entry.setTime(file.lastModified());
            jar.putNextEntry(entry);
            in = new BufferedInputStream(new FileInputStream(file));

            byte[] buffer = new byte[4096];
            while (true) {
                int count = in.read(buffer);
                if (count == -1) {
                    break;
                }
                jar.write(buffer, 0, count);
            }
            jar.closeEntry();
        } finally {
            if (in != null) {
                in.close();
            }
        }
    }

    /**
    * Returns the common path of a set of paths
    * @param paths the input paths
    */
    private static String getCommonPath(List<String> paths) throws IOException {
        int nbPaths = paths.size();
        if (nbPaths < 2) {
            return "";
        }

        // Escape because Windows backslash crashes split (regexp)
        String fileSep = java.util.regex.Pattern.quote(System.getProperty("file.separator"));

        String commonPath = "";
        String[][] folders = new String[nbPaths][];

        // Split each path in all its folders
        int k = 0;
        for (String path : paths) {
            String canonicalPath = new File(path).getCanonicalPath();
            folders[k++] = canonicalPath.split(fileSep);
        }

        // For each folder of first path
        for (int j = 0; j < folders[0].length; j++) {
            String folderToMatch = folders[0][j];

            // Compare with the folder at same position in all the other paths
            boolean allMatched = true;
            for (int i = 1; i < folders.length && allMatched; i++) {
                if (j >= folders[i].length) {
                    allMatched = false;
                    return commonPath;
                }
                allMatched &= (new File(folders[i][j]).compareTo(new File(folderToMatch)) == 0);
            }

            // Update common path
            if (allMatched) {
                commonPath += folderToMatch + File.separator;
            } else {
                return commonPath;
            }
        }

        return commonPath;
    }

    /**
    * Returns the relative path of a path given a base path
    * @param base the base path
    * @param path the path for which we want the relative path
    */
    private static String getRelativePath(String base, String path) {
        return new File(base).toURI().relativize(new File(path).toURI()).getPath();
    }

    private static void closeJarArchive(JarOutputStream jarOutputStream) throws ScilabJavaException {
        try {
            if (jarOutputStream != null) {
                jarOutputStream.close();
            }
        } catch (IOException e) {
            throw new ScilabJavaException(String.format("Cannot close jar stream : %s\n", e.getLocalizedMessage()));
        }
    }

    private static void deleteJarArchive(String jarFilePath) throws ScilabJavaException {
        File f = new File(jarFilePath);
        if (f.exists()) {
            if (f.canWrite()) {
                if (!f.delete()) {
                    throw new ScilabJavaException(String.format("Cannot delete jar archive %s.\n", jarFilePath));
                }
            } else {
                throw new ScilabJavaException(String.format("Cannot delete jar archive %s : File is write protected.\n", jarFilePath));
            }
        } else {
            throw new ScilabJavaException(String.format("Cannot delete jar archive %s : No such file.\n", jarFilePath));
        }
    }

    public static void main (String[] args) {
        String jarFilePath = args[0];
        String[] filePaths = java.util.Arrays.copyOfRange(args, 1, args.length);
        
        //ScilabJarCreator.createJarArchive(jarFilePath, filePaths);
    }

}
