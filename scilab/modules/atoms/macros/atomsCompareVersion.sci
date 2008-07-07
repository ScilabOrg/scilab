// Compare two versions ; returns 0 on equality, 1 if the first is greater
// than the second, -1 else
// june 2008 by Delphine

function result = atomsCompareVersion(v1, v2)
  if strtod(v1(1)) > strtod(v2(1))
    result = 1
  elseif strtod(v1(1)) < strtod(v2(1))
    result = -1
  else
    if size(v1, 1) >= 2 & size(v2, 1) >= 2
      result = atomsCompareVersion(v1(2:$), v2(2:$))
    else
      result = sign(size(v1, 1) - size(v2, 1))
    end
  end
endfunction
