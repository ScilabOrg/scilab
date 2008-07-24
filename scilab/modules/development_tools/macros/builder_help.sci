function builder_help(toolbox_path)
	mprintf('Building help...\n');
	mprintf('Please notice that you need JDK in your path to build the help.\n');
	devtools_run_builder(toolbox_path, 'help', 'builder_help.sce');
endfunction
