function builder_src_lang(lang, src_path)
	oldpath = pwd();
	
	if(exists('src_path', 'local')) then
		chdir(src_path);
	end
	
	if(isdir(lang)) then
		chdir(lang);
		exec('builder_' + lang + '.sce');
	end
	
	chdir(oldpath);
endfunction
