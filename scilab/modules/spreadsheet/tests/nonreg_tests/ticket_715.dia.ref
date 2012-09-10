// =============================================================================
// =============================================================================
// <-- JVM NOT MANDATORY -->
// =============================================================================
// <-- Non-regression test for bug 715 -->
//
// <-- URL -->
//  http://forge.scilab.org/index.php/p/csv-readwrite/issues/715/
//
// <-- Short Description -->
// Issue 715: csv_textscan crashed on an example.
// =============================================================================
PATH = SCI+"/modules/spreadsheet/tests/nonreg_tests/";
file_name = 'ticket_715.txt';
M1 = csv_read(PATH + '/' + file_name,[],[],"string");
DATA1 = csv_textscan(M1," ",".");
// =============================================================================
