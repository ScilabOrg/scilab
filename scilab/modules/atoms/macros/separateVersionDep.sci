// Séparation de la dépendance et de la version
// avril 2008 by Delphine

function [depend, version] = separateVersionDep(depend)
  version = ""
  index = strindex(depend, "(")
  if index > 0
    // index-1 pour laisser la ( avec la version
    temp = strsplit(depend,index-1) 
    depend = temp(1)
    version = temp(2)
    // On enlève les parenthèses et les espaces
    version = strsubst(version, "(", "")
    version = strsubst(version, ")", "")
    version = strsubst(version, " ", "")
    depend = strsubst(depend, " ", "")
  end
endfunction
