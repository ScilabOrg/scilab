// Recuperation des inforations de base
// avril 2008 by Delphine

// Champs présents dans le fichier DESCRIPTION
function [listeObl, listeOpt] = constant()
  // Champs obligatoires
  listeObl = ["Toolbox","Version","Title","Author","Maintainer","Description","License", "Category"]
  // Champs optionnels
  listeOpt = ["Date","Depends", "URL", "Entity"]
endfunction
