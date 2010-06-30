/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

//CHECKSTYLE:OFF

package org.scilab.modules.scinotes.utils;

import org.scilab.modules.localization.Messages;

/**
 * All messages used in SciNotes menus, dialogs, ...
 * @author Bruno JOFRET
 * @author Calixte DENIZET
 */
public class SciNotesMessages {

    public static final String DOTS = "...";

    public static final String SCILAB_EDITOR = Messages.gettext("SciNotes");
    public static final String ERROR_WITH_STRING = Messages.gettext("Error while reading the String");

    /** FILE MENU */
    public static final String FILE = Messages.gettext("File");
    public static final String NEW = Messages.gettext("New") + DOTS;
    public static final String OPEN = Messages.gettext("Open") + DOTS;
    public static final String OPEN_SOURCE_FILE_ON_KEYWORD = Messages.gettext("Open function source file") + DOTS;
    public static final String OPEN_URL = Messages.gettext("Open URL in web browser") + DOTS;
    public static final String SOURCE_OF = Messages.gettext("Source of '");
    public static final String RECENT_FILES = Messages.gettext("Recent Files");
    public static final String SAVE = Messages.gettext("Save");
    public static final String SAVE_AS = Messages.gettext("Save as") + DOTS;
    /**
     * "Save All" message
     */
    public static final String SAVE_ALL = Messages.gettext("Save All");
    public static final String PAGE_SETUP = Messages.gettext("Page Setup") + DOTS;
    public static final String PRINT_PREVIEW = Messages.gettext("Print Preview") + DOTS;
    public static final String PRINT = Messages.gettext("Print") + DOTS;
    public static final String CLOSE = Messages.gettext("Close");
    public static final String CLOSEALL = Messages.gettext("Close All");
    public static final String CLOSEALLBUTTHIS = Messages.gettext("Close All But This");
    public static final String EXIT = Messages.gettext("Exit");
    public static final String QUESTION_MARK = Messages.gettext("?");

    /** EDIT MENU */
    public static final String EDIT = Messages.gettext("Edit");
    public static final String UNDO = Messages.gettext("Undo");
    public static final String REDO = Messages.gettext("Redo");
    public static final String CUT = Messages.gettext("Cut");
    public static final String COPY = Messages.gettext("Copy");
    public static final String PASTE = Messages.gettext("Paste");
    public static final String SELECT_ALL = Messages.gettext("Select All");
    public static final String DELETE = Messages.gettext("Delete");

    /** TOOLS MENU */
    public static final String TOOLS = Messages.gettext("Tools");
    public static final String COMMENT_SELECTION = Messages.gettext("Comment Selection");
    public static final String UNCOMMENT_SELECTION = Messages.gettext("Uncomment Selection");
    public static final String TABIFY_SELECTION = Messages.gettext("Shift Right");
    public static final String UNTABIFY_SELECTION = Messages.gettext("Shift Left");
    public static final String INDENT = Messages.gettext("Correct Indentation");
    public static final String HELP_ON_TYPING = Messages.gettext("Help on typing");
    public static final String GENERATE_HELP = Messages.gettext("Generate help from function");
    public static final String OPEN_TAB_IN_NEW_WINDOW = Messages.gettext("Clone tab in a new window");
    public static final String CCLOSE_TAB_IN_NEW_WINDOW = Messages.gettext("Clone and close tab in a new window");
    public static final String REMOVE_TRAILING_WHITE = Messages.gettext("Remove trailing whitespaces");

    /** SEARCH MENU */
    public static final String SEARCH = Messages.gettext("Search");
    public static final String FIND_REPLACE = Messages.gettext("Find/Replace");
    public static final String FIND_NEXT = Messages.gettext("Find Next");
    public static final String FIND_PREVIOUS = Messages.gettext("Find Previous");
    public static final String GOTO_LINE = Messages .gettext("Goto line");

    /** VIEW MENU */
    public static final String VIEW = Messages.gettext("View");
    public static final String SHOW_HIDE_TOOLBAR = Messages.gettext("Show/Hide ToolBar");
    public static final String HIGHLIGHT_CURRENT_LINE = Messages.gettext("Highlight current line");
    public static final String WORD_WRAP = Messages.gettext("Word Wrap");
    public static final String LINE_NUMBERS_WHEREAMI = Messages.gettext("Whereami Line Numbering");
    public static final String LINE_NUMBERS_NOWHEREAMI = Messages.gettext("Normal Line Numbering");
    public static final String NO_LINE_NUMBERS = Messages.gettext("No Line Numbering");

    public static final String SET_COLORS = Messages.gettext("Set Colors") + DOTS;
    public static final String SET_FONT = Messages.gettext("Set Font") + DOTS;
    public static final String RESET_DEFAULT_FONT = Messages.gettext("Reset default font");

    public static final String SPLIT_HORIZONTALLY = Messages.gettext("Split horizontally");
    public static final String SPLIT_VERTICALLY = Messages.gettext("Split vertically");
    public static final String NO_SPLIT = Messages.gettext("No split");

    /** DOCUMENT MENU */
    public static final String DOCUMENT = Messages.gettext("Document");
    public static final String SYNTAX_TYPE = Messages.gettext("Syntax Type");
    public static final String SCILAB_STYLE = Messages.gettext("Scilab");
    public static final String PLAIN_TEXT_STYLE = "Plain Text";
    public static final String XML_STYLE = "XML";
    public static final String ENCODING_TYPE = Messages.gettext("Encoding");
    public static final String COLORIZE = Messages.gettext("Colorize");
    public static final String AUTO_INDENT = Messages.gettext("Enable Auto Reformat");
    public static final String EOL_TYPE = Messages.gettext("Line Endings");
    public static final String EOL_AUT0 = Messages.gettext("Auto (Default)");
    public static final String EOL_WINDOWS = Messages.gettext("Windows/DOS (CR + LF)");
    public static final String EOL_MACOS = Messages.gettext("Mac Classic (CR)");
    public static final String EOL_LINUX = Messages.gettext("Unix (LF)");

    /** EXECUTE MENU */
    public static final String EXECUTE = Messages.gettext("Execute");
    public static final String LOAD_INTO_SCILAB = Messages.gettext("Load Into Scilab");
    public static final String EVALUATE_SELECTION = Messages.gettext("Evaluate Selection");
    public static final String EVALUATE_FROM_BEGINNING = Messages.gettext("Evaluate from beginning...");
    public static final String EXECUTE_FILE_INTO_SCILAB = Messages.gettext("Execute File Into Scilab");

    /** HELP MENU */
    public static final String HELP = Messages.gettext("SciNotes help") + DOTS;
    public static final String HELP_ON_KEYWORD = Messages.gettext("Help on the current keyword");
    public static final String HELP_ON_SELECTED = Messages.gettext("Help on the selected text");
    public static final String HELP_ON_FOO = Messages.gettext("Help on selected text or keyword");
    public static final String HELP_ABOUT = Messages.gettext("Help about '");
    public static final String ABOUT = Messages.gettext("About") + DOTS;
    public static final String SCINOTES_VERSION = Messages.gettext("SciNotes");

    /** FIND/REPLACE DIALOG */
    public static final String FIND = Messages.gettext("Find:");
    public static final String FIND_BUTTON = Messages.gettext("Find");
    public static final String REPLACE_FIND = Messages.gettext("Replace/Find");
    public static final String REPLACE_WITH = Messages.gettext("Replace With :");
    public static final String DIRECTION = Messages.gettext("Direction");
    public static final String FORWARD = Messages.gettext("Forward");
    public static final String BACKWARD = Messages.gettext("Backward");
    public static final String SCOPE = Messages.gettext("Scope");
    public static final String ALL = Messages.gettext("All");
    public static final String SELECTED_LINES = Messages.gettext("Selected lines");
    public static final String OPTIONS = Messages.gettext("Options");
    public static final String CASE_SENSITIVE = Messages.gettext("Case sensitive");
    public static final String WRAP_SEARCH = Messages.gettext("Wrap search");
    public static final String WHOLE_WORD = Messages.gettext("Whole word");
    public static final String REGULAR_EXPRESSIONS = Messages.gettext("Regular expressions");
    public static final String REPLACE = Messages.gettext("Replace");
    public static final String REPLACE_ALL = Messages.gettext("Replace All");
    public static final String INVALID_REGEXP = Messages.gettext("Invalid regular expression : %s");
    public static final String PASSED_END_OF_DOCUMENT = Messages.gettext("You have passed the end of the document");
    public static final String PASSED_BEGIN_OF_DOCUMENT = Messages.gettext("You have passed the beginning of the document");
    public static final String END_OF_DOCUMENT = Messages.gettext("You have reached the end of the document");
    public static final String BEGIN_OF_DOCUMENT = Messages.gettext("You have reached the beginning of the document");
    public static final String STRING_NOT_FOUND = Messages.gettext("String %s not found");

    public static final String FILE_DOESNT_EXIST = Messages.gettext("The file %s doesn't exist\n Do you want to create it?");
    public static final String CLOSE_TAB_TIP = Messages.gettext("close this tab");
    public static final String THIS_FILE = Messages.gettext("This file");
    public static final String MODIFIED = Messages.gettext(" has been modified since last save\nsave it?");
    public static final String EXTERNAL_MODIFICATION = Messages.gettext("The file %s has been modified by another program !\n Overwrite it ?");
    public static final String EXTERNAL_MODIFICATION_INFO = Messages.gettext("Warning: the file has been modified by another program !");
    public static final String RELOAD = Messages.gettext("Reload");
    public static final String OVERWRITE = Messages.gettext("Overwrite");
    public static final String IGNORE = Messages.gettext("Ignore");
    public static final String REPLACE_FILE_TITLE = Messages.gettext("Replace File?");
    public static final String FILE_ALREADY_EXIST = Messages.gettext("File already exists");
    public static final String UNTITLED = Messages.gettext("Untitled ");
    public static final String OK = Messages.gettext("Ok");
    public static final String CANCEL = Messages.gettext("Cancel");
    public static final String DEFAULT = Messages.gettext("Default");
    public static final String NEXT_TAB = Messages.gettext("Next tab");
    public static final String PREVIOUS_TAB = Messages.gettext("Previous tab");
    public static final String EXECUTE_WARNING = Messages.gettext("You need to save your modifications before executing this file into Scilab.");
    public static final String BIG_FILE_WARNING = Messages.gettext("This file is very long. \"Execute File Into Scilab\" could be better.\nAre you sure to continue ?");


    public static final String ENTER_LINE_NUMBER = Messages .gettext("Enter line number");
    public static final String BINARY_FILE = Messages .gettext("This file is a binary, saving it will result in a corrupt file.");

    /* Status bar messages */
    public static final String LOADING = Messages .gettext("Loading") + DOTS;
    public static final String COLORIZATION_CANCELED = Messages .gettext("Colorization canceled");
    public static final String BINARY_FILE_MODE = Messages .gettext("Binary file: read-only mode");
    public static final String READ_ONLY_MODE = Messages .gettext("Read-only mode");

    /* About box */
    public static final String VERSION = Messages.gettext("SciNotes") + " 1.1";
    public static final String DIGITEO = "The Scilab Consortium (DIGITEO)";
    public static final String COPYRIGHT = Messages.gettext("Copyright (c)") + " 2009-2010";

    public static final String SCINOTES_ERROR = Messages.gettext("SciNotes error");
    public static final String COULD_NOT_SAVE_FILE = Messages.gettext("The file could not be saved.");
    public static final String COULD_NOT_CONVERT_FILE = Messages.gettext("The file could not be converted.");
    public static final String CANNOT_LOAD_DIRECTORY = Messages.gettext("%s is a directory.");
    public static final String MODIFICATIONS_WILL_BE_LOST = Messages.gettext("Your modifications will be lost, change encoding anyway?");
    public static final String CONTINUE = Messages.gettext("Continue ?");
    public static final String CANNOT_LOAD_COLORIZE = Messages.gettext("Cannot load \"scinotesConfiguration.xml\"");

    public static final String COULD_NOT_FIND_CONSOLE = Messages.gettext("Could not find the console nor the InterpreterManagement.");
    public static final String COULD_NOT_FIND_TMPFILE = Messages.gettext("Could not find the temporary file.");
    public static final String NO_TEXT_SELECTED = Messages.gettext("No text selected.");
    public static final String NO_TEXT_TO_EXECUTE = Messages.gettext("No text to execute.");
    public static final String COMPLETE = Messages.gettext("Complete");
    public static final String EDIT_SELECTION = Messages.gettext("Copy selection in a new tab");

    /**
     * "Change Colors" message
     */
    public static final String CHANGE_COLORS =  Messages.gettext("Change Colors");

    /**
     * "Change Color" message
     */
    public static final String CARET_COLOR =  Messages.gettext("Cursor Color");
    public static final String BGCOLOR =  Messages.gettext("Background Color");
    public static final String CHANGE_COLOR =  Messages.gettext("Change Color");
    public static final String ITALIC = Messages.gettext("Italic");
    public static final String STRIKETHROUGH = Messages.gettext("Strike Through");
    public static final String PREVIEW = Messages.gettext("Preview");
    public static final String UNDERLINE = Messages.gettext("Underline");
    public static final String SETTINGS = Messages.gettext("Settings");
    public static final String COLOR = Messages.gettext("Color");
    public static final String BOLD = Messages.gettext("Bold");

    /**
     * Navigator labels
     */
    public static final String CODE_NAVIGATOR = Messages.gettext("Code Navigator");
    public static final String ABSOLUTE = Messages.gettext("Absolute");
    public static final String RELATIVE = Messages.gettext("Relative");
    public static final String GO_TO_LINE = Messages.gettext("Go to line: ");
    public static final String NUMEROTATION = Messages.gettext("Numerotation");
    public static final String ALPHABETIC_ORDER = Messages.gettext("Alphabetic order");
    public static final String NATURAL_ORDER = Messages.gettext("Natural order");
    public static final String ORDER = Messages.gettext("Order");

    /**
     * Exit confirm message
     */
    public static final String EXIT_CONFIRM = Messages.gettext("Are you sure that you want to close SciNotes ?");
}
