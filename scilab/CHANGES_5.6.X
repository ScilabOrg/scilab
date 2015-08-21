                     Changes between version 5.5.1 and 5.6.0
                     =======================================

New Features
=============

* scatter plot with different mark colors is now available:
  TODO : save/load+display+doc of new properties


Xcos
====

* Palette browser - zoom feature is now available via CTRL(+), CTRL(-) and CTRL(mousewheel).
* Palette browser is now able to load SVG icons.
* Palette browser handles the action of going forward or backward.
* Palette browser enable users to add blocks to the most recent diagram by using the ENTER key.
* Palette browser includes a palette of blocks you have used most recently.
* Palette browser is now able to select multiple blocks by holding down the ctrl key.

* Bug #13942 fixed - Now the palette browser tree is always resized when expanded/collapsed.
* Bug #13906 fixed - It is now possible to navigate through the Palette browser with the arrow keys.
* Bug #12453 fixed - The left panel of the palette browser keeps its size when some category is enabled or disabled.
* Bug #10326 fixed - Adds a search engine for the palette browser.

Compilation
============

* --without-xcos now only disable Xcos compilation. Xcos Graphic interface is disabled
using --without-gui.
