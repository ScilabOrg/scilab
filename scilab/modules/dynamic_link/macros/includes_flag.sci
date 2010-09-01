function ret=includes_flag(path_strings)
if (getos() == "Windows") then
  ret='';
  for path=path_strings
      if ( ~exist(fullpath(path)) )
	error("Failed to add "+path);
      end
      ret=ret+' -I""'+fullpath(path)+'""';
  end
else
  ret="";
  for path=path_strings
      if( ~exist(fullpath(path)) )
	error("Failed to add "+path);
      end
      ret=ret+" -I"+fullpath(path);
  end
end
endfunction