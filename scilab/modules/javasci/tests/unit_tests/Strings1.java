/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

import javasci.* ;

class  Strings1 {

  public static void main(String[] args)
  {
    int col=4;
    int row=2;
    int r,c,k;
    String [] b=new String [row*col];
    String [] d=new String[row*col];

    SciStringArray a = new SciStringArray("A",row,col);
    Scilab.Exec("A=['Aaa','Bbb','Ccc','Ddd';'Eee','Fff','Ggg','Hhh'];");
    a.Get();
    System.out.println("----------------------------------------------------");
    for(r=1;r<=a.getNumberOfRows();r++)
    {
     for(c=1;c<=a.getNumberOfCols();c++)
     {
      System.out.print("A("+(r)+","+(c)+")="+a.GetElement(r,c)+"  ");
     }
     System.out.println(" ");
    }
    System.out.println("----------------------------------------------------");

    b=a.getData();

    for(r=0;r<row;r++)
    {
     for(c=0;c<col;c++)
     {
      System.out.print("b("+r+","+(c)+")="+ b[c*row+r] +"  ");
     }
     System.out.println(" ");
    }
    System.out.println("----------------------------------------------------");


    for (k=0;k<row*col;k++)
    {
      d[k]=new String("Sentence ")+k;
    }

    SciStringArray D = new SciStringArray("D",2,4,d);
    Scilab.Finish();
  }
}

