function s = archiveCompress(archive_name,files,arg_string)
    if exists("archive_name") & exists("files") & exists("arg_string") then
        i  = int32(length(length(arg_string)))
	arg_string_copy = strsplit(arg_string);
	for j = 1:i
	    if ~(arg_string_copy(i) == 'z' | arg_string_copy == 'j' | arg_string_copy == 'y' | arg_string_copy == 'Z') then
	        error(999, msprintf(_(" Wrong argument option.\n")));
	    end
	end
	files = strsplit(files,' ');
	file_length = int32(length(length(files)));
	file_list = "";
	for i = 1:file_length
	    filename = pathconvert(files(i),%f,%t,"u");
	    if isfile(filename) | isdir(filename) then
	        file_list = file_list + " " + filename;
	    else
	        error(999, msprintf(_(" Invalid file path.\n")));
	    end
	end
	s = archive_compress(archive_name,file_list,arg_string)
    else
        error(999, msprintf(_(" Wrong number of arguments.\n")));
    end
endfunction
