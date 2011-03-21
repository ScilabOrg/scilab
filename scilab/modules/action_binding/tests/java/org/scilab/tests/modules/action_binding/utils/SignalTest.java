/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.tests.modules.action_binding.utils;

import org.scilab.modules.action_binding.utils.Signal;
import org.testng.annotations.Test;

/**
 * Check of {@link Signal} class
 */
public class SignalTest {
	
	final String signal = "plop";
	
	final Runnable waiter = new Runnable() {
		@Override
		public void run() {
			Signal.wait(signal);
		}
	};
	
	final Runnable notifier = new Runnable() {
		@Override
		public void run() {
			Signal.notify(signal);
		}
	};
	
	@Test(timeOut=100)
	public void simpleNotify() throws InterruptedException {
		Thread tWaiter = new Thread(waiter);
		
		tWaiter.start();
		
		notifier.run();
		tWaiter.join();
	}
	
	@Test
	public void notifyBeforeWait() throws InterruptedException {
		Thread tWaiter = new Thread(waiter);
		Thread tNotifier = new Thread(notifier);
		
		// notify
		tNotifier.start();
		
		// then wait
		tWaiter.start();
		
		tNotifier.join();
		assert !tNotifier.isAlive();
		assert !tWaiter.isAlive();
	}
}
