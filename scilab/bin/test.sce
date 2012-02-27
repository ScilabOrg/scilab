
function r=Err(x)
	r=norm(x,1)
endfunction
rand('normal')

//define tools
function A=testmat1(a,n)
	//eigen values are given by a dilation of nth roots of 1
	A=diag(a*ones(1,n-1),1)+diag((1/a)*ones(1,n-1),-1)
	A(1,n)=1/a;A(n,1)=a
endfunction
clear sel
function t=sel(R),t=real(R)<0 ,endfunction

A=testmat1(3,5);Ac=testmat1(3+%i,5);

[U,n]=schur(A,'sel')



//[U,T]=schur(A)

//A=testmat1(1,5);E=testmat1(-2,5) ;

//[As,Es,Q,Dim]=schur(A,E,'c')

//A=testmat1(1,5);E=testmat1(-2,5) ;
//[As,Es,Q,Z]=schur(A,E)

//flag = 'c'
//[As,Es,Z,dim] = schur(A,E,flag)
//[Q,Z,dim] = schur(A,E,flag)
//[Z,dim] = schur(A,E,flag)
//[dim] = schur(A,E,flag)
//schur(A,E,flag) (ans = dim)


function t=sel(R)
printf("dans sel !\n");
t=real(R)<0;
endfunction

a=rand(3,3)
[e toto]=schur(a,sel)
//[e toto]=schur(rand(3,3),sel)

function t=sel(R,T)
printf("dans sel !\n");
t=real(R)<0;
endfunction


a=rand(3,3)+rand(3,3)*%i
b=rand(3,3)+rand(3,3)*%i
[AS, Es, Z, Dim] = schur(a, b, 'c')

/*********************************************/
cd TMPDIR;
A = rand(3,3)
Ac = rand(3,3) + %i*rand(3,3)



// equal to schur(A, 'c');
function t=sel(R)
t=real(R)<0;
endfunction

C=['int mytest(double* _real, double* _img) {' 
   'return *_real < 0 ? 1 : 0;}';];
mputl(C,TMPDIR+'/mytest.c');
lp=ilib_for_link('mytest','mytest.c',[],'c');
exec loader.sce


// equal to schur(Ac, 'd');
function t=sel(R)
t=norm(R)<1;
disp(norm(R));
endfunction



C=[ '#include ""doublecomplex.h"";'
    'extern double dpythags(double,double);'
    'int mytest(doublecomplex* _complex) {'
    'printf(""dpythags : %f\t%f\t"",_complex->r, _complex->i);'
    'printf(""= %f\n"",dpythags(_complex->r, _complex->i));'
    'if(dpythags(_complex->r, _complex->i) < 1) {return 1;}'
    'else{ return 0;} }';]
mputl(C,TMPDIR+'/mytest.c')
lp=ilib_for_link('mytest','mytest.c',[],'c');
exec loader.sce



[U, dim]=schur(Ac, 'd')
[U, dim]=schur(Ac, 'mytest')
[U, dim]=schur(Ac, sel)

/*********************************************/


cd TMPDIR;


mputl(C,TMPDIR+'/mytest.c')

//build and link
lp=ilib_for_link('mytest','mytest.c',[],'c');
exec loader.sce
link
sel = "mytest"




function ydot = f(t,yin)
    ydot(1)=-0.040*yin(1) + 1.0D4*yin(2)*yin(3);
    ydot(3)=3.0D7*yin(2)**2;
    ydot(2)=-ydot(1) - ydot(3);
endfunction

y(1)    = 1;
y(2)    = 0;
y(3)    = 0;
t       = 0;
rtol    = 1.0d-4;
atol(1) = 1.0d-6;
atol(2) = 1.0d-10;
atol(3) = 1.0d-6;
tout    = 0.4*exp((0:11)*log(10));
tout2   = 0.4*exp(12*log(10));

[yout w iw] = ode(y, t, tout, rtol, atol, f);
yout2 = ode(y, t, tout2, rtol, atol, f, w, iw);




      write (*,'(A, I3)') 'liw = ', liw
      write (*,'(A, I3)') 'ilw(1) = ', istk(ilw+20)
      write (*,'(A, I3)') 'ilw(2) = ', istk(ilw+21)
      write (*,'(A, E12.5)') 'dble(ilw(1)) = ', dble(istk(ilw+20))
      write (*,'(A, E12.5)') 'dble(ilw(2)) = ', dble(istk(ilw+21))
      write (*,'(A, I5)') 'l = ', l
      write (*,'(A, I5)') 'ilw = ', ilw
      write (*,'(A, I5)') 'ilc = ', ilc


/*********** module spreadsheet => macro resdxls *****************************/

    fil = SCI+'/modules/spreadsheet/tests/unit_tests/readxls.xls'
    fil=pathconvert(fil,%f,%t)
    sheets=mlist(['xls','sheets'],list())
    [fd,Text,Sheetnames,Abspos]=xls_open(fil)
    Text=['', Text]
    [Value,TextInd]=xls_read(fd,Abspos(1))
    T=matrix(Text(TextInd+1),size(Value))
    sheet=mlist(['xlssheet','name','text','value'], Sheetnames(1), T, Value)

    if Value==[] then
        sheet=mlist(['xlssheet','name','text','value'],Sheetnames(k),[],Value)
    else
        if size(Text,'*')==1 then
            T=emptystr(Value)
        else
            T=matrix(Text(TextInd+1),size(Value))
        end
        sheet=mlist(['xlssheet','name','text','value'],Sheetnames(k),T,Value)
    end
    sheets.sheets($+1)= sheet

    mclose(fd)
endfunction






a = 0;                  // interval of interpolation
b = 1;
n = 10;                 // nb of interpolation  points
m = 800;                // discretisation for evaluation
x = linspace(a,b,n)';   // abscissae of interpolation points
y = rand(x);            // ordinates of interpolation points
xx = linspace(a,b,m)';

splin(x,y,"not_a_knot")
splin(x,y,"fast")
splin(x,y,"monotone")







function v=f(x,y,z)
    v=cos(2*%pi*x).*sin(2*%pi*y).*cos(2*%pi*z)
endfunction

n = 10;  // n x n x n  interpolation points
x = linspace(0,1,n); 
y=x; 
z=x; // interpolation grid
[X,Y,Z] = ndgrid(x,y,z);
V = f(X,Y,Z);
tl = splin3d(x,y,z,V,[5 5 5]);
m = 10000;
// compute an approximated error
xp = grand(m,1,"def"); yp = grand(m,1,"def"); zp = grand(m,1,"def");
vp_exact = f(xp,yp,zp);
vp_interp = interp3d(xp,yp,zp, tl);
er = max(abs(vp_exact - vp_interp))




n = 15;
xy = grand(n,2,"unf",0,2*%pi);
z = cos(xy(:,1)).*cos(xy(:,2));
xyz = [xy z]

xyz =[
    5.1190599,    6.2384572,    0.3951584  ;
    0.8512271,    6.0161928,    0.6357094  ;
    5.6912586,    6.2609512,    0.8296621  ;
    5.2465137,    6.0625735,    0.4967495  ;
    0.7978817,    6.0802066,    0.6838905  ;
    6.0875757,    0.9903122,    0.5379700  ;
    5.7389098,    4.5605807,  - 0.1293741  ;
    1.3887978,    6.0984143,    0.1779146  ;
    3.9732303,    6.164494,   - 0.6689267  ;
    1.9362707,    6.0140573,  - 0.3445274  ;
    0.6128644,    0.6902817,    0.6307354  ;
    3.4382884,    3.0497051,    0.9522734  ;
    1.7498559,    5.014647,   - 0.0530175  ;
    1.1836389,    5.0283105,    0.1173044  ;
    3.4361579,    1.8662911,    0.2786702  ;
]

tl_coef = cshep2d(xyz)









function z=f(x,y)
    z = 1+ 50*(x.*(1-x).*y.*(1-y)).^2
endfunction

x = linspace(0,1,5);
[X,Y] = ndgrid(x,x);
X = X(:); Y = Y(:); Z = f(X,Y);




X=[0. 0.25 0.5 0.75 1. 0. 0.25 0.5 0.75 1. 0. 0.25 0.5 0.75 1. 0. 0.25 0.5 0.75 1. 0. 0.25 0.5 0.75 1.]';
Y=[0. 0. 0. 0. 0. 0.25 0.25 0.25 0.25 0.25 0.5 0.5 0.5 0.5 0.5 0.75 0.75 0.75 0.75 0.75 1. 1. 1. 1. 1.]';
Z=[1. 1. 1. 1. 1. 1. 1.0617981 1.1098633 1.0617981 1. 1. 1.1098633 1.1953125 1.1098633 1. 1. 1.0617981 1.1098633 1.0617981 1. 1. 1. 1. 1. 1.]';

S = cshep2d([X Y Z]);

m = 4;
xx = linspace(-1.5,0.5,m);
//[xp,yp] = ndgrid(xx,xx);

xp=[
  - 1.5       , - 1.5       , - 1.5       , - 1.5        ;
  - 0.8333333 , - 0.8333333 , - 0.8333333 , - 0.8333333  ;
  - 0.1666667 , - 0.1666667 , - 0.1666667 , - 0.1666667  ;
    0.5       ,   0.5       ,   0.5       ,   0.5    ;
];

yp=[
  - 1.5 , - 0.8333333 , - 0.1666667  ,  0.5  ;
  - 1.5 , - 0.8333333 , - 0.1666667  ,  0.5  ;
  - 1.5 , - 0.8333333 , - 0.1666667  ,  0.5  ;
  - 1.5 , - 0.8333333 , - 0.1666667  ,  0.5  ;
];

zp = eval_cshep2d(xp,yp,S)
[zp, a,b] = eval_cshep2d(xp,yp,S)
[zp, a, b, c, r, e] = eval_cshep2d(xp,yp,S)

/!\

ndgrid => .*.
grand

/!\
































