
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - 
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

examples = [ 'ext1c.sce' ; 
             'ext2c.sce' ; 
             'ext3c.sce' ; 
             'ext4c.sce' ; 
             'ext5c.sce' ; 
             'ext6c.sce' ; 
             'ext7c.sce' ; 
             'ext8c.sce' ; 
             'ext9c.sce' ; 
             'ext10c.sce' ; 
             'ext11c.sce' ; 'ext12c.sce' ;
             'ext13c.sce' ; 'ext14c.sce' ];
             
for i=1:size(examples,'*')
   disp('-----------------------------------');
   disp('Example : ' + examples(i));
   disp('-----------------------------------');
   exec(examples(i));
   
end             









