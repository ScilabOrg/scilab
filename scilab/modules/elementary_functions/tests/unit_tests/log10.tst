// 1. Interface
// ============

if execstr("log10()"   ,"errcatch") == 0 then pause, end
if execstr("log10(1,2)","errcatch") == 0 then pause, end


// 2. Singular Values
// ==================

// List of variables used

// hexInp		=> one column matrix of inputs in hexadecimal
// hexOut		=> one column matrix of expected result in hexadecimal
// lengthInp		=> size of hexInp i.e. the number of Input cases
// doubleOut	 	=> the converted decimal values(double) of hexOut
// computedResult	=> computed sine values of the data in hexInp
// computedDigits 	=> number of digits common in the computed and expected results
// expectedValue	=> the number of digits we expect to be common among the computed and exptected results for a positive result
// expectedDigits 	=> a one column matrix of expectedValue


hexInp = [
'3FF0000000000000'
'4024000000000000'
'4059000000000000'
'408F400000000000'
'40C3880000000000'
'40F86A0000000000'
'412E848000000000'
'416312D000000000'
'4197D78400000000'
'41CDCD6500000000'
'4202A05F20000000'
'42374876E8000000'
'426D1A94A2000000'
'42A2309CE5400000'
'42D6BCC41E900000'
'430C6BF526340000'
'4341C37937E08000'
'4376345785D8A000'
'43ABC16D674EC800'
'43E158E460913D00'
'4415AF1D78B58C40'
'444B1AE4D6E2EF50'
'4480F0CF064DD592'
'44B52D02C7E14AF6'
'44EA784379D99DB4'
'45208B2A2C280291'
'5ACE12D66744FF81'
'403CE41D8FA665FA'
'3FE62410EB7B7E10'
'3FEB0CF736F1AE1D'
'3FF89825F74AA6B7'
'4011705AF708C532'
'40209732BC3FB6F0'
'400819598B70B769'
'40296C0B463D632E'
'4060214115C6897E'
'406E5B46CC566C89'
'40B13AEDB3538379'
'41FC981659F2CA60'
'421FE67E6C6CEB38'
'424E16ED4CE49996'
'4253F00F03C41303'
'42751ED94D282C63'
'43BA275C0B3D6B93'
'459ACF0197DF0564'
'4722975C05D77D0C'
'498691810A4906EB'
'49C758976044BFD1'
'538AA8CCE883305B'
'53D6848181B7571C'
'63F4D0F00313488C'
'6443E238630D4B3B'
];

hexOut = [
'0000000000000000'
'3FF0000000000000'
'4000000000000000'
'4008000000000000'
'4010000000000000'
'4014000000000000'
'4018000000000000'
'401C000000000000'
'4020000000000000'
'4022000000000000'
'4024000000000000'
'4026000000000000'
'4028000000000000'
'402A000000000000'
'402C000000000000'
'402E000000000000'
'4030000000000000'
'4031000000000000'
'4032000000000000'
'4033000000000000'
'4034000000000000'
'4035000000000000'
'4036000000000000'
'4037000000000000'
'4038000000000000'
'4039000000000000'
'40602d4f53729e45'
'3ff75f49c6ad3bad'
'BFC479681C44DD78'
'BFB2AE5057CD8C44'
'3FC7E646F3FAB0D1'
'3FE476724BCF05C3'
'3FED67138D8E1FAB'
'3FDEA673C9A0BC62'
'3FF1AAB931FC4A83'
'4000E2C2079F4791'
'4003152E63907254'
'400D27FF5AE16374'
'4023C52DF27C62B2'
'402511C39D418079'
'4026D326D60C8089'
'402711C39D418079'
'402852D55DCA3530'
'40324674679EFEEE'
'403B511D58E9BF6E'
'4041578173B49D48'
'40471A7DFB3F2590'
'4047B680B5DE7EB4'
'40579C6C70A21751'
'4057F80F69D57ADE'
'4065B03DEAA9ABB2'
'4065DFC50D1EF669'
];


lengthInp = size(hexInp);

// The double values of hex digits given in hexOut
doubleOut = zeros(lengthInp(1,1), lengthInp(1,2));

// The result we compute using the inputs in hexInp
computedResult = zeros(lengthInp(1,1), lengthInp(1,2));

//The number of digits common among the computed and expected results
computedDigits = zeros(lengthInp(1,1), lengthInp(1,2));

// the number of siginificant digits we wish to be matched for a positive result
expectedValue = 51;

expectedDigits = zeros(lengthInp(1,1), lengthInp(1,2)) + expectedValue;

for i = 1:1:lengthInp(1,1)
	computedResult(i,1) = log10(flps_hex2double(hexInp(i,1)));
end

for i = 1:1:lengthInp(1,1)
	doubleOut(i,1) = flps_hex2double(hexOut(i,1));
end

computedDigits = assert_computedigits(computedResult, doubleOut, 2);

assert_checktrue(computedDigits > expectedDigits);


// 3. Not A Number
// ===============

//if ~isnan(log10(%nan)) then pause, end
//if ~isnan(log10(-%nan)) then pause, end


// 4. Limit values
// ===============


// 5. Properties
