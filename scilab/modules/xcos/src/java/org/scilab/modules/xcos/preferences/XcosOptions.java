/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.preferences;

import java.awt.Color;

import org.scilab.modules.commons.xml.XConfiguration;
import org.scilab.modules.commons.xml.XConfiguration.XConfAttribute;
import org.scilab.modules.xcos.graph.ScicosParameters;
import org.scilab.modules.xcos.io.XcosFileType;

/**
 * Xcos options
 */
public class XcosOptions {
    protected static final String PREFERENCES_XPATH = "//xcos/body/xcos-preferences";
    protected static final String EDITION_XPATH = "//xcos/edition/body/xcos-edition";
    protected static final String SIMULATION_XPATH = "//xcos/simulation/body/xcos-simulation";

    private static XcosOptions.Preferences preferences;
    private static XcosOptions.Edition edition;
    private static XcosOptions.Simulation simulation;

    public static class Preferences {
        private int numberOfRecentlyOpen = 5;
        private XcosFileType fileFormat = XcosFileType.ZCOS;

        private Preferences() {
        }

        /*
         * Preference accessors
         */

        public void setNumberOfRecentlyOpen(int numberOfRecentlyOpen) {
            this.numberOfRecentlyOpen = numberOfRecentlyOpen;
        }

        public void setFileFormat(final int fileFormat) {
            this.fileFormat = XcosFileType.values()[fileFormat];
        }

        /*
         * Implementation accessors
         */

        public int getNumberOfRecentlyOpen() {
            return numberOfRecentlyOpen;
        }

        public XcosFileType getFileFormat() {
            return fileFormat;
        }
    }

    public static class Edition {
        private String edgeStyle = "";
        private Color graphBackground;

        private boolean graphGridEnable = true;
        private int graphGrid = 10;

        private Edition() {
        }

        /*
         * Preference accessors
         */

        public void setEdgeStyle(final int edgeStyle) {
            switch (edgeStyle) {
                case 0: // straight
                    this.edgeStyle = "";
                    break;
                case 1: // horizontal
                    this.edgeStyle = ";noEdgeStyle=0;edgeStyle=elbowEdgeStyle;elbow=horizontal";
                    break;
                case 2: // vertical
                    this.edgeStyle = ";noEdgeStyle=0;edgeStyle=elbowEdgeStyle;elbow=vertical";
                    break;
            }
        }

        public void setGraphBackground(Color graphBackground) {
            this.graphBackground = graphBackground;
        }

        public void setGraphGridEnable(boolean graphGridEnable) {
            this.graphGridEnable = graphGridEnable;
        }

        public void setGraphGrid(int graphGrid) {
            this.graphGrid = graphGrid;
        }

        /*
         * Implementation accessors
         */

        public String getEdgeStyle() {
            return edgeStyle;
        }

        public Color getGraphBackground() {
            return graphBackground;
        }

        public boolean isGraphGridEnable() {
            return graphGridEnable;
        }

        public int getGraphGrid() {
            return graphGrid;
        }
    }

    @XConfAttribute
    public static class Simulation {

        private Simulation() {
        }

        @XConfAttribute(tag = "xcos-simulation", attributes = { "final-integration-time", "integrator-absolute-tolerance", "integrator-relative-tolerance",
                                                                "tolerance-on-time", "max-integration-interval", "real-time-scaling", "solver", "max-step-size", "trace"
                                                              })
        private void set(final String finalIntegrationTime, final String integratorAbsoluteTolerance, final String integratorRelativeTolerance,
                         final String toleranceOnTime, final String maxIntegrationTimeInterval, final String realTimeScaling, final int solver,
                         final String maximumStepSize, final int debugLevel) {

            ScicosParameters.FINAL_INTEGRATION_TIME = Double.parseDouble(finalIntegrationTime);
            ScicosParameters.INTEGRATOR_ABSOLUTE_TOLERANCE = Double.parseDouble(integratorAbsoluteTolerance);
            ScicosParameters.INTEGRATOR_RELATIVE_TOLERANCE = Double.parseDouble(integratorRelativeTolerance);
            ScicosParameters.TOLERANCE_ON_TIME = Double.parseDouble(toleranceOnTime);
            ScicosParameters.MAX_INTEGRATION_TIME_INTERVAL = Double.parseDouble(maxIntegrationTimeInterval);
            ScicosParameters.REAL_TIME_SCALING = Double.parseDouble(realTimeScaling);
            ScicosParameters.SOLVER = solver;
            ScicosParameters.MAXIMUM_STEP_SIZE = Double.parseDouble(maximumStepSize);
            ScicosParameters.DEBUG_LEVEL = debugLevel;
        }
    }

    public static void invalidate(XcosConfiguration.Options conf) {
        if (conf.preferences) {
            preferences = null;
        }
        if (conf.edition) {
            edition = null;
        }
        if (conf.simulation) {
            simulation = null;
        }
    }

    public static final XcosOptions.Preferences getPreferences() {
        if (preferences == null) {
            preferences = XConfiguration.get(XcosOptions.Preferences.class, XConfiguration.getXConfigurationDocument(), PREFERENCES_XPATH)[0];
        }

        return preferences;
    }

    public static final XcosOptions.Edition getEdition() {
        if (edition == null) {
            edition = XConfiguration.get(XcosOptions.Edition.class, XConfiguration.getXConfigurationDocument(), EDITION_XPATH)[0];
        }

        return edition;
    }

    public static final XcosOptions.Simulation getSimulation() {
        if (simulation == null) {
            simulation = XConfiguration.get(XcosOptions.Simulation.class, XConfiguration.getXConfigurationDocument(), SIMULATION_XPATH)[0];
        }

        return simulation;
    }
}
