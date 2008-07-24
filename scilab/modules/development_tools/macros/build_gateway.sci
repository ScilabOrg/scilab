function build_gateway(libname, names, files, gateway_path, libs, ldflags, ..
                       cflags, fflags, cc, makename, ismex)
	oldpath = pwd();
	
	if exists('gateway_path', 'local') then
		chdir(gateway_path);
	end
	
	if ~exists('libs', 'local') then libs = []; end
	if ~exists('ldflags', 'local') then ldflags = ""; end
	if ~exists('cflags', 'local') then cflags = ""; end
	if ~exists('fflags', 'local') then fflags = ""; end
	if ~exists('cc', 'local') then cc = ""; end
	if ~exists('makename', 'local') then makename = "Makelib"; end
	if ~exists('ismex', 'local') then ismex = %f; end
	
	ilib_build(libname, names, files, libs, makename, ldflags, cflags, ..
	           fflags, ismex, cc);
	
	chdir(oldpath);
endfunction