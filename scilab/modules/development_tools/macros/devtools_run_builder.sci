function devtools_run_builder(path, subdir, builder)
	oldpath = pwd();
	
	if(~exists('path', 'local')) then
		path = oldpath;
	else
		chdir(path);
	end
	
	if(isdir(subdir)) then
		chdir(subdir);
		exec(builder, -1);
	end
	
	chdir(oldpath);
endfunction
