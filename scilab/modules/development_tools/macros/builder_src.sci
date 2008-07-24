function builder_src(toolbox_path)
	if ~haveacompiler() then
	  error(gettext('This toolbox requires a compiler to build.'));
	end
	mprintf(gettext('Building sources...\n'));
	devtools_run_builder(toolbox_path, 'src', 'builder_src.sce');
endfunction
