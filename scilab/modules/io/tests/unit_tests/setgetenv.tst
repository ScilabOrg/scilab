// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Yung-Jang Lee <yjlee123@gmail.com>
// Copyright (C) 2009 - INRIA - Sylvestre Ledru
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

specialName='中文' ;     //  for CP950, BIG5,zh_TW, zh_TW.utf-8, zh_CN,zh_CN.utf-8
setenv('env',specialName);
if getenv('env') <> specialName then pause, end

specialName='ЁЂЄЉф';  // russian LANG=ru_RU, ru_RU.utf-8
setenv('env',specialName);
if getenv('env') <> specialName then pause, end

specialName='グゲゾタ'; // Japaness LANG=ja_JP ,
setenv('env',specialName);
if getenv('env') <> specialName then pause, end

specialName='메일링을';  //  korean LANG=ko_KR, ko_KR=utf-8
setenv('env',specialName);
if getenv('env') <> specialName then pause, end

specialName='æøå';    // LANG=fr_FR,C,en_US, and fr_FR.utf
setenv('env',specialName);
if getenv('env') <> specialName then pause, end

