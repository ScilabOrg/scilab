/**
 * The Collaviz Client graphic engine.
 * The graphic engine help to render scenes in canvas.
 * Scenes are tree oriented data model, the rendering is performed by a recursive tree walk.
 * For each object of the tree, the engine look for a drawer in the pool of existing drawer.
 * If a corresponding drawer is found, it's called with the object as argument.  
 */
package org.scilab.modules.graphic_objects.graphicEngine;