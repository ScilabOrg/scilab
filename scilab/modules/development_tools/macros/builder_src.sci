function builder_src(toolbox_path)
	if ~haveacompiler() then
	  error('This toolbox requires a compiler to build.');
	end
	mprintf('Building sources...\n');
	devtools_run_builder(toolbox_path, 'src', 'builder_src.sce');
endfunction
