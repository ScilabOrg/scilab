function s = archiveList(filename)
	filepath = pathconvert(filename,%f,%t,"u");
	if isfile(filename) then
	    output = archive_list(filepath);
	    output_split = strsplit(output,";");
	    if output_split(1)=="ERROR" then
	        error(999, msprintf(_("Sorry the Archive could not be read\n")));
	    else
	        s = output_split(2:length(length(output_split)))
	    end
	else
	    error(999, msprintf(_("No such file Exists.\n")));
	end
endfunction
