package org.scilab.modules.graphic_objects.JoGLView.editor;

import org.scilab.modules.graphic_objects.JoGLView.editor.SciTreeModel;
import org.scilab.modules.graphic_objects.graphicController.GraphicController;

import javax.swing.*;
import javax.swing.tree.DefaultTreeCellRenderer;
import javax.swing.tree.TreeCellRenderer;
import javax.swing.tree.TreeModel;
import java.awt.*;

/**
 * Author: lando
 * Date: 21 sept. 2010
 */
public class FigureFrame extends JFrame {
    private final String id;
    private final TreeModel treeModel;
    public FigureFrame(String id) {
        this.id = id;
        treeModel = new SciTreeModel(id);
        initialize();
    }

    private void initialize() {
        JTree tree = new JTree(treeModel);

        tree.setCellRenderer(new TreeCellRenderer() {
            private final TreeCellRenderer renderer = new DefaultTreeCellRenderer();
            public Component getTreeCellRendererComponent(JTree tree, Object value, boolean selected, boolean expanded, boolean leaf, int row, boolean hasFocus) {
                Object type = GraphicController.getController().getProperty((String) value, "Type");
                return renderer.getTreeCellRendererComponent(tree, type, selected, expanded, leaf, row, hasFocus);
            }
        });

        add(tree);
        setSize(400, 500);
        setVisible(true);
    }
}
