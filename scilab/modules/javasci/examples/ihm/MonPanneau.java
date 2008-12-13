/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

import java.awt.*;
import javasci.*; 
					 
public class MonPanneau extends Panel
{
    private Label lab;
    private Button bout;
    private int nbrDeClic;
    private TextField EditBox;
	
    public MonPanneau()
    {
	nbrDeClic=0;
	lab = new Label("le nbr de clic egale "+nbrDeClic);
	bout = new Button("Clic ici");
	EditBox = new TextField("Hello",10);
			
	this.setLayout(new GridLayout(3,1));
	this.add(lab);
	this.add(bout);
	this.add(EditBox);
		
	// Ecoute d'�v�nement
	bout.addActionListener(new MonEcouteurPourBouton(this));
		
	// NEW 
	EditBox.addTextListener(new MonEcouteurPourText(this));
		
		
    }
    public void YA_EU_UN_Click()
    {
	int i=0;
	nbrDeClic++;
	lab.setText("le nbr de clic egale "+nbrDeClic);
	Scilab.Exec("plot2d();quit");
	System.out.println("Avant Scilab.HaveAGraph = " + Scilab.HaveAGraph());
   
	do
	    {
		Scilab.Events();
		try
		    {
			Thread.sleep( 1 );
		    }
		catch ( InterruptedException f )
		    {
		    }

		System.out.println("Boucle Java sans fin "+i);
		i++;
	    }
	while (Scilab.HaveAGraph()!=false);

	Scilab.Events();

	System.out.println("Apres Scilab.HaveAGraph");
		
    }
    public void recupTexte()
    {
	String textelu;
	textelu = EditBox.getText();
	System.out.println("mon texte est = " + textelu);	
    }
}

