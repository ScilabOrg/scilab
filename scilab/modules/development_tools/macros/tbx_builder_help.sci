// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run the toolbox_path+"/help/builder_help.sce" script if it exists
// See devtools_run_builder
function tbx_builder_help(toolbox_path)
	mprintf(gettext('Building help...\n'));
	mprintf(gettext('Please notice that you need JDK in your path to build the help.\n'));
	devtools_run_builder(toolbox_path, 'help', 'builder_help.sce');
endfunction
