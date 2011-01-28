// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bernard HUGUENEY
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// recupération de plusieurs arguments ok

// <-- JVM NOT MANDATORY -->



args=[1,2];

function a= g(arg1); a=2*arg1; endfunction;

res=parallel_run(args, g);
if res<>[2, 4] then pause,end

arg1=[1,2;4,5]; arg2=[2,3];

function [a,b]= f(arg1, arg2); a=arg1'.*[.5,.25]; b=2*arg2; endfunction;
[res1, res2] =parallel_run(arg1, arg2,"f","constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,6]  then pause,end

arg1=[1,2;4,5]; arg2=[2];
[res1, res2] =parallel_run(arg1, arg2,f,"constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,4]  then pause,end

function [a,b]= f(arg1, arg2); a=arg1'.*[.5,.25]; b=2*arg2; endfunction;
[res1, res2] =parallel_run(arg1, arg2,"f","constant", [2;1]);
if res1<>[.5,1;1,1.25]  then pause,end
if res2 <>[4,4]  then pause,end

// median , sort ,...not yet in YaSp

args=[1,2,3,4,5];
function a= g(arg1); a=2*arg1; endfunction;
conf= tlist(["params","nb_workers"],2);
parallel_run(args, "g", conf);
parallel_run(args, g, conf);

conf=tlist(["params","nb_workers","shared_memory"],2,1);
parallel_run(args, g, conf);

function init_rand(n)
  rand('seed',n);
endfunction;

function res= rand_macro(nb)
  res= rand(1, nb);
endfunction
nb= 5;
conf= tlist(["params","nb_workers"],2);

// We use conf to force nb_workers = 2 even on monocore
res= parallel_run([nb,nb],'rand_macro',nb,conf);
// without seeding the 2 workers have the same rng.
if max(abs(res(:,1)-res(:,2))) > 10^-10 then pause,end

conf=tlist(["params","nb_workers","shared_memory", "prologue"],2,0, init_rand);
res= parallel_run([nb,nb],'rand_macro',nb,conf);
// when setting the seed, they should have different random numbers
if max(abs(res(:,1)-res(:,2))) < 10^-10 then pause,end

many_args= 1:10e3;
many_workers= 80;
for sched=0:1
  res= parallel_run(many_args, "g", tlist(['param','dynamic_scheduling','nb_workers'], sched, many_workers ));
  if res<>g(many_args) then pause,end;
end;

// test scheduling: sleeping is embarassingly parallel even on uniprocessors ! :)
function r=do_sleep(t);sleep(t*1000);r=1;endfunction
many_workers= 80;
sleep_time=2;
args=ones(1,many_workers)*sleep_time;
for sched=0:1
  tic();
  res= parallel_run(args, "do_sleep", tlist(['param','dynamic_scheduling', 'shared_memory', 'nb_workers'], sched, 1, many_workers ));
  if toc()>sleep_time+2 then pause,end;
end;
// test dynamic versus static scheduling : dynamic scheduling should be better on tasks of varying length.
args=(1:20)^2/100;
for sched=0:1
  tic();
  res= parallel_run(args, "do_sleep",  tlist(['param','dynamic_scheduling', 'shared_memory','nb_workers', 'chunk_size'], sched, 1, 5, 1 ));
  elapsed(sched+1)= toc();
end;
if elapsed(2)>elapsed(1) then pause,end;
