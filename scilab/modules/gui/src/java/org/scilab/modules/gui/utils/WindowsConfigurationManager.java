/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.gui.utils;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.UUID;

import org.w3c.dom.Attr;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.w3c.dom.Text;
import org.xml.sax.SAXException;

import org.flexdock.docking.Dockable;
import org.flexdock.docking.DockingConstants;
import org.flexdock.docking.DockingManager;
import org.flexdock.docking.activation.ActiveDockableTracker;
import org.flexdock.docking.state.LayoutNode;
import org.flexdock.perspective.persist.xml.LayoutNodeSerializer;
import org.flexdock.perspective.persist.xml.PersistenceConstants;

import org.scilab.modules.commons.ScilabCommons;
import org.scilab.modules.commons.ScilabCommonsUtils;
import org.scilab.modules.commons.xml.ScilabXMLUtilities;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.bridge.window.SwingScilabWindow;
import org.scilab.modules.gui.console.ScilabConsole;
import org.scilab.modules.gui.tab.Tab;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.window.ScilabWindow;
import org.scilab.modules.gui.window.Window;

/**
 *
 * Save the windows properties.
 *
 * @author Calixte DENIZET
 */
public class WindowsConfigurationManager {

    private static final int DEFAULTX = 0;
    private static final int DEFAULTY = 0;
    private static final int DEFAULTHEIGHT = 500;
    private static final int DEFAULTWIDTH = 500;

    private static final String SCI = "SCI";
    private static final String WINDOWS_CONFIG_FILE = System.getenv(SCI) + "/modules/gui/etc/WindowsConfiguration.xml";
    private static final String USER_WINDOWS_CONFIG_FILE = ScilabCommons.getSCIHOME() + "/WindowsConfiguration.xml";
    private static final String NULLUUID = new UUID(0L, 0L).toString();
    private static final Map<SwingScilabTab, EndedRestoration> endedRestoration = new HashMap<SwingScilabTab, EndedRestoration>();
    private static final List<String> alreadyRestoredWindows = new ArrayList<String>();
    private static final Map<String, Object> defaultWinAttributes = new HashMap<String, Object>();

    private static boolean oneTry;
    private static Document doc;

    static {
        defaultWinAttributes.put("x", new Integer(DEFAULTX));
        defaultWinAttributes.put("y", new Integer(DEFAULTY));
        defaultWinAttributes.put("height", new Integer(DEFAULTHEIGHT));
        defaultWinAttributes.put("width", new Integer(DEFAULTWIDTH));
    }

    /**
     * Create a copy of windows configuration file in the user directory
     */
    public static void createUserCopy() {
        if (isCopyNeeded()) {
            ScilabCommonsUtils.copyFile(new File(WINDOWS_CONFIG_FILE), new File(USER_WINDOWS_CONFIG_FILE));
            doc = null;
        }
    }

    /**
     * Read the file to modify
     */
    private static void readDocument() {
        if (doc == null) {
            createUserCopy();
            doc = ScilabXMLUtilities.readDocument(USER_WINDOWS_CONFIG_FILE);
        }

        if (doc == null && !oneTry) {
            System.err.println("Try to reload the default configuration file: " + WINDOWS_CONFIG_FILE);
            File f = new File(USER_WINDOWS_CONFIG_FILE);
            if (f.exists() && f.isFile()) {
                f.delete();
            }
            oneTry = true;
            readDocument();
        } else if (doc == null && oneTry) {
            System.err.println("Serious problem to copy and parse the configuration file.");
            System.err.println("Please check if you have the rights to write the file: " + USER_WINDOWS_CONFIG_FILE);
            System.err.println("If the previous file exists, please check if it is a valid XML");
            System.err.println("and if yes, please report a bug: http://bugzilla.scilab.org");
        }
    }

    /**
     * Write the document
     */
    private static void writeDocument() {
        ScilabXMLUtilities.writeDocument(doc, USER_WINDOWS_CONFIG_FILE);
    }

    /**
     * @return true if a copy is needed
     */
    private static final boolean isCopyNeeded() {
        return !new File(USER_WINDOWS_CONFIG_FILE).exists();
    }

    /**
     * Register an EndedRestoration, op.finish() will be executed when the tab restoration will be finished.
     * @param tab the associated tab
     * @param ended the closing operation
     */
    public static void registerEndedRestoration(SwingScilabTab tab, EndedRestoration ended) {
        endedRestoration.put(tab, ended);
    }

    /**
     * Register an EndedRestoration, op.finish() will be executed when the tab restoration will be finished.
     * @param tab the associated tab
     * @param ended the closing operation
     */
    public static void registerEndedRestoration(Tab tab, EndedRestoration ended) {
        registerEndedRestoration((SwingScilabTab) tab.getAsSimpleTab(), ended);
    }

    /**
     * Create a new node with parent element
     * @param parent the parent element
     * @param nodeName the node name
     * @param attr an array containing attribute name followed by its value: "attr1", 1, "attr2", true, ...
     * @return the created element
     */
    public static Element createNode(Element parent, String nodeName, Object[] attr) {
        readDocument();
        for (int i = 0; i < attr.length; i += 2) {
            if (attr[i].equals("uuid")) {
                removeNode(parent, nodeName, (String) attr[i + 1]);
            }
        }

        return ScilabXMLUtilities.createNode(doc, parent, nodeName, attr);
    }

    /**
     * Save the window properties
     * @param window the window
     */
    public static void saveWindowProperties(SwingScilabWindow window) {
        readDocument();

        Element root = doc.getDocumentElement();
        Element win = createNode(root, "Window", new Object[]{"uuid", window.getUUID(),
                                                              "x", (int) window.getLocation().getX(),
                                                              "y", (int) window.getLocation().getY(),
                                                              "width", (int) window.getSize().getWidth(),
                                                              "height", (int) window.getSize().getHeight()});
        LayoutNode layoutNode = window.getDockingPort().exportLayout();
        LayoutNodeSerializer serializer = new LayoutNodeSerializer();
        win.appendChild(serializer.serialize(doc, layoutNode));

        for (Dockable dockable : (Set<Dockable>) window.getDockingPort().getDockables()) {
            saveTabProperties((SwingScilabTab) dockable, false);
        }

        writeDocument();
    }

    /**
     * Restore a window with a given uuid
     * @param uuid the uuid
     * @param restoreTab if true the tab is restored too
     * @return the corresponding window
     */
    public static SwingScilabWindow restoreWindow(String uuid, String defaultTabUuid, boolean restoreTab) {
        readDocument();

        Element root = doc.getDocumentElement();
        Map<String, Object> attrs = new HashMap<String, Object>();
        boolean nullUUID = uuid.equals(NULLUUID);
        Element win = null;

        if (!nullUUID) {
            win = getElementWithUUID(root, "Window", uuid);
            if (win == null) {
                return null;
            }

            attrs.put("x", int.class);
            attrs.put("y", int.class);
            attrs.put("height", int.class);
            attrs.put("width", int.class);
            ScilabXMLUtilities.readNodeAttributes(win, attrs);
        } else {
            attrs = defaultWinAttributes;
        }

        Window w = ScilabWindow.createWindow();
        UIElementMapper.add(w);
        final SwingScilabWindow window = (SwingScilabWindow) w.getAsSimpleWindow();
        if (!nullUUID) {
            window.setUUID(uuid);
        } else {
            window.setUUID(UUID.randomUUID().toString());
        }
        window.setLocation(((Integer) attrs.get("x")).intValue(), ((Integer) attrs.get("y")).intValue());
        window.setSize(((Integer) attrs.get("width")).intValue(), ((Integer) attrs.get("height")).intValue());

        if (restoreTab) {
            if (win != null) {
                LayoutNodeSerializer serializer = new LayoutNodeSerializer();
                NodeList children = win.getElementsByTagName(PersistenceConstants.DOCKING_PORT_NODE_ELEMENT_NAME);
                LayoutNode layoutNode = (LayoutNode) serializer.deserialize((Element) children.item(0));
                window.getDockingPort().importLayout(layoutNode);
            } else if (defaultTabUuid != null && !defaultTabUuid.isEmpty()) {
                SwingScilabTab defaultTab = ScilabTabFactory.getInstance().getTab(defaultTabUuid);
                defaultTab.setParentWindowId(window.getElementId());
                DockingManager.dock(defaultTab, window.getDockingPort());
                ActiveDockableTracker.requestDockableActivation(defaultTab);
            }

            for (SwingScilabTab tab : (Set<SwingScilabTab>) window.getDockingPort().getDockables()) {
                tab.setParentWindowId(window.getElementId());
            }

            SwingScilabTab[] tabs = new SwingScilabTab[window.getNbDockedObjects()];
            tabs = ((Set<SwingScilabTab>) window.getDockingPort().getDockables()).toArray(tabs);

            if (!ScilabConsole.isExistingConsole() && tabs.length == 1 && tabs[0].getPersistentId().equals(NULLUUID)) {
                // null uuid is reserved to the console and in NW mode, there is no console.
                return null;
            } else {
                window.setVisible(true);
            }

            for (SwingScilabTab tab : tabs) {
                // each tab has now a window so it can be useful for the tab to set an icon window or to center a dialog...
                EndedRestoration ended = endedRestoration.get(tab);
                if (ended != null) {
                    ended.finish();
                    endedRestoration.remove(ended);
                }
            }

            if (tabs.length == 1) {
                // we remove undock and close buttons when there is only one View in the DockingPort
                SwingScilabTab.removeActions(tabs[0]);
            } else {
                // we add undock and close buttons
                for (SwingScilabTab tab : tabs) {
                    SwingScilabTab.addActions(tab);
                }
            }

            // Return only when the window is displayable
            while (!window.isDisplayable()) {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException e) {
                    System.err.println(e);
                }
            }
        }

        return window;
    }

    /**
     * Remove a window from the already restored windows
     * @param uuid the win uuid
     */
    public static final void removeWin(String uuid) {
        alreadyRestoredWindows.remove(uuid);
    }

    /**
     * Find all the dependencies of the given tab. The returned list contains parent before its children.
     * @param uuid the tab's uuid
     * @return a list of the elements with the given uuid
     */
    public static final Set<Element> getTabDependencies(String uuid) {
        Element root = doc.getDocumentElement();

        // Children
        List<Element> elements = ScilabXMLUtilities.getElementsWithAttributeEquals(root, "depends", uuid);

        Set<Element> list = new LinkedHashSet<Element>();
        Element el = getElementWithUUID(doc.getDocumentElement(), uuid);
        if (el != null) {
            //We add the parent
            list.add(el);
            // We add the children and their own children
            for (Element e : elements) {
                list.addAll(getTabDependencies(e.getAttribute("uuid")));
            }
        }

        return list;
    }

    /**
     * Create all the tabs depending of the tab with the given uuid.
     * The creation will respect the convention: parent before children.
     * @param uuid the tab uuid
     * @return the list of all the uuids to restore
     */
    public static final Set<Element> createDescendantTabs(String uuid) {
        Set<Element> list = getTabDependencies(uuid);
        for (Element e : list) {
            // All the tabs created in the factory will be cached so when Flexdock will restore the docking
            // it will use the same tab as created here.
            ScilabTabFactory factory = ScilabTabFactory.getInstance();
            factory.addTabFactory(e.getAttribute("load"), e.getAttribute("factory"));
            factory.getTab(e.getAttribute("uuid"));
        }

        return list;
    }

    /**
     * Useful for the following case: you have 3 tabs, A, B and C, C depends of B, A and C are docked in the same window.
     * You want to restore A, the window containing A needs to restore C too (and finally B).
     * There is no direct dependency between A and C but the fact that they're in the same dockport
     * implies a dependency.
     * @param elems the elems corresponding to the tabs to restore
     * @return tabs to restore
     */
    private static final Set<Element> createAdjacentTabs(Set<Element> elems) {
        Element root = doc.getDocumentElement();
        boolean jobFinished = true;
        Set<Element> toAdd = new LinkedHashSet<Element>();
        for (Element e : elems) {
            String winuuid = e.getAttribute("winuuid");
            if (!winuuid.isEmpty()) {
                List<Element> elements = ScilabXMLUtilities.getElementsWithAttributeEquals(root, "winuuid", winuuid);
                elements.removeAll(elems);
                jobFinished = jobFinished && elements.size() == 0;
                toAdd.addAll(elements);
            }
        }

        if (jobFinished) {
            return elems;
        }

        for (Element ee : toAdd) {
            elems.addAll(createDescendantTabs(getElderParent(ee).getAttribute("uuid")));
        }


        return createAdjacentTabs(elems);
    }

    /**
     * @param e the element
     * @return the elder parent of this element (elder for the attribute "depends")
     */
    private static final Element getElderParent(Element e) {
        Element root = doc.getDocumentElement();
        String dep = e.getAttribute("depends");
        if (!dep.isEmpty()) {
            return getElderParent(ScilabXMLUtilities.getElementsWithAttributeEquals(root, "uuid", dep).get(0));
        }

        return e;
    }

    /**
     * Starts the restoration of the tab with the given uuid
     * @param uuid the tab uuid to restore
     */
    private static final void startRestoration(String uuid) {
        Set<Element> list = createDescendantTabs(uuid);
        list = createAdjacentTabs(list);
        List<String> wins = new ArrayList<String>();
        List<String> tabsWithoutWin = new ArrayList<String>();
        for (Element e : list) {
            String winuuid = e.getAttribute("winuuid");
            if (winuuid.equals(NULLUUID)) {
                tabsWithoutWin.add(e.getAttribute("uuid"));
            } else if (!wins.contains(winuuid)) {
                wins.add(winuuid);
            }
        }

        for (String winuuid : wins) {
            if (!alreadyRestoredWindows.contains(winuuid)) {
                restoreWindow(winuuid, uuid, true);
                alreadyRestoredWindows.add(winuuid);
            }
        }

        for (String u : tabsWithoutWin) {
            SwingScilabWindow window = restoreWindow(NULLUUID, u, true);
            alreadyRestoredWindows.add(window.getUUID());
        }
    }

    /**
     * Search a node (child of root) with name nodeName and with a given uuid
     * @param root the root element
     * @param nodeName the node name
     * @param uuid the uuid
     * @return the corresponding element or null if it does not exist
     */
    public static final Element getElementWithUUID(Element root, String nodeName, String uuid) {
        if (uuid == null || uuid.isEmpty()) {
            return null;
        }
        NodeList list = root.getElementsByTagName(nodeName);
        int length = getNodeListLength(list);
        for (int i = 0; i < length; i++) {
            Element elem = (Element) list.item(i);
            if (elem.getAttribute("uuid").equals(uuid)) {
                return elem;
            }
        }

        return null;
    }

    /**
     * Search a node with a given uuid
     * @param uuid the uuid
     * @return the corresponding element or null if it does not exist
     */
    public static final Element getElementWithUUID(String uuid) {
        return getElementWithUUID(doc.getDocumentElement(), uuid);
    }

    /**
     * Search a node (child of root) with name nodeName and with a given uuid
     * @param root the root element
     * @param uuid the uuid
     * @return the corresponding element or null if it does not exist
     */
    public static final Element getElementWithUUID(Element root, String uuid) {
        if (uuid == null || uuid.isEmpty()) {
            return null;
        }
        List<Element> list = ScilabXMLUtilities.getElementsWithAttributeEquals(root, "uuid", uuid);
        if (list.size() != 0) {
            return list.get(0);
        }

        return null;
    }

    /**
     * Remove a node with a given uuid
     * @param parent the parent element
     * @param nodeName the node name
     * @param uuid the uuid
     */
    private static final void removeNode(Element parent, String nodeName, String uuid) {
        if (uuid == null || uuid.isEmpty()) {
            return;
        }
        Element e = getElementWithUUID(parent, nodeName, uuid);
        if (e != null) {
            parent.removeChild(e);
        }
    }

    /**
     * Save the tab properties
     * @param tab the tab
     * @param nullWin if true, the winuuid will be set to 0 (the tab is not docked)
     */
    public static void saveTabProperties(SwingScilabTab tab, boolean nullWin) {
        readDocument();
        ScilabTabFactory factory = ScilabTabFactory.getInstance();
        String uuid = tab.getPersistentId();
        Element root = doc.getDocumentElement();

        String app = factory.getApplication(uuid);
        if (app.isEmpty()) {
            return;
        }

        String winuuid;
        if (nullWin) {
            winuuid = NULLUUID;
        } else {
            winuuid = tab.getParentWindowUUID();
        }

        createNode(root, app, new Object[]{"winuuid", winuuid,
                                           "uuid", uuid,
                                           "load", factory.getPackage(uuid),
                                           "factory", factory.getClassName(uuid)});
        writeDocument();
    }

    /**
     * Make a dependency between two tabs
     * @param parentUUID the parent tab uuid
     * @param childUUID the child tab uuid
     */
    public static final void makeDependency(String parentUUID, String childUUID) {
        readDocument();
        Element e = getElementWithUUID(doc.getDocumentElement(), childUUID);
        if (e != null) {
            e.setAttribute("depends", parentUUID);
        }
        writeDocument();
    }

    /**
     * Remove a dependency with the parent tab
     * @param childUUID the child tab uuid
     */
    public static void removeDependency(String childUUID) {
        readDocument();
        Element e = getElementWithUUID(doc.getDocumentElement(), childUUID);
        if (e != null) {
            e.removeAttribute("depends");
        }
        writeDocument();
    }

    /**
     * Restore an application by its name
     * @param name the application name
     * @return true if the operation succeded
     */
    public static boolean restoreUUID(String uuid) {
        readDocument();
        Element elem = getElementWithUUID(uuid);
        if (elem == null) {
            return false;
        }

        startRestoration(uuid);

        return true;
    }

    /**
     * Get the uuids of an application
     * @param name the application anem
     * @return the corresponding uuids
     */
    public static String[] getApplicationUUIDs(String name) {
        readDocument();
        NodeList list = doc.getDocumentElement().getElementsByTagName(name);
        String[] uuids = new String[getNodeListLength(list)];
        for (int i = 0; i < uuids.length; i++) {
            if (list.item(i) instanceof Element) {
                Element elem = (Element) list.item(i);
                uuids[i] = elem.getAttribute("uuid");
            }
        }

        return uuids;
    }

    /**
     * Get the length of a NodeList (this is a workaround for a f... java bug)
     */
    private static final int getNodeListLength(NodeList list) {
        int length = 0;
        try {
            length = list.getLength();
        } catch (NullPointerException e) { }

        return length;
    }


    /**
     * Inner interface used to have something to execute when the restoration is finished
     */
    public interface EndedRestoration {

        /**
         * Stuff to do when the restoration is ended
         */
        public void finish();
    }
}
