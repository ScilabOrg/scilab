// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_clock(tree)
    // M2SCI function
    // Conversion function for Matlab clock()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    w = gettempvar()
    elt1=Operation(":",list(Cste(1),Cste(2)),list())
    elt2=Operation(":",list(Cste(6),Cste(9)),list())
    row=Operation("rc",list(elt1,elt2),list())

    // w = getdate();
    insert(Equal(list(w),Funcall("getdate",1,list(),list())))

    // w(3:5)=[] (then w with 7 values)
    imp=Operation(":",list(Cste(3),Cste(5)),list())
    w35=Operation("ins",list(w,imp),list())
    insert(Equal(list(w35),Cste([])))

    // w(6)=w(6)+w(7)/1000
    w7=Operation("ext",list(w,Cste(7)),list())
    w6ext=Operation("ext",list(w,Cste(6)),list())
    w6ins=Operation("ext",list(w,Cste(6)),list())
    div=Operation("/",list(w7,Cste(1000)),list())
    add=Operation("+",list(w6ext,div),list())
    insert(Equal(list(w6ins),add))

    // c= w(1:6)
    imp=Operation(":",list(Cste(1),Cste(6)),list())
    tree=Operation("ext",list(w,imp),tree.lhs)

    tree.out(1).dims=list(1,6)
    tree.out(1).type=Type(Double,Real)
endfunction
