c     Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
c     Copyright (C) 1985-2008 - INRIA - Carlos KLIMANN
c     Copyright (C) {date_debut_2}-{date_fin_2} - {organisme 2} -
c     {auteur 2}
c     ...
c     
c Copyright (C) 2012 - 2016 - Scilab Enterprises
c
c This file is hereby licensed under the terms of the GNU GPL v2.0,
c pursuant to article 5.3.4 of the CeCILL v.2.1.
c This file was originally licensed under the terms of the CeCILL v2.1,
c and continues to be available under such terms.
c For more information, see the COPYING file which you should have received
c along with this program.
C     /MEMBR ADD NAME=IMPINS,SSI=0
c     Copyright INRIA
      subroutine impins(mat1,dep1,lig1,col1,mat2,
     &     dep2,lig2,col2,matr,depr,ligr,colr)
c     !but
c     
c     Cette subroutine fait l'insertion  d'une matrice
c     de polynomes mat2 dans une autre mat1 d'apres deux vecteurs.
c     On suppose donnees depr, ligr et colr.
c     Le resultat viendra dans une troisieme matrice.
c     
c     !parametres d'appel
c     
c     call impins(mat1,dep1,lig1,col1,mat2,dep2,lig2,
c     1            col2,matr,depr,ligr,colr)
c     
c     ou
c     
c     mat1: matrice integer de polynomes
c     
c     dep1: matrice integere qui donne les deplacements relatifs des
c     elements de mat1
c     
c     lig1, col1: integers, dimensions de mat1
c     
c     mat2 ,dep2, lig2, col2: analogues aux correspondents 1
c     (correspond a m dans l'example)
c     
c     matr, depr, ligr, colr: parametres de sortie, matrice matr et
c     autres valeurs relationnes.
c     
c     
c     attention!: aucune de matrices mat1, mat2,matr,dep1, dep2 ou depr
c     doit coincider. Cas contraire, les resultats seraient
c     imprevisibles.
c     
c     
c     !
      integer mat1(*),mat2(*),matr(*)
      integer dep1(*),dep2(*),depr(*)
c     
      integer lig1,col1,lig2,col2,ligr,colr
      depr(1)=1
      kr=1
c     
      do 21 j=1,colr
         do 20 i=1,ligr
            kr=kr+1
            lr=depr(kr)
            if (lr .lt. 0) then
               goto 11
            elseif (lr .eq. 0) then
               goto 12
            else
               goto 13
            endif
 11         l2=-lr
            l=dep2(l2+1)-dep2(l2)
            call icopy(l,mat2(dep2(l2)),1,matr(depr(kr-1)),1)
            depr(kr)=depr(kr-1)+l
            goto 20
 12         continue
c     matr(depr(kr-1))=40
            depr(kr)=depr(kr-1)
            goto 20
 13         l1=lr
            l=dep1(l1+1)-dep1(l1)
            call icopy(l,mat1(dep1(l1)),1,matr(depr(kr-1)),1)
            depr(kr)=depr(kr-1)+l
c     
 20      continue
 21   continue
      return
      end
