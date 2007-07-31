// ==========================================
// Copyright INRIA
// Copyright Enpc
// last modified : 14th, December 2005
// ==========================================

mode(0);

// Hide the toolbar
toolbar(0,'off');

// Save the graphic context
kp = xget('pixmap');
curFont=xget('font');

path=get_absolute_file_path('anim7_1.sce');
getf(path+'anim7.sci');
demo_riemann();
xset('font',curFont(1),curFont(2));

xset("pixmap",0);

// Restore the graphic context
xset('pixmap',kp);

mode(-1);
