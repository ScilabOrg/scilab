// Recuperation des inforations de base
// avril 2008 by Delphine

// Fonction de récupération du repertoire contenant les toolboxes en fonction de l'OS
function rep = atomsToolboxDirectory()
  rep = pathconvert([SCIHOME+"/atoms"; "SCI/contrib"]);
endfunction
