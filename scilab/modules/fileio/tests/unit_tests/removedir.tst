// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// ============================================================================
// Unitary tests for removedir function
// ============================================================================

chdir(TMPDIR);

warning("off");
a = createdir("test_removedir");
if(a <> %T) then pause, end

cd("test_removedir");
if MSDOS then
	unix_w(jre_path()+"\bin\java.exe -cp "+SCI+"\modules\localization\tests\unit_tests CreateDir");
else
	unix_w(jre_path()+"/bin/java -classpath "+SCI+"/modules/localization/tests/unit_tests CreateDir");
end

chdir(TMPDIR);
b = removedir("test_removedir");
if (b <> %T) then pause,end
if(isdir("test_removedir") <> %F) then pause, end

