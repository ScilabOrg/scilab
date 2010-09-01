function ret=includes_flag(path_strings)
abs_path=pwd();
if MSDOS then
  ret='';
  for path=path_strings
      ret=ret+' -I""'+fullpath(path)+'""';
  end
else
  ret="";
  for path=path_strings
      ret=ret+" -I"+fullpath(path);
  end
end
endfunction