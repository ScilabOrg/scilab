// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================

// Create all needed directories
//==============================


chdir(TMPDIR);

copyfile(SCI+"/modules/localization/tests/unit_tests/CreateDir.class.bin", TMPDIR + "/CreateDir.class");
if getos() == 'Windows' then
	unix_w(jre_path()+"\bin\java.exe -cp " + TMPDIR + " CreateDir");
else
	unix_w(jre_path()+"/bin/java -classpath " + TMPDIR + " CreateDir");
end

