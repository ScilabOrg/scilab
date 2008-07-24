function build_help(toolbox_name, help_lang_path)
	if(~exists(help_lang_path, 'local'))
		help_lang_path = pwd();
	end
	
	xmltojar(help_lang_path, toolbox_name);
endfunction
