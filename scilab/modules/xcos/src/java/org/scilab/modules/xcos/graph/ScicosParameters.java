/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clement DAVID
 * Copyright (C) 2011 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos.graph;

import java.beans.PropertyChangeListener;
import java.beans.PropertyChangeSupport;
import java.beans.PropertyVetoException;
import java.beans.VetoableChangeListener;
import java.beans.VetoableChangeSupport;
import java.io.Serializable;

import org.scilab.modules.xcos.Controller;
import org.scilab.modules.xcos.Kind;
import org.scilab.modules.xcos.ObjectProperties;
import org.scilab.modules.xcos.preferences.XcosOptions;
import org.scilab.modules.xcos.VectorOfDouble;
import org.scilab.modules.xcos.VectorOfString;

/**
 * Contains Scicos specific parameters.
 *
 * This class is a java beans and follow standard properties accessors.
 *
 * @see <a
 *      href="http://java.sun.com/docs/books/tutorial/javabeans/properties/bound.html">JavaBeans
 *      Bound Properties</a>
 */
@SuppressWarnings(value = { "serial" })
public class ScicosParameters implements Serializable, Cloneable {
    /*
     * Default instance values from :
     * SCI/modules/scicos/macros/scicos_scicos/scicos_params.sci
     *
     * These values are preferences and thus can be updated by user settings.
     */

    /**
     * The related diagram id.
     */
    private long id;

    /**
     * The default level of information display.
     */
    public static int DEBUG_LEVEL = 0;

    /*
     * Bean properties
     */
    /**
     * Property bound to finalIntegrationTime modification.
     */
    public static final String FINAL_INTEGRATION_TIME_CHANGE = "finalIntegrationTime";
    /**
     * Property bound to integratorAbsoluteTolerance modification.
     */
    public static final String INTEGRATOR_ABSOLUTE_TOLERANCE_CHANGE = "integratorAbsoluteTolerance";
    /**
     * Property bound to integratorRelativeTolerance modification.
     */
    public static final String INTEGRATOR_RELATIVE_TOLERANCE_CHANGE = "integratorRelativeTolerance";
    /**
     * Property bound to toleranceOnTime modification.
     */
    public static final String TOLERANCE_ON_TIME_CHANGE = "toleranceOnTime";
    /**
     * Property bound to maxIntegrationTimeInterval modification.
     */
    public static final String MAX_INTEGRATION_TIME_INTERVAL_CHANGE = "maxIntegrationTimeInterval";
    /**
     * Property bound to realTimeScaling modification.
     */
    public static final String REAL_TIME_SCALING_CHANGE = "realTimeScaling";
    /**
     * Property bound to solver modification.
     */
    public static final String SOLVER_CHANGE = "solver";
    /**
     * Property bound to maximumStepSize modification.
     */
    public static final String MAXIMUM_STEP_SIZE_CHANGE = "maximumStepSize";
    /**
     * Property bound to debugLevel modification.
     */
    public static final String DEBUG_LEVEL_CHANGE = "debugLevel";
    /**
     * Property bound to context modification.
     */
    public static final String CONTEXT_CHANGE = "context";

    /**
     * Instance data
     */
    private int debugLevel;

    /*
     * Beans support, used to follow instance modification and validate changes.
     */
    private final PropertyChangeSupport pcs = new PropertyChangeSupport(this);
    private final VetoableChangeSupport vcs = new VetoableChangeSupport(this);

    /**
     * Default constructor
     *
     * Initialize parameters with their default values.
     */
    public ScicosParameters(long id) {
        /*
         * This call will update static values from the configuration.
         */
        XcosOptions.getSimulation();

        debugLevel = DEBUG_LEVEL;
    }

    /**
     * @return integration time
     */
    public double getFinalIntegrationTime() {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        return props.get(0);
    }

    /**
     * @param finalIntegrationTime
     *            set integration time
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setFinalIntegrationTime(double finalIntegrationTime) throws PropertyVetoException {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        double oldValue = props.get(0);

        vcs.fireVetoableChange(FINAL_INTEGRATION_TIME_CHANGE, oldValue, finalIntegrationTime);
        props.set(0, finalIntegrationTime);
        controller.setObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        pcs.firePropertyChange(FINAL_INTEGRATION_TIME_CHANGE, oldValue, finalIntegrationTime);
    }

    /**
     * @return integrator absolute tolerance
     */
    public double getIntegratorAbsoluteTolerance() {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        return props.get(1);
    }

    /**
     * @param integratorAbsoluteTolerance
     *            set integrator absolute tolerance
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setIntegratorAbsoluteTolerance(double integratorAbsoluteTolerance) throws PropertyVetoException {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        double oldValue = props.get(1);

        vcs.fireVetoableChange(INTEGRATOR_ABSOLUTE_TOLERANCE_CHANGE, oldValue, integratorAbsoluteTolerance);
        props.set(1, integratorAbsoluteTolerance);
        controller.setObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        pcs.firePropertyChange(INTEGRATOR_ABSOLUTE_TOLERANCE_CHANGE, oldValue, integratorAbsoluteTolerance);
    }

    /**
     * @return integrator relative tolerance
     */
    public double getIntegratorRelativeTolerance() {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        return props.get(2);
    }

    /**
     * @param integratorRelativeTolerance
     *            integrator relative tolerance
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setIntegratorRelativeTolerance(double integratorRelativeTolerance) throws PropertyVetoException {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        double oldValue = props.get(2);

        vcs.fireVetoableChange(INTEGRATOR_RELATIVE_TOLERANCE_CHANGE, oldValue, integratorRelativeTolerance);
        props.set(2, integratorRelativeTolerance);
        controller.setObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        pcs.firePropertyChange(INTEGRATOR_RELATIVE_TOLERANCE_CHANGE, oldValue, integratorRelativeTolerance);
    }

    /**
     * @return max step size
     */
    public double getMaximumStepSize() {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        return props.get(5);
    }

    /**
     * @param maximumStepSize
     *            set max step size
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setMaximumStepSize(double maximumStepSize) throws PropertyVetoException {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        double oldValue = props.get(5);

        vcs.fireVetoableChange(MAXIMUM_STEP_SIZE_CHANGE, oldValue, maximumStepSize);
        props.set(5, maximumStepSize);
        controller.setObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        pcs.firePropertyChange(MAXIMUM_STEP_SIZE_CHANGE, oldValue, maximumStepSize);
    }

    /**
     * @return max integration time
     */
    public double getMaxIntegrationTimeInterval() {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        return props.get(4);
    }

    /**
     * @param maxIntegrationTimeinterval
     *            set max integration time
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setMaxIntegrationTimeInterval(double maxIntegrationTimeinterval) throws PropertyVetoException {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        double oldValue = props.get(4);

        vcs.fireVetoableChange(MAX_INTEGRATION_TIME_INTERVAL_CHANGE, oldValue, maxIntegrationTimeinterval);
        props.set(4, maxIntegrationTimeinterval);
        controller.setObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        pcs.firePropertyChange(MAX_INTEGRATION_TIME_INTERVAL_CHANGE, oldValue, maxIntegrationTimeinterval);
    }

    /**
     * @return real time scaling
     */
    public double getRealTimeScaling() {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        return props.get(6);
    }

    /**
     * @param realTimeScaling
     *            set real time scaling
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setRealTimeScaling(double realTimeScaling) throws PropertyVetoException {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        double oldValue = props.get(6);

        vcs.fireVetoableChange(REAL_TIME_SCALING_CHANGE, oldValue, realTimeScaling);
        props.set(6, realTimeScaling);
        controller.setObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        pcs.firePropertyChange(REAL_TIME_SCALING_CHANGE, oldValue, realTimeScaling);
    }

    /**
     * <ul>
     * <li>0 : LSodar : Method: DYNAMIC, Nonlinear solver= DYNAMIC
     * <li>1 : Sundials/CVODE : Method: BDF, Nonlinear solver= FUNCTIONAL
     * <li>2 : Sundials/CVODE : Method: BDF, Nonlinear solver= FUNCTIONAL
     * <li>3 : Sundials/CVODE : Method: ADAMS, Nonlinear solver= NEWTON
     * <li>4 : Sundials/CVODE : Method: ADAMS, Nonlinear solver= FUNCTIONAL
     * <li>5 : DOPRI5 : Method: Dormand-Prince 4(5)
     * <li>6 : RK45 : Method: Runge-Kutta 4(5)
     * <li>7 : Implicit RK45 : Method: Runge-Kutta 4(5), Nonlinear solver= Fixed-point
     * <li>100 : Sundials/IDA : Method: BDF, Nonlinear solver= NEWTON
     * <li>101 : DDaskr : Method: BDF, Nonlinear solver= NEWTON
     * <li>102 : DDaskr : Method: BDF, Nonlinear solver= GMRES
     *
     *
     * @return solver value
     */
    public double getSolver() {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        return props.get(7);
    }

    /**
     * <ul>
     * <li>0 : LSodar : Method: DYNAMIC, Nonlinear solver= DYNAMIC
     * <li>1 : Sundials/CVODE : Method: BDF, Nonlinear solver= FUNCTIONAL
     * <li>2 : Sundials/CVODE : Method: BDF, Nonlinear solver= FUNCTIONAL
     * <li>3 : Sundials/CVODE : Method: ADAMS, Nonlinear solver= NEWTON
     * <li>4 : Sundials/CVODE : Method: ADAMS, Nonlinear solver= FUNCTIONAL
     * <li>5 : DOPRI5 : Method: Dormand-Prince 4(5)
     * <li>6 : RK45 : Method: Runge-Kutta 4(5)
     * <li>7 : Implicit RK45 : Method: Runge-Kutta 4(5), Nonlinear solver= FIXED-POINT
     * <li>100 : Sundials/IDA : Method: BDF, Nonlinear solver= NEWTON
     * <li>101 : DDaskr : Method: BDF, Nonlinear solver= NEWTON
     * <li>102 : DDaskr : Method: BDF, Nonlinear solver= GMRES
     *
     *
     * @param solver
     *            set solver
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setSolver(double solver) throws PropertyVetoException {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        double oldValue = props.get(7);

        vcs.fireVetoableChange(SOLVER_CHANGE, oldValue, solver);
        props.set(7, solver);
        controller.setObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        pcs.firePropertyChange(SOLVER_CHANGE, oldValue, solver);
    }

    /**
     * @return tolerance time
     */
    public double getToleranceOnTime() {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        return props.get(3);
    }

    /**
     * @param toleranceOnTime
     *            set tolerance time
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setToleranceOnTime(double toleranceOnTime) throws PropertyVetoException {
        final Controller controller = new Controller();
        VectorOfDouble props = new VectorOfDouble(8);
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        double oldValue = props.get(3);

        vcs.fireVetoableChange(TOLERANCE_ON_TIME_CHANGE, oldValue, toleranceOnTime);
        props.set(3, toleranceOnTime);
        controller.setObjectProperty(id, Kind.DIAGRAM, ObjectProperties.PROPERTIES, props);
        pcs.firePropertyChange(TOLERANCE_ON_TIME_CHANGE, oldValue, toleranceOnTime);
    }

    /**
     * Set the associated context if there is noticeable changes.
     *
     * @param context
     *            set context
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setContext(String[] context) throws PropertyVetoException {
        final Controller controller = new Controller();
        VectorOfString oldContext = new VectorOfString();
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.DIAGRAM_CONTEXT, oldContext);
        if (context == null) {
            throw new IllegalArgumentException("context must not be null");
        }
        String[] oldValue = new String[(int) oldContext.size()];
        for (int i = 0; i < oldContext.size(); ++i) {
            oldValue[i] = oldContext.get(i);
        }

        /*
         * Check for modification
         */
        boolean modified = false;

        if (oldValue.length != context.length) {
            modified = true;
        } else {
            // Same length so compare line per line
            for (int i = 0; i < oldValue.length; i++) {
                String indexedOld = oldValue[i];
                String indexedNew = context[i];

                if (!indexedOld.equals(indexedNew)) {
                    modified = true;
                    break;
                }
            }
        }

        /*
         * Apply context if modified
         */
        if (modified) {
            vcs.fireVetoableChange(CONTEXT_CHANGE, oldValue, context);
            VectorOfString newContext = new VectorOfString();
            for (int i = 0; i < context.length; ++i) {
                newContext.set(i, context[i]);
            }
            controller.setObjectProperty(id, Kind.DIAGRAM, ObjectProperties.DIAGRAM_CONTEXT, newContext);
            pcs.firePropertyChange(CONTEXT_CHANGE, oldValue, context);
        }
    }

    /**
     * @return current context
     */
    public String[] getContext() {
        final Controller controller = new Controller();
        VectorOfString Context = new VectorOfString();
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.DIAGRAM_CONTEXT, Context);

        String[] context = new String[(int) Context.size()];
        for (int i = 0; i < Context.size(); ++i) {
            context[i] = Context.get(i);
        }
        return context;
    }

    /**
     * @return current version
     */
    public String getVersion() {
        final Controller controller = new Controller();
        VectorOfString version = new VectorOfString();
        controller.getObjectProperty(id, Kind.DIAGRAM, ObjectProperties.VERSION_NUMBER, version);

        return version.get(0);
    }

    /**
     * @return current debug level
     */
    public int getDebugLevel() {
        return debugLevel;
    }

    /**
     * @param debugLevel
     *            change debug level
     * @throws PropertyVetoException
     *             when the value is not acceptable.
     */
    public void setDebugLevel(int debugLevel) throws PropertyVetoException {
        int oldValue = this.debugLevel;
        vcs.fireVetoableChange(DEBUG_LEVEL_CHANGE, oldValue, debugLevel);
        this.debugLevel = debugLevel;
        pcs.firePropertyChange(DEBUG_LEVEL_CHANGE, oldValue, debugLevel);
    }

    /*
     * PropertyChangeSupport proxy methods
     */

    /**
     * Each setXXX method fire a property change event. This method register a
     * new listener for all events.
     *
     * @param listener
     *            A listener
     */
    public void addPropertyChangeListener(PropertyChangeListener listener) {
        this.pcs.addPropertyChangeListener(listener);
    }

    /**
     * Each setXXX method fire a property change event. This method register a
     * new listener for a specific event. Each event name is equal to the field
     * name.
     *
     * @param propertyName
     *            the property name
     * @param listener
     *            A listener
     */
    public void addPropertyChangeListener(String propertyName, PropertyChangeListener listener) {
        this.pcs.addPropertyChangeListener(propertyName, listener);
    }

    /**
     * Each setXXX method fire a property change event. This method remove a
     * listener for all events.
     *
     * @param listener
     *            A listener
     */
    public void removePropertyChangeListener(PropertyChangeListener listener) {
        this.pcs.removePropertyChangeListener(listener);
    }

    /**
     * Each setXXX method fire a property change event. This method remove a
     * listener for a specific event. Each event name is equal to the field
     * name.
     *
     * @param propertyName
     *            the property name
     * @param listener
     *            A listener
     */
    public void removePropertyChangeListener(String propertyName, PropertyChangeListener listener) {
        this.pcs.removePropertyChangeListener(propertyName, listener);
    }

    /*
     * VetoableChangeSupport proxy methods
     */

    /**
     * Each setXXX method fire a vetoable change event. This method register a
     * new listener for all events.
     *
     * @param listener
     *            A listener
     */
    public void addVetoableChangeListener(VetoableChangeListener listener) {
        this.vcs.addVetoableChangeListener(listener);
    }

    /**
     * Each setXXX method fire a vetoable change event. This method register a
     * new listener for a specific event. Each event name is equal to the field
     * name.
     *
     * @param propertyName
     *            the property name
     * @param listener
     *            A listener
     */
    public void addVetoableChangeListener(String propertyName, VetoableChangeListener listener) {
        this.vcs.addVetoableChangeListener(propertyName, listener);
    }

    /**
     * Each setXXX method fire a vetoable change event. This method remove a
     * listener for all events.
     *
     * @param listener
     *            A listener
     */
    public void removeVetoableChangeListener(VetoableChangeListener listener) {
        this.vcs.removeVetoableChangeListener(listener);
    }

    /**
     * Each setXXX method fire a vetoable change event. This method remove a
     * listener for a specific event. Each event name is equal to the field
     * name.
     *
     * @param propertyName
     *            the property name
     * @param listener
     *            A listener
     */
    public void removeVetoableChangeListener(String propertyName, VetoableChangeListener listener) {
        this.vcs.removeVetoableChangeListener(propertyName, listener);
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Object clone() throws CloneNotSupportedException {
        return super.clone();
    }
}
