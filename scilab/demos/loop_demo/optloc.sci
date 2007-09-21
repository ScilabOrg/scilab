function demo_optloc()
//  demo_help demo_optloc
  stacksize(1D7)
  n=20; //le nombre de points

  //calcul des coordonnees des points
  Alpha=round(99*rand(n,1))+1;
  Beta=round(99*rand(n,1))+1;
  
  clf();
  SetPosition() ;
  curFig = gcf();
  curFig.figure_style = "new";
  toolbar(curFig.figure_id, "off");
  xselect();
  curAxe = gca();
  curAxe.mark_size = 4;
  plot2d(Alpha,Beta,style=-9,rect=[1 1 100 100])
  e = gce();
  e.children.mark_style = 9; 
  e.children.mark_size_unit = "point";
  
  curAxe.font_size = 1;
  curAxe.title.text = "Position des consommateurs et des services potentiels";
  curAxe.title.font_size = 5;

  realtimeinit(0.1);for k=1:10,realtime(k),end 

  //Choix des cout de contruction cj
  C=100*ones(n,1); // cout tous egaux
  timer();
  [X1,Y1]=optloc(Alpha,Beta,C);

  kf=find(Y1>0);
  plot2d(Alpha(kf),Beta(kf),style = [-9,2],leg='Ressources 1')
  e = gce();
  e.children.mark_style = 9; 
  e.children.mark_size_unit = "point";
  e.children.mark_foreground = 5;


  [ic,jc]=find(X1>0);
  xsegs([Alpha(ic) Alpha(jc)]',[Beta(ic),Beta(jc)]',12)




  C=800*ones(n,1); // cout tous egaux

  [X1,Y1]=optloc(Alpha,Beta,C);
  disp(timer())
  kf=find(Y1>0);

  plot2d(Alpha(kf),Beta(kf),style=[-9,2],leg='Ressources 2');
  e = gce();
  e.children.mark_style = 9; 
  e.children.mark_size_unit = "point";
  e.children.mark_foreground = 9;

  [ic,jc]=find(X1>0);
  xsegs([Alpha(ic) Alpha(jc)]',[Beta(ic),Beta(jc)]',15);
  realtimeinit(0.1);for k=1:30,realtime(k),end
  xdel() ;
endfunction

function [X,Y]=optloc(Alpha,Beta,C)
  n=size(Alpha,'*')
  if n<>size(Beta,'*')| n<>size(C,'*') then 
    error('dimension incorrectes')
  end
  // matrices colonnes
  Alpha=Alpha(:);Beta=Beta(:);C=C(:);
  printf("1\n");
  // calcul des distances euclidiennes entre les points
  //D(i,j)=( (Alpha(i)-Alpha(j))^2+(Beta(i)-Beta(j))^2)^(1/2)
  D=sqrt((Alpha*ones(1,n)-ones(n,1)*Alpha').^2+(Beta*ones(1,n)-ones(n,1)*Beta').^2);
  // determination des parametres de linpro
  // ======================================

  // critere a minimiser
  //  ---               ---  --- 
  //  \		      \    \   
  //Z= >(C(j)*Y(j)) +    >    > D(i,j)*X(i,j)
  //  /		      /    /   
  //  ---j	      ---i ---j

  // Z = p'*x  x=[X(:);Y(:)]

  p=[D(:);C(:)];

  // contraintes de bornes
  //  X(i,j)>=0
  //  Y(j)<=1  
  //  Y(j)>=0
  // les deux dernieres contraintes remplacent    Y(j)=0 ou Y(j)=1

  ci=[zeros(n*n,1);  //X(i,j)>=0
      zeros(n,1)  ]; //Y(j)>=0

  cs=[ones(n*n,1)    //X(i,j)<=1
      ones(n,1)  ];  //Y(j)<=1

  // contraintes lineaires d'inegalites
  //  X(i,j)<=Y(j)   ==>  X(i,j)-Y(j)<=0

  // c*x<=b
  b2=zeros(n*n,1);
  A2=[eye(n*n,n*n),-eye(n,n).*.ones(n,1)];  // X(i,j)-Y(j)
  
  printf("2\n");
  //contraintes d'egalites
  //  ---           
  //  \		  
  //   > X(i,j) = 1 
  //  /		  
  //  ---j	

  b1=ones(n,1);
  A1=[ones(1,n).*.eye(n,n) zeros(n,n)];

  //resolution
  x0=zeros(n*n+n,1);
  timer();
  [x,lagr,f]=linpro(p,[A1;A2],[b1;b2],ci,cs,n)//,x0)
  disp(timer())
  if max(abs(x-round(x)))>1.d-6 then warning('solution non entiere'),end

  printf("3\n");

  x=round(x)
  X=matrix(x(1:n^2),n,n)
  Y=x(n^2+1:$)
  printf("4\n");

endfunction

