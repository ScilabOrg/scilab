package org.scilab.modules.graphic_objects.util.MVC.tree.node;

import org.scilab.modules.graphic_objects.util.MVC.AbstractDataController;
import org.scilab.modules.graphic_objects.util.MVC.IDataController;
import org.scilab.modules.graphic_objects.util.MVC.IDataEvent;
import org.scilab.modules.graphic_objects.util.MVC.IDataListener;

/**
 * Author: Lando Pierre
 * Date: june 15, 2010
 */
public abstract class AbstractNodeController <nodeContentType extends INodeContent> extends AbstractDataController<INodeModel<nodeContentType>> implements INodeController<nodeContentType> {
    private final LocalListener listener = new LocalListener();

    public AbstractNodeController(INodeModel<nodeContentType> model) {
        super(model);
    }

    
    public nodeContentType getContent() {
        return getModel().getContent();
    }

    
    public void setContent(nodeContentType content) {
        nodeContentType oldContent = getContent();
        if (oldContent != content) {
            if (oldContent != null) {
                oldContent.removeListener(listener);
            }

            getModel().setContent(content);

            if (content != null) {
                content.addListener(listener);
            }
            fireUpdate();
        }
    }

    private class LocalListener implements IDataListener {

        
        public void update(IDataController sources, IDataEvent event) {
            fireUpdate(event);
        }
    }
}
