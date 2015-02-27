loadXcosLibs();
function [state, t] = scicosim(state, tcur, tf, sim, mod, tolerances)
    state=state
    t=tf
endfunction

function f()
    try
        exec("test/xcos/SRFLIPFLOP_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SRFLIPFLOP_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_on_block.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_on_block.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8235.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8235.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/HYSTERESIS_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/HYSTERESIS_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/IFTHEL_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/IFTHEL_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CONVERT_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CONVERT_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/QUANT_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/QUANT_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/INTEGRAL_m_2_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/INTEGRAL_m_2_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/OUTIMPL_f_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/OUTIMPL_f_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/MCLOCK_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/MCLOCK_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/SHIFT_internal_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SHIFT_internal_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12266.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12266.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/sigbuild.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/sigbuild.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/freq_div_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/freq_div_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/LOGIC_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/LOGIC_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/TOWS_c_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/TOWS_c_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/ramp_sawtooth.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/ramp_sawtooth.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_5403.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_5403.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/step_function_1_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/step_function_1_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12384.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12384.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/cfscope.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/cfscope.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12868.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12868.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/summation.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/summation.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_7396.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_7396.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CLKOUTV_f_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CLKOUTV_f_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/if_then_else.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/if_then_else.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/SWITCH2_m_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SWITCH2_m_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_13059.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_13059.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/integral_sawtooth_sin.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/integral_sawtooth_sin.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/dflipflop.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/dflipflop.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/BouncingBall_Modelica.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/BouncingBall_Modelica.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/EVTGEN_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/EVTGEN_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8480.cosf", -1);
        // debug_scicos is not implemented yet
        // scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8480.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/EXPRESSION_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/EXPRESSION_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/cmscope.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/cmscope.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_from_without_goto.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_from_without_goto.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8737.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8737.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/OUT_f_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/OUT_f_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_7315.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_7315.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Plant_DiscreteController.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Plant_DiscreteController.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Command.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Command.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12998.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12998.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/empty.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/empty.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/BACKLASH_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/BACKLASH_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/sinus.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/sinus.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Controller.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Controller.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_5444.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_5444.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/shift.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/shift.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_9386.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_9386.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/AFFICH_m_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/AFFICH_m_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8483.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8483.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Threshold_ZeroCrossing.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Threshold_ZeroCrossing.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/goto_demo.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/goto_demo.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Inverted_pendulum.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Inverted_pendulum.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_11913.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_11913.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/EVTDLY_c_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/EVTDLY_c_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_blocks_with_not_connected_link.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_blocks_with_not_connected_link.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Discrete-KalmanFilter.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Discrete-KalmanFilter.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/RAMP_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/RAMP_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Opamp_Amplifier.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Opamp_Amplifier.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/canimxy.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/canimxy.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/cmatview3d.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/cmatview3d.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/SATURATION_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SATURATION_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/iselect.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/iselect.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_9810.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_9810.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_7639.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_7639.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_cmscope.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_cmscope.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/ENDBLK_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/ENDBLK_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/BITSET_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/BITSET_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_13396.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_13396.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Kalman_1.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Kalman_1.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8884.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8884.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/writec_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/writec_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_port_block_on_diagram.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_port_block_on_diagram.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/pending_mux.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/pending_mux.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_9346.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_9346.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/noop_scalar.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/noop_scalar.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Lorenz.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Lorenz.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/with_ZeroCrossing.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/with_ZeroCrossing.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/text_f_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/text_f_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/intmul.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/intmul.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8735.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8735.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/fromws_c.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/fromws_c.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/RLC_Modelica.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/RLC_Modelica.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/OUTIMPL_f_internal_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/OUTIMPL_f_internal_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/sardines.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/sardines.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/IN_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/IN_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/cumsum.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/cumsum.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12874.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12874.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/PID_1_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/PID_1_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/intro_qq.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/intro_qq.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/INTEGRAL_m_1_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/INTEGRAL_m_1_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/INIMPL_f_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/INIMPL_f_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8441.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8441.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_cscope.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_cscope.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/SHIFT_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SHIFT_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/MIN_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/MIN_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Ball_Platform.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Ball_Platform.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/DLATCH_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/DLATCH_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/DAE_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/DAE_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8569.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8569.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_ports_with_negative_size_on_sb.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_ports_with_negative_size_on_sb.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Command_bode.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Command_bode.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/sawtooth_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/sawtooth_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/TKSCALE_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/TKSCALE_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/DELAY_f_1_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/DELAY_f_1_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CLKOUTV_f_internal_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CLKOUTV_f_internal_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/DELAY_f_1_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/DELAY_f_1_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/QUANT_f_Example_Quantization_Mode.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/QUANT_f_Example_Quantization_Mode.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/rfile_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/rfile_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/DC_DC_Buck_Converter.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/DC_DC_Buck_Converter.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/wfile_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/wfile_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_6814.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_6814.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Colpitts_Oscillator.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Colpitts_Oscillator.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_6386.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_6386.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/PID_internal.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/PID_internal.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_7985.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_7985.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8692.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8692.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Blocks.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Blocks.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Platform.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Platform.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/lut.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/lut.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/cscope.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/cscope.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/without_ZeroCrossing.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/without_ZeroCrossing.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_5638.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_5638.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/demo_Datatype.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/demo_Datatype.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Signal_Builder.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Signal_Builder.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/cevscpe.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/cevscpe.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/EVTVARDLY_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/EVTVARDLY_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_unconnected_sb_input.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_unconnected_sb_input.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_7015.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_7015.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/writec_f_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/writec_f_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/state.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/state.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/BITSET_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/BITSET_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CLKOUTV_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CLKOUTV_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/SRFLIPFLOP_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SRFLIPFLOP_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/delay_anal.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/delay_anal.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/gensin_4.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/gensin_4.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_ports_with_incompatible_size_on_sb.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_ports_with_incompatible_size_on_sb.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/matmul.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/matmul.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/event_and.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/event_and.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/clock_c_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/clock_c_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Cont-Disc.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Cont-Disc.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/ANDBLK_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/ANDBLK_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/integral.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/integral.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/System-Observer.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/System-Observer.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_9307.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_9307.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_6578.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_6578.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/DLATCH_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/DLATCH_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/rossler.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/rossler.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/M_freq_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/M_freq_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8822.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8822.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_6517.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_6517.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Kalman.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Kalman.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/canimxy3d.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/canimxy3d.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/SELF_SWITCH_on_off_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SELF_SWITCH_on_off_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_from_without_goto_on_sb.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_from_without_goto_on_sb.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/demo_watertank.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/demo_watertank.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Transformer.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Transformer.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_7424.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_7424.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/PID_3_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/PID_3_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/OUTIMPL_f_internal_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/OUTIMPL_f_internal_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/LOGIC_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/LOGIC_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_goto_same_tag_on_sb.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_goto_same_tag_on_sb.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/SAMPHOLD_m_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SAMPHOLD_m_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/RELAY_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/RELAY_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/MFCLCK_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/MFCLCK_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/cscopxy.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/cscopxy.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/select.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/select.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/pendulum_anim45.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/pendulum_anim45.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Temperature_Controller.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Temperature_Controller.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/SHIFT_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SHIFT_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12751.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12751.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/batch_simulation.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/batch_simulation.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/gain.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/gain.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/gensin_2.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/gensin_2.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_13318.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_13318.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Bouncing_ball.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Bouncing_ball.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/gensin_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/gensin_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_4715.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_4715.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Simple_Demo.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Simple_Demo.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/modelica_RLC.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/modelica_RLC.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Difference_amplifier.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Difference_amplifier.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/gensin.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/gensin.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Cont.Disc-Observer.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Cont.Disc-Observer.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/INIMPL_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/INIMPL_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Thermique_bloc.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Thermique_bloc.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_sb_port_numbering.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_sb_port_numbering.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_blocks_with_undefined_input.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_blocks_with_undefined_input.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CANIMXY_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CANIMXY_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/ESELECT_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/ESELECT_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/set_bit.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/set_bit.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Boost_Converter.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Boost_Converter.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Simple_Thermostat.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Simple_Thermostat.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/DELAYV_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/DELAYV_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/tows_c.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/tows_c.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/RLC.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/RLC.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Watertank.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Watertank.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/integral_multi_3.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/integral_multi_3.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/IN_f_internal_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/IN_f_internal_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Ifsub.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Ifsub.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/threshold.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/threshold.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/transferfcn.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/transferfcn.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/integral_multi.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/integral_multi.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/one_buffering_scopes.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/one_buffering_scopes.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_blocks_with_undefined_input_on_sb.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_blocks_with_undefined_input_on_sb.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Hydraulics.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Hydraulics.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/AllSum.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/AllSum.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/ABS_VALUE_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/ABS_VALUE_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/wfile_f_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/wfile_f_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12731.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12731.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/SELECT_m_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SELECT_m_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/BITCLEAR_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/BITCLEAR_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/with_ZeroCrossingSimple.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/with_ZeroCrossingSimple.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8630.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8630.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/step_function_2_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/step_function_2_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/BIGSOM_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/BIGSOM_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CONVERT_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CONVERT_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/constant.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/constant.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/noop_matrices.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/noop_matrices.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12057_cmscope.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12057_cmscope.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CLKINV_f_internal_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CLKINV_f_internal_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/dlatch.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/dlatch.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/PID_2_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/PID_2_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/TRASH_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/TRASH_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/samphold.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/samphold.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/IN_f_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/IN_f_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/srflipflop.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/srflipflop.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/EXTRACT_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/EXTRACT_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/integral_2.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/integral_2.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/IdealTransformer_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/IdealTransformer_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/NEGTOPOS_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/NEGTOPOS_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Fibonacci.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Fibonacci.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CANIMXY3D_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CANIMXY3D_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/LV.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/LV.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/integral_sawtooth.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/integral_sawtooth.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/TIME_DELAY_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/TIME_DELAY_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/TCLSS_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/TCLSS_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/scifunc_block_m_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/scifunc_block_m_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/extract_bits.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/extract_bits.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8042.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8042.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Extract_Activation_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Extract_Activation_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/VEress9-2D4.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/VEress9-2D4.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/rfile_f_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/rfile_f_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/integral_multi_2.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/integral_multi_2.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/RATELIMITER_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/RATELIMITER_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/BITCLEAR_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/BITCLEAR_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/switch2_m.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/switch2_m.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bounce.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bounce.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/AND_Gate.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/AND_Gate.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/cmscope-multi-frequency.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/cmscope-multi-frequency.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/OUT_f_internal_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/OUT_f_internal_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Scilab_Block.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Scilab_Block.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/fromws_tows_c.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/fromws_tows_c.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12869.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12869.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Bridge_Rectifier.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Bridge_Rectifier.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/jkflipflop.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/jkflipflop.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/gensin_3.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/gensin_3.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/cscopxy3d.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/cscopxy3d.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CMAT3D_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CMAT3D_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_6797.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_6797.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_ports_with_incompatible_size.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_ports_with_incompatible_size.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/SHIFT_internal_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/SHIFT_internal_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12424.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12424.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/IPendulum.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/IPendulum.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/inputfile.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/inputfile.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/without_ZeroCrossingSimple.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/without_ZeroCrossingSimple.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/INTMUL_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/INTMUL_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/INTMUL_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/INTMUL_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/controller2.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/controller2.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/ANDLOG_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/ANDLOG_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/FROMWSB_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/FROMWSB_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_blocks_with_updated_context_on_sb.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_blocks_with_updated_context_on_sb.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/EDGE_TRIGGER_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/EDGE_TRIGGER_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/OldGainTest.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/OldGainTest.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/derivative.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/derivative.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/JKFLIPFLOP_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/JKFLIPFLOP_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8348.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8348.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_4898.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_4898.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/test_affich_m.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/test_affich_m.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/BARXY_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/BARXY_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CLSS_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CLSS_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/GENSQR_f_diagram_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/GENSQR_f_diagram_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8744.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8744.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/VARIABLE_DELAY_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/VARIABLE_DELAY_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12057_cscope.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12057_cscope.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8674.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8674.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/susp.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/susp.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/DELAY_f_2_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/DELAY_f_2_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/modulo_count_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/modulo_count_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/DERIV_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/DERIV_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/INIMPL_f_internal_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/INIMPL_f_internal_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/disease.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/disease.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_goto_same_tag.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_goto_same_tag.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Switched_capacitor_integrator.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Switched_capacitor_integrator.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CLKINV_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CLKINV_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/IF_block.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/IF_block.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CSCOPE_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CSCOPE_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/WRITEAU_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/WRITEAU_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_8867.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_8867.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Table_Lookup.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Table_Lookup.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/EXTRACTBITS_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/EXTRACTBITS_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/JKFLIPFLOP_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/JKFLIPFLOP_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/deadband_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/deadband_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_11193.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_11193.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/NOR_Gate.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/NOR_Gate.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/INTRPLBLK_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/INTRPLBLK_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/OUT_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/OUT_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/cmatview.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/cmatview.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_ports_with_negative_size.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_ports_with_negative_size.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/fibo.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/fibo.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_10781.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_10781.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/EXTRACTBITS_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/EXTRACTBITS_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_6527.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_6527.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/context_update.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/context_update.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/clear_bit.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/clear_bit.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/outputfile.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/outputfile.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/text_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/text_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/pendulum_anim5.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/pendulum_anim5.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CLSS.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CLSS.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/END_c_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/END_c_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/nonlinear.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/nonlinear.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/deadband_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/deadband_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_12414.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_12414.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/error_blocks_with_updated_context.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/error_blocks_with_updated_context.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/ODE_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/ODE_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/VARIABLE_DELAY_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/VARIABLE_DELAY_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CLKSOMV_f_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CLKSOMV_f_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/logic.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/logic.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CLKINV_f_fr_FR.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CLKINV_f_fr_FR.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/counter_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/counter_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CMATVIEW_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CMATVIEW_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Chaos_Modelica.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Chaos_Modelica.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/bug_5701.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/bug_5701.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/OUTIMPL_f_en_US.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/OUTIMPL_f_en_US.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/CEVENTSCOPE_Example.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/CEVENTSCOPE_Example.cosf");
    end
endfunction
f(); clear f

function f()
    try
        exec("test/xcos/Bouncing.cosf", -1);
        scicos_simulate(scs_m, list());
    catch
        disp(lasterror(), "test/xcos/Bouncing.cosf");
    end
endfunction
f(); clear f

