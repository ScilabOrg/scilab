// Vérification de la bonne contruction d'une toolbox
// avril 2008 by Delphine

function result = checkToolbox(nom)
  // On enlève les charactères spéciaux
  nom = substituteString(nom)
  // On va dans le repertoire contenant les toolboxes
  rep = toolboxDirectory()
  d = rep + nom
  // Si le dossier n'existe pas
  if ~(isdir(d))
    displayMessage("Le dossier de la toolbox " + nom + " n''existe pas ou est mal nomme")
    result = %f
    return result
  end
  cd (d)
  if ~checkDescription()
    displayMessage("Le fichier DESCRIPTION n''est pas valide")
    result = %f
    return result
  end
    if ~checkDescriptionFunctions()
    displayMessage("Le fichier DESCRIPTION-FUNCTIONS n''est pas valide")
    result = %f
    return result
  end
  if ~checkLoader()
    displayMessage("Le fichier loader.sce n''est pas present")
    result = %f
    return result
  end
  if ~checkTest()
    displayMessage("Il serait interessant que des test soient disponibles")
  end
  if ~checkHelp()
    displayMessage("Il serait interessant qu''une aide soit disponible")
  end
  if ~checkDemo()
    displayMessage("Il serait interessant qu''une demonstration soit disponible")
  end
  if ~checkReadme()
    displayMessage("Il serait interessant que le fichier readme soit present")
  end
  if ~checkLicense()
    displayMessage("Il serait interessant que le fichier license soit present")
  end
  result = %t
  return result
endfunction

function result = checkDescription()
  // Si le fichier DESCRIPTION est présent
  if ls("DESCRIPTION") <> []
    // Lecture du fichier description qu'on stocke dans un tableau
    tab = readFile("DESCRIPTION")
    // Création d'un "tableau de hash"
    desc = listDescription()
    // Vérification de la présence d'un seul espace après les :
    [n, m] = size(desc)
      for i=1:n
        ind = strindex(tab(i),':')
        if (part([tab(i)],[ind+1]) <> " " | part([tab(i)],[ind+2])== " ")
          displayMessage("The fields "  + tab(i) + " should have one and only one whitespace after the :")
          result = %f
          return result
        end
      end
    // Remplissage du tableau de hash
    desc = hashTable(desc, tab)
    // Verification que tous les champs obligatoires sont remplis (Toolbox, Version, License, Description, Title, Author and Maintainer)
    [listeObl, listeOpt] = constant()
    for i=1:7
      if desc(listeObl(i)) == []
        displayMessage("Le champs " + listeObl(i) + " est absent de la DESCRIPTION")
        result = %f
        return result
      end
    end
    // Verification que le nom correspond bien
    if desc("Toolbox") <> nom
      displayMessage("Le nom de la toolbox present dans le fichier DESCRIPTION n''est pas le bon")
      result = %f
      return result
    end
    // Verification que la version est au bon format
    v = desc("Version")
    if regexp(v, '/[0-9]+\.[0-9]+\.?[0-9]*$/') <> 1
      disp("The version is a sequence of at least two (and usually three) non-negative integers separated by single ''.'' characters.")
      result = %f
      return result
    end
  // Soit le fichier DESCRIPTION n'existe pas
  else
    displayMessage("Le fichier DESCRIPTION n''existe pas ou n''est pas a la bonne place")
    result = %f
    return result
  end
  result = %t
  return result
endfunction

function result = checkDescriptionFunctions()
  result = %t
  // Si le fichier DESCRIPTION-FUNCTIONS est présent
  if ls("DESCRIPTION-FUNCTIONS") <> []
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
        descFunct(string(numberFunction + 1)) = tab(i)
        numberFunction = numberFunction + 1
      end
    end
    // Vérification qu'aucune fonction ne porte le nom d'une fonction de scilab
    // recupération des fonctions scilab
    scilabFunction = listPrimitives()
    // Récupération de chacun des noms de fonction
    for i=1:numberFunction
      ind = strindex(descFunct(string(i)),'-')
      funct = strsplit(descFunct(string(i)), ind)
      funct = funct(1)
      funct = strsubst(funct, "-", "")
      funct = strsubst(funct, " ", "")
      // On cherche le nom de la fonction dans la liste des fonctions de scilab
      if strindex(scilabFunction, " " + funct + " ") <> []
        displayMessage("La fonction " + funct + " existe deja dans scilab. Merci de changer de nom.")
        result = %f
      end
    end
  // Soit le fichier DESCRIPTION-FUNCTIONS n'existe pas
  else
    displayMessage("Le fichier DESCRIPTION-FUNCTIONS n''existe pas ou n''est pas a la bonne place")
    result = %f
  end
  return result
endfunction

// Remplissage d'une liste à partir du fichier DESCRIPTION
function listDesc = hashTable(listDesc, tabDesc)
  [listeObl, listeOpt] = constant()
  // On crée toutes les "cases" pour que même si un champs optionnel n'est pas présent dans le tableau, la case existe.
  [o, p] = size(listeOpt)
  for i=1:p
    listDesc(listeOpt(i))= ""
  end
  [n, m] = size(tabDesc)
  for i=1:n
    ind = strindex(tabDesc(i),':')
    // Si ind = [] on est dans la n-ième ligne du champs précédent
    if ind == []
      listDesc(temp(1)) = listDesc(temp(1)) + tabDesc(i)
    else
      // ind+1 pour enlever l'espace avant le 2ème champ
      temp = strsplit(tabDesc(i),ind+1)
      // On retire le ": "
      temp(1) = strsubst(temp(1), ": ", "")
      listDesc(temp(1))= temp(2)
    end
  end
endfunction

function var = listPrimitives()
  // recupération des fonctions scilab
  [primitives,commandes] = what();
  listLib = librarieslist();
  [n, m] = size(listLib);
  for i=1:n
    [o, m] = size(primitives);
    temp = libraryinfo(listLib(i));
    [p, m] = size(temp);
    for j=1:p
      primitives(o + j) = temp(j);
    end
  end
  [o, m] = size(primitives);
  var = " "
  for i=1:o
    var = var + primitives(i) + " ";
  end
endfunction

function result = checkLoader()
  if ~ls("loader.sce") <> []
    result = %f
    return result
  end
  result = %t
  return result
endfunction

function result = checkTest()
  if ~isdir("unit tests") <> []
    result = %f
    return result
  end
  result = %t
  return result
endfunction

function result = checkHelp()
  if ~isdir("help") <> []
    result = %f
    return result
  end
  result = %t
  return result  
endfunction

function result = checkDemo()
  if ~isdir("demos") <> []
    result = %f
    return result
  end
  result = %t
  return result  
endfunction

function result = checkReadme()
  if ~ls("readme.txt") <> []
    result = %f
    return result
  end
  result = %t
  return result  
endfunction

function result = checkLicense()
  if ~ls("license.txt") <> []
    result = %f
    return result
  end
  result = %t
  return result  
endfunction
