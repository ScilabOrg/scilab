// ======================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
//  This file is distributed under the same license as the Scilab package.
// =======================================================================

testRootDir = fullfile(TMPDIR, "ilib_build_jar");
mkdir(testRootDir);

function path = createSubDir(parentDir, subDir, removeExistingDir)
    path = fullfile(parentDir, subDir);
    if isdir(path) & removeExistingDir then
        removedir(path);
    end
    mkdir(path);
endfunction

function sourceCode = createSourceCode(className, packageName, importPackageName)
    sourceCode = msprintf('package %s;\n', packageName);
    if ~isempty(importPackageName) then
        sourceCode = [sourceCode; msprintf('import %s.*;\n', importPackageName)]
    end
    sourceCode = [sourceCode; msprintf('public class %s {}\n', className)];
endfunction

function filePath = addSourceToPackage(className, packageName, packageSrcPath, sourceCode)
    if ~isempty(packageName) then
        packagePath = strsubst(packageName, '.', filesep());
        path = createSubDir(packageSrcPath, packagePath, %F);
    else
        path = packageSrcPath;
    end

    filePath = fullfile(path, className + '.java');

    fd = mopen(filePath, 'wt');
    if isempty(sourceCode) then
        sourceCode = createSourceCode(className, packageName, '');
    end
    mputl(sourceCode, fd);
    mclose(fd);
endfunction

function checkJar(jarFilePath, classNames)
    javaclasspath(jarFilePath);
    for i = 1:size(classNames, '*')
        expectedClassName = classNames(i);
        classobj = jimport(expectedClassName, %f);
        aclass = classobj.class;
        className = aclass.getName(jvoid);
        assert_checkequal(className, expectedClassName);
    end
endfunction

function checkCompileError(buildJarCmd)
    compileError = msprintf(_('jcompile: An error occured: Cannot compile the code'));
    execstr(buildJarCmd, 'errcatch');
    errMsg = lasterror();
    assert_checktrue(errMsg <> []);
    assert_checktrue(strstr(errMsg, compileError) <> []);
endfunction

function jarFilePath = buildJar1Package1Class(packageName, className, rootPath, sourceCode)
    packageSrcPath = createSubDir(testRootDir, packageName, %T);
    jarFilePath = fullfile(testRootDir, packageName + '.jar');
    addSourceToPackage(className, packageName, packageSrcPath, '');
    ilib_build_jar(jarFilePath, packageName, packageSrcPath);
endfunction


// Test build a JAR of a simple package with one class
packageName = 'testpackage';
className = 'Foo';
jarFilePath = buildJar1Package1Class(packageName, className, testRootDir, '');
checkJar(jarFilePath, packageName + '.' + className);

// Test build a JAR of a package with two classes
packageName = 'testpackage2';
packageSrcPath = createSubDir(testRootDir, packageName, %T);
jarFilePath = fullfile(testRootDir, packageName + '.jar');
addSourceToPackage('Foo1', packageName, packageSrcPath, '');
addSourceToPackage('Foo2', packageName, packageSrcPath, '');
ilib_build_jar(jarFilePath, packageName, packageSrcPath);
checkJar(jarFilePath, packageName + ['.Foo1', '.Foo2']);

// Test build a JAR of one 'standard' package
packageName = 'org.scilab.test.mypackage';
packageSrcPath = createSubDir(testRootDir, packageName, %T);
jarFilePath = fullfile(testRootDir, packageName + '.jar');
addSourceToPackage('Foo1', packageName, packageSrcPath, '');
addSourceToPackage('Foo2', packageName + '.folder', packageSrcPath, '');
ilib_build_jar(jarFilePath, packageName, packageSrcPath);
checkJar(jarFilePath, packageName + ['.Foo1', '.folder.Foo2']);

// Test build a JAR of two packages
jarFilePath = fullfile(testRootDir, 'testmultipackages.jar');
// package1
packageName1 = 'org.scilab.test.package1';
package1SrcPath = createSubDir(testRootDir, packageName1, %T);
addSourceToPackage('Foo1', packageName1, package1SrcPath, '');
addSourceToPackage('Foo2', packageName1 + '.folder', package1SrcPath, '');
// package2
packageName2 = 'org.scilab.test.package2';
package2SrcPath = createSubDir(testRootDir, packageName2, %T);
addSourceToPackage('FooA', packageName2, package2SrcPath, '');
addSourceToPackage('FooB', packageName2 + '.folder', package2SrcPath, '');
// build
ilib_build_jar(jarFilePath, [packageName1, packageName2], [package1SrcPath, package2SrcPath]);
checkJar(jarFilePath, ..
    [packageName1 + '.Foo1', packageName1 + '.folder.Foo2', ..
     packageName2 + '.FooA', packageName2 + '.folder.FooB']);

// Test compilation errors
packageName = 'testpackagecompileerrors';
packageSrcPath = createSubDir(testRootDir, packageName, %T);
jarFilePath = fullfile(testRootDir, packageName + '.jar');
className = '1234Foo';
sourceCode = createSourceCode(className, packageName, '');
javaFilePath = addSourceToPackage(className, packageName, packageSrcPath);
checkCompileError("ilib_build_jar(jarFilePath, packageName, packageSrcPath)");

// Test dependency packages argument
// create dependency package (in another directory, so that it is not in classpath)
dependencyPackageName = 'dependencypackage';
dependencyJarFilePath = buildJar1Package1Class(dependencyPackageName, 'Dummy', '');
// remove dependency package class directory, because it is in class path
jimsBinPath = fullfile(TMPDIR, 'JIMS/bin');
removedir(fullfile(jimsBinPath, dependencyPackageName));
// create package that uses the dependency
packageName = 'testpackagedependencies';
packageSrcPath = createSubDir(testRootDir, packageName, %T);
jarFilePath = fullfile(testRootDir, packageName + '.jar');
className = 'Bar';
sourceCode = createSourceCode(className, packageName, dependencyPackageName);
// check compile error without specifying dependency
addSourceToPackage(className, packageName, packageSrcPath, sourceCode);
checkCompileError("ilib_build_jar(jarFilePath, packageName, packageSrcPath)");
// check it is ok with specifying dependency
ilib_build_jar(jarFilePath, packageName, packageSrcPath, dependencyJarFilePath);
checkJar(jarFilePath, packageName + '.' + className);

