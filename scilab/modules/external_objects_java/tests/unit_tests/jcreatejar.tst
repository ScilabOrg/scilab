// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

jimport java.io.FileInputStream;
jimport java.util.zip.ZipInputStream;
jimport java.util.zip.ZipEntry;

test_root_dir = fullfile(TMPDIR, "jcreatejar");
mkdir(test_root_dir);

function dir = createSubDir(dir, subDir)
    dir = fullfile(dir, subDir);
    mkdir(dir);
endfunction

function file_path = addFileToPackage(file_name, package, package_dir)
    dest_dir = package_dir;
    if ~isempty(package) then
        file_dirs = strsplit(package, '.'),
        for i = 1:size(file_dirs, '*')
            dest_dir = createSubDir(dest_dir, file_dirs(i));
        end
    end
    file_path = fullfile(dest_dir, file_name);

    fd = mopen(file_path, 'wt');
    mputl(file_name, fd);
    mclose(fd);
endfunction

function filePaths = getJarFileList(zipFilePath)
    filePaths = [];

    fileInputStream = FileInputStream.new(zipFilePath)
    zipInputStream = ZipInputStream.new(fileInputStream);

    zipEntry = jinvoke(zipInputStream, "getNextEntry");
    while ~isempty(zipEntry)
        isDirectory = jinvoke(zipEntry, "isDirectory");
        if ~isDirectory then
        zipEntryName = jinvoke(zipEntry, "getName");
        filePaths = [filePaths; zipEntryName];
    end

    jinvoke(zipInputStream, "closeEntry");
    zipEntry = jinvoke(zipInputStream, "getNextEntry");
  end
endfunction

function checkJar(jar_path, expected_jar_files)
    assert_checktrue(isfile(jar_path));
    jar_files = getJarFileList(jar_path);
    assert_checkequal(jar_files, expected_jar_files);
endfunction

// Test create jar with one file, by giving the dir path
package_name = 'packageOneClassGivingDirPath';
jar_src_path = createSubDir(test_root_dir, package_name);
addFileToPackage('FooDir', '', jar_src_path);
jar_dest_path = fullfile(test_root_dir, package_name + '.jar');
jcreatejar(jar_dest_path, jar_src_path);
checkJar(jar_dest_path, ['META-INF/MANIFEST.MF'; 'FooDir']);

// Test create jar with one file, by giving the file path
package_name = 'packageOneClassGivingFilePath';
jar_src_path = createSubDir(test_root_dir, package_name);
file_path = addFileToPackage('FooFile', '', jar_src_path);
jar_dest_path = fullfile(test_root_dir, package_name + '.jar');
jcreatejar(jar_dest_path, file_path);
checkJar(jar_dest_path, ['META-INF/MANIFEST.MF'; 'FooFile']);

// Test create jar with two files, by giving the dir path
package_name = 'packageTwoClassesGivingDirPath';
jar_src_path = createSubDir(test_root_dir, package_name);
addFileToPackage('FooDir1', '', jar_src_path);
addFileToPackage('FooDir2', '', jar_src_path);
jar_dest_path = fullfile(test_root_dir, package_name + '.jar');
jcreatejar(jar_dest_path, jar_src_path);
checkJar(jar_dest_path, ['META-INF/MANIFEST.MF'; 'FooDir1'; 'FooDir2']);

// Test create jar with two files, by giving the file paths
package_name = 'packageTwoClassesGivingFilePaths';
jar_src_path = createSubDir(test_root_dir, package_name);
file_path1 = addFileToPackage('FooFile1', '', jar_src_path);
file_path2 = addFileToPackage('FooFile2', '', jar_src_path);
jar_dest_path = fullfile(test_root_dir, package_name + '.jar');
jcreatejar(jar_dest_path, [file_path1, file_path2]);
checkJar(jar_dest_path, ['META-INF/MANIFEST.MF'; 'FooFile1'; 'FooFile2']);

// Test create jar with two files and one folder, by giving the dir path
package_name = 'packageOneFolderGivingDirPath';
jar_src_path = createSubDir(test_root_dir, package_name);
addFileToPackage('FooRoot', '', jar_src_path);
addFileToPackage('FooFolder', 'folder', jar_src_path);
jar_dest_path = fullfile(test_root_dir, package_name + '.jar');
jcreatejar(jar_dest_path, jar_src_path);
checkJar(jar_dest_path, ['META-INF/MANIFEST.MF'; 'folder/FooFolder'; 'FooRoot']);

// Test create a standard package 'org.scilab.test.package'
package_name = 'org.scilab.test.mypackage';
jar_src_path = createSubDir(test_root_dir, package_name);
addFileToPackage('FooPackage1', package_name, jar_src_path);
addFileToPackage('FooPackage2', package_name, jar_src_path);
jar_dest_path = fullfile(test_root_dir, package_name + '.jar');
jcreatejar(jar_dest_path, jar_src_path);
checkJar(jar_dest_path, ..
    ['META-INF/MANIFEST.MF'; ..
     'org/scilab/test/mypackage/FooPackage1'; ..
     'org/scilab/test/mypackage/FooPackage2']);

// Test file root path argument
package_name = 'packageFilesRootPath';
jar_src_path = createSubDir(test_root_dir, package_name);
addFileToPackage('image1', 'images', jar_src_path);
addFileToPackage('image2', 'images', jar_src_path);
addFileToPackage('icon1', 'images/icon', jar_src_path);
jcreatejar(jar_dest_path, jar_src_path, jar_src_path);
checkJar(jar_dest_path, ..
    ['META-INF/MANIFEST.MF'; ..
     'images/icon/icon1'
     'images/image1'; ..
     'images/image2']);

// Test manifest file path argument
// TODO
