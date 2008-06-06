// Découpage de la version en 2 ou 3 chiffres
// avril 2008 by Delphine

function version = decoupVersion(version)
  index = strindex(version, ".")
  version = strsplit(version,index)
  version = strsubst(version, ".", "")
  version = strsubst(version, " ", "")
endfunction
