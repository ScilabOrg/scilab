sys.path.append("tests/graphic_tests")
import commonScilabStart

click("1297931922922.png")
click("Untitled1.png")
paste("2+42")
type("l",KEY_CTRL)
click(Pattern("FilaEdit.png").targetOffset(-19,0))
click(Pattern("sc.png").targetOffset(-116,6))
click("1297932329690.png")
if not exists("242BHS44.png"):
	print "could not found the text 44"
click("1297932418868.png")
click("1297932428914.png")
# focus on the console
click("Startupexecu.png")
paste("a=[2,42; 32, 3];\n")
paste("disp(a);\n")
dragDrop("8.png", "2.png")
click("1297932560233.png")
click("1297932586980.png")
paste("\n;clc();\n")
paste("disp(a*2);\n")
if not exists("8464.png"):
	print "could not found the text"
import commonScilabStop

