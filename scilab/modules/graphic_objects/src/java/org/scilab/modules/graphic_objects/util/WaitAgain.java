/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Pierre LANDO
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

package org.scilab.modules.graphic_objects.util;

/**
 * Author: Lando Pierre
 * Date: january 14, 2010
 */

public abstract class WaitAgain {
	private long releaseDate;
	Waiter waiter = null;
	
	public void waitAgain(long releaseDelta) {
		releaseDate = System.currentTimeMillis() + releaseDelta;
		if(waiter == null) {
			waiter = new Waiter();
			waiter.start();
		}
	}
	
	private class Waiter extends Thread {
		public synchronized void run() {
			long remainingTime = releaseDate - System.currentTimeMillis();
			while(remainingTime > 0) {
				try {
					wait(remainingTime);
				} catch (InterruptedException e) {
					return;
				}
				remainingTime = releaseDate - System.currentTimeMillis();
			}
			execute();
			waiter = null;
		}
	}
	
	abstract public void execute();
}
