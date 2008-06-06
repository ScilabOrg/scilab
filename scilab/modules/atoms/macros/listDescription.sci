// Création d'une contenant les informations du fichier DESCRIPTION
// avril 2008 by Delphine

function listDesc = listDescription()
  [listeObl, listeOpt] = constant()
  [n, m] = size(listeObl)
  for i=1:m
    listDesc(listeObl(i))= []
  end
  [n, o] = size(listeOpt)
  for i=1:o
    listDesc(listeOpt(i))= []
  end
endfunction
