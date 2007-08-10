function ChangeTextFont(h)
//Turpitude en attendant que l'on puisse tourner le text
//filled (scilab5) voir rotate_compound.sci case text

  if argn(2)==0 then h=gcf(),end
  select h.type
  case "Figure"   then
    n=size(h.children,'*');
    for k=1:n,   ChangeTextFont(h.children(k));end
  case "Axes"   then
    n=size(h.children,'*');
    for k=1:n,   ChangeTextFont(h.children(k));end
  case "Compound"   then
    n=size(h.children,'*');
    for k=1:n,   ChangeTextFont(h.children(k));end
  case "Text" then
    disp(h.user_data,h.text_box_mode,h.font_angle)
    if h.user_data == "filled"&h.font_angle <> 0 then
      sz=5;st=h.font_style;W=h.text_box(1),H=h.text_box(2),t=h.text;
      while %t
	rect=xstringl(0,0,t,st,sz)
	if (rect(3)<=W&rect(4)<H)|sz<1 then break,end
	sz=sz-1
      end
      h.font_size=sz
    end
  end
endfunction
