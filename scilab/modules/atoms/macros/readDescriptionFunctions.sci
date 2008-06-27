// Parsing of the Description-Function file
// mai 2008 by Delphine

function descFunct = readDescriptionFunctions(nom)
  global numberFunction
  // On va dans le repertoire contenant les toolboxes
  rep = toolboxDirectory()
  d = rep + nom
  // Gestion des OS differents
  if getos() == "Windows"
    directory = d + "\DESCRIPTION-FUNCTIONS"
  else // linux et mac
    directory = d + "/DESCRIPTION-FUNCTIONS"
  end
  // Soit on trouve le dossier en local et le fichier DESCRIPTION-FUNCTIONS est présent
  if (isdir(d) & ls(directory) <> [])
    cd (d)
    // Lecture du fichier description qu'on stocke dans un tableau
    tab = readFile("DESCRIPTION-FUNCTIONS")
    // création d'une liste
    [n, m] = size(tab)
    numberFunction = 0
    for i=1:n
      ind = strindex(tab(i),'-')
      // Si ind = [] on est dans la n-ième ligne du champs précédent
      if ind == []
        descFunct(string(numberFunction)) = descFunct(string(numberFunction)) + tab(i)
      else
        numberFunction = numberFunction + 1
        descFunct(string(numberFunction)) = tab(i)
      end
    end
  else
    displayMessage("Le fichier DESCRIPTION-FUNCTIONS de la toolbox " + nom + " est manquant")
    descFunct = ""
  end
  return descFunct
endfunction
