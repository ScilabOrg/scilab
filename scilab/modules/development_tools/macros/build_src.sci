function build_src(names, files, flag, src_path, libs, ldflags, cflags, ..
                   fflags, cc, libname, loadername, makename)
	oldpath = pwd();
	
	if ~exists('src_path', 'local') then
		src_path = oldpath;
	else
		chdir(src_path);
	end
	
	if ~exists('libs', 'local') then libs = []; end
	if ~exists('ldflags', 'local') then ldflags = ""; end
	if ~exists('cflags', 'local') then cflags = ""; end
	if ~exists('fflags', 'local') then fflags = ""; end
	if ~exists('cc', 'local') then cc = ""; end
	if ~exists('libname', 'local') then libname = ""; end
	if ~exists('loadername', 'local') then loadername = "loader.sce"; end
	if ~exists('makename', 'local') then makename = "Makelib"; end
	
	ilib_for_link(names, files, libs, flag, makename, loadername, ..
	              libname, ldflags, cflags, fflags, cc);
	
	chdir(oldpath);
endfunction
