// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// --------------------------------------
// !!!!! TODO : Need to unload JARs !!!!!
// !!!!! (not possible in JIMS yet) !!!!!
// !!!!! The JARS cannot be deleted !!!!!
// !!!!! => Unit tests will fail    !!!!!
// --------------------------------------

test_src_dir = get_absolute_file_path('jcreatejar.tst');

function dir = createSubDir(dir, subDir)
    dir = fullfile(dir, subDir);
    mkdir(dir);
endfunction

function addClassToPackage(package_dir, class_name, in_folder)
    src_classes_dir = fullfile(test_src_dir, 'classes');
    if in_folder then
        dir = createSubDir(package_dir, 'folder');
        copyfile(fullfile(src_classes_dir, class_name), dir);
    else
        copyfile(fullfile(src_classes_dir, class_name), package_dir);
    end
endfunction


test_root_dir = fullfile(TMPDIR, "jcreatejar");
mkdir(test_root_dir);

// Test a package with one class
jar_input_dir = createSubDir(test_root_dir, "package");
addClassToPackage(jar_input_dir, 'Foo.class', %F);
jar_path = fullfile(test_root_dir, "package.jar");
jcreatejar(jar_path, jar_input_dir);
javaclasspath(jar_path);
jimport Foo;
jremove Foo;
jremove();

// Test a package with folders
jar_input_dir = createSubDir(test_root_dir, "packageWithFolders");
addClassToPackage(jar_input_dir, 'FooA.class', %F);
addClassToPackage(jar_input_dir, 'FooB.class', %T);
jar_path = fullfile(test_root_dir, "packageWithFolders.jar");
jcreatejar(jar_path, jar_input_dir);
javaclasspath(jar_path);
jimport FooA;
// TODO : fix test FooB
//jimport folder.FooB;

// Test a standard package 'org.scilab.test.package'
// Need to set the root of package as 3rd argument
jar_input_dir = createSubDir(test_root_dir, "org.scilab.test.mypackage");
dir = createSubDir(jar_input_dir, "org");
dir = createSubDir(dir, "scilab");
dir = createSubDir(dir, "test");
dir = createSubDir(dir, "mypackage");
addClassToPackage(dir, 'Foo1.class', %F);
addClassToPackage(dir, 'Foo2.class', %T);
jar_path = fullfile(test_root_dir, "org.scilab.test.mypackage.jar");
jcreatejar(jar_path, jar_input_dir, jar_input_dir);
javaclasspath(jar_path);
jimport org.scilab.test.mypackage.Foo1;
jimport org.scilab.test.mypackage.folder.Foo2;

// --------------------------------------
// !!!!! TODO : Need to unload JARs !!!!!
// !!!!! (not possible in JIMS yet) !!!!!
// !!!!! The JARS cannot be deleted !!!!!
// !!!!! => Unit tests will fail    !!!!!
// --------------------------------------