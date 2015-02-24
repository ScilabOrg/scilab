
                     Changes between version 5.5.1 and 5.6.0
                     =======================================


Compilation
============

* --without-xcos now only disable Xcos compilation. Xcos Graphic interface is
disabled using --without-gui.


Scilab Bug Fixes
================

* Bug #13758 fixed - x_mdialog interpreted 'F' and 'T' as boolean values
                     whereas it should interpret '%F' and '%T' instead.
