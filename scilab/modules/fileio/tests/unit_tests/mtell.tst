// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Cedric DELAMARRE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

//ouvrir le fichier 
fileTest = 'SCI/modules/fileio/tests/unit_tests/text.txt';

fd = mopen(fileTest,'rt');

mgetl(fd,1);
if mtell(fd) <> 253 then pause,end
mgetl(fd,1);
if mtell(fd) <> 504 then pause,end
mgetl(fd,1);
if mtell(fd) <> 914 then pause,end

mclose(fd);

ierr = execstr('meof(0)','errcatch');
if ierr <> 999 then pause,end
ierr = execstr('meof(5)','errcatch');
if ierr <> 999 then pause,end
ierr = execstr('meof(6)','errcatch');
if ierr <> 999 then pause,end
