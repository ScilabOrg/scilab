// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//typename

[typs,nams]=typename();
if or(typs<>[1;2;4;5;6;7;8;9;10;11;13;14;15;16;17;128;129;130]) then pause,end
if or(nams<>["s";"p";"b";"sp";"spb";"msp";"i";"h";"c";"m";"mc";"f";"l";"tl";"ml";"ptr";"ip";"fptr"])  then pause,end
typename("mytype",1024);
[typs,nams]=typename();
if typs($)<>1024|nams($)<>"mytype"  then pause,end
typename("",1024);
