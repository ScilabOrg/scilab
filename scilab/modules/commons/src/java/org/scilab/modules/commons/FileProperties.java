package org.scilab.modules.commons;

import java.io.File;

public class FileProperties {

    private long[] propr;

    public FileProperties(File f) {
	propr = CommonFileUtils.getFileProperties(f.getAbsolutePath());
    }

    public boolean isFile() {
	return propr[0] != 0;
    }

    public boolean isDirectory() {
	return propr[1] != 0;
    }

    public long lastModified() {
	return propr[2];
    }

    public long length() {
	return propr[3];
    }

    public boolean canRead() {
	return propr[4] != 0;
    }

    public boolean canWrite() {
	return propr[5] != 0;
    }
}
