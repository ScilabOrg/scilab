// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

mode(1)
//Decode ole file, extract and open Excel stream
[fd,SST,Sheetnames,Sheetpos] = xls_open('SCI/modules/spreadsheet/demos/xls/Test1.xls')
//Read first data sheet
[Value,TextInd] = xls_read(fd,Sheetpos(1))
//close the spreadsheet stream
mclose(fd)
