// Fonction de lecture d'un fichier ligne à ligne
// avril 2008 by Delphine

function tab = atomsReadFile(nom)
  // On a vérifié la présence du fichier avant
  // Il faudra verifier qu'on a les droits pour lire
  fd = mopen(nom,"r")
  [n, tab] = mfscanf(-1, fd, '%80[^\n]\n')
  mclose(fd)
endfunction
