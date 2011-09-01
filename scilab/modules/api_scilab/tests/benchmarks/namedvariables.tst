// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
stacksize('max');
ulink();
BENCH_NAME = "namedvariables";
ilib_verbose(0);
if haveacompiler() then
  cd TMPDIR;
  mkdir(BENCH_NAME);
  cd(BENCH_NAME);
  DEST = TMPDIR + "/" + BENCH_NAME + "/" + BENCH_NAME + ".c";
  copyfile(SCI + "/modules/api_scilab/tests/benchmarks/" + BENCH_NAME + ".c", DEST);
  assert_checkequal(isfile(DEST), %T);

  gw_functions = [BENCH_NAME,"sci_" + BENCH_NAME];

  ilib_build("lib" + BENCH_NAME, gw_functions, BENCH_NAME + ".c", []);
  exec loader.sce;

// <-- BENCH NB RUN : 100 -->

// <-- BENCH START -->
namedvariables();
// <-- BENCH END -->  
end

