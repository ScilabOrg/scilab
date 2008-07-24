function builder_macros(toolbox_path)
	mprintf(gettext('Building macros...\n'));
	devtools_run_builder(toolbox_path, 'macros', 'buildmacros.sce');
endfunction
