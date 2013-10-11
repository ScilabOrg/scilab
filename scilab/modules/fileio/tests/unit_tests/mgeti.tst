// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

function writedata(name, str)
    f = mopen(TMPDIR + "/" + name + ".dat", "wb");
    data = hex2dec(str);
    mput(data, "uc", f);
    mclose(f);
endfunction

function res = readdata(name)
    f = mopen(TMPDIR + "/" + name + ".dat", "rb");
    res = mgeti(3, name, f);
    mclose(f);
endfunction
//
//prepare data
//

ul1     = uint32(123); //0x7B
ul2     = uint32(45678);//0xB26E
ul3     = uint32(123456);//0x0001E240
l1      = int32(-123); //0x85
l2      = int32(-23456);//0xA460
l3      = int32(-123456);//0xFFFE1DC0
l3t     = int32(7616);//0x1DC0

us1     = uint16(123); //0x7B
us2     = uint16(45678);//0xB26E
us3     = uint16(123456);//0x0001E240
s1      = int16(-123); //0x85
s2      = int16(-23456);//0xA460
s3      = int16(-123456);//0xFFFE1DC0
s3t     = int16(7616);//0x1DC0

//unsigned long big endian
writedata("ulb", ["00" "00" "00" "00" "00" "00" "00" "7B" "00" "00" "00" "00" "00" "00" "B2" "6E" "00" "00" "00" "00" "00" "01" "E2" "40"]);
//unsigned long little endian
writedata("ull", ["7B" "00" "00" "00" "00" "00" "00" "00" "6E" "B2" "00" "00" "00" "00" "00" "00" "40" "E2" "01" "00" "00" "00" "00" "00"]);
//unsigned short big endian
writedata("usb", ["00" "7B" "B2" "6E" "E2" "40"]);
//unsigned short little endian
writedata("usl", ["7B" "00" "6E" "B2" "40" "E2"]);

//long big endian
writedata("lb", ["FF" "FF" "FF" "FF" "FF" "FF" "FF" "85" "FF" "FF" "FF" "FF" "FF" "FF" "A4" "60" "FF" "FF" "FF" "FF" "FF" "FE" "1D" "C0"]);
//long little endian
writedata("ll", ["85" "FF" "FF" "FF" "FF" "FF" "FF" "FF" "60" "A4" "FF" "FF" "FF" "FF" "FF" "FF" "C0" "1D" "FE" "FF" "FF" "FF" "FF" "FF"]);
//short big endian
writedata("sb", ["FF" "85" "A4" "60" "1D" "C0"]);
//short little endian
writedata("sl", ["85" "FF" "60" "A4" "C0" "1D"]);

res = readdata("ulb");
assert_checkequal(res, [ul1, ul2, ul3]);
res = readdata("ull");
assert_checkequal(res, [ul1, ul2, ul3]);
res = readdata("usb");
assert_checkequal(res, [us1, us2, us3]);
res = readdata("usl");
assert_checkequal(res, [us1, us2, us3]);

res = readdata("lb");
assert_checkequal(res, [l1, l2, l3]);
res = readdata("ll");
assert_checkequal(res, [l1, l2, l3]);
res = readdata("sb");
assert_checkequal(res, [s1, s2, s3]);
res = readdata("sl");
assert_checkequal(res, [s1, s2, s3]);

// check default output type of mgeti
file1 = fullfile(TMPDIR,'test1.bin');
fd1=mopen(file1,'wb');
mput(1996,'ull',fd1);
mclose(fd1);

fd1=mopen(file1,'rb');
a=mgeti(1);
assert_checkequal(typeof(a), "int32");
assert_checkequal(a, int32(1996));
