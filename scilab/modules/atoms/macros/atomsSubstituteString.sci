// Substitution of special characters in the toolbox's name
// avril 2008 by Delphine

function str = atomsSubstituteString(str)
    str = strsubst(str, " ", "_")
    str = strsubst(str, "é", "e")
    str = strsubst(str, "è", "e")
    str = strsubst(str, "ê", "e")
    str = strsubst(str, "à", "a")
    str = strsubst(str, "â", "a")
    str = strsubst(str, "ù", "u")
    str = strsubst(str, "ô", "o")
    str = strsubst(str, "î", "i")
    str = strsubst(str, "ç", "c")
    str = strsubst(str, "/", "")
    str = strsubst(str, "\", "")
    return str
endfunction
