// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre Koumar
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function childrenNode = getChildrenNode(tree, node)

	[lhs,rhs]=argn(0);

	//Input arguments checking
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"), "getChildrenNode",2));
		return;
	end

	// Check 1st and 2nd inputs : tree & (node or position)
	if rhs == 2 then
		if (typeof(tree) == 'Tree') then
			myTree = tree;
			isNode = %F;
			isPosition = %F;
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Tree expected.\n"), "getChildrenNode",1));
			return;
		end
		
		if (typeof(node) == 'Tree') then
			myNode = node;
			isNode = %T;
				
		elseif (type(node) == 10) then
			myPosition = node;
			isPosition = %T;
				
		else
			error(msprintf(gettext("%s: Wrong type for input argument #%d: Tree or String expected.\n"), "getChildrenNode",2));
			return;				
		end
	end
	
	// Find children nodes
	function childrenNode = findChildren(myTree, childrenNode)
	
		childrenNode($+1) = myTree;

		for index = 3:size(myTree)
			childrenNode = findChildren(myTree(index), childrenNode)
		end

	endfunction

	if isNode then
		// First find if the node exists and if we have multiple matching nodes
		result = findNode(myTree, myNode)
	end
	
	if isPosition then
		// First find if the position exists and if we have multiple matching nodes
		result = findNode(myTree, myPosition)	
	end

	if (size(result) == 1) then
		parentNode = result(1);
		
		result2 = list();
		// List containing children nodes		
		result2 = findChildren(parentNode, result2);

		childrenNode = list();
		for index = 2:size(result2)
			childrenNode($+1) = result2(index);
		end
	else
		childrenNode = list();
	end

endfunction
