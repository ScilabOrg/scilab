function builder_gateway_lang(lang, gateway_path)
	oldpath = pwd();
	
	if(exists('gateway_path', 'local')) then
		chdir(gateway_path);
	end
	
	if(isdir(lang)) then
		chdir(lang);
		exec('builder_gateway_' + lang + '.sce', -1);
	end
	
	chdir(oldpath);
endfunction
