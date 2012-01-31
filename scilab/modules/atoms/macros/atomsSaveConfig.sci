// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Simon GARESTE <simon.gareste@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Save the config file in a config.bak file, so that we don't lose information while testing

function atomsSaveConfig(force)

    rhs=argn(2);

    load("SCI/modules/atoms/macros/atoms_internals/lib");

    if rhs > 1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsSaveConfig",0,1));
    end
    
    if rhs==0 then
        force=%F;
    end

    if isfile(atomsPath("system","user")+"config.bak") then
	    if ~force then
	        error(msprintf("The file config.bak already exists, aborting.\n"));
	    elseif isfile(atomsPath("system","user")+"config") then
        	    copyfile(atomsPath("system","user")+"config",atomsPath("system","user")+"config.bak");
		        msprintf(gettext("config file saved to %s"),atomsPath("system","user")+"config.bak");
	        else
		    error(msprintf("No config available, aborting.\n"));
	    end
    elseif isfile(atomsPath("system","user")+"config") then
	    copyfile(atomsPath("system","user")+"config",atomsPath("system","user")+"config.bak");
        msprintf(gettext("config file saved to %s"),atomsPath("system","user")+"config.bak");
    else
        mputl("",atomsPath("system","user")+'config');
	mputl("",atomsPath("system","user")+'config.bak');
    end


endfunction
