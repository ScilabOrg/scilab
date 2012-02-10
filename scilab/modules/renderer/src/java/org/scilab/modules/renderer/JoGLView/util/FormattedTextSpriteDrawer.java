package org.scilab.modules.renderer.JoGLView.util;

import org.scilab.forge.scirenderer.sprite.SpriteDrawer;
import org.scilab.forge.scirenderer.sprite.SpriteDrawingTools;
import org.scilab.forge.scirenderer.sprite.SpriteManager;
import org.scilab.forge.scirenderer.sprite.TextEntity;
import org.scilab.modules.graphic_objects.figure.ColorMap;
import org.scilab.modules.graphic_objects.textObject.Font;
import org.scilab.modules.graphic_objects.textObject.FormattedText;
import org.scilab.modules.renderer.utils.textRendering.FontManager;

import javax.swing.Icon;
import java.awt.Dimension;

import static org.scilab.modules.console.utils.ScilabSpecialTextUtilities.compileLaTeXExpression;

/**
 * @author Pierre Lando
 */
public class FormattedTextSpriteDrawer implements SpriteDrawer {
    private final TextEntity textEntity;
    private final Dimension dimension;
    private final Icon icon;

    public FormattedTextSpriteDrawer(ColorMap colorMap, SpriteManager spriteManager, FormattedText formattedText) {
        if (formattedText != null) {
            String text = formattedText.getText();
            Font font = formattedText.getFont();
            if (isLatex(text)) {
                icon = compileLaTeXExpression(text.substring(1, text.length() - 1), (int) (FontManager.scilabSizeToAwtSize(font.getSize())+ .5));
                dimension = new Dimension(icon.getIconWidth(), icon.getIconHeight());
                textEntity = null;
            } else {
                icon = null;
                textEntity = new TextEntity(formattedText.getText());

                textEntity.setFont(FontManager.getSciFontManager().getFontFromIndex(font.getStyle(), font.getSize()));
                textEntity.setText(formattedText.getText());
                textEntity.setTextColor(ColorFactory.createColor(colorMap, font.getColor()));
                textEntity.setTextUseFractionalMetrics(font.getFractional());
                textEntity.setTextAntiAliased(false);

                dimension = spriteManager.getSize(textEntity);
            }
        } else {
            icon = null;
            textEntity = null;
            dimension = new Dimension();
        }
    }

    @Override
    public void draw(SpriteDrawingTools drawingTools) {
        if (textEntity != null) {
            drawingTools.draw(textEntity, 0, 0);
        } else if (icon != null) {
            drawingTools.draw(icon, 0, 0);
        }
    }

    @Override
    public OriginPosition getOriginPosition() {
        return OriginPosition.UPPER_LEFT;
    }

    public Dimension getSpriteSize() {
        return new Dimension(dimension);
    }

    /**
     * Return true if the given string represent a latex entity.
     * @param string the given string.
     * @return true if the given string represent a latex entity.
     */
    private boolean isLatex(String string) {
        return (string.length() >= 2) && string.endsWith("$") && string.startsWith("$");
    }
}
