loadXcosLibs();
function [state, t] = scicosim(state, tcur, tf, sim, mod, tolerances)
    state=state
    t=tf
endfunction
deletefile("scilab.log");
diary("scilab.log", "new");

exec("test/xcos/Switched_capacitor_integrator.cosf", -1);
// scilab interpreter crash
pause
scicos_log("TRACE")
scicos_simulate(scs_m, list());

diary([], "close")
