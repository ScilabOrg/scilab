function builder_help_lang(lang, help_path)
	oldpath = pwd();
	
	if(exists('help_path', 'local')) then
		chdir(help_path);
	end
	
	if(isdir(lang)) then
		chdir(lang);
		exec('build_help.sce', -1);
	end
	
	chdir(oldpath);
endfunction	
