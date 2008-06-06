// Séparation du signe >=, <= ou =  et de la version
// avril 2008 by Delphine

function [signe, version] = separateSignVersion(version)
  index = strindex(version, "=")
  version = strsplit(version,index)
  signe = version(1)
  version = version(2)
endfunction
