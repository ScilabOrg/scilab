/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.commons;

import org.w3c.dom.Document;

import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfigurationEvent;
import org.scilab.modules.commons.xml.XConfigurationListener;
import static org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;
import org.scilab.modules.localization.WindowsDefaultLanguage;

public class ScilabGeneralPrefs implements XConfigurationListener {

    private static final String ENV_PATH = "//general/body/environment";
    private static final String LANG_PATH = "//general/body/languages";
    private static final ScilabGeneralPrefs instance = new ScilabGeneralPrefs();

    private ScilabGeneralPrefs() { }

    public static ScilabGeneralPrefs getInstance() {
        return instance;
    }

    public void configurationChanged(XConfigurationEvent e) {
        if (e.getModifiedPaths().contains(ENV_PATH)) {
            Document doc = XConfiguration.getXConfigurationDocument();
            GeneralEnvironment ge = XConfiguration.get(GeneralEnvironment.class, doc, ENV_PATH)[0];
            ScilabCommons.setieee(ge.code);
            ScilabCommons.setformat(ge.format, ge.width);
        }
        if (e.getModifiedPaths().contains(LANG_PATH)) {
            Document doc = XConfiguration.getXConfigurationDocument();
            Language language = XConfiguration.get(Language.class, doc, LANG_PATH)[0];
            WindowsDefaultLanguage.setdefaultlanguage(language.lang);
        }
    }

    @XConfAttribute
    private static class GeneralEnvironment {

        public int code;
        public String format;
        public int width;

        private GeneralEnvironment() { }

        @XConfAttribute(attributes = {"fpe", "printing-format", "width"})
        private void set(int fpe, String format, int width) {
            this.code = fpe;
            this.format = format;
            this.width = Math.min(Math.max(0, width), 25);
        }
    }

    @XConfAttribute
    private static class Language {

        public String lang;

        private Language() { }

        @XConfAttribute(attributes = {"lang"})
        private void set(String lang) {
            this.lang = lang;
        }
    }


}
