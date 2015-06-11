function s = ArchiveExtract(filename,arg_list)
	if exists("filename") & exists("arg_list") then
	    filepath = fullpath(filename);
	    perm_flag = 0;
	    if isfile(filepath) then
                for i = 1:length(length(arg_list))
	            if arg_list(i) == 'p' then
	                perm_flag = 1
                    elseif length(arg_list) == 0 then
		        perm_flag = 0
		    else
		        error(999, msprintf(_(" Wrong input argument\n")));
		    end
		end
	        s = archive_extract(filepath,perm_flag);
		if s == 1 then
		    s = -1
		end
	   else
	       error(999, msprintf(_("Sorry the file does not exist\n")));
	   end
	else
	    error(999, msprintf(_("Insufficient Arguments\n")));
	end
endfunction
