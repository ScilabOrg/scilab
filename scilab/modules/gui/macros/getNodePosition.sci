// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function position = getNodePosition(tree, node)


	[lhs,rhs]=argn(0);

	//Input arguments checking
	if rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "getNodePosition",1,2));
		return;
	end

	// Check 1st input : tree
	if rhs >= 1 then
		if (typeof(tree) == 'Tree') then
			myTree = tree;
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Tree expected.\n"), "getNodePosition",1));
			return;
		end
		// Check 2nd input : node
		if rhs == 2 then
			if (typeof(node) == 'Tree') then
				myNode = node;
			else
				error(msprintf(gettext("%s: Wrong type for input argument #%d: Tree expected.\n"), "getNodePosition",2));
				return;				
			end
		end
	end

	
	// Find node(s) position(s)
	function r = findPos(myTree, myNode, r, curpos)
	
		if equalsTree(myTree, myNode) then
			r($+1) = curpos;
		end
		
		for index = 3:size(myTree)
			if curpos ~= "root" then
				r = findPos(myTree(index), myNode, r, curpos+"."+string(index-2))
			else
				r = findPos(myTree(index), myNode, r, string(index-2))
			end
		end

	endfunction
	
	// List of matching nodes
	r = list();

	position = findPos(myTree, myNode, r, "root");


endfunction
