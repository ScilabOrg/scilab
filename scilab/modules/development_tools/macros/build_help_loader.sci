function build_help_loader(toolbox_title, help_lang_path)
	oldpath = pwd();
	
	if(~exists('help_lang_path', 'local')) then
		help_lang_path = oldpath;
	else
		chdir(help_lang_path);
	end
	
	loader = [
	  "// ====================================================================",
	  "// Copyright INRIA 2008",
	  "// Allan CORNET",
	  "// ====================================================================",
	  "path = get_absolute_file_path(''addchapter.sce'');",
	  "add_help_chapter(''"+toolbox_title+"'',path,%F);",
	  "clear path add_help_chapter;",
	  "// ===================================================================="
	];

	mputl(loader, 'addchapter.sce');
	chdir(oldpath);
endfunction
