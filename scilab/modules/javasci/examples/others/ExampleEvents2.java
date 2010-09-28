/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file is released into the public domain
 *
 */

import javasci.Scilab; 

class LoopEvent extends Thread
{
    boolean continueLoop = true;
    public void run()
        {
            System.out.println("Press 'q' and 'return' to quit.");
            Scilab.Exec("plot2d();");
            do 
            {
                Scilab.Events();
            }
            while ( (Scilab.HaveAGraph() != false) && (continueLoop == true) );
            Scilab.Finish();
            System.out.println("Graphics window closed");
            this.stop();

        }
      
    public void close()
        {
            continueLoop = false;
        }
}

class  ExampleEvents2
{

    public static void main(String[] args)  throws Exception

        {
            char c;
            LoopEvent thread = new LoopEvent();
            thread.start();
      
            while (System.in.read()!='q')
            {
                try
                {
                    Thread.sleep( 1 );
                }
                catch ( InterruptedException e )
                {
                }
            }
            System.out.println("'q' pressed.");
            thread.close();
      
        }

}

