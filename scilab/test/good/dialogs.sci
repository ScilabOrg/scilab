function rep=x_message(comment,btns)
// Copyright INRIA
// message - dialogue affichant un message
//%Syntaxe
// message(comment)
//%Parametres
//comment : vecteur de chaine contenant le texte du message
//%Remarques
//Dans l'environnement Xwindow  cette  macro provoque  l'ouverture d'une
//fenetre de message. L'utilisateur doit confirmer la lecture du message
//en cliquant dans "ok"
//
//Dans un autre environnement la macros affiche le message dans
// la fenetre scilab et attend que l'utilisateur confirme la lecture
// par un retour-chariot
//%exemple
//  message(['Identification du systeme';'methode des moindres carres'])
//!
rep=[]
[lhs,rhs]=argn(0)
comment=matrix(comment,prod(size(comment)),1)
if rhs==1 then
  write(%IO(2),[comment;' ';'Ok ?';' '])
else
  if size(btns,'*')==1
    write(%IO(2),[comment;' ';'Ok ?';' '])
  else
    write(%IO(2),[comment;' ';btns(:);' '])
    str=readline()
    rep=find(str==btns)
  end
end


//str=read(%IO(1),1,1,'(a)')
endfunction

function str=x_dialog(comment,default)
// Copyright INRIA
// dialog - dialogue pour l'acquisition d'une reponse
//%Syntaxe
// str=dialog(comment [,default])
//%Parametres
//comment : vecteur de chaine contenant le texte commentaire de la
//          reponse demandee
//default : reponse par defaut, peut etre donnee par un vecteur de
//          chaines
//
//str     : vecteur de chaine de caracteres correspondant a la reponse
//%Remarques
//Dans l'environnement Xwindow  cette  macro provoque  l'ouverture d'une
//fenetre de dialogue. L'utilisateur devant alors y  entrer et/ou editer
//sa reponse eventuellement su plusieurs lignes
//
//Dans un autre environnement la macros affiche le  commentaire   dans
//la fenetre basile et attend que l'utilisateur fournisse le texte de
// sa reponse ou simplement retour-chariot (reponse par defaut).
// Dans ce cas la reponse doit etre donnee en une seule ligne
//%exemple
// gain=evstr(dialog('donnez le gain du transfert','0.75'))
// bode(evstr(dialog(['BODE';'Entrez le nom du systeme'],'h')),0.01,100)
//!
[lhs,rhs]=argn(0)
if rhs==1 then default=' ',end
comment=matrix(comment,prod(size(comment)),1)
default=default(1) // for lmitool
write(%IO(2),[comment;'reponse par defaut :'+default;' '])
str=[]
while %t do
  s=readline()
  if part(s,1)=='o'|part(s,1)=='c' then break,end
  str=[str;s]
end
if str==' ' then str=default,end
write(%IO(2),'o[k]/c[ancel]')
rep=s
if part(rep,1)=='c' then str=[],end


endfunction

function str=x_mdialog(description,labels,valuesini)
// Copyright INRIA
//mdialog - dialogue d'acquisition de plusieures reponses textuelles
//%Syntaxe
//str=dialog(description,labels [,valuesini])
//%Parametres
//description : vecteur de chaines contenant le texte commentaire des
//             reponses demandees
//labels      : vecteur des chaines de caracteres donnant le texte associe
//              a chacun des champs question
//valuesini   : vecteur des chaines de caracteres donnant la reponse par
//              defaut pour chacun des champs.
//
//str     : chaine de caracteres correspondant a la reponse
//%Remarques
//Dans l'environnement Xwindow  cette  macro provoque  l'ouverture d'une
//fenetre de dialogue. L'utilisateur devant alors y  entrer et/ou editer
//sa reponse.
//
//Dans un autre environnement la macros affiche le  commentaire   dans
//la fenetre basile et attend que l'utilisateur fournisse le texte de
// sa reponse ou simplement retour-chariot (reponse par defaut).
// Dans ce cas la reponse doit etre donnee en une seule ligne
//%exemple
// sig=mdialog('specifier le signal sinusoidal',..
//             ['amplitude';'frequence';'phase    ';],['1';'10';'0'])
// ampl=evstr(sig(1))
// freq=evstr(sig(2))
// ph=evstr(sig(3))
//
// rep=mdialog(['Simulation du systeme';'avec un regulateur PI'],..
//                      ['gain proportionnel';'gain integral'],[' ';' '])
//%Voir aussi
//  x_dialog, x_choose, x_message, dialog, choose, message
//!
[lhs,rhs]=argn(0)
n=prod(size(labels))
if rhs==2 then valuesini(n)=' ',end

  write(%IO(2),description)
  n=prod(size(labels))
//  disp('ici');pause
  for k=1:n
    write(%IO(2), labels(k) +' valeur par defaut :'+valuesini(k))
    str(k,1)=readline()
    if str(k)==' ' then str(k)=valuesini(k),end
  end
  write(%IO(2),'o[k]/c[ancel]')
  rep=readline()
  if part(rep,1)=='c' then str=[],end
//  disp('la');pause

endfunction

function num=x_choose(tochoose,comment,button)
// choose - dialogue de selection
//%Syntaxe
// num=choose(tochoose,comment)
//%Parametres
//tochoose  : vecteur de chaines  de caracteres correspondants a  chacun
//            des choix possibles
//
//comment : vecteur de chaine contenant le texte commentaire a ce choix
//
//button  : chaine de caractere donnant le texte a apparaitre dans le bouton
//          de refus de choix
//%Remarques
//Dans l'environnement Xwindow  cette  macro provoque  l'ouverture d'une
//fenetre de selection. L'utilisateur devant alors  designer a la souris
//sa selection.
//
//Dans un autre environnement la macros affiche le texte des  choix dans
//la fenetre basile et attend que l'utilisateur reponde par le numero du
//choix qu'il veut effectuer la reponse 0 signifiant le refus des choix.
//%Exemple
//  choose(['PID';'Adaptatif';'LQG'],'Choisissez un type de regulateur')
//!
// Copyright INRIA
[lhs,rhs]=argn(0)
n=prod(size(tochoose))
tochoose=matrix(tochoose,n,1)
comment=matrix(comment,prod(size(comment)),1)
if rhs==2 then button='Annuler',end

sep=[],
for l=0:n,sep=[sep;':'],end
lmx=maxi([length(tochoose);length(comment)])

tochoose=[string(0:n)', [button;tochoose]]

write(%IO(2),[comment;' ';mat2tab(tochoose)])
num=-1
while num<0|num>n  then
  write(%IO(2),'Donnez le numero de votre choix')
  t=readline()
  [num,ierr]=evstr(t)
end
disp('choose:'+string(num))

endfunction

function tab=mat2tab(str,del)
//Etant   donne une  matrice  de chaine  de  caracteres str, mat2tab(str)
//retourne un vecteur colonne de chaines de caracteres  representant les
//elements de str repartis sous forme d'un tableau.
//mat2tab(str,del) (ou del est un vecteur de chaines a un ou deux elements)
//  retourne le meme tableau ou les colonnes sont separees par la chaine de
//   caracteres contenue dans del(1) et lignes par del(2) s'il exite
//%Exemple
//  tt=['0','a';'1','b';'2','c']
//  write(%io(2),mat2tab(tt))
//  write(%io(2),mat2tab(tt,'|'))
//  write(%io(2),mat2tab(tt,['|','-']))
//!
//origine S Steer 1991
// Copyright INRIA
[lhs,rhs]=argn(0)
job=0
if rhs==1 then
  delc=' ',
else
  delc=del(1)
  if prod(size(del))==2 then
    dell=del(2)
    dell=part(dell,1)
    job=1
  end
end
blk='                              ';blk=blk+blk+blk

[m,n]=size(str);len=length(str)
lmx=[];for col=len,lmx=[lmx,maxi(col)+1],end

ln=sum(lmx)+(n+1)*length(delc)
if job==1 then
  rd=delc
  for l=2:ln-1,rd=rd+dell,end
  rd=rd+delc
  tab=rd
else
  tab=[]
end
for l=1:m
  ll=delc+str(l,1)
  for k=2:n

    ll=ll+part(blk,1:lmx(k-1)-len(l,k-1))+delc+str(l,k)
  end
  ll=ll+part(blk,1:lmx(n)-len(l,n))+delc
  tab=[tab;ll]
  if job==1 then tab=[tab;rd],end
end
endfunction

function [btn,xc,yc,win,Cmenu]=xclick();
// Copyright INRIA
str=readline()
rep=evstr('list('+str+')')
if size(rep)<3 then pause,end
btn=rep(1)
xc=rep(2)
yc=rep(3)
mrk=xget('mark')
xset('mark',0,5)
plot2d(xc,yc,-1,'000')
plot2d(xc,yc,-1,'000')
xset('mark',mrk(1),mrk(2))
win=0
if size(rep)>=4 then
  win=rep(4),
end
if size(rep)>=5 then
  Cmenu=rep(5)
else
  Cmenu=[]
end
endfunction

function rep=xgetmouse(flag);
// Copyright INRIA
str=readline()
rep=evstr('['+str+']')
endfunction

function str=readline()
// Copyright INRIA
  global LineCount
  LineCount=LineCount+1
comm='//'
blank=' '
while %t do
  rep=read(%IO(1),1,1,'(a)')
  k=strindex(rep,comm)
  if k<>[] then
    k=k(1)
    if k>1&part(rep,1:k)<>part(blank,1:k) then
      str=stripblanks(part(rep,1:k-1))
      com=part(rep,k+1:length(rep))
      if part(str,1:3)=='-->' then
	execstr(part(str,4:length(str)))
      else
	break
      end
    end
  else
    n=length(rep)
    str=stripblanks(rep)
    com=emptystr()
    if part(str,1:3)=='-->' then
      execstr(part(str,4:length(str)))
    else
      break
    end
  end
end
bl='=';txt=com+':'+str;txt=part(bl,ones(1,60-length(txt)))+txt
write(%io(2),[' ';txt;' '])
endfunction

function c=getcolor(title,cini)
// Copyright INRIA
str=readline()
c=evstr('['+str+']')
endfunction

function [m,pt,btn]=getmenu(datas,pt)
// Copyright INRIA
[lhs,rhs]=argn(0)
n=size(datas,1)-3
if rhs<2 then
  [btn,xc,yc]=xclick()
  pt=[xc,yc]
else
  xc=pt(1);yc=pt(2)
end
str=readline()
m=evstr('['+str+']')
endfunction

function hilitemenu(m,datas)
// Copyright INRIA
xclip()
c=datas(m,:)
xm=c(1),ym=c(3),lm=c(2)-c(1);hm=c(3)-c(4)
thick=xget('thickness');xset('thickness',6)
xrect(xm,ym,lm,hm)
xset('thickness',thick)
xclip(datas(size(datas,1)-2,:))
endfunction

function unhilitemenu(m,datas)
// Copyright INRIA
xclip()
c=datas(m,:)
xm=c(1),ym=c(3),lm=c(2)-c(1);hm=c(3)-c(4)
thick=xget('thickness');xset('thickness',6)
xrect(xm,ym,lm,hm)
xset('thickness',thick)
xclip(datas(size(datas,1)-2,:))
endfunction

function setmenubar(datas,menus)
// Copyright INRIA
xclip()
alu=xget('alufunction')
xset('alufunction',6)
nm=prod(size(menus))
[w,klm]=maxi(length(menus));
b=xstringl(0,0,menus(klm));
for km=1:nm
  x=datas(km,1);lm=datas(km,2)-x;ym=datas(km,3);hm=-datas(km,4)+ym
  xrects([x;ym;lm;hm],-1)
  xstringb(x,ym-hm+(b(4)-b(2))/2,menus(km),lm,hm)
end
xset('alufunction',alu)
xclip(datas(nm+1,:))
endfunction

function erasemenubar(datas)
// Copyright INRIA
nm=size(datas,1)-3
xclip()
mrect=datas(nm+2,:)
xclea(mrect(1),mrect(2),mrect(3),mrect(4))
xclip(datas(nm+1,:))
endfunction
