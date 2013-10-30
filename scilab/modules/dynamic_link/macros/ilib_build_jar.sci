// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// Returns all the java source files contained in a path tree
function out_java_files = find_java_files(path, in_java_files)
  out_java_files = in_java_files;
  for f = listfiles(path)'
    subpath = fullfile(path, f);
    if isdir(subpath) then
      out_java_files = [out_java_files; find_java_files(subpath)];
    else
      if fileext(subpath) == '.java' then
        out_java_files = [out_java_files; subpath];
      end
    end
  end
endfunction

// Returns the jar compile path (containing built classes)
// if package is 'com.foo.package' the path will be:
//   TMPDIR/JIMS/bin/com/foo/package
function jar_compile_path = get_jar_compile_path(jar_name)
  jims_compile_path = fullfile(TMPDIR, "JIMS/bin");
  jar_compile_subpath = strsubst(jar_name, '.', filesep());
  jar_compile_path = fullfile(jims_compile_path, jar_compile_subpath);
endfunction

// Returns the jar build root path (the root directory that will be jar-ed)
//  and the jar build leaf path (the directory containing the jar stuff)
// if package is 'com.foo.package' the returned paths will be:
//   TMPDIR/JIMS/bin/com.foo.package
//   TMPDIR/JIMS/bin/com.foo.package/com/foo/package
function [jar_build_root_path, jar_build_leaf_path] = get_jar_build_path(jar_name)
  jims_build_path = fullfile(TMPDIR, "JIMS/bin");

  jar_build_root_path = fullfile(jims_build_path, jar_name);
  if ~isdir(jar_build_root_path) then
    mkdir(jar_build_root_path);
  end

  // Create all the package subdirs
  jar_build_leaf_path = jar_build_root_path;
  jar_dirs = strsplit(jar_name, '.');
  for i = 1:size(jar_dirs, '*')
    jar_build_leaf_path = fullfile(jar_build_leaf_path, jar_dirs(i));
    if ~isdir(jar_build_leaf_path) then
      mkdir(jar_build_leaf_path);
    end
  end
endfunction

// Builds a jar from a java src path
function ilib_build_jar(jar_src_path, jar_path)
  [jar_dir, jar_name] = fileparts(jar_path);

  // Delete jar existent compile dir (built classes)
  jar_compile_path = get_jar_compile_path(jar_name);
  if isdir(jar_compile_path) then
    mdelete(jar_compile_path);
  end

  // Find sources
  java_files = find_java_files(jar_src_path, []);
  if java_files == [] then
    error(msprintf("No sources in %s to compile for jar %s", jar_src_path, jar_path));
  end

   // Compile sources
  jcompile(java_files);

  // Checks jar compilation dir (containing classes) is here
  if ~isdir(jar_compile_path) then
    error(msprintf("Cannot find jar compilation dir %s", jar_compile_path));
  end

  // Copy jar compilation dir to jar build dir (the directory that will be jar-red)
  [jar_build_root_path, jar_build_leaf_path] = get_jar_build_path(jar_name);
  copyfile(jar_compile_path, jar_build_leaf_path);

  // Checks jar built dir has been created
  if ~isdir(jar_build_leaf_path) then
    error(msprintf("Cannot find jar build dir %s", jar_build_path));
  end

  // Delete target jar if already exists
  if isfile(jar_path) then
    mdelete(jar_path);
  end

  // Create jar
  jcreatejar(jar_path, jar_build_root_path);
  if ~isfile(jar_path) then
    error(msprintf("Cannot create jar %s", jar_path));
  end
endfunction
