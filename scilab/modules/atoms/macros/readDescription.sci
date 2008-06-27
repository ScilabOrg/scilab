// Parsing of the Description file
// avril 2008 by Delphine

function desc = readDescription(nom)
  // On va dans le repertoire contenant les toolboxes
  rep = toolboxDirectory()
  d = rep + nom
  // Gestion des OS differents
  if getos() == "Windows"
    directory = d + "\DESCRIPTION"
  else // linux et mac
    directory = d + "/DESCRIPTION"
  end
  // Soit on trouve le dossier en local et le fichier DESCRIPTION est présent
  if (isdir(d) & ls(directory) <> [])
    cd (d)
    // Lecture du fichier description qu'on stocke dans un tableau
    tab = readFile("DESCRIPTION")
    // Création d'un "tableau de hash"
    desc = listDescription()
    // Remplissage du tableau de hash
    desc = hashTable(desc, tab)
    // Traitement du nom de fichier (caractères spéciaux) pour assurer la validité du nom dossier & chemins d'accès. 
    desc("Toolbox") = substituteString(desc("Toolbox"))
    // On rajoute le champs fonction
    clearglobal numberFunction
    desc("Function") = readDescriptionFunctions(nom)
  // Soit on va voir sur le net
  else
  	cd (rep)
  	// Création d'un "tableau de hash"
	desc = listDescription()
	// Liste des site à parcourir
	listMirror = toolboxMirror()
	[n, m] = size(listMirror)
	clearglobal numberFunction
	global numberFunction
	numberFunction = 0
	for i=1:m
	  // On récupère le fichier sur le site et on met une copie dans le dossier sous le nom de TOOLBOXES
  	  if dlFile(listMirror(i), "TOOLBOXES")
        // Lecture du fichier TOOLBOXES qu'on stocke dans un tableau
        tab = readFile("TOOLBOXES")
        // On supprime le fichier temporaire créé
        if ~removeFile("TOOLBOXES")
	      disp("Veuillez supprimer le fichier TOOLBOXES dans le repertoire courant")
	    end
        // On rempli le tableau avec les différentes toolboxes
        desc = hashTable2(desc, tab)
        // Traitement des noms de fichier (caractères spéciaux) pour assurer la validité du nom dossier & chemins d'accès.
        [a, b] = size(desc("Toolbox"))
        for j=1:a
          desc("Toolbox")(j) = substituteString(desc("Toolbox")(j))
        end
      else
        result = %f
        return result
      end
    end
  end
  result = %t
  return result
endfunction

// Récupération d'un fichier sur le web
function result = dlFile(web, fileWeb)
	temp = unix_g("wget " + web + "/TOOLBOXES" + " -O " + fileWeb)
	// Si le fichier n'est pas present
	if temp == ""
	  displayMessage("Verifiez la validite du repository")
	  if ~removeFile(fileWeb)
	    disp("Veuillez supprimer le fichier " + fileWeb + " dans le repertoire courant")
	  end
	  result = %f
	  return result
	end
	result = %t
	return result
endfunction

// Fonction de suppression d'un fichier
function result = removeFile(fileR)
  result = deletefile(fileR)
  return result
endfunction

// Remplissage du tableau de hash simple (fichier DESCRIPTION)
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

// Remplissage du tableau de hash (fichier TOOLBOXE)
function listDesc = hashTable2(listDesc, tabDesc)
  global numberFunction
  [listeObl, listeOpt] = constant()
  [n, m] = size(tabDesc)
  [nbTool, m] = size(listDesc("Toolbox"))
  [o, p] = size(listeOpt)
  inFunct = %f
  for i=1:n
    // On crée un flag pour savoir si on est dans la partie function
    if tabDesc(i) == "--"
      inFunct = %t
    elseif tabDesc(i) == "//"
      inFunct = %f
    end
    if tabDesc(i) <> "//" & ~inFunct
      ind = strindex(tabDesc(i),':')
      // ind+1 pour enlever l'espace avant le 2ème champ
      temp = strsplit(tabDesc(i),ind+1)
      // On retire le ": "
      temp(1) = strsubst(temp(1), ": ", "")
      listDesc(temp(1))(nbTool+1)= temp(2)
    elseif tabDesc(i) == "--"
      clear tmp
      nbFunct = 0
    elseif tabDesc(i) <> "--" & inFunct
      ind = strindex(tabDesc(i),'-')
      // Si ind = [] on est dans la n-ième ligne du champs précédent
      if ind == []
        tmp(string(nbFunct)) = tmp(string(nbFunct)) + tabDesc(i)
      else
        nbFunct = nbFunct + 1
        tmp(string(nbFunct)) = tabDesc(i)
        if numberFunction < nbFunct
          numberFunction = nbFunct
        end
      end
    elseif tabDesc(i) == "//"
      // Au cas ou une des toolbox n'ait pas les fonctions, pour éviter le bug : "Undefined variable: tmp"
      if isdef("tmp")
        listDesc("Function")(nbTool+1)= tmp
      end
      // On crée les "cases" optionnel pour que même si ce champs n'est pas présent dans le tableau, la case existe.
      // Mais seulement si le // est suivi d'autre chose
      if i <> n
        for j=1:p
          listDesc(listeOpt(j))(nbTool+2) = ""
        end
      end
      nbTool=nbTool+1
    end
  end
endfunction
