/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Clement DAVID
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

package org.scilab.modules.xcos;

import java.util.Enumeration;
import javax.swing.SwingUtilities;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;

import org.scilab.modules.xcos.explorer.ExplorerTreeNodeData;


/**
 * Implement a basic model explorer using a tree view
 */
public class BrowserView extends View {
    private final DefaultTreeModel model;

    /**
     * Default constructor
     */
    public BrowserView() {
        DefaultMutableTreeNode root = new DefaultMutableTreeNode(new ExplorerTreeNodeData(), true);

        model = new DefaultTreeModel(root);
    }

    public DefaultTreeModel getModel() {
        return model;
    }

    /*
     * Implement some View methods on the Event Dispatch Thread
     */

    @Override
    public void objectCreated(final long uid, final Kind kind) {
        if (model == null) {
            return;
        }
        if (kind == Kind.PORT) {
            return;
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                objectCreatedOnEDT(uid, kind);
            }
        });
    }

    @Override
    public void objectDeleted(final long uid, final Kind kind) {
        if (model == null) {
            return;
        }
        if (kind == Kind.PORT) {
            return;
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                objectDeletedOnEDT(uid, kind);
            }
        });
    }

    @Override
    public void propertyUpdated(final long uid, final Kind kind, final ObjectProperties property, final UpdateStatus status) {
        if (model == null) {
            return;
        }
        if (status != UpdateStatus.SUCCESS) {
            return;
        }

        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                propertyUpdatedOnEDT(uid, kind, property, status);
            }
        });
    }

    public void objectCreatedOnEDT(final long uid, final Kind kind) {
        DefaultMutableTreeNode root = (DefaultMutableTreeNode) model.getRoot();
        int position = model.getChildCount(root);

        final DefaultMutableTreeNode node = new DefaultMutableTreeNode(new ExplorerTreeNodeData(uid, kind), true);
        model.insertNodeInto(node, root, position);
    }

    public void objectDeletedOnEDT(final long uid, final Kind kind) {
        DefaultMutableTreeNode node = lookupForUID(uid);
        model.removeNodeFromParent(node);
    }

    public void propertyUpdatedOnEDT(final long uid, final Kind kind, final ObjectProperties property, final UpdateStatus status) {
        switch (kind) {
            case DIAGRAM:
                break;

            /* Manage the children case to append them to associated diagram */
            case ANNOTATION:
            case BLOCK:
            case LINK:
                if (property == ObjectProperties.PARENT_DIAGRAM) {
                    final Controller controller = new Controller();
                    long[] v = { 0 };
                    if (!controller.getObjectProperty(uid, kind, property, v)) {
                        return;
                    }

                    final DefaultMutableTreeNode parent = lookupForUID(v[0]);
                    final DefaultMutableTreeNode current = lookupForUID(uid);
                    if (current.getParent() == parent) {
                        return;
                    }

                    int position = parent.getChildCount();
                    // current parent is re-set'ed and its old parent is also updated
                    model.insertNodeInto(current, parent, position);
                }
                break;

            default:
                break;
        }
    }

    /*
     * Utilities
     */

    /**
     * Lookup for the UID in the TreeModel and return the result or null if not found
     * @param uid the uid to look for
     * @return the found {@link DefaultMutableTreeNode} or <code>null</code> if not found
     */
    private DefaultMutableTreeNode lookupForUID(final long uid) {
        final DefaultMutableTreeNode root = (DefaultMutableTreeNode) model.getRoot();

        DefaultMutableTreeNode node;
        for (Enumeration<DefaultMutableTreeNode> e = root.depthFirstEnumeration(); e.hasMoreElements();) {
            node = e.nextElement();
            ExplorerTreeNodeData data = (ExplorerTreeNodeData) node.getUserObject();
            if (data.getId() == uid) {
                return node;
            }
        }

        return null;
    }

}
