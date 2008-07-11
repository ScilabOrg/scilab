function atomsUnregisterToolbox(name)
	
	if fileinfo(TMPDIR+"/loaded_toolboxes") <> [] then
		load(TMPDIR+"/loaded_toolboxes","loaded_toolboxes");
	else
		return;
	end
	
	toolbox_num = find( loaded_toolboxes == name );
	
	if toolbox_num<>[] then
		loaded_toolboxes(toolbox_num,:)   = []; // delete the name
	end
	
	save(TMPDIR+"/loaded_toolboxes",loaded_toolboxes);
	
endfunction
