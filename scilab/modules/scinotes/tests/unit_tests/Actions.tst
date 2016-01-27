// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

f=TMPDIR + "/file_to_correct.sci";
mputl(["a=''abcdef''									";
       "if a==1";
       "   b=1+""abcdef''         ";
       "   			c=2";
       " d=3			";
       "					        if		                 		";
       "a=''abcdef""""""";
       "           end";
       "				end              "], f);

scinotes(f, ["indent" "trailing" "quote"]);

good = ["a=""abcdef""";
        "if a==1";
	"    b=1+""abcdef""";
	"    c=2";
	"    d=3";
	"    if";
	"        a=""abcdef""""""";
	"    end";
	"end"]

corrected = mgetl(f);

if (good <> corrected) then pause, end;