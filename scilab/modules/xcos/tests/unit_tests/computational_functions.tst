// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- XCOS TEST -->
//
// <-- Short Description -->
// Check that all computational function are available.

//
// load the source file list for languages
//

ilib_verbose(0);

BLOCKS_C = [..
"absblk", ..
"absolute_value", ..
"acos_blk", ..
"acosh_blk", ..
"affich2", ..
"andlog", ..
"asin_blk", ..
"asinh_blk", ..
"assignment", ..
"atan_blk", ..
"atanh_blk", ..
"automat", ..
"backlash", ..
"bidon", ..
"bit_clear_16", ..
"bit_clear_32", ..
"bit_clear_8", ..
"bit_set_16", ..
"bit_set_32", ..
"bit_set_8", ..
"bounce_ball", ..
"bouncexy", ..
"canimxy", ..
"canimxy3d", ..
"cdummy", ..
"cevscpe", ..
"cfscope", ..
"cmat3d", ..
"cmatview", ..
"cmscope", ..
"constraint_c", ..
"convert", ..
"cosblk", ..
"cos_blk", ..
"cosh_blk", ..
"counter", ..
"cscope", ..
"cscopxy", ..
"cscopxy3d", ..
"csslti4", ..
"cstblk4", ..
"cstblk4_m", ..
"cumsum_c", ..
"cumsum_m", ..
"cumsum_r", ..
"cumsumz_c", ..
"cumsumz_m", ..
"cumsumz_r", ..
"curve_c", ..
"dband", ..
"deadband", ..
"delay4", ..
"delay4_i16", ..
"delay4_i32", ..
"delay4_i8", ..
"delay4_ui16", ..
"delay4_ui32", ..
"delay4_ui8", ..
"deriv", ..
"diffblk_c", ..
"dmmul", ..
"dmmul1", ..
"dollar4", ..
"dollar4_m", ..
"dsslti4", ..
"edgetrig", ..
"evaluate_expr", ..
"evtdly4", ..
"evtvardly", ..
"expblk_m", ..
"extdiag", ..
"extdiagz", ..
"extract", ..
"extract_bit_16_LH", ..
"extract_bit_16_LSB", ..
"extract_bit_16_MSB0", ..
"extract_bit_16_MSB1", ..
"extract_bit_16_RB0", ..
"extract_bit_16_RB1", ..
"extract_bit_16_UH0", ..
"extract_bit_16_UH1", ..
"extract_bit_32_LH", ..
"extract_bit_32_LSB", ..
"extract_bit_32_MSB0", ..
"extract_bit_32_MSB1", ..
"extract_bit_32_RB0", ..
"extract_bit_32_RB1", ..
"extract_bit_32_UH0", ..
"extract_bit_32_UH1", ..
"extract_bit_8_LH", ..
"extract_bit_8_LSB", ..
"extract_bit_8_MSB0", ..
"extract_bit_8_MSB1", ..
"extract_bit_8_RB0", ..
"extract_bit_8_RB1", ..
"extract_bit_8_UH0", ..
"extract_bit_8_UH1", ..
"extract_bit_u16_MSB1", ..
"extract_bit_u16_RB1", ..
"extract_bit_u16_UH1", ..
"extract_bit_u32_MSB1", ..
"extract_bit_u32_RB1", ..
"extract_bit_u32_UH1", ..
"extract_bit_u8_MSB1", ..
"extract_bit_u8_RB1", ..
"extract_bit_u8_UH1", ..
"extractor", ..
"extractz", ..
"exttril", ..
"exttrilz", ..
"exttriu", ..
"exttriuz", ..
"foriterator", ..
"fromws_c", ..
"gain", ..
"gainblk", ..
"gainblk_i16e", ..
"gainblk_i16n", ..
"gainblk_i16s", ..
"gainblk_i32e", ..
"gainblk_i32n", ..
"gainblk_i32s", ..
"gainblk_i8e", ..
"gainblk_i8n", ..
"gainblk_i8s", ..
"gainblk_ui16e", ..
"gainblk_ui16n", ..
"gainblk_ui16s", ..
"gainblk_ui32e", ..
"gainblk_ui32n", ..
"gainblk_ui32s", ..
"gainblk_ui8e", ..
"gainblk_ui8n", ..
"gainblk_ui8s", ..
"hystheresis", ..
"integral_func", ..
"integralz_func", ..
"invblk4", ..
"logic", ..
"logicalop", ..
"logicalop_i16", ..
"logicalop_i32", ..
"logicalop_i8", ..
"logicalop_m", ..
"logicalop_ui16", ..
"logicalop_ui32", ..
"logicalop_ui8", ..
"lookup2d", ..
"lookup_c", ..
"mat_bksl", ..
"matbyscal", ..
"matbyscal_e", ..
"matbyscal_s", ..
"mat_cath", ..
"mat_catv", ..
"mat_det", ..
"mat_diag", ..
"mat_div", ..
"mat_expm", ..
"mathermit_m", ..
"mat_inv", ..
"mat_lu", ..
"matmul2_e", ..
"matmul2_m", ..
"matmul2_s", ..
"matmul_i16e", ..
"matmul_i16n", ..
"matmul_i16s", ..
"matmul_i32e", ..
"matmul_i32n", ..
"matmul_i32s", ..
"matmul_i8e", ..
"matmul_i8n", ..
"matmul_i8s", ..
"matmul_m", ..
"matmul_ui16e", ..
"matmul_ui16n", ..
"matmul_ui16s", ..
"matmul_ui32e", ..
"matmul_ui32n", ..
"matmul_ui32s", ..
"matmul_ui8e", ..
"matmul_ui8n", ..
"matmul_ui8s", ..
"mat_pinv", ..
"mat_reshape", ..
"mat_sing", ..
"mat_sqrt", ..
"mat_sum", ..
"mat_sumc", ..
"mat_suml", ..
"mat_svd", ..
"mattran_m", ..
"mat_vps", ..
"mat_vpv", ..
"matz_abs", ..
"matz_absc", ..
"matz_bksl", ..
"matz_cath", ..
"matz_catv", ..
"matz_conj", ..
"matz_det", ..
"matz_diag", ..
"matz_div", ..
"matz_expm", ..
"matz_inv", ..
"matz_lu", ..
"matzmul2_m", ..
"matzmul_m", ..
"matz_pinv", ..
"matz_reim", ..
"matz_reimc", ..
"matz_reshape", ..
"matz_sing", ..
"matz_sqrt", ..
"matz_sum", ..
"matz_sumc", ..
"matz_suml", ..
"matz_svd", ..
"matztran_m", ..
"matz_vps", ..
"matz_vpv", ..
"m_frequ", ..
"minmax", ..
"modulo_count", ..
"mswitch", ..
"multiplex", ..
"plusblk", ..
"prod", ..
"product", ..
"ramp", ..
"ratelimiter", ..
"readau", ..
"readc", ..
"relationalop", ..
"relational_op", ..
"relational_op_i16", ..
"relational_op_i32", ..
"relational_op_i8", ..
"relational_op_ui16", ..
"relational_op_ui32", ..
"relational_op_ui8", ..
"relay", ..
"ricc_m", ..
"rndblk_m", ..
"rndblkz_m", ..
"root_coef", ..
"rootz_coef", ..
"samphold4", ..
"samphold4_m", ..
"satur", ..
"scalar2vector", ..
"scicosexit", ..
"selector", ..
"selector_m", ..
"shift_16_LA", ..
"shift_16_LC", ..
"shift_16_RA", ..
"shift_16_RC", ..
"shift_32_LA", ..
"shift_32_LC", ..
"shift_32_RA", ..
"shift_32_RC", ..
"shift_8_LA", ..
"shift_8_LC", ..
"shift_8_RA", ..
"shift_8_RC", ..
"shift_u16_RA", ..
"shift_u32_RA", ..
"shift_u8_RA", ..
"signum", ..
"sin_blk", ..
"sinh_blk", ..
"step_func", ..
"submat", ..
"submatz", ..
"sum", ..
"summation", ..
"summation_i16e", ..
"summation_i16n", ..
"summation_i16s", ..
"summation_i32e", ..
"summation_i32n", ..
"summation_i32s", ..
"summation_i8e", ..
"summation_i8n", ..
"summation_i8s", ..
"summation_ui16e", ..
"summation_ui16n", ..
"summation_ui16s", ..
"summation_ui32e", ..
"summation_ui32n", ..
"summation_ui32s", ..
"summation_ui8e", ..
"summation_ui8n", ..
"summation_ui8s", ..
"summation_z", ..
"switch2", ..
"switch2_m", ..
"switchn", ..
"tablex2d_c", ..
"tan_blk", ..
"tanh_blk", ..
"tcslti4", ..
"tcsltj4", ..
"time_delay", ..
"tows_c", ..
"variable_delay", ..
"whileiterator", ..
"writeau", ..
"writec", ..
"zcross2", ..
];

BLOCKS_FORTRAN = [
"csslti", ..
"cstblk", ..
"delay", ..
"delayv", ..
"demux", ..
"diffblk", ..
"dlradp", ..
"dollar", ..
"dsslti", ..
"eselect", ..
"evtdly", ..
"expblk", ..
"forblk", ..
"fsv", ..
"gensin", ..
"gensqr", ..
"hltblk", ..
"ifthel", ..
"integr", ..
"intplt", ..
"intpol", ..
"intrp2", ..
"intrpl", ..
"invblk", ..
"iocopy", ..
"logblk", ..
"lookup", ..
"lsplit", ..
"lusat", ..
"maxblk", ..
"memo", ..
"mfclck", ..
"minblk", ..
"mux", ..
"pload", ..
"powblk", ..
"qzcel", ..
"qzflr", ..
"qzrnd", ..
"qztrn", ..
"readf", ..
"rndblk", ..
"samphold", ..
"sawtth", ..
"sciblk", ..
"selblk", ..
"sinblk", ..
"sqrblk", ..
"sum2", ..
"sum3", ..
"tanblk", ..
"tcslti", ..
"tcsltj", ..
"timblk", ..
"trash", ..
"writef", ..
"zcross", ..
];

BLOCKS_CPP = [
"affich2", ..
];

//
// load the scicos libs
//

//
// Link them
//

// binary case or source case
if getos() <> "Windows" then
  LIB = SCI + "/../../lib/scilab/libsciscicos_blocks" + getdynlibext();
  if ~isfile(LIB) then
    LIB = SCI + "/modules/scicos_blocks/.libs/libsciscicos_blocks" + getdynlibext();
    if ~isfile(LIB) then pause, end
  end
  if execstr("link(LIB, BLOCKS_C, ""c"");", "errcatch") <> 0 then pause, end
  if execstr("link(LIB, BLOCKS_FORTRAN, ""f"");", "errcatch") <> 0 then pause, end
  if execstr("link(LIB, BLOCKS_CPP, ""c"");", "errcatch") <> 0 then pause, end

else
  LIBCCPP = SCI + "/bin/scicos_blocks" + getdynlibext();
  LIBFOR = SCI + "/bin/scicos_blocks_f" + getdynlibext();
  if ~isfile(LIBCCPP) then pause, end
  if ~isfile(LIBFOR) then pause, end

  if execstr("link(LIBCCPP, BLOCKS_C, ""c"");", "errcatch") <> 0 then pause, end
  if execstr("link(LIBCCPP, BLOCKS_CPP, ""c"");", "errcatch") <> 0 then pause, end
  if execstr("link(LIBFOR, BLOCKS_FORTRAN, ""f"");", "errcatch") <> 0 then pause, end
end

ulink();
