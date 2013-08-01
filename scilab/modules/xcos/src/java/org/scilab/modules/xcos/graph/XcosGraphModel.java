/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
package org.scilab.modules.xcos.graph;

import java.util.Arrays;
import java.util.Deque;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

import org.scilab.modules.types.ScilabType;
import org.scilab.modules.xcos.Xcos;
import org.scilab.modules.xcos.block.BasicBlock;
import org.scilab.modules.xcos.block.BasicBlock.SimulationFunctionType;
import org.scilab.modules.xcos.port.command.CommandPort;
import org.scilab.modules.xcos.port.control.ControlPort;
import org.scilab.modules.xcos.port.input.InputPort;
import org.scilab.modules.xcos.port.output.OutputPort;

import com.mxgraph.model.mxGeometry;
import com.mxgraph.model.mxGraphModel;
import com.mxgraph.model.mxICell;

/**
 * A model is used by any listener to perform undoable modification of the graph
 * content.
 *
 * All handled changes are defined as static inner classes and named methods on
 * this class. Setters on each model class ({@link BasicBlock},
 * {@link BasicLink}, etc...) should not be used anymore.
 */
public class XcosGraphModel extends mxGraphModel {

    /**
     * Block properties to track
     */
    private static class BlockProperties {
        private final String interfaceFunctionName;
        private final String simulationFunctionName;
        private final SimulationFunctionType simulationFunctionType;

        private final ScilabType exprs;
        private final ScilabType realParameters;
        private final ScilabType integerParameters;
        private final ScilabType objectsParameters;

        private final ScilabType nbZerosCrossing;
        private final ScilabType nmode;

        private final ScilabType state;
        private final ScilabType dState;
        private final ScilabType oDState;

        private final ScilabType equations;

        private final boolean dependsOnU;
        private final boolean dependsOnT;
        private final String blockType;

        public BlockProperties(String interfaceFunctionName, String simulationFunctionName, SimulationFunctionType simulationFunctionType, ScilabType exprs,
                               ScilabType realParameters, ScilabType integerParameters, ScilabType objectsParameters, ScilabType nbZerosCrossing, ScilabType nmode,
                               ScilabType state, ScilabType dState, ScilabType oDState, ScilabType equations, boolean dependsOnU, boolean dependsOnT, String blockType) {
            super();
            this.interfaceFunctionName = interfaceFunctionName;
            this.simulationFunctionName = simulationFunctionName;
            this.simulationFunctionType = simulationFunctionType;
            this.exprs = exprs;
            this.realParameters = realParameters;
            this.integerParameters = integerParameters;
            this.objectsParameters = objectsParameters;
            this.nbZerosCrossing = nbZerosCrossing;
            this.nmode = nmode;
            this.state = state;
            this.dState = dState;
            this.oDState = oDState;
            this.equations = equations;
            this.dependsOnU = dependsOnU;
            this.dependsOnT = dependsOnT;
            this.blockType = blockType;
        }

        public BlockProperties(final BasicBlock value) {
            this.interfaceFunctionName = value.getInterfaceFunctionName();
            this.simulationFunctionName = value.getSimulationFunctionName();
            this.simulationFunctionType = value.getSimulationFunctionType();
            this.exprs = value.getExprs();
            this.realParameters = value.getRealParameters();
            this.integerParameters = value.getIntegerParameters();
            this.objectsParameters = value.getObjectsParameters();
            this.nbZerosCrossing = value.getNbZerosCrossing();
            this.nmode = value.getNmode();
            this.state = value.getState();
            this.dState = value.getDState();
            this.oDState = value.getODState();
            this.equations = value.getEquations();
            this.dependsOnU = value.isDependsOnU();
            this.dependsOnT = value.isDependsOnT();
            this.blockType = value.getBlockType();
        }
    }

    /**
     * {@inheritDoc}
     */
    public XcosGraphModel() {
        super();
    }


    public void setBlockValues(final BasicBlock cell, final BasicBlock value) {
        if (value == null) {
            return;
        }

        execute(new BlockPropertiesChange(this, cell, value));
    }

    /**
     * Inner callback to update the block property of the given {@link BasicBlock} using block setters and return the previous value
     * @param cell the block to work on
     * @param value the block properties to set
     * @return the previous block properties
     */
    private final BlockProperties blockPropertiesForCellChanged(BasicBlock cell, BlockProperties value) {

        final BlockProperties oldValue = new BlockProperties(cell.getInterfaceFunctionName(), cell.getSimulationFunctionName(), cell.getSimulationFunctionType(), cell.getExprs(), cell.getRealParameters(), cell.getIntegerParameters(), cell.getObjectsParameters(), cell.getNbZerosCrossing(), cell.getNmode(), cell.getState(), cell.getDState(), cell.getODState(), cell.getEquations(), cell.isDependsOnU(), cell.isDependsOnT(), cell.getBlockType());

        cell.setInterfaceFunctionName(value.interfaceFunctionName);
        cell.setSimulationFunctionName(value.simulationFunctionName);
        cell.setSimulationFunctionType(value.simulationFunctionType);
        cell.setExprs(value.exprs);
        cell.setRealParameters(value.realParameters);
        cell.setIntegerParameters(value.integerParameters);
        cell.setObjectsParameters(value.objectsParameters);
        cell.setNbZerosCrossing(value.nbZerosCrossing);
        cell.setNmode(value.nmode);
        cell.setState(value.state);
        cell.setDState(value.dState);
        cell.setODState(value.oDState);
        cell.setEquations(value.equations);
        cell.setDependsOnU(value.dependsOnU);
        cell.setDependsOnT(value.dependsOnT);
        cell.setBlockType(value.blockType);

        return oldValue;
    }

    public static class BlockPropertiesChange extends mxAtomicGraphModelChange {
        private final XcosGraphModel model;
        private final BasicBlock cell;

        private BlockProperties value;
        private BlockProperties previous;

        public BlockPropertiesChange(XcosGraphModel model, BasicBlock cell, String interfaceFunctionName, String simulationFunctionName,
                                     SimulationFunctionType simulationFunctionType, ScilabType exprs, ScilabType realParameters, ScilabType integerParameters,
                                     ScilabType objectsParameters, ScilabType nbZerosCrossing, ScilabType nmode, ScilabType state, ScilabType dState, ScilabType oDState,
                                     ScilabType equations, boolean dependsOnU, boolean dependsOnT, String blockType) {
            super();

            this.model = model;
            this.cell = cell;

            value = new BlockProperties(interfaceFunctionName, simulationFunctionName, simulationFunctionType, exprs, realParameters, integerParameters, objectsParameters, nbZerosCrossing, nmode, state, dState, oDState, equations, dependsOnU, dependsOnT, blockType);
            previous = value;
        }

        public BlockPropertiesChange(XcosGraphModel model, BasicBlock cell, BasicBlock value) {
            super();

            this.model = model;
            this.cell = cell;

            this.value = new BlockProperties(value);
            previous = this.value;
        }

        @Override
        public void execute() {
            value = previous;
            previous = model.blockPropertiesForCellChanged(cell, previous);
        }

    }
}
