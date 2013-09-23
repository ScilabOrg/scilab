// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Pedro SOUZA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_rot_light()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("rot_plot3d.dem.sce");

    // DEMO START

    my_plot_desc          = "Rotating light";
    my_handle.figure_name = my_plot_desc;

    my_handle.figure_name = my_plot_desc;
    plot3d;l=light_create();s=gce();a=gca();

    plot3d;a=gca();

    for i=1:360
        sleep(5);
        a.rotation_angles = [70 i];
    end

    // DEMO END

endfunction


demo_rot_light();
clear demo_rot_light;

