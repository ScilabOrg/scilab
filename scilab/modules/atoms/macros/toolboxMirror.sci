// Recuperation des inforations de base
// avril 2008 by Delphine

// Fonction de récupération de la liste des Mirror
function listMirror = toolboxMirror()
  if getos() == "Windows"
    listMirror = ["http://128.93.23.238/scilab/bin/windows/contrib"]
  else // linux et mac
    listMirror = ["http://128.93.23.238/scilab/src/contrib"]
  end
endfunction
