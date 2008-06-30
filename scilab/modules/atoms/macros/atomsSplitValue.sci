// S'il y a plusieurs valeurs on split en plusieurs string
// avril 2008 by Delphine

function fieldValue = atomsSplitValue(fieldValue, separate)
  index = strindex(fieldValue, separate)
  if length(index) > 0
    fieldValue = strsplit(fieldValue,index)
    //On enleve le separateur et les expaces qui trainent en fin field
    fieldValue = strsubst(fieldValue, separate, "")
    fieldValue = strsubst(fieldValue, " ", "")
  end
endfunction
