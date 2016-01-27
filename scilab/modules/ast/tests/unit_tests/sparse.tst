//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Antoine ELIAS
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===
//
//
// <-- CLI SHELL MODE -->

// sparse function

//from dense form
s=[ 1,0,0,0,0,0,0;
0,1,0,0,0,0,0;
0,0,1,0,0,0,0;
0,0,1,1,0,0,0;
0,0,1,1,1,0,0;
0,0,1,1,0,1,0;
0,0,1,1,0,1,1];
sp=sparse(s);

assert_checkequal(size(s), size(sp));
assert_checkequal(full(sp), s);

//from coord
sp = sparse([1 1;2 2;3 3;4 3;4 4;5 3;5 4;5 5;6 3;6 4;6 6;7 3;7 4;7 6;7 7], [1 1 1 1 1 1 1 1 1 1 1 1 1 1 1]);
assert_checkequal(size(s), size(sp));
assert_checkequal(full(sp), s);

//with size
newsp = sparse([], [], [10,20]);
assert_checkequal(size(newsp), [10,20]);

//concatenation
spc = [sp [1; 0; 1; 0; 1; 0; 1]];
sc = [s [1; 0; 1; 0; 1; 0; 1]];
assert_checkequal(full(spc), sc);

spc = [sp ;[1 0 1 0 1 0 1]];
sc = [s ;[1 0 1 0 1 0 1]];
assert_checkequal(full(spc), sc);

spc = [[1; 0; 1; 0; 1; 0; 1] sp];
sc = [[1; 0; 1; 0; 1; 0; 1] s];
assert_checkequal(full(spc), sc);

spc = [[1 0 1 0 1 0 1]; sp];
sc = [[1 0 1 0 1 0 1]; s];
assert_checkequal(full(spc), sc);

sc = [s s];
spc = [sp sp];
assert_checkequal(full(spc), sc);

sc = [s; s];
spc = [sp; sp];
assert_checkequal(full(spc), sc);

sc = [s s];
spc = [sp sp];
assert_checkequal(full(spc), sc);

A = sparse(1);
A = [A 0];
A = [A 0];
A = [A 0];

assert_checkequal(full(A), [1 0 0 0]);
assert_checkequal(size(A, "c"), 4);

//extraction
assert_checkequal(full(sp(:)), s(:));
assert_checkequal(size(sp(:)), size(s(:)));
assert_checkequal(full(sp(1:7, 1:7)), s(1:7, 1:7));
assert_checkequal(size(sp(1:7, 1:7)), size(s(1:7, 1:7)));
assert_checkequal(full(sp(1:4, :)), s(1:4, :));
assert_checkequal(size(sp(1:4, :)), size(s(1:4, :)));
assert_checkequal(full(sp(:, 2:6)), s(:, 2:6));
assert_checkequal(size(sp(:, 2:6)), size(s(:, 2:6)));
assert_checkequal(full(sp(:, $)), s(:, $));
assert_checkequal(size(sp(:, $)), size(s(:, $)));

//next tests get from old module sparse (Scilab 5.4)
ij=[1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6];
v=[1;2;3;4;5;6;7;8;9];
ij1=[1 6;1 2;6 5];
v1=[-1;-1;-1];
vc=v+%i*(21:29)';
v1c=v1+%i*[0;0.3;-1.2];
zer=sparse([],[],[6,6]);

a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=sparse([1 6;1 2;6 5],[-1;-1;-1],[6 6]);
ma=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],-(1:9),[6 6]);
mb=sparse([1 6;1 2;6 5],-[-1;-1;-1],[6 6]);
ac=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],(1:9)+%i*(21:29),[6 6]);
bc=sparse([1 6;1 2;6 5],[-1;-1;-1]+%i*[0;0.3;-1.2],[6 6]);
mac=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],-(1:9)-%i*(21:29),[6 6]);
mbc=sparse([1 6;1 2;6 5],[1;1;1]+%i*[0;-0.3;1.2],[6 6]);


//-------------------------------------------------------------
//test des primitives sparse, full
//--------------------------------------------------------------
//  -- for real matrix
assert_checkequal(full(sparse(0.3)), 0.3);
v=0*ones(1,3);v(3)=5;v(1)=1;
assert_checkequal(full(sparse([1 1;1 3],[1 5])), v);
v=0*ones(1,300);v(300)=5;v(1)=1;
assert_checkequal(full(sparse([1 1;1 300],[1 5])), v);
v=0*ones(3,1);v(3)=5;v(1)=1;
assert_checkequal(full(sparse([1 1;3 1],[1 5])), v);
v=0*ones(300,1);v(300)=5;v(1)=1;
assert_checkequal(full(sparse([1 1;300 1],[1 5])), v);
sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
a=[0,0,3,0,2,1;;5,0,0,4,0,0;0,0,0,0,0,0;0,0,7,6,0,0;8,0,0,0,0,0;0,0,0,0,0,9];
assert_checkequal(full(sp), a);
sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[8 6]);a(8,6)=0;
assert_checkequal(full(sp), a);
assert_checkequal(full(sparse([],[],[4 10])), 0*ones(4,10));
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);
a=[0;10;11;12;0;13];
assert_checkequal(full(v), a);
v=sparse([1 2;1 3;1 4;1 6],[10;11;12;13],[1,6]);
a=[0,10,11,12,0,13];
assert_checkequal(full(v), a);
//  -- for complex matrix
sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],(1:9)+%i*(21:29),[6 6]);
a=[0,0,3+%i*23,0,2+%i*22,1+%i*21
5+%i*25,0,0,4+%i*24,0,0
0,0,0,0,0,0
0,0,7+%i*27,6+%i*26,0,0
8+%i*28,0,0,0,0,0
0,0,0,0,0,9+%i*29];
assert_checkequal(full(sp), a);
sp=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],(1:9)+%i*(21:29),[8 6]);
a(8,6)=0;
assert_checkequal(full(sp), a);
v=sparse([2 1;3 1;4 1;6 1],[10-3*%i;11;12+5*%i;13+0.5*%i],[6,1]);
a=[0;10-%i*3;11;12+%i*5;0;13+%i*0.5];
assert_checkequal(full(v), a);
v=sparse([1 2;1 3;1 4;1 6],[10-3*%i;11;12+5*%i;13+0.5*%i],[1,6]);
a=[0,10-%i*3,11,12+%i*5,0,13+%i*0.5];
assert_checkequal(full(v), a);
//-----------------------------------------------
// addition and substraction tests
//-----------------------------------------------
// -- real real
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=sparse([1 6;1 2;6 5],[-1;-1;-1],[6 6]);
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);
vt=sparse([1 2;1 3;1 4;1 6],[10;11;12;13],[1,6]);
ma=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],-(1:9),[6 6]);
mb=sparse([1 6;1 2;6 5],-[-1;-1;-1],[6 6]);
zer=sparse([],[],[6,6]);
ac=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],(1:9)+%i*(21:29),[6 6]);
bc=sparse([1 6;1 2;6 5],[-1;-1;-1]+%i*[0;0.3;-1.2],[6 6]);
vc=sparse([2 1;3 1;4 1;6 1],[10-3*%i;11;12+5*%i;13+0.5*%i],[6,1]);
vct=sparse([1 2;1 3;1 4;1 6],[10-3*%i;11;12+5*%i;13+0.5*%i],[1,6]);
mac=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],-(1:9)-%i*(21:29),[6 6]);
mbc=sparse([1 6;1 2;6 5],[1;1;1]+%i*[0;-0.3;1.2],[6 6]);

assert_checkequal(full(v+sparse([],[],[6,1])), full(v));
assert_checkequal(full(sparse([],[],[6,1])+v), full(v));
assert_checkequal(full(v+v), full(v)+full(v));
assert_checkequal(full(v-v), full(v)-full(v));
assert_checkequal(full(vt+sparse([],[],[1,6])), full(vt));
assert_checkequal(full(vt+vt), full(vt)+full(vt));
assert_checkequal(full(vt-vt), full(vt)-full(vt));
assert_checkequal(full(zer+zer), 0*ones(6,6));
assert_checkequal(full(a+a), full(a)+full(a));
assert_checkequal(full(b+b), full(b)+full(b));
assert_checkequal(full(a+zer), full(a));
assert_checkequal(full(zer+a), full(a));
assert_checkequal(full(b+a), full(b)+full(a));
assert_checkequal(full(a+b), full(b)+full(a));
assert_checkequal(full(a+ma), full(a)+full(ma));
assert_checkequal(full(a-a), full(a)-full(a));
assert_checkequal(full(a-ma), full(a)-full(ma));
assert_checkequal(full(b-mb), full(b)-full(mb));
assert_checkequal(full(a-zer), full(a));
assert_checkequal(full(zer-a), -full(a));
assert_checkequal(full(a-mb), full(a)-full(mb));
//  -- real complex
assert_checkequal(full(sparse([],[],[6,1])+vc), full(vc));
assert_checkequal(full(v+vc), full(v)+full(vc));
assert_checkequal(full(v-vc), full(v)-full(vc));
assert_checkequal(full(vt+vct), full(vt)+full(vct));
assert_checkequal(full(vt-vct), full(vt)-full(vct));
assert_checkequal(full(a+ac), full(a)+full(ac));
assert_checkequal(full(b+bc), full(b)+full(bc));
assert_checkequal(full(a+bc), full(a)+full(bc));
assert_checkequal(full(zer+ac), full(zer)+full(ac));
assert_checkequal(full(b+ac), full(b)+full(ac));
assert_checkequal(full(a-ac), full(a)-full(ac));
assert_checkequal(full(b-bc), full(b)-full(bc));
assert_checkequal(full(a-bc), full(a)-full(bc));
assert_checkequal(full(zer-ac), full(zer)-full(ac));
assert_checkequal(full(b-ac), full(b)-full(ac));
// -- complex real
assert_checkequal(full(vc+v), full(vc)+full(v));
assert_checkequal(full(vc-v), full(vc)-full(v));
assert_checkequal(full(vct+vt), full(vct)+full(vt));
assert_checkequal(full(vct-vt), full(vct)-full(vt));
assert_checkequal(full(ac+a), full(ac)+full(a));
assert_checkequal(full(bc+b), full(bc)+full(b));
assert_checkequal(full(ac+b), full(ac)+full(b));
assert_checkequal(full(ac+zer), full(zer)+full(ac));
assert_checkequal(full(bc+a), full(bc)+full(a));
assert_checkequal(full(ac-a), full(ac)-full(a));
assert_checkequal(full(bc-b), full(bc)-full(b));
assert_checkequal(full(ac-b), full(ac)-full(b));
assert_checkequal(full(ac-zer), -full(zer)+full(ac));
assert_checkequal(full(bc-a), full(bc)-full(a));
// -- complex complex
assert_checkequal(full(vc+vc), full(vc)+full(vc));
assert_checkequal(full(vc-vc), full(vc)-full(vc));
assert_checkequal(full(vct+vct), full(vct)+full(vct));
assert_checkequal(full(vct-vct), full(vct)-full(vct));
assert_checkequal(full(ac+ac), full(ac)+full(ac));
assert_checkequal(full(bc+bc), full(bc)+full(bc));
assert_checkequal(full(ac+bc), full(ac)+full(bc));
assert_checkequal(full(bc+ac), full(bc)+full(ac));
assert_checkequal(real(full(ac-ac)), full(zer));
assert_checkequal(imag(full(ac-ac)), full(zer));
assert_checkequal(real(full(bc-bc)), full(zer));
assert_checkequal(imag(full(bc-bc)), full(zer));
assert_checkequal(full(ac-bc), full(ac)-full(bc));
assert_checkequal(full(bc-ac), full(bc)-full(ac));
// sparse full and full sparse operation (soft coded apoerations)
assert_checkequal(full(a+1), full(a)+1);
assert_checkequal(full(1+a), full(a)+1);
assert_checkequal(full(a+ones(6,6)), full(a)+ones(6,6));
assert_checkequal(full(ones(6,6)+a), full(a)+ones(6,6));
assert_checkequal(full(a+2*eye()), full(a)+2*eye());
assert_checkequal(full(2*eye()+a), full(a)+2*eye());
assert_checkequal(full(a-1), full(a)-1);
assert_checkequal(full(1-a), 1-full(a));
assert_checkequal(full(a-ones(6,6)), full(a)-ones(6,6));
assert_checkequal(full(ones(6,6)-a), ones(6,6)-full(a));
assert_checkequal(full(a-2*eye()), full(a)-2*eye());
assert_checkequal(full(2*eye()-a), 2*eye()-full(a));
assert_checkequal(full(ac+1), full(ac)+1);
assert_checkequal(full(1+ac), full(ac)+1);
assert_checkequal(full(ac+ones(6,6)), full(ac)+ones(6,6));
assert_checkequal(full(ones(6,6)+ac), full(ac)+ones(6,6));
assert_checkequal(full(ac+2*eye()), full(ac)+2*eye());
assert_checkequal(full(2*eye()+ac), full(ac)+2*eye());
assert_checkequal(full(ac-1), full(ac)-1);
assert_checkequal(full(1-ac), 1-full(ac));
assert_checkequal(full(ac-ones(6,6)), full(ac)-ones(6,6));
assert_checkequal(full(ones(6,6)-ac), ones(6,6)-full(ac));
assert_checkequal(full(ac-2*eye()), full(ac)-2*eye());
assert_checkequal(full(2*eye()-ac), 2*eye()-full(ac));
assert_checkequal(full(ac+full(bc)), full(ac)+full(bc));
assert_checkequal(full(ac-full(bc)), full(ac)-full(bc));
assert_checkequal(full(full(ac)+full(bc)), full(ac)+full(bc));

//-----------------------------------------------
// multiplication tests
//-----------------------------------------------
// real x real
// sparse scalar , saclar sparse
assert_checkequal(full(a*2), full(a)*2);
assert_checkequal(full(2*a), full(a)*2);
assert_checkequal(a*[], []);
assert_checkequal([]*a, []);
c=rand(6,6);
assert_checkequal(a*c, full(a)*c);
assert_checkequal(c*a, c*full(a));
// sparse sparse
assert_checkequal(full(zer*zer), full(zer));
assert_checkequal(full(a*zer), full(zer));
assert_checkequal(full(zer*a), full(zer));
assert_checkequal(full(a*a), full(a)*full(a));
assert_checkequal(full(b*b), full(b)*full(b));
assert_checkequal(full(a*b), full(a)*full(b));
assert_checkequal(full(b*a), full(b)*full(a));
// complex x real real x complex
// sparse scalar , scalar sparse
assert_checkequal(full(ac*2), full(ac)*2);
assert_checkequal(full(2*ac), full(ac)*2);
assert_checkequal(full((2+%i)*a), (2+%i)*full(a));
assert_checkequal(full(a*(2+%i)), (2+%i)*full(a));
assert_checkequal(ac*[], []);
assert_checkequal([]*ac, []);
c=rand(6,6);
cc=c+rand(6,6)*%i;
assert_checkequal(ac*c, full(ac)*c);
assert_checkequal(cc*a, cc*full(a));
// sparse sparse
assert_checkequal(real(full(ac*zer)), full(zer));
assert_checkequal(imag(full(ac*zer)), full(zer));
assert_checkequal(real(full(zer*ac)), full(zer));
assert_checkequal(imag(full(zer*ac)), full(zer));
assert_checkequal(full(ac*a), full(ac)*full(a));
assert_checkequal(full(ac*a), full(ac)*full(a));
assert_checkequal(full(bc*b), full(bc)*full(b));
assert_checkequal(full(a*bc), full(a)*full(bc));
assert_checkequal(full(b*ac), full(b)*full(ac));
// // complex x complex
assert_checkequal(ac*cc, full(ac)*cc);
assert_checkequal(cc*ac, cc*full(ac));
// sparse sparse
assert_checkequal(full(ac*ac), full(ac)*full(ac));
assert_checkequal(full(bc*bc), full(bc)*full(bc));
assert_checkequal(full(bc*ac), full(bc)*full(ac));
//----------------------------------------------------------
// element wise multiplication tests
//----------------------------------------------------------
assert_checkequal(full(ac.*2), full(ac)*2);
assert_checkequal(full((2).*ac), full(ac)*2);
assert_checkequal(a.*[], []);
assert_checkequal([].*a, []);
c=rand(6,6);
//if norm(a*c-full(a)*c) >100*%eps then pause,end
//if norm(c*a-c*full(a)) >100*%eps then pause,end
// sparse sparse
assert_checkequal(full(zer.*zer), full(zer));
assert_checkequal(full(a.*zer), full(zer));
assert_checkequal(full(zer.*a), full(zer));
assert_checkequal(full(a.*a), full(a).*full(a));
assert_checkequal(full(b.*b), full(b).*full(b));
assert_checkequal(full(a.*b), full(a).*full(b));
assert_checkequal(full(b.*a), full(b).*full(a));
// complex x real real x complex
// sparse scalar , scalar sparse
assert_checkequal(full((2+%i).*a), (2+%i).*full(a));
assert_checkequal(full(a.*(2+%i)), (2+%i).*full(a));
assert_checkequal(ac.*[], []);
assert_checkequal([].*ac, []);
c=rand(6,6);
cc=c+rand(6,6)*%i;
assert_checkequal(full(ac.*c), full(ac).*c);
assert_checkequal(full(cc.*a), cc.*full(a));
// sparse sparse
assert_checkequal(full(ac.*zer), full(zer)*%i);
assert_checkequal(full(zer.*ac), full(zer)*%i);
assert_checkequal(full(ac.*a), full(ac).*full(a));
assert_checkequal(full(bc.*b), full(bc).*full(b));
assert_checkequal(full(a.*bc), full(a).*full(bc));
assert_checkequal(full(b.*ac), full(b).*full(ac));
// // complex x complex
assert_checkequal(full(ac.*cc), full(ac).*cc);
assert_checkequal(full(cc.*ac), cc.*full(ac));
// sparse sparse
assert_checkequal(full(ac.*ac), full(ac).*full(ac));
assert_checkequal(full(bc.*bc), full(bc).*full(bc));
assert_checkequal(full(bc.*ac), full(bc).*full(ac));
// ----------------------------------------------------------
// test de la transposition
//-----------------------------------------------------------
assert_checkequal(full(a'), full(a)');
assert_checkequal(full(ac'), full(ac)');
assert_checkequal(full(zer'), full(zer)' );
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);
assert_checkequal(full(v'), full(v)');
assert_checkequal(full((v')'), full(v));
vc=sparse([2 1;3 1;4 1;6 1],[10-3*%i;11;12+5*%i;13+0.5*%i],[6,1]);
assert_checkequal(full(vc'), full(vc)');
assert_checkequal(full((vc')'), full(vc));
// ----------------------------------------------------------
// test des concatenation
//-----------------------------------------------------------
assert_checkequal(full([a]), full(a));
assert_checkequal(full([a b]), [full(a) full(b)]);
assert_checkequal(full([a;b]), [full(a);full(b)]);
assert_checkequal(full([a []]), full(a));
assert_checkequal(full([a;[]]), full(a));
assert_checkequal(full([a zer]), [full(a) full(zer)]);
assert_checkequal(full([zer;b]), [full(zer);full(b)]);
assert_checkequal(full([ac]), full(ac));
assert_checkequal(full([ac b]), [full(ac) full(b)]);
assert_checkequal(full([ac;b]), [full(ac);full(b)]);
assert_checkequal(full([ac []]), full(ac));
assert_checkequal(full([ac;[]]), full(ac));
assert_checkequal(full([a bc]), [full(a) full(bc)]);
assert_checkequal(full([a;bc]), [full(a);full(bc)]);
assert_checkequal(full([ac bc]), [full(ac) full(bc)]);
assert_checkequal(full([ac;bc]), [full(ac);full(bc)]);
// ----------------------------------------------------------
// test des extractions
//-----------------------------------------------------------
af=full(a);
assert_checkequal(full(a(1,3)), af(1,3));
assert_checkequal(full(a(1,4)), af(1,4));
assert_checkequal(full(a(1,:)), af(1,:));
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);vf=full(v);
assert_checkequal(full(v(:)), vf(:));
assert_checkequal(full(v(3:4)), vf(3:4));
assert_checkequal(full(v([1 5])), vf([1 5]));
assert_checkequal(full(v([4 3])), vf([4 3]));
assert_checkequal(full(v([4 4])), vf([4 4]));
assert_checkequal(full(v([1 1])), vf([1 1]));
v=v';vf=vf';
assert_checkequal(full(v(:)), vf(:));
assert_checkequal(full(v(3:4)), vf(3:4));
assert_checkequal(full(v([1 5])), vf([1 5]));
assert_checkequal(full(v([4 3])), vf([4 3]));
assert_checkequal(full(v([4 4])), vf([4 4]));
assert_checkequal(full(v([1 1])), vf([1 1]));
acff=full(ac);
assert_checkequal(full(ac(1,3)), acff(1,3));
assert_checkequal(full(ac(1,4)), acff(1,4));
assert_checkequal(full(ac(1,:)), acff(1,:));
vc=sparse([2 1;3 1;4 1;6 1],[10-3*%i;11;12+5*%i;13+0.5*%i],[6,1]);vcf=full(vc);
assert_checkequal(full(vc(:)), vcf(:));
assert_checkequal(full(vc(3:4)), vcf(3:4));
assert_checkequal(full(vc([1 5])), vcf([1 5]));
assert_checkequal(full(vc([4 3])), vcf([4 3]));
assert_checkequal(full(vc([4 4])), vcf([4 4]));
assert_checkequal(full(vc([1 1])), vcf([1 1]));
vc=vc';vcf=vcf';
assert_checkequal(full(vc(:)), vcf(:));
assert_checkequal(full(vc(3:4)), vcf(3:4));
assert_checkequal(full(vc([1 5])), vcf([1 5]));
assert_checkequal(full(vc([4 3])), vcf([4 3]));
assert_checkequal(full(vc([4 4])), vcf([4 4]));
assert_checkequal(full(vc([1 1])), vcf([1 1]));
// ----------------------------------------------------------
// test des insertions
//-----------------------------------------------------------

a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
vt=sparse([1 2;1 3;1 4;1 6],[10;11;12;13],[1,6]);
// full line insertion
//----------------------
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(7,:)=vt;A=full(a);Vt=full(vt);A(7,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(8,:)=vt;A=full(a);Vt=full(vt);A(8,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1([1 3],:)=[vt;2*vt];A=full(a);Vt=full(vt);A([1 3],:)=[Vt;2*Vt];
assert_checkequal(full(a1), A);
a1=a;a1([3 1],:)=[vt;2*vt];A=full(a);Vt=full(vt);A([3 1],:)=[Vt;2*Vt];
assert_checkequal(full(a1), A);
a1=a;a1([1 3 1],:)=[vt;2*vt;3*vt];A=full(a);Vt=full(vt);A([1 3 1],:)=[Vt;2*Vt;3*Vt];
assert_checkequal(full(a1), A);
//  insert zero vector
vt=sparse([],[],[1,6]);
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(7,:)=vt;A=full(a);Vt=full(vt);A(7,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(8,:)=vt;A=full(a);Vt=full(vt);A(8,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1([1 3],:)=[vt;2*vt];A=full(a);Vt=full(vt);A([1 3],:)=[Vt;2*Vt];
assert_checkequal(full(a1), A);
a1=a;a1([3 1],:)=[vt;2*vt];A=full(a);Vt=full(vt);A([3 1],:)=[Vt;2*Vt];
assert_checkequal(full(a1), A);
a1=a;a1([1 3 1],:)=[vt;2*vt;3*vt];A=full(a);Vt=full(vt);A([1 3 1],:)=[Vt;2*Vt;3*Vt];
assert_checkequal(full(a1), A);
a=sparse([],[],[6,6]);
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(7,:)=vt;A=full(a);Vt=full(vt);A(7,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(8,:)=vt;A=full(a);Vt=full(vt);A(8,:)=Vt;
assert_checkequal(full(a1), A);
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4],10:15,[2,6]);
a1=a;a1([1 3],:)=b;A=full(a);B=full(b);A([1 3],:)=B;
assert_checkequal(full(a1), A);
a1=a;a1([3 1],:)=b;A=full(a);B=full(b);A([3 1],:)=B;
assert_checkequal(full(a1), A);
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4;3 3;3 5],10:17,[3,6]);
a1=a;a1([1 3 1],:)=b;A=full(a);B=full(b);A([1 3 1],:)=B;
assert_checkequal(full(a1), A);
//  insert zero vector
vt=sparse([],[],[1,6]);
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(1,:)=vt;A=full(a);Vt=full(vt);A(1,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);Vt=full(vt);A(3,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(7,:)=vt;A=full(a);Vt=full(vt);A(7,:)=Vt;
assert_checkequal(full(a1), A);
a1=a;a1(8,:)=vt;A=full(a);Vt=full(vt);A(8,:)=Vt;
assert_checkequal(full(a1), A);
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4],10:15,[2,6]);
a1=a;a1([1 3],:)=b;A=full(a);B=full(b);A([1 3],:)=B;
assert_checkequal(full(a1), A);
a1=a;a1([3 1],:)=b;A=full(a);B=full(b);A([3 1],:)=B;
assert_checkequal(full(a1), A);
b=sparse([1 1;1 3;1 6;2 1;2 2;2 4;3 3;3 5],10:17,[3,6]);
a1=a;a1([1 3 1],:)=b;A=full(a);B=full(b);A([1 3 1],:)=B;
assert_checkequal(full(a1), A);
// full column insertion
//----------------------
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
v=sparse([2 1;3 1;4 1;6 1],[10;11;12;13],[6,1]);
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,3)=v;A=full(a);V=full(v);A(:,3)=V;
assert_checkequal(full(a1), A);
//
a1=a;a1(:,7)=v;A=full(a);V=full(v);A(:,7)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,8)=v;A=full(a);V=full(v);A(:,8)=V;
assert_checkequal(full(a1), A);
b=sparse([1 2;2 1;3 1;3 2;4 1;6 2],10:15,[6,2]);
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
assert_checkequal(full(a1), A);
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
assert_checkequal(full(a1), A);
b=sparse([1 2;2 1;2 3;3 1;3 2;4 1;5 3;6 2],10:17,[6,3]);
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
assert_checkequal(full(a1), A);
v=sparse([],[],[6,1]);
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,3)=v;A=full(a);V=full(v);A(:,3)=V;
assert_checkequal(full(a1), A);
//
a1=a;a1(:,7)=v;A=full(a);V=full(v);A(:,7)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,8)=v;A=full(a);V=full(v);A(:,8)=V;
assert_checkequal(full(a1), A);
b=sparse([],[],[6,2]);
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
assert_checkequal(full(a1), A);
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
assert_checkequal(full(a1), A);
b=sparse([],[],[6,3]);
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
assert_checkequal(full(a1), A);
a=sparse([],[],[6,6]);
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,3)=v;A=full(a);V=full(v);A(:,3)=V;
assert_checkequal(full(a1), A);
//
a1=a;a1(:,7)=v;A=full(a);V=full(v);A(:,7)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,8)=v;A=full(a);V=full(v);A(:,8)=V;
assert_checkequal(full(a1), A);
b=sparse([1 2;2 1;3 1;3 2;4 1;6 2],10:15,[6,2]);
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
assert_checkequal(full(a1), A);
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
assert_checkequal(full(a1), A);
b=sparse([1 2;2 1;2 3;3 1;3 2;4 1;5 3;6 2],10:17,[6,3]);
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
assert_checkequal(full(a1), A);
v=sparse([],[],[6,1]);
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,3)=v;A=full(a);V=full(v);A(:,3)=V;
assert_checkequal(full(a1), A);
//
a1=a;a1(:,7)=v;A=full(a);V=full(v);A(:,7)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,8)=v;A=full(a);V=full(v);A(:,8)=V;
assert_checkequal(full(a1), A);
b=sparse([1 2;2 1;3 1;3 2;4 1;6 2],10:15,[6,2]);
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
assert_checkequal(full(a1), A);
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
assert_checkequal(full(a1), A);
b=sparse([1 2;2 1;2 3;3 1;3 2;4 1;5 3;6 2],10:17,[6,3]);
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
assert_checkequal(full(a1), A);
v=sparse([],[],[6,1]);
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,1)=v;A=full(a);V=full(v);A(:,1)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);V=full(v);A(:,2)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,3)=v;A=full(a);V=full(v);A(:,3)=V;
assert_checkequal(full(a1), A);
//
a1=a;a1(:,7)=v;A=full(a);V=full(v);A(:,7)=V;
assert_checkequal(full(a1), A);
a1=a;a1(:,8)=v;A=full(a);V=full(v);A(:,8)=V;
assert_checkequal(full(a1), A);
b=sparse([],[],[6,2]);
a1=a;a1(:,[1 3])=b;A=full(a);B=full(b);A(:,[1 3])=B;
assert_checkequal(full(a1), A);
a1=a;a1(:,[3 1])=b;A=full(a);B=full(b);A(:,[3 1])=B;
assert_checkequal(full(a1), A);
b=sparse([],[],[6,3]);
a1=a;a1(:,[1 3 1])=b;A=full(a);B=full(b);A(:,[1 3 1])=B;
assert_checkequal(full(a1), A);
// row column insertion
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=sparse([1 2;1 3;3 3],-(1:3),[3,3]);
a1=a;a1(1,1)=sparse(30);A=full(a);A(1,1)=30;
assert_checkequal(full(a1), A);
a1=a;a1(1,6)=sparse(30);A=full(a);A(1,6)=30;
assert_checkequal(full(a1), A);
a1=a;a1(1,8)=sparse(30);A=full(a);A(1,8)=30;
assert_checkequal(full(a1), A);
a1=a;a1(1:3,1:3)=b;A=full(a);A(1:3,1:3)=full(b);
assert_checkequal(full(a1), A);
a1=a;a1(1:3,6:8)=b;A=full(a);A(1:3,6:8)=full(b);
assert_checkequal(full(a1), A);
a1=a;a1(6:8,1:3)=b;A=full(a);A(6:8,1:3)=full(b);
assert_checkequal(full(a1), A);
a1=a;a1([3 2 1],1:3)=b;A=full(a);A([3 2 1],1:3)=full(b);
assert_checkequal(full(a1), A);
a1=a;a1([1 2 1],1:3)=b;A=full(a);A([1 2 1],1:3)=full(b);
assert_checkequal(full(a1), A);
a1=a;a1([3 2 1],[3 2 1])=b;A=full(a);A([3 2 1],[3 2 1])=full(b);
assert_checkequal(full(a1), A);
a1=a;a1([1 2 1],[3 2 1])=b;A=full(a);A([1 2 1],[3 2 1])=full(b);
assert_checkequal(full(a1), A);
a1=a;a1([1 2 1],[1 2 1])=b;A=full(a);A([1 2 1],[1 2 1])=full(b);
assert_checkequal(full(a1), A);
//sparse full
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
vt=11:16;
// full line insertion
//----------------------
a1=a;a1(1,:)=vt;A=full(a);A(1,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);A(3,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1(1,:)=vt;A=full(a);A(1,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);A(3,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1(7,:)=vt;A=full(a);A(7,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1(8,:)=vt;A=full(a);A(8,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1([1 3],:)=[vt;2*vt];A=full(a);A([1 3],:)=[vt;2*vt];
assert_checkequal(full(a1), A);
a1=a;a1([3 1],:)=[vt;2*vt];A=full(a);A([3 1],:)=[vt;2*vt];
assert_checkequal(full(a1), A);
a1=a;a1([1 3 1],:)=[vt;2*vt;3*vt];A=full(a);A([1 3 1],:)=[vt;2*vt;3*vt];
assert_checkequal(full(a1), A);
a=sparse([],[],[6,6]);
a1=a;a1(1,:)=vt;A=full(a);A(1,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);A(3,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1(1,:)=vt;A=full(a);A(1,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1(3,:)=vt;A=full(a);A(3,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1(7,:)=vt;A=full(a);A(7,:)=vt;
assert_checkequal(full(a1), A);
a1=a;a1(8,:)=vt;A=full(a);A(8,:)=vt;
assert_checkequal(full(a1), A);
b=[1:6;11:16];
a1=a;a1([1 3],:)=b;A=full(a);A([1 3],:)=b;
assert_checkequal(full(a1), A);
a1=a;a1([3 1],:)=b;A=full(a);A([3 1],:)=b;
assert_checkequal(full(a1), A);
b=[1:6;11:16;21:26];
a1=a;a1([1 3 1],:)=b;A=full(a);A([1 3 1],:)=b;
assert_checkequal(full(a1), A);
// full column insertion
//----------------------
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
v=(1:6)';
a1=a;a1(:,1)=v;A=full(a);A(:,1)=v;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);A(:,2)=v;
assert_checkequal(full(a1), A);
a1=a;a1(:,1)=v;A=full(a);A(:,1)=v;
assert_checkequal(full(a1), A);
a1=a;a1(:,2)=v;A=full(a);A(:,2)=v;
assert_checkequal(full(a1), A);
a1=a;a1(:,3)=v;A=full(a);A(:,3)=v;
assert_checkequal(full(a1), A);
//
a1=a;a1(:,7)=v;A=full(a);A(:,7)=v;
assert_checkequal(full(a1), A);
a1=a;a1(:,8)=v;A=full(a);A(:,8)=v;
assert_checkequal(full(a1), A);
b=[(1:6)' (11:16)'];
a1=a;a1(:,[1 3])=b;A=full(a);A(:,[1 3])=b;
assert_checkequal(full(a1), A);
a1=a;a1(:,[3 1])=b;A=full(a);A(:,[3 1])=b;
assert_checkequal(full(a1), A);
b=[(1:6)' (11:16)' (21:26)'];
a1=a;a1(:,[1 3 1])=b;A=full(a);A(:,[1 3 1])=b;
assert_checkequal(full(a1), A);
//********
// row column insertion
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=-[1 2 3;4 5 6;7 8 9];
a1=a;a1(1,1)=30;A=full(a);A(1,1)=30;
assert_checkequal(full(a1), A);
a1=a;a1(1,6)=30;A=full(a);A(1,6)=30;
assert_checkequal(full(a1), A);
a1=a;a1(1,8)=30;A=full(a);A(1,8)=30;
assert_checkequal(full(a1), A);
a1=a;a1(1:3,1:3)=b;A=full(a);A(1:3,1:3)=b;
assert_checkequal(full(a1), A);
a1=a;a1(1:3,6:8)=b;A=full(a);A(1:3,6:8)=b;
assert_checkequal(full(a1), A);
a1=a;a1(6:8,1:3)=b;A=full(a);A(6:8,1:3)=b;
assert_checkequal(full(a1), A);
a1=a;a1([3 2 1],1:3)=b;A=full(a);A([3 2 1],1:3)=b;
assert_checkequal(full(a1), A);
a1=a;a1([1 2 1],1:3)=b;A=full(a);A([1 2 1],1:3)=b;
assert_checkequal(full(a1), A);
a1=a;a1([3 2 1],[3 2 1])=b;A=full(a);A([3 2 1],[3 2 1])=b;
assert_checkequal(full(a1), A);
a1=a;a1([1 2 1],[3 2 1])=b;A=full(a);A([1 2 1],[3 2 1])=b;
assert_checkequal(full(a1), A);
a1=a;a1([1 2 1],[1 2 1])=b;A=full(a);A([1 2 1],[1 2 1])=b;
assert_checkequal(full(a1), A);
// vector insertion
v=sparse([1 1;3 1;6 1],[10 11 12],[6 1]);
v1=v;v1(1)=33;V=full(v);V(1)=33;
assert_checkequal(full(v1), V);
v1=v;v1(2)=33;V=full(v);V(2)=33;
assert_checkequal(full(v1), V);
v1=v;v1(8)=33;V=full(v);V(8)=33;
assert_checkequal(full(v1), V);
v1=v;v1([1 2 8])=[5;10;33];V=full(v);V([1 2 8])=[5;10;33];
assert_checkequal(full(v1), V);
v1=v;v1(:)=[];
assert_checkequal(full(v1), []);
v1=v;v1(1)=sparse(33);V=full(v);V(1)=33;
assert_checkequal(full(v1), V);
v1=v;v1(2)=sparse(33);V=full(v);V(2)=33;
assert_checkequal(full(v1), V);
v1=v;v1(8)=sparse(33);V=full(v);V(8)=33;
assert_checkequal(full(v1), V);
v1=v;v1([1 2 8])=sparse([5;10;33]);V=full(v);V([1 2 8])=[5;10;33];
assert_checkequal(full(v1), V);
v1=v;v1([1 2 1])=sparse([5;10;33]);V=full(v);V([1 2 1])=[5;10;33];
assert_checkequal(full(v1), V);
v1=v;v1(:)=[];
assert_checkequal(full(v1), []);
v1=v;v1(:)=sparse([2 1],44,[6 1]);V=full(v);V(:)=[0;44;0;0;0;0];
assert_checkequal(full(v1), V);
v=v';
v1=v;v1(1)=33;V=full(v);V(1)=33;
assert_checkequal(full(v1), V);
v1=v;v1(2)=33;V=full(v);V(2)=33;
assert_checkequal(full(v1), V);
v1=v;v1(8)=33;V=full(v);V(8)=33;
assert_checkequal(full(v1), V);
v1=v;v1([1 2 8])=[5 10 33];V=full(v);V([1 2 8])=[5 10 33];
assert_checkequal(full(v1), V);
v1=v;v1(1)=sparse(33);V=full(v);V(1)=33;
assert_checkequal(full(v1), V);
v1=v;v1(2)=sparse(33);V=full(v);V(2)=33;
assert_checkequal(full(v1), V);
v1=v;v1(8)=sparse(33);V=full(v);V(8)=33;
assert_checkequal(full(v1), V);
v1=v;v1([1 2 8])=sparse([5 10 33]);V=full(v);V([1 2 8])=[5 10 33];
assert_checkequal(full(v1), V);
v1=v;v1([1 2 1])=sparse([5 10 33]);V=full(v);V([1 2 1])=[5 10 33];
assert_checkequal(full(v1), V);
v1=v;v1(:)=sparse([1 2],44,[1,6]);V=full(v);V(:)=[0 44 0 0 0 0];
assert_checkequal(full(v1), V);
v1=v;v1(1)=[];V=full(v);V(1)=[];
assert_checkequal(full(v1), V);
//test des comparaisons
a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=sparse([1 6;1 2;6 5],[10;-1;-1],[6 6]);
assert_checkequal(full(a==a), full(a)==full(a));
assert_checkequal(full(a<>a), (full(a)<>full(a)));
assert_checkequal(full(a>sparse(5)), (full(a)>5));
assert_checkequal(full(a<sparse(5)), (full(a)<5));
assert_checkequal(full(a>=sparse(5)), (full(a)>=5));
assert_checkequal(full(a<=sparse(5)), (full(a)<=5));
assert_checkequal(full(sparse(5)>a), (5>full(a)));
assert_checkequal(full(sparse(5)<a), (5<full(a)));
assert_checkequal(full(sparse(5)>=a), (5>=full(a)));
assert_checkequal(full(sparse(5)<=a), (5<=full(a)));
assert_checkequal(full(b>a), (full(b)>full(a)));
assert_checkequal(full(b<a), (full(b)<full(a)));
assert_checkequal(full(b>=a), (full(b)>=full(a)));
assert_checkequal(full(b<=a), (full(b)<=full(a)));
assert_checkequal(full(a==full(a)), full(a)==full(a));
assert_checkequal(full(a<>full(a)), (full(a)<>full(a)));
assert_checkequal(full(a>5), (full(a)>5));
assert_checkequal(full(a<5), (full(a)<5));
assert_checkequal(full(a>=5), (full(a)>=5));
assert_checkequal(full(a<=5), (full(a)<=5));
assert_checkequal(full(5>a), (5>full(a)));
assert_checkequal(full(5<a), (5<full(a)));
assert_checkequal(full(5>=a), (5>=full(a)));
assert_checkequal(full(5<=a), (5<=full(a)));
assert_checkequal(full(b>full(a)), (full(b)>full(a)));
assert_checkequal(full(b<full(a)), (full(b)<full(a)));
assert_checkequal(full(b>=full(a)), (full(b)>=full(a)));
assert_checkequal(full(b<=full(a)), (full(b)<=full(a)));
assert_checkequal(full(full(a)==a), full(a)==full(a));
assert_checkequal(full(full(a)<>a), (full(a)<>full(a)));
assert_checkequal(full(full(a)>sparse(5)), (full(a)>5));
assert_checkequal(full(full(a)<sparse(5)), (full(a)<5));
assert_checkequal(full(full(a)>=sparse(5)), (full(a)>=5));
assert_checkequal(full(full(a)<=sparse(5)), (full(a)<=5));
assert_checkequal(full(full(b)>a), (full(b)>full(a)));
assert_checkequal(full(full(b)<a), (full(b)<full(a)));
assert_checkequal(full(full(b)>=a), (full(b)>=full(a)));
assert_checkequal(full(full(b)<=a), (full(b)<=full(a)));

a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],-(1:9),[6 6]);
b=sparse([1 6;1 2;6 5],[10;-1;-1],[6 6]);
assert_checkequal(full(a==a), full(a)==full(a));
assert_checkequal(full(a<>a), (full(a)<>full(a)));
assert_checkequal(full(a>sparse(5)), (full(a)>5));
assert_checkequal(full(a<sparse(5)), (full(a)<5));
assert_checkequal(full(a>=sparse(5)), (full(a)>=5));
assert_checkequal(full(a<=sparse(5)), (full(a)<=5));
assert_checkequal(full(sparse(5)>a), (5>full(a)));
assert_checkequal(full(sparse(5)<a), (5<full(a)));
assert_checkequal(full(sparse(5)>=a), (5>=full(a)));
assert_checkequal(full(sparse(5)<=a), (5<=full(a)));
assert_checkequal(full(b>a), (full(b)>full(a)));
assert_checkequal(full(b<a), (full(b)<full(a)));
assert_checkequal(full(b>=a), (full(b)>=full(a)));
assert_checkequal(full(b<=a), (full(b)<=full(a)));
assert_checkequal(full(a==full(a)), full(a)==full(a));
assert_checkequal(full(a<>full(a)), (full(a)<>full(a)));
assert_checkequal(full(a>5), (full(a)>5));
assert_checkequal(full(a<5), (full(a)<5));
assert_checkequal(full(a>=5), (full(a)>=5));
assert_checkequal(full(a<=5), (full(a)<=5));
assert_checkequal(full(5>a), (5>full(a)));
assert_checkequal(full(5<a), (5<full(a)));
assert_checkequal(full(5>=a), (5>=full(a)));
assert_checkequal(full(5<=a), (5<=full(a)));
assert_checkequal(full(b>full(a)), (full(b)>full(a)));
assert_checkequal(full(b<full(a)), (full(b)<full(a)));
assert_checkequal(full(b>=full(a)), (full(b)>=full(a)));
assert_checkequal(full(b<=full(a)), (full(b)<=full(a)));
assert_checkequal(full(full(a)==a), full(a)==full(a));
assert_checkequal(full(full(a)<>a), (full(a)<>full(a)));
assert_checkequal(full(full(a)>sparse(5)), (full(a)>5));
assert_checkequal(full(full(a)<sparse(5)), (full(a)<5));
assert_checkequal(full(full(a)>=sparse(5)), (full(a)>=5));
assert_checkequal(full(full(a)<=sparse(5)), (full(a)<=5));
assert_checkequal(full(full(b)>a), (full(b)>full(a)));
assert_checkequal(full(full(b)<a), (full(b)<full(a)));
assert_checkequal(full(full(b)>=a), (full(b)>=full(a)));
assert_checkequal(full(full(b)<=a), (full(b)<=full(a)));

a=sparse([1 1;3 1;6 1],[10 11 12],[6 1]);
assert_checkequal(full(a==a), full(a)==full(a));
assert_checkequal(full(a<>a), (full(a)<>full(a)));
assert_checkequal(full(a>sparse(5)), (full(a)>5));
assert_checkequal(full(a<sparse(5)), (full(a)<5));
assert_checkequal(full(a>=sparse(5)), (full(a)>=5));
assert_checkequal(full(a<=sparse(5)), (full(a)<=5));
assert_checkequal(full(sparse(5)>a), (5>full(a)));
assert_checkequal(full(sparse(5)<a), (5<full(a)));
assert_checkequal(full(sparse(5)>=a), (5>=full(a)));
assert_checkequal(full(sparse(5)<=a), (5<=full(a)));
assert_checkequal(full(a==full(a)), full(a)==full(a));
assert_checkequal(full(a<>full(a)), (full(a)<>full(a)));
assert_checkequal(full(a>5), (full(a)>5));
assert_checkequal(full(a<5), (full(a)<5));
assert_checkequal(full(a>=5), (full(a)>=5));
assert_checkequal(full(a<=5), (full(a)<=5));
assert_checkequal(full(5>a), (5>full(a)));
assert_checkequal(full(5<a), (5<full(a)));
assert_checkequal(full(5>=a), (5>=full(a)));
assert_checkequal(full(5<=a), (5<=full(a)));
assert_checkequal(full(full(a)==a), full(a)==full(a));
assert_checkequal(full(full(a)<>a), (full(a)<>full(a)));
assert_checkequal(full(full(a)>sparse(5)), (full(a)>5));
assert_checkequal(full(full(a)<sparse(5)), (full(a)<5));
assert_checkequal(full(full(a)>=sparse(5)), (full(a)>=5));
assert_checkequal(full(full(a)<=sparse(5)), (full(a)<=5));
a=a';
assert_checkequal(full(a==a), full(a)==full(a));
assert_checkequal(full(a<>a), (full(a)<>full(a)));
assert_checkequal(full(a>sparse(5)), (full(a)>5));
assert_checkequal(full(a<sparse(5)), (full(a)<5));
assert_checkequal(full(a>=sparse(5)), (full(a)>=5));
assert_checkequal(full(a<=sparse(5)), (full(a)<=5));
assert_checkequal(full(sparse(5)>a), (5>full(a)));
assert_checkequal(full(sparse(5)<a), (5<full(a)));
assert_checkequal(full(sparse(5)>=a), (5>=full(a)));
assert_checkequal(full(sparse(5)<=a), (5<=full(a)));
assert_checkequal(full(a==full(a)), full(a)==full(a));
assert_checkequal(full(a<>full(a)), (full(a)<>full(a)));
assert_checkequal(full(a>5), (full(a)>5));
assert_checkequal(full(a<5), (full(a)<5));
assert_checkequal(full(a>=5), (full(a)>=5));
assert_checkequal(full(a<=5), (full(a)<=5));
assert_checkequal(full(5>a), (5>full(a)));
assert_checkequal(full(5<a), (5<full(a)));
assert_checkequal(full(5>=a), (5>=full(a)));
assert_checkequal(full(5<=a), (5<=full(a)));
assert_checkequal(full(full(a)==a), full(a)==full(a));
assert_checkequal(full(full(a)<>a), (full(a)<>full(a)));
assert_checkequal(full(full(a)>sparse(5)), (full(a)>5));
assert_checkequal(full(full(a)<sparse(5)), (full(a)<5));
assert_checkequal(full(full(a)>=sparse(5)), (full(a)>=5));
assert_checkequal(full(full(a)<=sparse(5)), (full(a)<=5));

a=sparse([1 1;3 1;6 1],[-10 -11 -12],[6 1]);
assert_checkequal(full(a==a), full(a)==full(a));
assert_checkequal(full(a<>a), (full(a)<>full(a)));
assert_checkequal(full(a>sparse(5)), (full(a)>5));
assert_checkequal(full(a<sparse(5)), (full(a)<5));
assert_checkequal(full(a>=sparse(5)), (full(a)>=5));
assert_checkequal(full(a<=sparse(5)), (full(a)<=5));
assert_checkequal(full(sparse(5)>a), (5>full(a)));
assert_checkequal(full(sparse(5)<a), (5<full(a)));
assert_checkequal(full(sparse(5)>=a), (5>=full(a)));
assert_checkequal(full(sparse(5)<=a), (5<=full(a)));
assert_checkequal(full(a==full(a)), full(a)==full(a));
assert_checkequal(full(a<>full(a)), (full(a)<>full(a)));
assert_checkequal(full(a>5), (full(a)>5));
assert_checkequal(full(a<5), (full(a)<5));
assert_checkequal(full(a>=5), (full(a)>=5));
assert_checkequal(full(a<=5), (full(a)<=5));
assert_checkequal(full(5>a), (5>full(a)));
assert_checkequal(full(5<a), (5<full(a)));
assert_checkequal(full(5>=a), (5>=full(a)));
assert_checkequal(full(5<=a), (5<=full(a)));
assert_checkequal(full(full(a)==a), full(a)==full(a));
assert_checkequal(full(full(a)<>a), (full(a)<>full(a)));
assert_checkequal(full(full(a)>sparse(5)), (full(a)>5));
assert_checkequal(full(full(a)<sparse(5)), (full(a)<5));
assert_checkequal(full(full(a)>=sparse(5)), (full(a)>=5));
assert_checkequal(full(full(a)<=sparse(5)), (full(a)<=5));
a=a';
assert_checkequal(full(a==a), full(a)==full(a));
assert_checkequal(full(a<>a), (full(a)<>full(a)));
assert_checkequal(full(a>sparse(5)), (full(a)>5));
assert_checkequal(full(a<sparse(5)), (full(a)<5));
assert_checkequal(full(a>=sparse(5)), (full(a)>=5));
assert_checkequal(full(a<=sparse(5)), (full(a)<=5));
assert_checkequal(full(sparse(5)>a), (5>full(a)));
assert_checkequal(full(sparse(5)<a), (5<full(a)));
assert_checkequal(full(sparse(5)>=a), (5>=full(a)));
assert_checkequal(full(sparse(5)<=a), (5<=full(a)));
assert_checkequal(full(a==full(a)), full(a)==full(a));
assert_checkequal(full(a<>full(a)), (full(a)<>full(a)));
assert_checkequal(full(a>5), (full(a)>5));
assert_checkequal(full(a<5), (full(a)<5));
assert_checkequal(full(a>=5), (full(a)>=5));
assert_checkequal(full(a<=5), (full(a)<=5));
assert_checkequal(full(5>a), (5>full(a)));
assert_checkequal(full(5<a), (5<full(a)));
assert_checkequal(full(5>=a), (5>=full(a)));
assert_checkequal(full(5<=a), (5<=full(a)));
assert_checkequal(full(full(a)==a), full(a)==full(a));
assert_checkequal(full(full(a)<>a), (full(a)<>full(a)));
assert_checkequal(full(full(a)>sparse(5)), (full(a)>5));
assert_checkequal(full(full(a)<sparse(5)), (full(a)<5));
assert_checkequal(full(full(a)>=sparse(5)), (full(a)>=5));
assert_checkequal(full(full(a)<=sparse(5)), (full(a)<=5));

a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
b=sparse([1 6;1 2;5 5],[10;-1;-1],[5 6]);
assert_checkfalse(a==b);
assert_checktrue(a<>b);

// Sparse does not accept hypermatrices as input arguments
A = hypermat([2 2 3], 1:12);
msg = _("%s: Wrong size for input argument #%d: A m-by-n matrix expected.\n");
funcname = "sparse";
assert_checkerror("sparse(A)", msprintf(msg, funcname, 1));

assert_checkerror("sparse(1, A, 1)", msprintf(msg, funcname, 2));
assert_checkerror("sparse(1, 1, A)", msprintf(msg, funcname, 3));
