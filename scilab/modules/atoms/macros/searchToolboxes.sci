// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Research of a toolbox

function result = searchToolboxes(keyWord, typeSearch)
  // If there is no type
  if argn(2) == 1
    typeSearch = "all"
  end
  result = %f
  listTool = []
  // We shape
  keyWord = splitWord(keyWord)
  typeSearch = convstr(typeSearch,"l")
  [a, b] = size(keyWord)
  global numberFunction
  listDesc = atomsReadDesc("")
  [n, m] = size(listDesc("Toolbox"))
  // We check Toolboxes one to one
  for i=1:n
    // Differents value case of typeSearch
    if typeSearch == "all"
      // Research in Description + Title + Toolbox + Category + Author
      titleTool = convstr(listDesc("Title")(i),"l")
      descTool = convstr(listDesc("Description")(i),"l")
      nameTool = convstr(listDesc("Toolbox")(i),"l")
      catTool = convstr(listDesc("Category")(i),"l")
      authorTool = convstr(listDesc("Author")(i),"l")
      functionTool = listDesc("Function")(i)
      // Comparaison keyword and words (we research the key-word position in the word, if it's == [] the word is not found; in this case, it's useless to compare the other keywords)
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
      // Research in Author
      authorTool = convstr(listDesc("Author")(i),"l")
      // Comparaison keyword and words
      toolFind = %t
      for j=1:a
        if strindex(authorTool, keyWord(j)) == []
          toolFind = %f
          break
        end
      end
    elseif typeSearch == "entity"
      // Research in Entity
      entityTool = convstr(listDesc("Entity")(i),"l")
      // Comparaison keyword and words
      toolFind = %t
      for j=1:a
        if strindex(entityTool, keyWord(j)) == []
          toolFind = %f
          break
        end
      end 
    elseif typeSearch == "function"
      // Research in Function
      functionTool = listDesc("Function")(i)
      // Reading lines to lines of the functions
      // Comparaison keyword and words
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
      atomsDisplayMessage("The research field " + typeSearch + " doesn''t exist")
      return result
    end
    // If we found the words
    if toolFind
      [n, m] = size(listTool)
      // We check if the Toolbox suit to the scilab version
      if atomsVerifVersionScilab(listDesc("ScilabVersion")(i))
        listTool(n+1) = listDesc("Toolbox")(i) + " - " + listDesc("Title")(i)
      else
        listTool(n+1) = listDesc("Toolbox")(i) + " - " + listDesc("Title")(i) + " - Warning this toolbox (Version " + listDesc("Version")(i) + ") isn''t compatible with our scilab version"
      end
      result = %t
    end
  end
  if ~result
    disp("No Toolbox corresponds at the research")
  else
    listTool = unique(listTool)
    [n, m] = size(listTool)
    if n == 1
      disp("The following Toolbox corresponds at the research :")
      disp(listTool)
    else
      disp("The following Toolboxes corresponds at the research :")
      for i=1:n
        disp("- " + listTool(i))
      end
    end
  end
  return result
endfunction

// We split the sentences on a words array
function var = splitWord(var)
  // We cross everything in small letter
  var = convstr(var,"l")
  // We split (error if the last caracter is a " ")
  var = atomsSplitValue(var, " ")
endfunction
