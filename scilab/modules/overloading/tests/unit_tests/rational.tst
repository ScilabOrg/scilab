// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->


s=poly(0,'s');
z = s^0;
h=1/(2*s);
assert_checkequal(size(h), [1 1]);
assert_checkequal(h.num, 1*z);
assert_checkequal(h.den, 2*s);

//basic operations with 2D matrices
H=[h h];
assert_checkequal(H.num, [1 1].*z);
assert_checkequal(H.den, [2*s 2*s]);

H=[h;h];
assert_checkequal(H.num, [1;1].*z);
assert_checkequal(H.den, [2*s;2*s]);

H=[h 3];
assert_checkequal(H.num, [1*z 3]);
assert_checkequal(H.den, [2*s 1]);

H=[h;5];
assert_checkequal(H.num, [1*z;5]);
assert_checkequal(H.den, [2*s;1]);

H=[3 h];
assert_checkequal(H.num, [3 1*z]);
assert_checkequal(H.den, [1 2*s]);

H=[5;h];
assert_checkequal(H.num, [5;1*z]);
assert_checkequal(H.den, [1;2*s]);

H=[3 h;s 2];
assert_checkequal(H(1,1), rlist(3,1));
assert_checkequal(H(1,2), h);
assert_checkequal(H(1, [2 1]), [h rlist(3,1)]);
assert_checkequal(H(1, [2 2]), [h h]);
assert_checkequal(H(1,:), [rlist(3,1) h]);
assert_checkequal(H(:,1), [rlist(3,1); s]);
assert_checkequal(H([2 1], :), [s rlist(2,1);rlist(3,1) h]);
assert_checkequal(H([1 1], :), [rlist(3,1) h;rlist(3,1) h]);

assert_checkequal(matrix(H, -1, 1), [3*z;s;h;2*z]);

H=h;H(1,3)=1/s;
assert_checkequal(H.num, [1 0 1].*z);
assert_checkequal(H.den, [2*s 1 s]);

H=h;H(3,1)=1/s;
assert_checkequal(H.num, [1 0 1]'.*z);
assert_checkequal(H.den, [2*s 1 s]');

H=h;H(1,3)=1.5;
assert_checkequal(H.num, [1 0 1.5].*z);
assert_checkequal(H.den, [2*s 1 1]);

H=h;H(3,1)=1.5;
assert_checkequal(H.num, [1 0 1.5]'.*z);
assert_checkequal(H.den, [2*s 1 1]');

H=1.5;H(1,3)=1/s;
assert_checkequal(H.num, [1.5 0 1].*z);
assert_checkequal(H.den, [1 1 s]);

H=1.5;H(3,1)=1/s;
assert_checkequal(H.num, [1.5 0 1]'.*z);
assert_checkequal(H.den, [1 1 s]');

H=[h s;1 h];H(:,1)=[];
assert_checkequal(H, [s;h]);

H=[h s;1 h];H(2,:)=[];
assert_checkequal(H, [h s]);

H=h+h;c=coeff(H.den,1);
assert_checkequal(H.num/c, 1*z);
assert_checkequal(H.den/c, s);

H=h+1;
assert_checkequal(H.num, 1+2*s);
assert_checkequal(H.den, 2*s);

H=1+h;
assert_checkequal(H.num, 1+2*s);
assert_checkequal(H.den, 2*s);

H=h+[];
assert_checkequal(H, h);

H=[]+h;
assert_checkequal(H, h);

H=h+s;
assert_checkequal(H.num, 1+2*s^2);
assert_checkequal(H.den, 2*s);

H=s+h;
assert_checkequal(H.num, 1+2*s^2);
assert_checkequal(H.den, 2*s);

H=h-h;
assert_checkequal(H.num, 0*z);
assert_checkequal(H.den, 1*s^0);

H=h-1;
assert_checkequal(H.num, 1-2*s);
assert_checkequal(H.den, 2*s);

H=1-h;
assert_checkequal(H.num, -1+2*s);
assert_checkequal(H.den, 2*s);

H=[h h+1]-1;
assert_checkequal(H(1,1), h-1);
assert_checkequal(H(1,2), h);

H=[h h+1]-2*h;
assert_checkequal(H.num, [-2 -2+4*s]);
assert_checkequal(H.den, [4*s 4*s]);

H=-2*h+[h h+1];
assert_checkequal(H.num, [-2 -2+4*s]);
assert_checkequal(H.den, [4*s 4*s]);

// *

H=h*h;
assert_checkequal(H.num, 1*z);
assert_checkequal(H.den, 4*s^2);

H=h*2;
assert_checkequal(H.num, 2*z);
assert_checkequal(H.den, 2*s);

H=2*h;
assert_checkequal(H.num, 2*z);
assert_checkequal(H.den, 2*s);

H=h*s;
assert_checkequal(H.num, 1*z);
assert_checkequal(H.den, 2*z);

H=s*h;
assert_checkequal(H.num, 1*z);
assert_checkequal(H.den, 2*z);

H=[h h+1]*h;
assert_checkequal(H, [h*h (h+1)*h]);

H=h*[h h+1];
assert_checkequal(H, [h*h, (h+1)*h]);

H=[h h+1]*2;
assert_checkequal(H, [h*2, (h+1)*2]);

H=2*[h h+1];
assert_checkequal(H, [h*2, (h+1)*2]);

H=[h h+1]*s;
assert_checkequal(H, [h*s, (h+1)*s]);

H=s*[h h+1];
assert_checkequal(H, [h*s, (h+1)*s]);

H=[h 1;s 3]*[1;h];
assert_checkequal(H, [h+h;s+3*h]);

H=[h 1]*[h 1;s 3];
assert_checkequal(H, [h*h+s, h+3]);

H=[h 1;s 3]*[1;2];
assert_checkequal(H, [h+2;s+6]);

H=[2 1]*[h 1;s 3];
assert_checkequal(H, [2*h+s, 2+3]);

// .*

H=h.*h;
assert_checkequal(H.num, 1*z);
assert_checkequal(H.den, 4*s^2);

H=h.*2;
assert_checkequal(H.num, 2*z);
assert_checkequal(H.den, 2*s);

H=2 .*h;
assert_checkequal(H.num, 2*z);
assert_checkequal(H.den, 2*s);

H=h.*s;
assert_checkequal(H.num, 1*z);
assert_checkequal(H.den, 2*z);

H=s.*h;
assert_checkequal(H.num, 1*z);
assert_checkequal(H.den, 2*z);

H=[h h+1].*h;
assert_checkequal(H, [h*h (h+1)*h]);

H=h.*[h h+1];
assert_checkequal(H, [h*h (h+1)*h]);

H=[h h+1].*2;
assert_checkequal(H, [h*2, (h+1)*2]);

H=2 .*[h h+1];
assert_checkequal(H, [h*2, (h+1)*2]);

H=[h h+1].*s;
assert_checkequal(H, [h*s, (h+1)*s]);

H=s.*[h h+1];
assert_checkequal(H, [h*s, (h+1)*s]);

H=[3 h;s 2].*[3 h;s 2];
assert_checkequal(H, [9 h*h; s*s 4]);


// /

H=h/2;
assert_checkequal(H.num, 0.5*z);
assert_checkequal(H.den, 2*s);

H=h/s;
assert_checkequal(H.num, 1*z);
assert_checkequal(H.den, 2*s^2);

H=h/(h+1);
assert_checkequal(H.num, 2*z);
assert_checkequal(H.den, 2+4*s);

H=[h h-1]/2;
assert_checkequal(H, [h/2 (h-1)/2]);

H=[h h-1]/s;
assert_checkequal(H, [h/s (h-1)/s]);

H=1/h;
assert_checkequal(H.num, 2*s);
assert_checkequal(H.den, 1*z);

H=[1 2]/h;
assert_checkequal(H, [1/h 2/h]);

H=[s+1 s-2]/h;
assert_checkequal(H, [(s+1)/h (s-2)/h]);

H=[h+1 (h+1)*(h-1)]/h;
assert_checkequal(H, [(h+1)/h ((h+1)*(h-1))/h]);

H=(eye(2,2)/[3 h;s 2])*[3 h;s 2];
assert_checkequal(coeff(H.num)./(coeff(H.den, 0) + coeff(H.den, 2)), eye(2,2));

// ./
H=h./2;
assert_checkequal(H.num, 0.5*z);
assert_checkequal(H.den, 2*s);

H=h./s;
assert_checkequal(H.num, 1*z);
assert_checkequal(H.den, 2*s^2);

H=h./(h+1);
assert_checkequal(H.num, 2*z);
assert_checkequal(H.den, 2+4*s);

H=[h h-1]./2;
assert_checkequal(H, [h/2 (h-1)/2]);

H=[h h-1]./s;
assert_checkequal(H, [h/s (h-1)/s]);

H=1 ./h;
assert_checkequal(H.num, 2*s);
assert_checkequal(H.den, 1*z);

H=[1 2]./h;
assert_checkequal(H, [1/h 2/h]);

H=[s+1 s-2]./h;
assert_checkequal(H, [(s+1)/h (s-2)/h]);

H=[h+1 (h+1)*(h-1)]./h;
assert_checkequal(H, [(h+1)/h ((h+1)*(h-1))/h]);

H=[3 h;s 2]./[3 h;s 2];
assert_checkequal(coeff(H.num)./coeff(H.den), ones(2,2));

// .\

H=2 .\h;
assert_checkequal(H.num, 0.5*z);
assert_checkequal(H.den, 2*s);

H=s.\h;
assert_checkequal(H.num, 1*z);
assert_checkequal(H.den, 2*s^2);

H=(h+1).\h;
assert_checkequal(H.num, 2*z);
assert_checkequal(H.den, 2+4*s);

H=2 .\[h h-1];
assert_checkequal(H, [h/2 (h-1)/2]);

H=s.\[h h-1];
assert_checkequal(H, [h/s (h-1)/s]);

H=1 ./h;
assert_checkequal(H.num, 2*s);
assert_checkequal(H.den, 1*z);

H=h.\[1 2];
assert_checkequal(H, [1/h 2/h]);

H=h.\[s+1 s-2];
assert_checkequal(H, [(s+1)/h (s-2)/h]);

H=h.\[h+1 (h+1)*(h-1)];
assert_checkequal(H, [(h+1)/h ((h+1)*(h-1))/h]);

H=[3 h;s 2]./[3 h;s 2];
assert_checkequal(coeff(H.num)./coeff(H.den), ones(2,2));

// hypermatrices of rationnals

clear H;H(1,1,2)=h;
assert_checkequal(H(1,1,1), rlist(0, 1, []));
assert_checkequal(H(1,1,2), h);

H(2,1,2)=h+1;
assert_checkequal(size(H), [2 1 2]);
assert_checkequal(H(:,1,1), rlist([0; 0], [1; 1]));
assert_checkequal(H([2 2], 1, 2), [h+1;h+1]);

clear H;H(1,1,2)=h;H(2,1,1)=1;
assert_checkequal(H(:,1,1), rlist([0; 1], [1; 1]));
assert_checkequal(H(:,1,2), [h;0]);

clear H;H(1,1,2)=h;H(1,1,:)=3;
assert_checkequal(H(1,1,1), rlist(3,1));
assert_checkequal(H(1,1,2), rlist(3,1));

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;
assert_checkequal(H(1:2,1,1), rlist([%s;%s], [1;1]));

clear H;H(2,2,2)=s;H(2,1,1)=h;
assert_checkequal(H(:,:,1), [0 0;h 0]);
assert_checkequal(H(:,:,2), rlist([0 0;0 s], [1 1;1 1]));

clear H;H(2,2,2)=8;H(2,1,1)=h;
assert_checkequal(H(:,:,1), [0 0;h 0]);
assert_checkequal(H(:,:,2), rlist([0 0;0 8], [1 1;1 1]));

clear H;H(2,2,2)=8;H(2,1,1)=h;
H(:,:,1)=[];
assert_checkequal(H, rlist([0 0;0 8], [1 1;1 1]));

clear H;H(2,2,2)=8;H(2,1,1)=h;
H2=H(2,:,:);H(1,:,:)=[];
assert_checkequal(H, H2);

clear H;H(2,2,2)=8;H(2,1,1)=h;
H2=H(:,2,:);H(:,1,:)=[];
assert_checkequal(H, H2);

clear H;H(2,2,2)=h;H=H+1;
assert_checkequal(H(:,:,1), rlist([1 1;1 1], [1 1;1 1]));
assert_checkequal(H(:,:,2), [1 1;1 h+1]);

clear H;H(2,2,2)=h;H=1+H;
assert_checkequal(H(:,:,1), rlist([1 1;1 1], [1 1;1 1]));
assert_checkequal(H(:,:,2), [1 1;1 h+1]);

clear H;H(2,2,2)=h;H=H+s;
assert_checkequal(H(:,:,1), rlist([s s;s s], [1 1;1 1]));
assert_checkequal(H(:,:,2), [s s;s h+s]);

clear H;H(2,2,2)=h;H=s+H;
assert_checkequal(H(:,:,1), rlist([s s; s s], [1 1;1 1]));
assert_checkequal(H(:,:,2), [s s;s h+s]);

clear H;H(2,2,2)=h;H=H-1;
assert_checkequal(H(:,:,1), -1*rlist([1 1;1 1], [1 1;1 1]));
assert_checkequal(H(:,:,2), [-1 -1;-1 h-1]);

clear H;H(2,2,2)=h;H=1-H;
assert_checkequal(H(:,:,1), rlist([1 1;1 1], [1 1;1 1]));
assert_checkequal(H(:,:,2), [1 1;1 1-h]);

clear H;H(2,2,2)=h;H=H-s;
assert_checkequal(H(:,:,1), rlist([-s -s;-s -s], [1 1;1 1]));
assert_checkequal(H(:,:,2), [-s -s;-s h-s]);

clear H;H(2,2,2)=h;H=s-H;
assert_checkequal(H(:,:,1), rlist([s s;s s], [1 1;1 1]));
assert_checkequal(H(:,:,2), [s s;s -h+s]);

clear H;H(2,2,2)=s;H(2,1,1)=h;H=H+H;
assert_checkequal(H(:,:,1), [0 0;h+h 0]);
assert_checkequal(H(:,:,2), rlist([0 0;0 s+s], [1 1;1 1]));

clear H;H(2,2,2)=s;H(2,1,1)=h;H=H-H;
assert_checkequal(H(:,:,1), [0 0;h-h 0]);
assert_checkequal(H(:,:,2), rlist([0 0;0 0], [1 1;1 1]));

// *
clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=H*h;
assert_checkequal(H(:,:,1), [s*h;s*h]);
assert_checkequal(H(:,:,2), [h*h; 0/1]);

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=H*2;
assert_checkequal(H(:,:,1), rlist([s*2;s*2], [1;1]));
assert_checkequal(H(:,:,2), [h*2;0]);

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=2*H;
assert_checkequal(H(:,:,1), rlist([s*2;s*2], [1;1]));
assert_checkequal(H(:,:,2), [h*2;0]);

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=H*s;
assert_checkequal(H(:,:,1), rlist([s*s;s*s], [1;1]));
assert_checkequal(H(:,:,2), [h*s;0]);

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=s*H;
assert_checkequal(H(:,:,1), rlist([s*s;s*s], [1;1]));
assert_checkequal(H(:,:,2), [h*s;0]);

// .*
clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=H.*h;
assert_checkequal(H(:,:,1), [s*h;s*h]);
assert_checkequal(H(:,:,2), [h*h;0/1]);

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=h.*H;
assert_checkequal(H(:,:,1), [s*h;s*h]);
assert_checkequal(H(:,:,2), [h*h;0/1]);

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=H.*2;
assert_checkequal(H(:,:,1), rlist([s*2;s*2], [1;1]));
assert_checkequal(H(:,:,2), [h*2;0]);

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=2 .*H;
assert_checkequal(H(:,:,1), rlist([s*2;s*2], [1;1]));
assert_checkequal(H(:,:,2), [h*2;0]);

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=H.*s;
assert_checkequal(H(:,:,1), rlist([s*s;s*s], [1;1]));
assert_checkequal(H(:,:,2), [h*s;0]);

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=s.*H;
assert_checkequal(H(:,:,1), rlist([s*s;s*s], [1;1]));
assert_checkequal(H(:,:,2), [h*s;0]);

clear H;H(1,1,2)=h;H(1:2,1,1)=%s;H=H.*H;
assert_checkequal(H(:,:,1), rlist([s*s;s*s], [1;1]));
assert_checkequal(H(:,:,2), [h*h;0]);


