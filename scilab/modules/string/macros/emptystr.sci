// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function result = emptystr(varargin)
	[lhs, rhs] = argn(0);

	if rhs == 0 then
		result = "";
	else
		if rhs == 1 then
			[x,y] = size(varargin(1));
			result = "";
			result(x,y) = "";
		else
			if rhs == 2 then
				result = "";
				result(varargin(1),varargin(2)) = "";
			end
		end
	end
	
endfunction

