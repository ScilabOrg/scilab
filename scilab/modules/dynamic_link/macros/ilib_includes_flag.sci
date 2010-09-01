function ret=ilib_includes_flag(path_strings)

if ( typeof(path_strings)~="string" )
  error(gettext("Wrong argument type : matrix of strings expected"));
end

if (getos() == "Windows") then
  ret='';
else
  ret="";
end

for path=path_strings
    if ( ~isdir(fullpath(path)) )
      error(gettext("Failed to add include path : ")+path);
    end
    if (getos() == "Windows") then  
      ret=ret+' -I""'+fullpath(path)+'""';
    else
      ret=ret+" -I"+fullpath(path);
    end
end
endfunction