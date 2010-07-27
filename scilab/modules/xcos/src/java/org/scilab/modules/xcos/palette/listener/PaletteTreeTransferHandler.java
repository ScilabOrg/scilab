/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Clément DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.xcos.palette.listener;

import java.awt.Point;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.datatransfer.UnsupportedFlavorException;
import java.io.IOException;
import java.io.StringWriter;
import java.util.Arrays;

import javax.swing.JComponent;
import javax.swing.JTree;
import javax.swing.TransferHandler;
import javax.swing.tree.DefaultTreeModel;
import javax.swing.tree.TreeNode;
import javax.swing.tree.TreePath;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.JAXBException;
import javax.xml.bind.Marshaller;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.scilab.modules.xcos.palette.PaletteManager;
import org.scilab.modules.xcos.palette.model.Category;
import org.scilab.modules.xcos.palette.model.PaletteNode;

/**
 * Used to modify the palette tree
 */
public class PaletteTreeTransferHandler extends TransferHandler {

	private static final Log LOG = LogFactory.getLog(PaletteTreeTransferHandler.class);

	/**
	 * Default constructor 
	 */
	public PaletteTreeTransferHandler() {
		super();
	}
	
	/**
	 * @param c the current component
	 * @return {@code COPY_OR_MOVE}
	 * @see javax.swing.TransferHandler#getSourceActions(javax.swing.JComponent)
	 */
	@Override
	public int getSourceActions(JComponent c) {
		return COPY_OR_MOVE;
	}
	
	/**
	 * Reload the tree when the action is done
	 * @param source the source
	 * @param data the data
	 * @param action current action
	 * @see javax.swing.TransferHandler#exportDone(javax.swing.JComponent, java.awt.datatransfer.Transferable, int)
	 */
	@Override
	protected void exportDone(JComponent source, Transferable data, int action) {
		final PaletteTransferable transferable = (PaletteTransferable) data;
		
		if (action == MOVE && transferable.isMoved()) {
			updateTree((JTree) source, data);
		}
	}

	/**
	 * Update the palette tree after a drag
	 * 
	 * @param t the source component
	 * @param data the transferable
	 */
	private void updateTree(JTree t, Transferable data) {
		PaletteManager.getInstance().saveConfig();

		final DefaultTreeModel model = (DefaultTreeModel) t.getModel();
		final TreePath path = t.getSelectionPath();
		PaletteNode newNode;
		
		try {
			newNode = (PaletteNode) data.getTransferData(PaletteTransferable.PALETTE_FLAVOR);
		} catch (UnsupportedFlavorException e) {
			LOG.error(e);
			return;
		} catch (IOException e) {
			LOG.error(e);
			return;
		} 
		
		model.nodeStructureChanged(newNode.getParent());
		model.nodeStructureChanged((TreeNode) path.getParentPath().getLastPathComponent());
	}
	
	/**
	 * @param c the current component
	 * @return the new transferable
	 * @see javax.swing.TransferHandler#createTransferable(javax.swing.JComponent)
	 */
	@Override
	protected Transferable createTransferable(JComponent c) {
		if (c == null) {
			return null;
		}
		
		final JTree tree = (JTree) c;
		final PaletteNode node = (PaletteNode) tree.getSelectionPath().getLastPathComponent();
		
		return new PaletteTransferable(node);
	}
	
	/**
	 * @param support the current support
	 * @return true for a Category area, false otherwise
	 * @see javax.swing.TransferHandler#canImport(javax.swing.TransferHandler.TransferSupport)
	 */
	@Override
	public boolean canImport(TransferSupport support) {
		final JTree tree = (JTree) support.getComponent();
		final Point point = support.getDropLocation().getDropPoint();
		final TreePath path = tree.getPathForLocation(point.x, point.y);
		
		if (path != null) {
			return Arrays.asList(support.getDataFlavors()).contains(
					PaletteTransferable.PALETTE_FLAVOR);
		}
		
		return false;
	}
	
	/**
	 * @param support the current support
	 * @return true if the import has been successfully performed, false otherwise.
	 * @see javax.swing.TransferHandler#importData(javax.swing.TransferHandler.TransferSupport)
	 */
	@Override
	public boolean importData(TransferSupport support) {
		final JTree tree = PaletteManager.getInstance().getView().getTree();
		final JTree.DropLocation location = tree.getDropLocation();
		
		Category newParent = (Category) location.getPath().getLastPathComponent();
		final int index = location.getChildIndex();
		
		final Transferable t = support.getTransferable();
		
		try {
			PaletteNode child = (PaletteNode) t.getTransferData(PaletteTransferable.PALETTE_FLAVOR);
			
			final Category oldParent = child.getParent();
			oldParent.getNode().remove(child);
			child.setParent(newParent);

			if (index == -1) {
				newParent.getNode().add(child);
			} else {
				newParent.getNode().add(index, child);
			}
		} catch (UnsupportedFlavorException e) {
			LOG.error(e);
			return false;
		} catch (IOException e) {
			LOG.error(e);
			return false;
		}
		
		return true;
	}
	
	/**
	 * Implement transferable for any Palette node.
	 */
	private static class PaletteTransferable implements Transferable {
		/**
		 * The palette data flavor
		 */
		public static final DataFlavor PALETTE_FLAVOR = new DataFlavor(PaletteNode.class, null);
		private static final DataFlavor STRING_FLAVOR = DataFlavor.stringFlavor;
		
		private final PaletteNode node;
		private boolean moved;
		
		/**
		 * Default constructor
		 * @param node the selected node
		 */
		public PaletteTransferable(PaletteNode node) {
			this.node = node;
			moved = false;
		}
		
		/**
		 * @return the node or null if it has been moved
		 */
		public final PaletteNode getNode() {
			return node;
		}
		
		/**
		 * @return the moved flag
		 */
		public final boolean isMoved() {
			return moved;
		}
		
		/**
		 * @param flavor the current flavor
		 * @return the associated {@link PaletteNode}
		 * @throws UnsupportedFlavorException when the flavor is not supported
		 * @throws IOException when deserialization fails
		 * @see java.awt.datatransfer.Transferable#getTransferData(java.awt.datatransfer.DataFlavor)
		 */
		@Override
		public Object getTransferData(DataFlavor flavor)
				throws UnsupportedFlavorException, IOException {
			Object ret = null;
			
			if (flavor.equals(STRING_FLAVOR) && getNode() instanceof Category) {
				try {
					final JAXBContext jaxbContext = JAXBContext.newInstance(PaletteManager.MODEL_CLASS_PACKAGE);
					final Marshaller marshaller = jaxbContext.createMarshaller();
					marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);
					
					StringWriter writer = new StringWriter();
					marshaller.marshal(node, writer);
					ret = writer.toString();
				} catch (JAXBException e) {
					throw new IOException(e);
				}
			} else if (flavor.equals(PALETTE_FLAVOR)) {
				ret = node;
				moved = true;
			}
			
			if (ret == null) {
				throw new UnsupportedFlavorException(flavor);
			} else {
				return ret;
			}
		}

		/**
		 * @return the valid data flavors
		 * @see java.awt.datatransfer.Transferable#getTransferDataFlavors()
		 */
		@Override
		public DataFlavor[] getTransferDataFlavors() {
			final Category parent = getNode().getParent(); 
			if (parent.getParent() == null && parent.getChildAt(0) == getNode()) {
				return new DataFlavor[] {STRING_FLAVOR};
			} else if (getNode() instanceof Category) {
				return new DataFlavor[] {PALETTE_FLAVOR, STRING_FLAVOR};
			} else {
				return new DataFlavor[] {PALETTE_FLAVOR};
			}
		}

		/**
		 * @param flavor the flavor
		 * @return true if the flavor match the right mime type; false otherwise.
		 * @see java.awt.datatransfer.Transferable#isDataFlavorSupported(java.awt.datatransfer.DataFlavor)
		 */
		@Override
		public boolean isDataFlavorSupported(DataFlavor flavor) {
			return Arrays.asList(getTransferDataFlavors()).contains(flavor);
		}
	}
}
