// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 3663 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3663
//
// <-- Short Description -->
// getshortpathname & getlongpathname return '' and not a path with some special pathname

if getos() == 'Windows' then

  name_dir = 'L.P blabla';
  cd(TMPDIR);
  
  mkdir(name_dir);
  [a,b] = getshortpathname(name_dir);
  if b <> %t then pause,end
  if isempty(a) then pause,end
  // short pathname are managed by Windows
  // on Windows 7, it returns LDEE2~1.PBL
  if (a <> 'L3746~1.PBL' & a <> 'LDEE2~1.PBL') then pause,end
  
  [a,b] = getshortpathname(TMPDIR + '\' + name_dir);
  if b <> %t then pause,end
  if isempty(a) then pause,end
  
  [a,b] = getlongpathname(name_dir);
  if b <> %t then pause,end
  if isempty(a) then pause,end
  if a <> name_dir then pause,end
  
  [a,b] = getlongpathname(TMPDIR + '\' + name_dir);
  if b <> %t then pause,end
  if isempty(a) then pause,end
  
end
