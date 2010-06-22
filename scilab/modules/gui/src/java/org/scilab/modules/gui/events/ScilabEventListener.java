/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.gui.events;

import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.event.MouseEvent;
import java.awt.event.MouseWheelEvent;
import java.awt.event.ComponentEvent;
import java.awt.event.MouseListener;
import java.awt.event.MouseMotionListener;
import java.awt.event.ComponentListener;

import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.gui.utils.SciTranslator;
/*
 * This class is to manage scilab callback through seteventhandler
 * it means call a dedicated scilab function like this :
 * function my_eventhandler(windowsId, mouse X, mouse Y, mouse Button)
 */
public class ScilabEventListener implements KeyListener, MouseListener, MouseMotionListener, ComponentListener {

	private String callback;
	private int windowsId;
	private int mouseX = 0;
	private int mouseY = 0;
	private SciTranslator eventTranslator = new SciTranslator();
	private boolean freedom = true;
	private boolean inCanvas = false;

	public ScilabEventListener(String callback, int windowsId) {
		eventTranslator.setClickAction(SciTranslator.UNMANAGED);
		this.callback = callback;
		this.windowsId	= windowsId;
	}

	private void callScilab() {
		// @FIXME : choose to send it to scilab or to display it
		//
		InterpreterManagement.requestScilabExec(callback+'('+windowsId+','+mouseX+','+mouseY+','+eventTranslator.getClickAction()+')');
		//
		//System.out.println("call " + callback+'('+windowsId+','+mouseX+','+mouseY+','+eventTranslator.getClickAction()+')');
	}

	private void invokeScilab() {
		// @FIXME : choose to send it to scilab or to display it
		//
		InterpreterManagement.requestScilabExec(callback+'('+windowsId+','+mouseX+','+mouseY+','+eventTranslator.javaClick2Scilab()+')');
		//
		//System.out.println("invoke " + callback+'('+windowsId+','+mouseX+','+mouseY+','+eventTranslator.javaClick2Scilab()+')');
	}

	public void keyPressed(KeyEvent keyEvent) {
		if (inCanvas) {
			if (Character.isJavaIdentifierStart(keyEvent.getKeyChar())) {
				eventTranslator.setClickAction(SciTranslator.javaKey2Scilab(keyEvent.getKeyChar(), keyEvent.isControlDown()));
				callScilab();
			}
			else {
				int keyChar;
				if (keyEvent.isShiftDown()) {
					keyChar = keyEvent.getKeyCode();
				}
				else {
					keyChar = Character.toLowerCase(keyEvent.getKeyCode());
					callScilab();
				}
				eventTranslator.setClickAction(SciTranslator.javaKey2Scilab(keyChar,
						keyEvent.isControlDown()));
				callScilab();
			}
		}
	}

	public void keyReleased(KeyEvent arg0) {
		if(inCanvas && eventTranslator.getClickAction() != SciTranslator.UNMANAGED) {
			eventTranslator.setClickAction(-eventTranslator.getClickAction());
			callScilab();
			eventTranslator.setClickAction(SciTranslator.UNMANAGED);
		}
	}

	public void keyTyped(KeyEvent arg0) {
		// Do nothing !!!
	}

	public void mouseClicked(MouseEvent arg0) {
		mouseX = arg0.getX();
		mouseY = arg0.getY();
		if (arg0.getClickCount() == 1) {
			eventTranslator.setClickAction(
					SciTranslator.javaButton2Scilab(
							arg0.getButton(),
							SciTranslator.CLICKED,
							arg0.isControlDown()));
		}
		else {
			/* Means mouseEvent.getClickCount() >= 2 */
			eventTranslator.setClickAction(
					SciTranslator.javaButton2Scilab(
							arg0.getButton(),
							SciTranslator.DCLICKED,
							arg0.isControlDown()));
			// To unlock javaClick2Scilab done in launchfilter
			synchronized (eventTranslator) {
				eventTranslator.notify();
			}
		}
	}

	public void mouseEntered(MouseEvent arg0) {
		inCanvas = true;
	}

	public void mouseExited(MouseEvent arg0) {
		inCanvas = false;
	}

	public void mousePressed(MouseEvent arg0) {
		if (this.freedom) {
			this.freedom = false;
			mouseX = arg0.getX();
			mouseY = arg0.getY();
			eventTranslator.setClickAction(
					SciTranslator.javaButton2Scilab(
							arg0.getButton(),
							SciTranslator.PRESSED,
							arg0.isControlDown()));
			Thread launchMe = new Thread() {
				public void run() {
					invokeScilab();
					freedom = true;
				}
			};
			launchMe.start();
		}
	}

	public void mouseReleased(MouseEvent arg0) {
		// TODO Auto-generated method stub
		if (eventTranslator.getClickAction() == SciTranslator.UNMANAGED) {
			eventTranslator.setClickAction(
					SciTranslator.javaButton2Scilab(arg0.getButton(),
							SciTranslator.RELEASED,
							arg0.isControlDown()));
			mouseX = arg0.getX();
			mouseY = arg0.getY();
			callScilab();
		}
	}

	public void mouseDragged(MouseEvent arg0) {
		if (eventTranslator.getClickAction() == eventTranslator.javaButton2Scilab(MouseEvent.BUTTON1, SciTranslator.PRESSED, false)) { /* If LEFT BUTTON PRESSED */
			this.freedom = false;
			mouseX = arg0.getX();
			mouseY = arg0.getY();
			callScilab();
			freedom = true;
			eventTranslator.setClickAction(SciTranslator.SCIMOVED);
		} else {
			eventTranslator.setClickAction(SciTranslator.SCIMOVED);
			mouseX = arg0.getX();
			mouseY = arg0.getY();
			callScilab();
		}
	}

	public void mouseMoved(MouseEvent arg0) {
		eventTranslator.setClickAction(SciTranslator.SCIMOVED);
		mouseX = arg0.getX();
		mouseY = arg0.getY();
		callScilab();
	}

	public void componentHidden(ComponentEvent arg0) {
                if (arg0.getID()==ComponentEvent.COMPONENT_HIDDEN) {
                    eventTranslator.setClickAction(SciTranslator.WINDOW_HIDDEN);
                };
		mouseX = 0;
		mouseY = 0;
		callScilab();
	}

	public void componentMoved(ComponentEvent arg0) {
                if (arg0.getID()==ComponentEvent.COMPONENT_MOVED) {
                    eventTranslator.setClickAction(SciTranslator.WINDOW_MOVED);
                };
		mouseX = 0;
		mouseY = 0;
		callScilab();
	}

	public void componentShown(ComponentEvent arg0) {
                if (arg0.getID()==ComponentEvent.COMPONENT_SHOWN) {
                    eventTranslator.setClickAction(SciTranslator.WINDOW_SHOWN);
                };
		mouseX = 0;
		mouseY = 0;
		callScilab();
	}

	public void componentResized(ComponentEvent arg0) {
                if (arg0.getID()==ComponentEvent.COMPONENT_RESIZED) {
                    eventTranslator.setClickAction(SciTranslator.WINDOW_RESIZED);
                };
		mouseX = 0;
		mouseY = 0;
		callScilab();
	}

}
