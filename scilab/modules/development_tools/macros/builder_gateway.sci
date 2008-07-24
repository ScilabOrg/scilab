function builder_gateway(toolbox_path)
	if ~haveacompiler() then
	  error(gettext('This toolbox requires a compiler to build.'));
	end
	mprintf(gettext('Building gateway...\n'));
	devtools_run_builder(toolbox_path, 'sci_gateway', 'builder_gateway.sce');
endfunction
