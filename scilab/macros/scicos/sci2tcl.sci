function tt=sci2tcl(ttx)
//Author: Enrico Segre
//sledgehammer approach: transform every char of the string to \xNN
// in order to escape tcl sensibles like []{}; while giving a chance
// to extended ascii chars, without getting in trouble with \xHH+NN 
// occurrences
//This should work for handy nonliterals like \mu, ^2, ^3 in iso8859-1,
// no idea what really happens with multi-byte encodings...
  tt=[];
  for j=1:size(ttx,"*")
    tt=[tt,strcat("\x"+dec2hex(ascii(ttx(j))))]
  end
  tt=strcat(tt,"\n")  // Note: was once "\n " -- indent intentional?
endfunction


