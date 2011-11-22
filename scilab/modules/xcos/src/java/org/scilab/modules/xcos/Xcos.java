/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos;

import static org.scilab.modules.xcos.utils.FileUtils.delete;
import static org.scilab.modules.xcos.utils.FileUtils.exists;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.InvocationTargetException;
import java.net.URL;
import java.util.ArrayDeque;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Vector;
import java.util.logging.LogManager;

import javax.swing.ImageIcon;
import javax.swing.SwingUtilities;
import javax.swing.SwingWorker;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.action_binding.InterpreterManagement;
import org.scilab.modules.graph.utils.ScilabExported;
import org.scilab.modules.gui.bridge.tab.SwingScilabTab;
import org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory;
import org.scilab.modules.gui.tabfactory.ScilabTabFactory;
import org.scilab.modules.gui.utils.ClosingOperationsManager;
import org.scilab.modules.gui.utils.WindowsConfigurationManager;
import org.scilab.modules.localization.Messages;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.SuperBlock;
import org.scilab.modules.xcos.configuration.ConfigurationManager;
import org.scilab.modules.xcos.configuration.model.DocumentType;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.palette.PaletteBlockCtrl;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.PaletteBlock;
import org.scilab.modules.xcos.palette.model.PreLoaded;
import org.scilab.modules.xcos.palette.view.PaletteManagerView;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

import com.mxgraph.model.mxGraphModel;
import com.mxgraph.view.mxStylesheet;

/**
 * Xcos entry point class
 */
// CSOFF: ClassFanOutComplexity
// CSOFF: ClassDataAbstractionCoupling
public final class Xcos extends AbstractScilabTabFactory {
    /**
     * The current Xcos version
     */
    public static final String VERSION = "1.0";
    /**
     * The current Xcos tradename
     */
    public static final String TRADENAME = "Xcos";
    public static final ImageIcon ICON = new ImageIcon(
            System.getenv("SCI")
                    + "/modules/gui/images/icons/32x32/apps/utilities-system-monitor.png");

    private static final String LOAD_XCOS_LIBS_LOAD_SCICOS = "loadXcosLibs(); loadScicos();";

    /*
     * Dependencies version
     */
    private static final List<String> MXGRAPH_VERSIONS = Arrays.asList(
            "1.8.0.0", "1.8.0.1");
    private static final List<String> HDF5_VERSIONS = Arrays.asList(
            "[1, 8, 4]", "[1, 8, 5]", "[1, 8, 6]", "[1, 8, 7]");
    private static final List<String> BATIK_VERSIONS = Arrays.asList("1.7");

    private static final String UNABLE_TO_LOAD_JGRAPHX = Messages
            .gettext("Unable to load the jgraphx library.\nExpecting version %s ; Getting version %s .");
    private static final String UNABLE_TO_LOAD_JHDF5 = Messages
            .gettext("Unable to load the hdf5-java (jhdf5) library. \nExpecting version %s ; Getting version %s .");
    private static final String UNABLE_TO_LOAD_HDF5 = Messages
            .gettext("Unable to load the native HDF5 library.");
    private static final String UNABLE_TO_LOAD_BATIK = Messages
            .gettext("Unable to load the Batik library. \nExpecting version %s ; Getting version %s .");

    private static final String CALLED_OUTSIDE_THE_EDT_THREAD = "Called outside the EDT thread.";
    private static final Log LOG = LogFactory.getLog(Xcos.class);

    /** common shared instance */
    private static volatile Xcos sharedInstance;

    /*
     * Instance data
     */
    private final List<XcosDiagram> diagrams;

    /*
     * Instance handlers
     */
    private final PaletteManager palette;
    private final ConfigurationManager configuration;
    private final mxStylesheet styleSheet;

    /*
     * Cache
     */
    private DocumentType cachedDocumentType;

    /**
     * Construct an Xcos instance.
     * 
     * There must be only one Xcos instance per Scilab application
     */
    private Xcos() {
        /*
         * Read the configuration to support dynamic (before Xcos launch)
         * settings.
         */
        try {
            LogManager.getLogManager().readConfiguration();
        } catch (final SecurityException e) {
            LOG.error(e);
        } catch (final IOException e) {
            LOG.error(e);
        }

        /* Check the dependencies at startup time */
        // checkDependencies();

        /*
         * Allocate synchronized communications data
         */
        diagrams = Collections.synchronizedList(new Vector<XcosDiagram>());

        /*
         * Register as an AbstractScilabTabFactory
         */
        ScilabTabFactory.getInstance().addTabFactory(this);

        /*
         * get the handlers instance
         */
        palette = PaletteManager.getInstance();
        configuration = ConfigurationManager.getInstance();
        styleSheet = new mxStylesheet();

        try {
            FileUtils.decodeStyle(styleSheet);
        } catch (final IOException e) {
            LOG.error(e);
        }

        ScilabTabFactory.getInstance().addTabFactory(this);
    }

    /**
     * Check the dependencies and the version dependencies.
     * 
     * This method use runtime class loading to handle ClassNotFoundException.
     * 
     * This method catch any exception and rethrow it with a well defined
     * message. Thus it doesn't pass the IllegalCatch metrics.
     */
    // CSOFF: IllegalCatch
    // CSOFF: MagicNumber
    private void checkDependencies() {
        final ClassLoader loader = ClassLoader.getSystemClassLoader();

        /* JGraphx */
        String mxGraphVersion = "";
        try {
            final Class<?> klass = loader.loadClass("com.mxgraph.view.mxGraph");
            mxGraphVersion = (String) klass.getDeclaredField("VERSION").get(
                    null);

            if (MXGRAPH_VERSIONS != null
                    && !MXGRAPH_VERSIONS.contains(mxGraphVersion)) {
                throw new Exception();
            }
        } catch (final Throwable e) {
            throw new RuntimeException(String.format(UNABLE_TO_LOAD_JGRAPHX,
                    MXGRAPH_VERSIONS.get(0), mxGraphVersion), e);
        }

        /* HDF5 */
        final int[] libVersion = new int[3];
        try {
            final Class<?> klass = loader.loadClass("ncsa.hdf.hdf5lib.H5");

            /* hdf5-java */
            int ret = (Integer) klass.getMethod("H5get_libversion",
                    libVersion.getClass()).invoke(null, libVersion);
            if (ret < 0) {
                throw new Exception();
            }

            if (!HDF5_VERSIONS.contains(Arrays.toString(libVersion))) {
                throw new Exception();
            }

            /* hdf5 */
            ret = (Integer) klass.getMethod("H5check_version", int.class,
                    int.class, int.class).invoke(null, libVersion[0],
                    libVersion[1], libVersion[2]);
            if (ret < 0) {
                throw new RuntimeException(UNABLE_TO_LOAD_HDF5);
            }

        } catch (final Throwable e) {
            if (!(e instanceof RuntimeException)) {
                throw new RuntimeException(String.format(UNABLE_TO_LOAD_JHDF5,
                        HDF5_VERSIONS.get(0), Arrays.toString(libVersion)), e);
            }
        }

        /* Batik */
        String batikVersion = null;
        try {
            final Class<?> klass = loader.loadClass("org.apache.batik.Version");
            batikVersion = klass.getPackage().getImplementationVersion()
                    .split("\\+")[0];

            if (!BATIK_VERSIONS.contains(batikVersion)) {
                throw new Exception();
            }

        } catch (final Throwable e) {
            throw new RuntimeException(String.format(UNABLE_TO_LOAD_BATIK,
                    BATIK_VERSIONS.get(0), batikVersion), e);
        }
    }

    // CSON: MagicNumber
    // CSON: IllegalCatch

    /**
     * @return the per Scilab application, Xcos instance
     */
    public static synchronized Xcos getInstance() {
        if (sharedInstance == null) {
            sharedInstance = new Xcos();

            /*
             * Lazy loading of HDF5 libraries to avoid first drag lag.
             */
            (new SwingWorker<Void, Void>() {

                @Override
                protected Void doInBackground() throws Exception {
                    try {
                        final Category root = PaletteManager.getInstance()
                                .getRoot();

                        final PaletteBlock b = ((PreLoaded) root.getNode().get(
                                0)).getBlock().get(0);
                        new PaletteBlockCtrl(b).getTransferable();
                    } catch (IndexOutOfBoundsException e) {
                        LOG.debug(e);
                    } catch (ClassCastException e) {
                        LOG.debug(e);
                    }
                    return null;
                }
            }).execute();

            LOG.trace("Session started");
        }

        return sharedInstance;
    }

    /**
     * Clear the shared instance.
     */
    private static synchronized void clearInstance() {
        sharedInstance = null;
        LOG.trace("Session ended");
    }

    /**
     * @return the already opened diagrams
     */
    public List<XcosDiagram> getDiagrams() {
        return diagrams;
    }

    /**
     * @return the global shared styleSheet
     */
    public mxStylesheet getStyleSheet() {
        return styleSheet;
    }

    /**
     * Open a file from it's filename.
     * 
     * This method must be called on the EDT thread. For other use, please use
     * the {@link #xcos(String)} method.
     * 
     * @param filename
     *            the file to open. If null an empty diagram is created.
     */
    public void open(final File filename) {
        if (!SwingUtilities.isEventDispatchThread()) {
            LOG.error(CALLED_OUTSIDE_THE_EDT_THREAD);
        }

        XcosDiagram diag = null;

        if (filename != null) {
            configuration.addToRecentFiles(filename);

            /*
             * looking for an already opened diagram
             */
            for (final XcosDiagram diagram : diagrams) {
                if (diagram.getSavedFile() != null
                        && diagram.getSavedFile().equals(filename)) {
                    diag = diagram;
                    break;
                }
            }
        }

        if (diag == null) {

            /*
             * Allocate and setup a new diagram
             */
            diag = new XcosDiagram();
            diag.installListeners();

            if (filename != null) {
                // wait the end of the load before displaying the tab.
                diag = diag.openDiagramFromFile(filename);
            }

            if (diag != null) {
                diagrams.add(diag);
            }
        }

        /*
         * Create a visible window
         */
        XcosTab.restore(diag);
    }

    /**
     * Close a diagram.
     * 
     * This method must be called on the EDT thread.
     * 
     * @param diagram
     *            the diagram to close
     * @param force
     *            true, if the diagram must always be closed at the return.
     * @return the closing status (true, the diagram has been closed; false,
     *         otherwise)
     */
    public boolean close(final XcosDiagram diagram, final boolean force) {
        if (!SwingUtilities.isEventDispatchThread()) {
            LOG.error(CALLED_OUTSIDE_THE_EDT_THREAD);
        }

        final boolean status = diagram.close(force);

        if (status) {
            diagram.setOpened(false);
            diagrams.remove(diagram);

            if (diagrams.isEmpty()) {
                Xcos.closeSession();
            } else {
                // we must also close the session is no diagram is visible
                for (final XcosDiagram diag : diagrams) {
                    if (XcosTab.get(diag) != null) {
                        return true;
                    }
                }
                Xcos.closeSession();
            }
            return true;
        }
        return false;
    }

    /**
     * Close the current xcos session.
     * 
     * This method must be called on the EDT thread. For other use, please use
     * the {@link #closeXcosFromScilab()} method.
     */
    public static synchronized void closeSession() {
        if (!SwingUtilities.isEventDispatchThread()) {
            LOG.error(CALLED_OUTSIDE_THE_EDT_THREAD);
        }

        /* Doesn't instantiate xcos on close operation */
        if (sharedInstance == null) {
            return;
        }

        final Xcos instance = sharedInstance;
        final List<XcosDiagram> diagrams = instance.diagrams;

        /*
         * We are looping in the inverted order because we have to close latest
         * add diagrams (eg SuperBlockDiagrams) before any others.
         * 
         * Furthermore the closeDiagram operation modify the diagram list. Thus
         * we must *NOT* use i-- there.
         */
        for (int i = diagrams.size() - 1; i >= 0; i = diagrams.size() - 1) {
            instance.close(diagrams.get(i), true);
        }

        if (PaletteManagerView.get() != null) {
            PaletteManagerView.close(null);
        }

        /* terminate any remaining simulation */
        InterpreterManagement.requestScilabExec("haltscicos");

        /* Saving modified data */
        instance.palette.saveConfig();
        instance.configuration.saveConfig();
    }

    /**
     * Debug main function
     * 
     * @param args
     *            command line args (Not used)
     */
    public static void main(final String[] args) {
        xcos();
    }

    /*
     * Scilab exported methods.
     * 
     * All the following methods must use SwingUtilities method to assert that
     * the operations will be called on the EDT thread.
     * 
     * @see modules/xcos/src/jni/Xcos.giws.xml
     * 
     * @see sci_gateway/xcos_gateway.xml
     * 
     * @see modules/xcos/sci_gateway/cpp/sci_*.cpp
     */

    /**
     * Entry point without filename.
     * 
     * This method invoke Xcos operation on the EDT thread.
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void xcos() {
        final Xcos instance = getInstance();

        /* load scicos libraries (macros) */
        InterpreterManagement.requestScilabExec(LOAD_XCOS_LIBS_LOAD_SCICOS);

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                PaletteManager.setVisible(true);
                instance.open(null);
            }
        });
    }

    /**
     * Entry point with filename
     * 
     * This method invoke Xcos operation on the EDT thread.
     * 
     * @param fileName
     *            The filename
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void xcos(final String fileName) {
        final Xcos instance = getInstance();
        final File filename = new File(fileName);

        /* load scicos libraries (macros) */
        InterpreterManagement.requestScilabExec(LOAD_XCOS_LIBS_LOAD_SCICOS);

        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    instance.open(filename);
                }
            });
        } catch (final InterruptedException e) {
            LOG.error(e);
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }
    }

    /**
     * Close the current xcos session from any thread.
     * 
     * This method invoke Xcos operation on the EDT thread. Please prefer using
     * {@link #closeSession()} when the caller is on the EDT thread.
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void closeXcosFromScilab() {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    closeSession();
                    clearInstance();
                }
            });
        } catch (final InterruptedException e) {
            LOG.error(e);
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }
    }

    /**
     * Look in each diagram to find the block corresponding to the given uid and
     * display a warning message.
     * 
     * This method invoke Xcos operation on the EDT thread.
     * 
     * @param uid
     *            A String as UID.
     * @param message
     *            The message to display.
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void warnCellByUID(final String[] uid, final String message) {
        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    final ArrayDeque<String> deque = new ArrayDeque<String>(
                            Arrays.asList(uid));

                    String id;
                    BasicBlock block = null;

                    // specific case with an empty array
                    if (deque.isEmpty()) {
                        return;
                    }

                    // first element
                    id = deque.pop();
                    for (XcosDiagram diagram : getInstance().getDiagrams()) {
                        block = (BasicBlock) ((mxGraphModel) diagram.getModel())
                                .getCell(id);
                        if (block != null) {
                            break;
                        }
                    }

                    // loop to get only the last diagram
                    while (block instanceof SuperBlock && !deque.isEmpty()) {
                        block.getParentDiagram()
                                .warnCellByUID(
                                        block.getId(),
                                        XcosMessages.ERROR_UNABLE_TO_COMPILE_THIS_SUPER_BLOCK);

                        final SuperBlock superBlock = (SuperBlock) block;
                        id = deque.pop();

                        if (!Xcos.getInstance().getDiagrams()
                                .contains(superBlock.getChild())) {
                            block.openBlockSettings(null);
                        }
                        block = (BasicBlock) ((mxGraphModel) superBlock
                                .getChild().getModel()).getCell(id);
                    }

                    // We are unable to find the block with the right id
                    if (block == null) {
                        return;
                    }

                    // finally perform the action on the last block
                    final XcosDiagram parent = block.getParentDiagram();
                    parent.warnCellByUID(block.getId(), message);

                    SwingUtilities.invokeLater(new Runnable() {
                        @Override
                        public void run() {
                            /*
                             * Focus on an existing diagram
                             */
                            XcosTab.restore(parent);
                        }
                    });

                }
            });
        } catch (final InterruptedException e) {
            LOG.error(e);
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }
    }

    /**
     * This function convert a Xcos diagram to Scilab variable.
     * 
     * This method invoke Xcos operation on the EDT thread.
     * 
     * @param xcosFile
     *            The xcos diagram file
     * @param h5File
     *            The target file
     * @param overwrite
     *            Does the file will be overwritten ?
     * @return Not used (compatibility)
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static int xcosDiagramToHDF5(final String xcosFile,
            final String h5File, final boolean overwrite) {
        final File file = new File(xcosFile);

        if (exists(h5File)) {
            if (!overwrite) {
                return 1;
            } else {
                delete(h5File);
            }
        }

        if (!file.exists()) {
            return 1;
        }

        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    final XcosDiagram diagram = new XcosDiagram();
                    diagram.openDiagramFromFile(file);
                    diagram.dumpToHdf5File(h5File);
                }
            });
        } catch (final InterruptedException e) {
            throw new RuntimeException(e);
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }

        return 0;
    }

    /**
     * Open a diagram by uid.
     * 
     * This method invoke Xcos operation on the EDT thread.
     * 
     * @param uid
     *            UID path to a block.
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void xcosDiagramOpen(final String[] uid) {
        final ArrayDeque<String> deque = new ArrayDeque<String>(
                Arrays.asList(uid));

        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    String id;
                    Object block = null;

                    // specific case with an empty array
                    if (deque.isEmpty()) {
                        return;
                    }

                    // first element
                    id = deque.pop();
                    for (XcosDiagram diagram : getInstance().getDiagrams()) {
                        block = ((mxGraphModel) diagram.getModel()).getCell(id);
                        if (block instanceof SuperBlock) {
                            break;
                        }
                    }

                    // loop to get only the last diagram
                    while (block instanceof SuperBlock & !deque.isEmpty()) {
                        final SuperBlock superBlock = (SuperBlock) block;
                        id = deque.pop();

                        superBlock.openBlockSettings(null);
                        block = ((mxGraphModel) superBlock.getChild()
                                .getModel()).getCell(id);
                    }

                    // finally perform the action
                    if (block instanceof SuperBlock) {
                        ((SuperBlock) block).openBlockSettings(null);
                    }
                }
            });
        } catch (final InterruptedException e) {
            LOG.error(e);
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }
    }

    /**
     * Close a diagram by uid.
     * 
     * This method invoke Xcos operation on the EDT thread.
     * 
     * @param uid
     *            The diagram id path
     */
    @ScilabExported(module = "xcos", filename = "Xcos.giws.xml")
    public static void xcosDiagramClose(final String[] uid) {
        final ArrayDeque<String> deque = new ArrayDeque<String>(
                Arrays.asList(uid));

        try {
            SwingUtilities.invokeAndWait(new Runnable() {
                @Override
                public void run() {
                    String id;
                    Object block = null;

                    // specific case with an empty array
                    if (deque.isEmpty()) {
                        closeSession();
                    }

                    // first element
                    id = deque.pop();
                    for (XcosDiagram diagram : getInstance().getDiagrams()) {
                        block = ((mxGraphModel) diagram.getModel()).getCell(id);
                        if (block instanceof SuperBlock) {
                            break;
                        }
                    }

                    // loop to get only the last diagram
                    while (block instanceof SuperBlock & !deque.isEmpty()) {
                        final SuperBlock superBlock = (SuperBlock) block;
                        id = deque.pop();

                        superBlock.openBlockSettings(null);
                        block = ((mxGraphModel) superBlock.getChild()
                                .getModel()).getCell(id);
                    }

                    // finally perform the action
                    if (block instanceof SuperBlock) {
                        ((SuperBlock) block).closeBlockSettings();
                    }
                }
            });
        } catch (final InterruptedException e) {
            LOG.error(e);
        } catch (final InvocationTargetException e) {
            Throwable throwable = e;
            String firstMessage = null;
            while (throwable != null) {
                firstMessage = throwable.getLocalizedMessage();
                throwable = throwable.getCause();
            }

            throw new RuntimeException(firstMessage, e);
        }
    }

    /**
     * Look for the parent diagram of the cell in the diagram hierarchy.
     * 
     * @param cell
     *            the cell to search for
     * @return the associated diagram
     */
    public static XcosDiagram findParent(Object cell) {
        final Xcos instance = getInstance();
        for (XcosDiagram diag : instance.getDiagrams()) {
            if (diag.getModel().contains(cell)) {
                if (cell instanceof BasicBlock) {
                    ((BasicBlock) cell).setParentDiagram(diag);
                }
                return diag;
            }
        }
        return null;
    }

    /*
     * @see org.scilab.modules.gui.tabfactory.AbstractScilabTabFactory
     */

    /**
     * Create/restore a tab for a given uuid
     * 
     * @param uuid
     *            the specific uuid
     * @return the tab instance
     */
    @Override
    public synchronized SwingScilabTab getTab(String uuid) {
        if (uuid == null) {
            return null;
        }

        SwingScilabTab tab = ScilabTabFactory.getInstance().getFromCache(uuid);

        // Palette manager restore
        if (tab == null) {

            if (PaletteManagerView.UUID.equals(uuid)) {
                PaletteManagerView.restore(null);
                tab = PaletteManagerView.get();
            }
        }

        // diagram (tab or viewport) restore
        if (tab == null) {
            cache(uuid);
            if (cachedDocumentType == null) {
                return null;
            }

            final XcosDiagram graph = loadDiagram();
            if (uuid.equals(cachedDocumentType.getUuid())) {
                XcosTab.restore(graph);
                tab = XcosTab.get(graph);

                ClosingOperationsManager.addDependencyWithRoot(tab);
            } else if (uuid.equals(cachedDocumentType.getViewport())) {
                ViewPortTab.restore(graph);
                tab = ViewPortTab.get(graph);

                ClosingOperationsManager.addDependency(
                        (SwingScilabTab) XcosTab.get(graph), tab);
                WindowsConfigurationManager.makeDependency(
                        graph.getDiagramTab(), tab.getPersistentId());
            } else {
                return null;
            }
        }

        if (uuid != null) {
            WindowsConfigurationManager.restorationFinished(tab);
        }
        ScilabTabFactory.getInstance().addToCache(tab);

        return tab;
    }

    /**
     * Load the diagram according to the cachedDocumentType
     * 
     * @return the loaded diagram or null on error
     */
    private XcosDiagram loadDiagram() {
        final URL url;
        final File f;
        try {
            url = new URL(cachedDocumentType.getUrl());
            f = new File(url.toURI());
        } catch (Exception e) {
            return null;
        }

        XcosDiagram graph = new XcosDiagram();
        graph = graph.openDiagramFromFile(f);

        if (cachedDocumentType.getPath() != null) {
            final String[] path = cachedDocumentType.getPath().split("/");
            for (String id : path) {
                final Object cell = ((mxGraphModel) graph.getModel())
                        .getCell(id);

                if (cell instanceof SuperBlock) {
                    SuperBlock b = (SuperBlock) cell;

                    b.openBlockSettings(null);
                    graph = b.getChild();
                }
            }

        }

        return graph;
    }

    @Override
    public synchronized boolean isAValidUUID(String uuid) {
        // check the Palette manager view (static uuid)
        if (PaletteManagerView.UUID.equals(uuid)) {
            return true;
        }

        /*
         * Cache and check against cache to ease next getTab(uuid) call
         */
        cache(uuid);
        return cachedDocumentType != null;
    }

    /**
     * Cache the {@link DocumentType} for the specific uuid
     * 
     * @param uuid
     *            the uuid
     */
    private void cache(String uuid) {
        /*
         * Handle a non null cache
         */
        if (cachedDocumentType != null) {
            final boolean isTab = uuid.equals(cachedDocumentType.getUuid());
            final boolean isViewport = uuid.equals(cachedDocumentType
                    .getViewport());

            if (isTab || isViewport) {
                return;
            } else {
                cachedDocumentType = null;
            }
        }

        /*
         * Invalid cache, look for the right one
         */
        final ConfigurationManager config = ConfigurationManager.getInstance();
        final List<DocumentType> docs = config.getSettings().getTab();
        for (DocumentType d : docs) {
            final boolean isTab = uuid.equals(d.getUuid());
            final boolean isViewport = uuid.equals(d.getViewport());

            if (isTab || isViewport) {
                cachedDocumentType = d;
                break;
            }
        }
    }

    @Override
    public String getPackage() {
        return TRADENAME;
    }

    @Override
    public String getClassName() {
        return Xcos.class.getName();
    }

    @Override
    public String getApplication() {
        return TRADENAME;
    }
}
// CSON: ClassDataAbstractionCoupling
// CSON: ClassFanOutComplexity
