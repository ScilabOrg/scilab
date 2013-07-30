// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 12085 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12085
//
// <-- Short Description -->
// On Windows csvWrite wrote wrong EOL.

fileOut = TMPDIR + "/bug_12085.csv";
stringOut = [1,1;1,1];
data_ref = [49 44 49];

if getos() == "Windows" then
    eol = [13 10]; //CRLF
elseif getos()=="Darwin" then
    eol = [13]; //CR
else
    eol = [10]; //LF
end

ref = [data_ref, eol, data_ref, eol];

csvWrite(stringOut, fileOut);
stringIn = mgetl(fileOut);

info = fileinfo(fileOut);
fd = mopen(fileOut, "rb");
data_read = mget(info(1), "c", fd);
mclose(fd);

assert_checkequal(data_read, ref);
