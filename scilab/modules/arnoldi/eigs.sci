// macro pour eigs
function [d, v] = eigs(varargin)
	lhs = argn(1);
	disp(lhs)

	rhs = argn(2);
	disp(rhs)

	if (rhs == 0 | rhs > 5) then
		error(msprintf(gettext("%s : Wrong number for input argument(s) : %d to %d expected.\n"), "eigs", 1, 5));
	end		

	if (rhs >= 1) then
		if ((typeof(varargin(1)) <> "constant") & (typeof(varargin(1)) <> "function") & (typeof(varargin(1)) <> "sparse"))
			error(msprintf(gettext("%s: Wrong type for input argument(s) #%d: a matrice or function expected"), "eigs",1));
		end
		if (isreal(varargin(1)))
			resid = rand(size(varargin(1),"r"),1);
		else
			resid = rand(size(varargin(1),"r"),1).*%i;
		end
	end
	
	maxiter = 300;
	tol = %eps;
	ncv = [];
	cholB = 0;
	info = int32(0);
	//resid = zeros(size(varargin(1),"r"),1);
	
	select rhs
	case 1
		nev =  min(size(varargin(1),'r'), 6)
		select lhs
		case 1
			d = %_eigs(varargin(1), [], nev, 'LM', maxiter, tol, ncv, cholB, resid, info);
		case 2
			[d, v] = %_eigs(varargin(1), [], nev, 'LM', maxiter, tol, ncv, cholB, resid, info)
		end
	
	case 2
		nev = min(size(varargin(1),'r'), 6)
		select lhs
		case 1
			d = %_eigs(varargin(1), varargin(2), nev, 'LM', maxiter, tol, ncv, cholB, resid, info);
		case 2
			[d, v] = %_eigs(varargin(1), varargin(2), nev, 'LM', maxiter, tol, ncv, cholB, resid, info)
		end

	case 3
		select lhs
		case 1
			d = %_eigs(varargin(1), varargin(2), varargin(3), 'LM', maxiter, tol, ncv, cholB, resid, info);
		case 2
			[d, v] = %_eigs(varargin(1), varargin(2), varargin(3), 'LM', maxiter, tol, ncv, cholB, resid, info)
		end
	
	case 4
		select lhs
		case 1
			d = %_eigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);
		case 2
			[d, v] = %_eigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info)
		end

	case 5
		select lhs
		case 1
			opts = varargin(5);
			if (~isstruct(opts)) then
				error(msprintf(gettext("%s: Wrong type for input argument(s): %d a structure expected"), "eigs",5));
			end
			if (and(~isfield(opts, ["tol", "maxiter", "ncv", "resid", "cholB"])))
				error(msprintf(gettext("%s: Wrong type for input argument(s): The fieldname does not exist. \nUse opts with tol, maxiter, ncv, resid, cholB"), "eigs"));
			end
			if (isfield(opts,"tol"))
				tol = opts.tol;
			end
			if (isfield(opts,"maxiter"))
				maxiter = opts.maxiter;
			end
			if (isfield(opts, "ncv"))
				ncv = opts.ncv;
			end
			if (isfield(opts,"resid"))
				resid = opts.resid;
				info = int32(1);
				if (and(resid==0))
					info = int32(0);
				end
				
			end
			if (isfield(opts,"cholB"))
				cholB = opts.cholB;
			end
			d = %_eigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);

		case 2
			opts = varargin(5);
			if (~isstruct(opts)) then
				error(msprintf(gettext("%s: Wrong type for input argument(s): %d a structure expected"), "eigs",5));
			end
			if (and(~isfield(opts, ["tol", "maxiter", "ncv", "resid", "cholB"])))
				error(msprintf(gettext("%s: Wrong type for input argument(s): The fieldname does not exist. \nUse opts with tol, maxiter, ncv, resid, cholB"), "eigs"));
			end
			if (isfield(opts,"tol"))
				tol = opts.tol;
			end
			if (isfield(opts,"maxiter"))
				maxiter = opts.maxiter;
			end
			if (isfield(opts, "ncv"))
				ncv = opts.ncv;
			end
			if (isfield(opts,"resid"))
				resid = opts.resid;
				info = int32(1);
				if (and(resid==0))
					info = int32(0);
				end
				
			end
			if (isfield(opts,"cholB"))
				cholB = opts.cholB;
			end
			[d, v] = %_eigs(varargin(1), varargin(2), varargin(3), varargin(4), maxiter, tol, ncv, cholB, resid, info);
		end
	end
		
endfunction

