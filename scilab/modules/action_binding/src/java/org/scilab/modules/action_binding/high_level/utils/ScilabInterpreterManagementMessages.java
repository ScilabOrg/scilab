package org.scilab.modules.action_binding.high_level.utils;

import org.scilab.modules.localization.Messages;

public final class ScilabInterpreterManagementMessages {

	/* Interpreter errors */
    public static final String SCILAB_SAMECOMMAND = Messages.gettext("Same command executed again");
    public static final String SCILAB_UNABLE = Messages.gettext("Unable to communicate with the interpreter");
	
	
	/** This class is a static singleton, thus it must not be instantiated */
	private ScilabInterpreterManagementMessages() {
	}
}
