package org.scilab.modules.xcos.actions.workers;

import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.buildCall;
import static org.scilab.modules.action_binding.highlevel.ScilabInterpreterManagement.synchronousScilabExec;

import java.beans.PropertyChangeEvent;
import java.beans.PropertyChangeListener;
import java.io.IOException;
import java.util.List;
import java.util.concurrent.ExecutionException;

import javax.lang.model.type.NullType;
import javax.swing.SwingWorker;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.graph.actions.base.GraphActionManager;
import org.scilab.modules.xcos.actions.StartAction;
import org.scilab.modules.xcos.actions.StopAction;
import org.scilab.modules.xcos.graph.XcosDiagram;
import org.scilab.modules.xcos.utils.FileUtils;
import org.scilab.modules.xcos.utils.XcosMessages;

public class ScilabGraphWorker extends SwingWorker<NullType, String> {
	private final XcosDiagram graph;
	private final Action action;

	private static ScilabGraphWorker currentWorker;

	/**
	 * Available action mode of the worker
	 */
	public static enum Action {
		DUMP, COMPILE, SIMULATE
	}

	public static synchronized void start(final XcosDiagram graph, Action action) {
		if (currentWorker == null) {
			currentWorker = new ScilabGraphWorker(graph, action);
			currentWorker.execute();
		} else {
			throw new IllegalStateException();
		}
	}

	private static synchronized void clearState() {
		currentWorker = null;
	}

	public static synchronized void stop() {
		if (currentWorker != null) {
			currentWorker.cancel(true);
			currentWorker = null;
		} else {
			throw new IllegalStateException();
		}
	}

	public ScilabGraphWorker(final XcosDiagram graph, Action action) {
		super();
		this.graph = graph;
		this.action = action;

		addPropertyChangeListener(new PropertyChangeListener() {

			@Override
			public void propertyChange(PropertyChangeEvent evt) {
				if ("state".equals(evt.getPropertyName())) {
					switch (getState()) {
					case PENDING:
					case STARTED:
						graph.setReadOnly(true);

						GraphActionManager.setEnable(StartAction.class, false);
						GraphActionManager.setEnable(StopAction.class, true);
						break;

					case DONE:
						clearState();
						graph.setReadOnly(false);

						GraphActionManager.setEnable(StartAction.class, true);
						GraphActionManager.setEnable(StopAction.class, false);
						break;

					default:
						break;
					}
				}

			}
		});
	}

	@Override
	protected NullType doInBackground() throws Exception {
		publish(XcosMessages.EXPORT_IN_PROGRESS);
		setProgress(30);
		final String cmd;
		switch (action) {
		case DUMP:
			cmd = createDumpCommand(graph);
			publish(XcosMessages.EXPORT_IN_PROGRESS);
			break;
		case COMPILE:
			cmd = createCompilationCommand(graph);
			publish(XcosMessages.COMPILATION_IN_PROGRESS);
			break;
		case SIMULATE:
			cmd = createSimulationCommand(graph);
			publish(XcosMessages.SIMULATION_IN_PROGRESS);
			break;
		default:
			cmd = "";
			break;
		}
		setProgress(60);
		synchronousScilabExec(cmd);
		publish(XcosMessages.EMPTY_INFO);
		setProgress(100);
		return null;
	}

	@Override
	protected void process(List<String> chunks) {
		final String msg;
		if (chunks.size() > 0) {
			msg = chunks.get(chunks.size() - 1);
		} else {
			msg = XcosMessages.EMPTY_INFO;
		}

		graph.info(msg);
	}

	@Override
	protected void done() {
		try {
			if (!isCancelled()) {
				get();
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		} catch (ExecutionException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Create the dump command String
	 * 
	 * @param diagram
	 *            the working diagram
	 * @return the command string
	 * @throws IOException
	 *             when temporary files must not be created.
	 */
	private static String createDumpCommand(final XcosDiagram diagram)
			throws IOException {
		String cmd;
		final StringBuilder command = new StringBuilder();

		/*
		 * Log compilation info
		 */
		final Log log = LogFactory.getLog(StartAction.class);
		log.trace("start export");

		/*
		 * Import a valid scs_m structure into Scilab
		 */
		final String temp = FileUtils.createTempFile();
		diagram.dumpToHdf5File(temp);

		command.append(buildCall("import_from_hdf5", temp));

		cmd = command.toString();
		return cmd;
	}

	/**
	 * Create the compilation command String
	 * 
	 * @param diagram
	 *            the working diagram
	 * @return the command string
	 * @throws IOException
	 *             when temporary files must not be created.
	 */
	private static String createCompilationCommand(final XcosDiagram diagram)
			throws IOException {
		String cmd;
		final StringBuilder command = new StringBuilder();

		/*
		 * Log compilation info
		 */
		final Log log = LogFactory.getLog(StartAction.class);
		log.trace("start compilation");

		/*
		 * Import a valid scs_m structure into Scilab
		 */
		final String temp = FileUtils.createTempFile();
		diagram.dumpToHdf5File(temp);

		command.append(buildCall("import_from_hdf5", temp));
		command.append(buildCall("scicos_debug", diagram.getScicosParameters()
				.getDebugLevel()));

		/*
		 * Compile
		 */
		command.append("; scicos_cpr=xcos_compile(scs_m); ");

		cmd = command.toString();
		return cmd;
	}

	/**
	 * Create the simulation command String
	 * 
	 * @param diagram
	 *            the working diagram
	 * @return the command string
	 * @throws IOException
	 *             when temporary files must not be created.
	 */
	private static String createSimulationCommand(final XcosDiagram diagram)
			throws IOException {
		String cmd;
		final StringBuilder command = new StringBuilder();

		/*
		 * Log compilation info
		 */
		final Log log = LogFactory.getLog(StartAction.class);
		log.trace("start simulation");

		/*
		 * Import a valid scs_m structure into Scilab
		 */
		final String temp = FileUtils.createTempFile();
		diagram.dumpToHdf5File(temp);

		command.append(buildCall("import_from_hdf5", temp));
		command.append(buildCall("scicos_debug", diagram.getScicosParameters()
				.getDebugLevel()));

		/*
		 * Simulate
		 */
		command.append("xcos_simulate(scs_m, 4); ");

		cmd = command.toString();
		return cmd;
	}
}