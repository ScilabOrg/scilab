/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - Calixte Denizet
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

package org.scilab.modules.renderer.utils.textRendering;

import java.awt.geom.Rectangle2D;
import java.awt.Color;
import java.util.HashMap;
import java.nio.Buffer;
import java.nio.ByteBuffer;

import javax.media.opengl.GL;
import javax.media.opengl.glu.GLU;
import javax.media.opengl.GLException;
import com.sun.opengl.util.awt.TextRenderer;
import com.sun.opengl.util.texture.TextureData;
import com.sun.opengl.util.texture.Texture;
import com.sun.opengl.util.texture.TextureCoords;
import com.sun.opengl.util.texture.TextureIO;

import org.scilab.modules.renderer.textDrawing.MathMLObjectGL;
import org.scilab.modules.renderer.textDrawing.TeXObjectGL;
import org.scilab.modules.renderer.textDrawing.SpecialTextObjectGL;

/**
 * Special Text Renderer (for MathML and LaTeX)
 * @author Calixte Denizet
 */
public class SpecialTextRenderer {

    private static HashMap<String, SpecialTextObjectGL> table = new HashMap<String, SpecialTextObjectGL>();
        
    /* I use the TextRenderer to render a string which isn't in MathML or LaTeX format
       although it starts with a '<' or '$'*/
    private TextRenderer textrenderer;

    private Color color = Color.black;
    private float fontSize;
    
    private static GL gl = null;

    /**
     * Default constructor.
     * @param textrenderer a TextRenderer to display bad MathML code
     * @param fontSize The size of the font
     */
    public SpecialTextRenderer(TextRenderer textrenderer, float fontSize) {
		this.textrenderer = textrenderer;
		this.fontSize = fontSize;

		GL currentGL = null;

		try{
			currentGL = GLU.getCurrentGL();
		} catch (GLException e) {}

		if (gl != currentGL) {
		    gl = currentGL;
		    table.clear();
		}
    }
    
    /**
     * Construct and return a MathML object.
     * @param content the MathML code
     * @return Returns the MathML object
     */
    public SpecialTextObjectGL getContent(final String content) {
		SpecialTextObjectGL spe;
		if (!table.containsKey(content)) {
			try {
				spe = getSpecialTextObjectGL(content);
				if (gl != null)
				        createTexture(spe);
				table.put(content, spe);
				return spe;
			} catch (SpecialTextException e) {
				table.put(content, null);
				return null;
			}
		}
    
		spe = table.get(content);
		if (spe != null && spe.setFontSize(fontSize))
		        replaceTexture(spe);
		
		return spe;
    }
    
    /**
     * Get the boundaries.
     * @param content the special code
     * @return Returns the boundaries
     */
    public Rectangle2D getBounds(String content) {
		SpecialTextObjectGL spe = getContent(content);
		if (spe != null) {	
			return new Rectangle2D.Float(0, 0, spe.getWidth(), spe.getHeight());
		} else {
			return textrenderer.getBounds(content);
		}
    }
    
    /**
     * Redefine setColor with four channels.
     * @param r red channel
     * @param g green channel
     * @param b blue channel
     * @param a alpha channel
     */
    public void setColor(float r, float g, float b, float a) {
	        color = new Color(r, g, b, a);
    }
    
    /**
     * Modify the font size of the renderer.
     * @param fontSize font size to use
     */
    public void setFontSize(float fontSize) {
	        this.fontSize = fontSize;
    }

    private static void createTexture(SpecialTextObjectGL spe) {
	        /* If the buffer is null, it must be regenerated before getting width and height */
		Buffer buf = spe.getBuffer();
		
		Texture t = TextureIO.newTexture(new TextureData(GL.GL_RGBA, (int) spe.getWidth(), (int) spe.getHeight(), 0, GL.GL_RGBA, GL.GL_UNSIGNED_BYTE, false, false, false, buf, null));
		
		spe.setTexture(t);
    }
    
    private static void replaceTexture(SpecialTextObjectGL spe) {
	        spe.getTexture().dispose();
	        createTexture(spe);
    }
    
    /**
     * Draw special content to the screen.
     * @param content the special code
     * @param x the x position
     * @param y the y position
     * @param z the z position
     * @param scaleFactor the scale factor used in the TextRenderer
     */
    public void draw3D(String content, float x, float y, float z, float scaleFactor) {
		SpecialTextObjectGL spe = getContent(content);
		if (spe == null) {
			textrenderer.draw3D(content, x, y, z, scaleFactor);
			return;
	        }
		
		float width = spe.getWidth();
		float height = spe.getHeight();
		gl.glPushAttrib(GL.GL_ALL_ATTRIB_BITS);
		gl.glPushMatrix();

		/* The following code handles the case where the label is colored */
		Texture t = spe.getTexture();
		TextureCoords tc = t.getImageTexCoords();
		
		gl.glEnable(GL.GL_BLEND);
		gl.glBlendFunc(GL.GL_SRC_ALPHA, GL.GL_ONE_MINUS_SRC_ALPHA);
				
		/* required to correctly render pre-colored text */
		if (spe.getIsColored()) {
			gl.glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		}
		
		/* the following lines fix a strange behaviour of GL_ADD on Windows */
		float[] f = new float[4];
		gl.glGetFloatv(GL.GL_CURRENT_COLOR, f, 0);
		f[0] = 1 - f[0];
		f[1] = 1 - f[1];
		f[2] = 1 - f[2];
		gl.glTexEnvfv(GL.GL_TEXTURE_ENV, GL.GL_TEXTURE_ENV_COLOR, f, 0);
		
		gl.glTexEnvi(GL.GL_TEXTURE_ENV, GL.GL_TEXTURE_ENV_MODE, GL.GL_BLEND);
		
		gl.glTranslatef(x, y, z);
		
		t.enable();
		t.bind();

		gl.glBegin(gl.GL_QUADS);
		gl.glTexCoord2f(tc.left(),tc.bottom()); gl.glVertex2d(0, 0);
		gl.glTexCoord2f(tc.right(),tc.bottom()); gl.glVertex2d(width, 0);
		gl.glTexCoord2f(tc.right(),tc.top()); gl.glVertex2d(width, height);
		gl.glTexCoord2f(tc.left(),tc.top()); gl.glVertex2d(0, height);
		gl.glEnd();
		t.disable();
		gl.glPopMatrix();
		gl.glPopAttrib();
    }

    /**
     * Return the specialTextObjectGL
     *
     * @param content the message itself
     * @return The specialTextObjectGL
     */
    private SpecialTextObjectGL getSpecialTextObjectGL(String content) throws SpecialTextException {
		switch (content.charAt(0)) {
			case '<': 
				return new MathMLObjectGL(content, color, fontSize);
			case '$':
				return new TeXObjectGL(content, color, fontSize);
			default:
				return null;
		}
    }
}
