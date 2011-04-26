// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimbase_outputcmd --
//   Calls back user's output command
// Arguments
//   this : the current object
//   state : the state of the algorithm,
//     "init", "done", "iter"
//   data : the data to pass to the client output command
//   stop : set to true to stop the algorithm
//
function stop = optimbase_outputcmd ( this , ...
   state , data )
  if this.outputcommand <> "" then
    typout = type(this.outputcommand)
    if ( or (typout==[11 13]) ) then
      stop = this.outputcommand ( state , data )
    else
	  __optimbase_output_f__ = this.outputcommand(1)
      stop = __optimbase_output_f__ ( state , data , this.outputcommand(2:$) )
    end
  end
endfunction

