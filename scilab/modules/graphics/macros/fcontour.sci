// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function fcontour(xr,yr,f,nz,teta,alpha,leg,flag,ebox,zlev)
    //fcontour(xr,yr,f,nz,[teta,alpha,leg,flag,ebox,zlev])
    // Trace des courbes de niveau de la surface
    // d\'efinie par un external f  ( ex macro [y]=f(x))
    // on calcule d'abord f sur la grille definie par xr.yr
    // xr et yr sont des vecteurs implicites donnant les
    // abscisses et les ordonn\'ees des points de la grille
    // - x est une matrice de taille (1,n1)
    // - y est une matrice de taille (1,n2)
    // nz : permet de specifier les niveaux cherches
    //    si nz est un nombre c'est le nombre de courbes de niveau demandees
    //	regulierement espacees entre zmin et zmax
    //    si est un vecteur, il specifie les valeurs de z pour lesquelles
    //      on veut les courbes de niveau
    //
    // Les arguments suivants sont optionnels et sont identiques a ceux de
    //    plot3d (sauf zlev), il permettent de dessiner des courbes de niveau
    //    sur  un graphique 3d.
    //    Seule la signification de flag(1) est differente :
    //     flag(1)=0, les courbes de niveaux sont dessinees
    //         sur un graphique 3d, sur la surface definie par (x,y,z)
    //     flag(1)=1, les courbes de biveaux sont dessinees
    //         sur un graphique 3d, sur le plan defini par z=zlev
    //     flag(1)=2, les courbes de biveaux sont dessinees
    //         sur un graphique 2d.
    // Exemple : taper fcontour() pour voir un exemple .
    // deff('[z]=Surf(x,y)','z=x**2+y**2');
    // fcontour(Surf,-1:0.1:1,-1:0.1:1,10);
    //
    //!

    [lhs,rhs]=argn(0)

    if rhs == 0 then   // demo
        deff("[z]=Surf(x,y)","z=x**2+y**2");
        fcontour(-1:0.1:1,-1:0.1:1,Surf,10);
        return
    end

    if rhs<3,
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "fcontour", 3));
    end
    opts=[]
    if rhs<4,
        nz=10,
    else
        if exists("theta","local")==1 then opts=[opts,"theta=theta"],end
        if exists("alpha","local")==1 then opts=[opts,"alpha=alpha"],end
        if exists("leg"  ,"local")==1 then opts=[opts,"leg=leg"]    ,end
        if exists("flag" ,"local")==1 then opts=[opts,"flag=flag"]  ,end
        if exists("ebox" ,"local")==1 then opts=[opts,"ebox=ebox"]  ,end
        if exists("zlev" ,"local")==1 then opts=[opts,"zlev=zlev"]  ,end
    end;

    if type(f)==11 then comp(f),end;
    execstr("contour(xr,yr,feval(xr,yr,f),nz,"+strcat(opts,",")+")")
endfunction
