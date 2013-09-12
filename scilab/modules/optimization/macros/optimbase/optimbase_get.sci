// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


//
// optimbase_get --
//   Get the value for the given key.
//   If the key is unknown, generates an error.
//   This command corresponds with options which are not
//   available directly to the user interface, but are computed internally.
//
function value = optimbase_get (this,key)
    [lhs,rhs]=argn();
    if ( rhs <> 2 ) then
        errmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "optimbase_get", 2);
        error(errmsg)
    end
    select key
    case "-funevals" then
        value = this.funevals;
    case "-iterations" then
        value = this.iterations;
    case "-xopt" then
        value = this.xopt;
    case "-fopt" then
        value = this.fopt;
    case "-historyxopt" then
        if ( ~this.storehistory ) then
            errmsg = msprintf(gettext("%s: History disabled ; enable -storehistory option."),"optimbase_get")
            error(errmsg)
        else
            value = this.historyxopt;
        end
    case "-historyfopt" then
        if ( ~this.storehistory ) then
            errmsg = msprintf(gettext("%s: History disabled ; enable -storehistory option."),"optimbase_get")
            error(errmsg)
        else
            value = this.historyfopt;
        end
    case "-fx0" then
        value = this.fx0;
    case "-status" then
        value = this.status;
    case "-logstartup" then
        value = this.logstartup;
    else
        errmsg = msprintf(gettext("%s: Unknown key %s"),"optimbase_get",key)
        error(errmsg)
    end
endfunction

