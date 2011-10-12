// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- JVM NOT MANDATORY -->
//
testFile=TMPDIR + '/testFile.bin';
fd = mopen(testFile,'wb');

// file should be empty
if mtell(fd) <> 0 then pause, end
for j=1:100
    for i=1:j
        mput(i,'d');
    end
    if mtell(fd) <> 8 * j then pause, end
    mseek(0);
end
mclose(fd);

try
    mtell(fd);
catch
    errorMessage = sprintf(gettext("%s: Error while opening, reading or writing.\n"), "mtell");
    [message, ierr]=lasterror();
    if message <> errorMessage then bugmes();quit;end
end

fd = mopen(testFile,'wb');

// file should be empty
if mtell(fd) <> 0 then pause, end
for j=1:100
    for i=1:j
        mput(i,'ul');
    end
    if mtell(fd) <> 4 * j then pause, end
    mseek(0);
end
mclose(fd);
