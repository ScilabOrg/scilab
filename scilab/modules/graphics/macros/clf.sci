// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function clf(varargin)

if size(varargin)==0 then
  h=gcf()
  job='clear'
elseif size(varargin)==1 then
  if type(varargin(1))==1 then // win num given
    num=varargin(1)
    //f=gcf();
    h=[];for k=1:size(num,'*'),h=[h;get_figure_handle(num(k))];end
    //scf(f);
    job='clear'
  elseif type(varargin(1))==9 then // handle given
    h=varargin(1);job='clear'
  elseif type(varargin(1))==10 then // job given
    h=gcf()
    job=varargin(1)
  end
elseif size(varargin)==2 then
  if type(varargin(1))==1 then // win num given
    num=varargin(1)
    //f=gcf();
    h=[];for k=1:size(num,'*'),h=[h;get_figure_handle(num(k))];end
    //scf(f);
  elseif type(varargin(1))==9 then // handle given
    h=varargin(1);
  end
  job=varargin(2);
else
  error('clf must be called with at most 2 arguments')
end
if and(job<>['clear','reset']) then 
  error('clf: job argument possible values are '"clear"' or '"reset"'')
end
for k=1:size(h,'*') 
  hk=h(k)
  if stripblanks(hk.type)<>'Figure' then
    error('handle should be of type '"Figure"'')
  end
  c=hk.children;
  vis=hk.visible;hk.visible='off';

  for i=1:size(c,'*'),delete(c(i)),end
  hk.visible=vis
  if job=='reset' then
    props=['axes_size','auto_resize','figure_name','color_map','pixmap','pixel_drawing_mode','background','visible','rotation_style']
    df=gdf()
    for p=props
      hk(p)=df(p);
    end
  end
end
endfunction
