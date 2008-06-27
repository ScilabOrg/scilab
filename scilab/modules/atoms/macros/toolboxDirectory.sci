// Recuperation des inforations de base
// avril 2008 by Delphine

// Fonction de récupération du repertoire contenant les toolboxes en fonction de l'OS
function rep = toolboxDirectory()
  if getos() == "Windows"
    rep = SCIHOME + "\atoms"
  else // linux et mac
    rep = SCIHOME + "/atoms"
  end
endfunction
