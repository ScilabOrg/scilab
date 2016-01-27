//
//  Copyright (C) 2013 - S/E - Sylvestre Ledru
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
//
//
// <-- JVM MANDATORY -->
//

jimport java.lang.String;

s = String.new("Hello JIMS !!");
s1 = s.toUpperCase();
s2 = s.toLowerCase();
[S1 S2] = junwraprem(jwrap(s1), jwrap(s2));
jremove s

assert_checkequal(S1,"HELLO JIMS !!");
assert_checkequal(S2,"hello jims !!");
