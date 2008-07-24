function builder_gateway(toolbox_path)
	if ~haveacompiler() then
	  error('This toolbox requires a compiler to build.');
	end
	mprintf('Building gateway...\n');
	devtools_run_builder(toolbox_path, 'sci_gateway', 'builder_gateway.sce');
endfunction
