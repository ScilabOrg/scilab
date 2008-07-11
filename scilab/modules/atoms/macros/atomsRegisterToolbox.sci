function atomsRegisterToolbox(name,path)
	
	if fileinfo(TMPDIR+"/loaded_toolboxes") <> [] then
		load(TMPDIR+"/loaded_toolboxes","loaded_toolboxes");
	else
		loaded_toolboxes = [];
	end
	
	loaded_toolboxes = [ loaded_toolboxes ; name path ];
	
	save(TMPDIR+"/loaded_toolboxes",loaded_toolboxes);
	
endfunction
