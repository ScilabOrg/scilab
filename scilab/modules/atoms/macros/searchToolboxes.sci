// Recherche d'une toolbox
// avril 2008 by Delphine

function result = searchToolboxes(keyWord, typeSearch)
  // S'il n'y a pas de type
  if argn(2) == 1
    typeSearch = "all"
  end
  result = %f
  listTool = []
  // On met en forme
  keyWord = splitWord(keyWord)
  typeSearch = convstr(typeSearch,"l")
  [a, b] = size(keyWord)
  global numberFunction
  listDesc = atomsReadDesc("")
  [n, m] = size(listDesc("Toolbox"))
  // On regarde les Toolboxes une à une
  for i=1:n
    // Différents cas de valeur de typeSearch
    if typeSearch == "all"
      // Recherche dans Description + Title + Toolbox + Category + Author
      titleTool = convstr(listDesc("Title")(i),"l")
      descTool = convstr(listDesc("Description")(i),"l")
      nameTool = convstr(listDesc("Toolbox")(i),"l")
      catTool = convstr(listDesc("Category")(i),"l")
      authorTool = convstr(listDesc("Author")(i),"l")
      functionTool = listDesc("Function")(i)
      // Comparaison mot-clé et mots (on recherche la position du mot-clé dans le mot, si c'est == [] c'est qu'il n'a pas été trouvé; auquel cas, inutile de comparer les autres mot-clés)
      toolFind = %t
      for j=1:a
        if strindex(titleTool, keyWord(j)) == [] & strindex(descTool, keyWord(j)) == [] & strindex(nameTool, keyWord(j)) == [] & strindex(catTool, keyWord(j)) == [] & strindex(authorTool, keyWord(j)) == []
          toolFind = %f
          for k=1:numberFunction
            if strindex(convstr(functionTool(string(k)), "l"), keyWord(j)) <> []
              toolFind = %t
            end
          end
          if ~toolFind
            break
          end
        end
      end
    elseif typeSearch == "author"
      // Recherche dans Author
      authorTool = convstr(listDesc("Author")(i),"l")
      // Comparaison mot-clé et mots
      toolFind = %t
      for j=1:a
        if strindex(authorTool, keyWord(j)) == []
          toolFind = %f
          break
        end
      end
    elseif typeSearch == "entity"
      // Recherche dans Entity
      entityTool = convstr(listDesc("Entity")(i),"l")
      // Comparaison mot-clé et mots
      toolFind = %t
      for j=1:a
        if strindex(entityTool, keyWord(j)) == []
          toolFind = %f
          break
        end
      end 
    elseif typeSearch == "function"
      // Recherche dans Function
      functionTool = listDesc("Function")(i)
      // Lecture ligne à ligne des fonctions
      // Comparaison mot-clé et mots
      toolFind = %t
      for j=1:a
        for k=1:numberFunction
          toolFind = %f
          if strindex(convstr(functionTool(string(k)), "l"), keyWord(j)) <> []
            toolFind = %t
            break
          end
        end
      end 
    else
      atomsDisplayMessage("Le champs de recherche " + typeSearch + " n''existe pas")
      return result
    end
    // Si on a trouvé tous les mots
    if toolFind
      [n, m] = size(listTool)
      // On regarde si la Toolbox concorde avec la version du scilab
      if atomsVerifVersionScilab(listDesc("ScilabVersion")(i))
        listTool(n+1) = listDesc("Toolbox")(i) + " - " + listDesc("Title")(i)
      else
        listTool(n+1) = listDesc("Toolbox")(i) + " - " + listDesc("Title")(i) + " - Attention cette Toolbox (Version " + listDesc("Version")(i) + ") n''est pas compatible avec votre version de Scilab"
      end
      result = %t
    end
  end
  if ~result
    atomsDisplayMessage("Aucune Toolbox ne correspond a la recherche")
  else
    listTool = unique(listTool)
    [n, m] = size(listTool)
    if n == 1
      atomsDisplayMessage("La Toolbox suivante correspond a la recherche :")
      atomsDisplayMessage(listTool)
    else
      atomsDisplayMessage("Les Toolboxes suivantes correspondent a la recherche :")
      for i=1:n
        atomsDisplayMessage("- " + listTool(i))
      end
    end
  end
  return result
endfunction

// On split les phrases en un tableau de mots
function var = splitWord(var)
  // On passe tout en minuscule
  var = convstr(var,"l")
  // On split (erreur si le dernier caractère est un " ")
  var = atomsSplitValue(var, " ")
endfunction
