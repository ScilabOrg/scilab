# Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
# Copyright (C) 2004 - INRIA - Fabrice Leray
# 
#
# This file must be used under the terms of the CeCILL.
# This source file is licensed as described in the file COPYING, which
# you should have received as part of this distribution.  The terms
# are also available at    
# http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

set pwd [pwd]
cd [file dirname [info script]]
variable DEMODIR [pwd]
cd $pwd



variable DEMODIR

lappend ::auto_path [file dirname  "$env(SCIPATH)/modules/tclsci/tcl/BWidget"]
namespace inscope :: package require BWidget
package require BWidget


set sourcedir [file join "$env(SCIPATH)" "modules" "tclsci" "tcl" "utils"]
set sourcedir2 [file join "$env(SCIPATH)" "modules" "graphics" "tcl" "ged"]

source [file join $sourcedir Notebook.tcl]
source [file join $sourcedir Combobox.tcl]
source [file join $sourcedir Balloon.tcl]
source [file join $sourcedir2 ObjectsBrowser.tcl]

package require combobox 2.3
catch {namespace import combobox::*}

#package require lemonTree
catch {namespace import LemonTree::*}

global envSCIHOME MAIN_WINDOW_POSITION TICK_WINDOW_POSITION msdos
set ged_listofpref "MAIN_WINDOW_POSITION TICK_WINDOW_POSITION"
set MAIN_WINDOW_POSITION "+0+0"
set TICK_WINDOW_POSITION "+0+0"

set envSCIHOME $SCIHOME
set preffilename $envSCIHOME/.GedPreferences.tcl
catch {source $preffilename}


global SELOBJECT
global ged_handle_list_size
global lalist
global curgedindex
global curgedobject

global curdata
global curvis
global curarrowsize curthick curlinestyle segsVAL nbcolsegscolor segscolorVAL
global curmarkmode curlinemode curmarksize curmarksizeunit curmarkforeground curmarkbackground
global RED GREEN BLUE ncolors
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox letext
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox

#To update foreground color grey ("off"), black ("on") for checkbutton boxes
proc OnOffForeground { frame flag } {
    
    if { $flag == "on" } {
	$frame configure -foreground black
    } else {
	$frame configure -foreground grey
    }
}

set NBheight 320
set NBwidth  295

set Wheight [expr $NBheight + 115]
set Wwidth  [expr $NBwidth  + 270]

set smallPad  4
set mediumPad 8

#create the font we will use
set gedFont {Arial -12}

set ww .axes
catch {destroy $ww}

toplevel $ww
wm title $ww "Segs Object"
wm iconname $ww "SO"
wm geometry $ww [expr $Wwidth]x[expr $Wheight]$MAIN_WINDOW_POSITION
wm maxsize  $ww $Wwidth $Wheight
wm protocol $ww WM_DELETE_WINDOW "DestroyGlobals; destroy $ww "

set topf  [frame $ww.topf]
set titf1 [TitleFrame $topf.titf1 -text "Graphic Editor" -font $gedFont]

set parent  [$titf1 getframe]
set pw1  [PanedWindow $parent.pw -side top]
set pane3  $pw1  



# Make a frame scrollable

set sw [ScrolledWindow $pane3.sw -relief sunken -borderwidth 2]
# pack $sw -in .sw -fill both -expand true 
set sf [ScrollableFrame $sw.f]
$sw setwidget $sf
set uf [$sf getframe]


set w $uf
set fra [frame $w.frame -borderwidth 0]
pack $fra  -anchor w -fill both

#------------------------------------------------

set theframe $fra

#adding 15.06.2005
set topflabel  [frame $theframe.topflabel]
set titf1label [TitleFrame $topflabel.titflabel1 -text "Objects Browser" -font $gedFont]
set titf1axes  [TitleFrame $topflabel.titfaxes1 -text "Object Properties" -font $gedFont]

set w [$titf1label getframe]

pack $titf1label -padx 4 -side left -fill both -expand yes
pack $topflabel -fill x -pady 0
pack $titf1axes  -pady 0 -padx 4 -fill both -expand yes

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both
#end adding


set wfortree $w

for {set i 1} {$i<=$ged_handle_list_size} {incr i} { 
    set OBJECTSARRAY($i) $SELOBJECT($i)
}

set curgedobject $SELOBJECT($curgedindex)

set tree  [Tree $wfortree.tree \
	       -yscrollcommand {$wfortree.y set} -xscrollcommand {$wfortree.x set} \
	       -width 20 -height 10 \
	       -background white -opencmd {LemonTree::open $wfortree.tree} \
	       -selectbackground blue -selectforeground white ]

pack [scrollbar $wfortree.x -orient horiz -command {$wfortree.tree xview}] -side bottom -fill x
pack [scrollbar $wfortree.y -command {$wfortree.tree yview}] -side right -fill y
pack $tree -fill both -expand 1 -side left

$tree bindText  <1> {LemonTree::Info $tree}
$tree bindImage <1> {LemonTree::Info $tree}

LemonTree::add $tree root FIGURE    currentfigure  "Figure(1)"

# I open the tree to browse all the nodes (to know what nodes I have and what their names are)
$tree opentree n1

set allnodes [$tree selection get]

#I close quickly the tree because openreeatnode expanded the tree...
$tree closetree n1


# I directly point onto the current curgedobject (current Axes or Figure or picked entity)
LemonTree::finddata $tree $allnodes $curgedobject


#adding 15.06.2005
set w [$titf1axes getframe]

set uf $w
#------------------------------------------------

set largeur 14

Notebook:create $uf.n -pages {Style Data Color Clipping} -pad 0 -height $NBheight -width $NBwidth
pack $uf.n -in $uf -fill both -expand 1

########### Style onglet ##########################################
###################################################################
set w [Notebook:frame $uf.n Style]

frame $w.frame -borderwidth 0
pack $w.frame -anchor w -fill both

#visibility
frame $w.frame.vis -borderwidth 0
pack $w.frame.vis  -in $w.frame  -side top -fill x 
label $w.frame.vislabel  -text "Visibility:" -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.visib  -text "on"\
    -variable curvis  -onvalue "on" -offvalue "off" \
    -command "toggleVis $w.frame.visib" -font $gedFont
OnOffForeground $w.frame.visib $curvis

pack $w.frame.vislabel -in $w.frame.vis  -side left
pack $w.frame.visib  -in $w.frame.vis    -side left -fill x -padx $smallPad

#Line mode
frame $w.frame.linelinemode  -borderwidth 0
pack $w.frame.linelinemode  -in $w.frame  -side top  -fill x

label $w.frame.linemodelabel -height 0 -text "Line mode:" -width 0  -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.linemode  -text "on" -indicatoron 1 \
    -variable curlinemode -onvalue "on" -offvalue "off" \
    -command "toggleLinemode $w.frame.linemode" -font $gedFont
OnOffForeground $w.frame.linemode $curlinemode

pack $w.frame.linemodelabel  -in $w.frame.linelinemode  -side left 
pack $w.frame.linemode   -in $w.frame.linelinemode   -side left  -fill x -pady 0 -padx $smallPad


#Line Style
frame $w.frame.linest  -borderwidth 0
pack $w.frame.linest  -in $w.frame  -side top  -fill x

label $w.frame.stylelabel  -height 0 -text "Line:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.style \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 14 \
    -textvariable curlinestyle \
    -editable false \
    -command [list SelectLineStyle ] -font $gedFont
eval $w.frame.style list insert end [list "solid" "dash" "dash dot" "longdash dot" "bigdash dot" "bigdash longdash" "dot" "double dot"]

#Add thickness here
combobox $w.frame.thickness \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 4 \
    -textvariable curthick \
    -editable true \
    -command [list SelectThickness ] -font $gedFont
eval $w.frame.thickness list insert end [list "0.0" "1.0" "2.0" "3.0" "4.0" "5.0" "6.0" "8.0" "10.0"]


pack $w.frame.stylelabel -in $w.frame.linest   -side left
pack $w.frame.style   -in $w.frame.linest   -side left -padx $mediumPad
pack $w.frame.thickness  -in $w.frame.linest  -expand 1 -pady 0 -padx $smallPad

#Arrow size
frame $w.frame.ar -borderwidth 0
pack $w.frame.ar  -in $w.frame -side top   -fill x

label $w.frame.arrowlab -text "Arrow size:" -font $gedFont -anchor e -width $largeur
entry $w.frame.arrow -relief sunken  -textvariable curarrowsize -width 10 -font $gedFont
pack  $w.frame.arrowlab -in $w.frame.ar -side left
pack  $w.frame.arrow -in $w.frame.ar -side left -fill x -pady 0 -padx $mediumPad 
bind  $w.frame.arrow <Return> {SelectArrowSize}
bind  $w.frame.arrow <KP_Enter> {SelectArrowSize}
bind  $w.frame.arrow <FocusOut> {SelectArrowSize}


#Mark mode
frame $w.frame.linemarkmode  -borderwidth 0
pack $w.frame.linemarkmode  -in $w.frame  -side top  -fill x

label $w.frame.markmodelabel -height 0 -text "Mark mode:" -width 0  -font $gedFont -anchor e -width $largeur
checkbutton $w.frame.markmode  -text "on" -indicatoron 1 \
    -variable curmarkmode -onvalue "on" -offvalue "off" \
    -command "toggleMarkmode $w.frame.markmode" -font $gedFont
OnOffForeground $w.frame.markmode $curmarkmode

pack $w.frame.markmodelabel  -in $w.frame.linemarkmode  -side left 
pack $w.frame.markmode   -in $w.frame.linemarkmode   -side left  -fill x -pady 0 -padx $smallPad


#Mark style
frame $w.frame.linemarkst  -borderwidth 0
pack $w.frame.linemarkst  -in $w.frame  -side top  -fill x

label $w.frame.markstylelabel  -height 0 -text "Mark style:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.markstyle \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curmarkstyle \
    -editable false \
    -command [list SelectMarkStyle ] -font $gedFont
eval $w.frame.markstyle list insert end [list "dot" "plus" "cross" "star" "filled diamond" "diamond" "triangle up" "triangle down" "diamond plus" "circle" "asterisk" "square" "triangle right" "triangle left" "pentagram"]


pack $w.frame.markstylelabel  -in $w.frame.linemarkst   -side left
pack $w.frame.markstyle   -in $w.frame.linemarkst   -expand 1 -fill x -pady 0 -padx $mediumPad


#Mark size
frame $w.frame.mksize  -borderwidth 0
pack $w.frame.mksize  -side top -fill x

label $w.frame.marksizelabel -height 0 -text "Mark size:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w.frame.marksize \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curmarksize \
    -editable true \
    -command [list SelectMarkSize ] -font $gedFont
eval $w.frame.marksize list insert end [list "0.5" "1.0" "2.0" "3.0" "4.0" "6.0" "8.0" "10.0" "15.0" "20.0" "25.0" "30.0"]


#Add Mark size unit
label $w.frame.marksizeunitlabel  -height 0 -text "Mark size:" -width 0 -font $gedFont -anchor e -width $largeur
combobox $w.frame.marksizeunit \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 8 \
    -textvariable curmarksizeunit \
    -editable false \
    -command [list SelectMarkSizeUnit ] -font $gedFont
eval $w.frame.marksizeunit list insert end [list "point" "tabulated"]

pack  $w.frame.marksizeunitlabel -in $w.frame.mksize -side left
pack  $w.frame.marksizeunit -in $w.frame.mksize -side left -padx $mediumPad
pack  $w.frame.marksize -in $w.frame.mksize -side left  -fill x  -expand 1 -pady 0 -padx $mediumPad

#Mark foreground
frame $w.frame.markf  -borderwidth 0
pack $w.frame.markf  -in $w.frame -side top  -fill x

label $w.frame.markflabel -height 0 -text "Mark foreground:" -width 0 -font $gedFont -anchor e -width $largeur
scale $w.frame.markforeground -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setMarkForeground $w.frame.markforeground" -tickinterval 0  -font $gedFont

pack $w.frame.markflabel -in $w.frame.markf -side left
pack $w.frame.markforeground  -in  $w.frame.markf -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.markforeground set $curmarkforeground


#Mark background
frame $w.frame.markb  -borderwidth 0
pack $w.frame.markb  -in $w.frame -side top  -fill x

label $w.frame.markblabel -height 0 -text "Mark background:" -width 0 -font $gedFont -anchor e -width $largeur
scale $w.frame.markbackground -orient horizontal -from -2 -to $ncolors \
	 -resolution 1.0 -command "setMarkBackground $w.frame.markbackground" -tickinterval 0  -font $gedFont

pack $w.frame.markblabel -in $w.frame.markb -side left
pack $w.frame.markbackground  -in  $w.frame.markb -side left -expand 1 -fill x -pady 0 -padx $smallPad
$w.frame.markbackground set $curmarkbackground

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both

#exit button
frame $w.buttons
pack $w.buttons -side bottom -fill x -pady 0
button $w.buttons.dismiss -text Quit -command "destroy $ww"  -font $gedFont
pack $w.buttons.dismiss  -side bottom -expand 1


########### Data onglet ###########################################
###################################################################
set w [Notebook:frame $uf.n Data]

frame $w.frame2 -borderwidth 0
pack $w.frame2 -anchor w -fill both


frame $w.frame2.curdataframeX  -borderwidth 0
pack $w.frame2.curdataframeX  -in $w.frame2  -side top  -fill x

label $w.frame2.polydatalabelX  -height 0 -text "X and Y Data:" -width 0   -font $gedFont -anchor e -width $largeur
combobox $w.frame2.polydataX \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 3 \
    -textvariable curdata \
    -editable false \
    -command [list SelectData ]  -font $gedFont
eval $w.frame2.polydataX list insert end [list $curdata "----" "Edit data..."]
pack $w.frame2.polydatalabelX -in $w.frame2.curdataframeX  -side left
pack $w.frame2.polydataX   -in $w.frame2.curdataframeX  -expand 1 -fill x -pady 0 -padx $mediumPad

#sep bar
frame $w.sep -height 2 -borderwidth 1 -relief sunken
pack $w.sep -fill both


#exit button
frame $w.buttons
button $w.b -text Quit -command "DestroyGlobals; destroy $ww"  -font $gedFont
pack $w.b -side bottom 


########### Color vector onglet ###################################
###################################################################
set w2 [Notebook:frame $uf.n Color]

frame $w2.frame2 -borderwidth 0
pack $w2.frame2 -anchor w -fill both

frame $w2.frame2.fdata -borderwidth 0
pack $w2.frame2.fdata  -in $w2.frame2 -side top -fill x


canvas $w2.frame2.c1 -width 310 -height 265 -yscrollcommand {$w2.frame2.ysbar set}
scrollbar $w2.frame2.ysbar -orient vertical -command   {$w2.frame2.c1 yview}

$w2.frame2.c1 create text 70 10 -anchor c -text "Segs color" -font $gedFont -anchor w

for {set i 1} {$i<=$nbcolsegscolor} {incr i} {
    set bb [expr 10+(25*$i)]
    $w2.frame2.c1 create text 10 $bb -anchor c -text $i
    entry  $w2.frame2.c1.data$i  -relief sunken  -textvariable segscolorVAL($i) -width 10 -font $gedFont
    bind  $w2.frame2.c1.data$i <Return> "setSegsColorData $i "
    bind  $w2.frame2.c1.data$i <KP_Enter> "setSegsColorData $i "
    bind  $w2.frame2.c1.data$i <FocusOut> "setSegsColorData $i "
#location help balloon	
    set_balloon $w2.frame2.c1.data$i "Row: $i"
 
    $w2.frame2.c1 create window 70 $bb -anchor c -window $w2.frame2.c1.data$i
    
}

$w2.frame2.c1 configure -scrollregion [$w2.frame2.c1 bbox all] -yscrollincrement 0.1i

pack  $w2.frame2.ysbar -side right -fill y
pack  $w2.frame2.c1

#sep bar
frame $w2.sep -height 2 -borderwidth 1 -relief sunken
pack $w2.sep -fill both


#exit button
frame $w2.buttons
button $w2.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
pack $w2.b -side bottom 


########### Clipping onglet #######################################
###################################################################

set w9 [Notebook:frame $uf.n Clipping]

frame $w9.frame -borderwidth 0
pack $w9.frame -anchor w -fill both

set letext ""

#Clip state
#frame $w9.frame.clpwarning  -borderwidth 0

frame $w9.frame.clpstat  -borderwidth 0
pack $w9.frame.clpstat  -in $w9.frame -side top -fill x -pady 0

label $w9.frame.cliplabel  -height 0 -text "Clip state:" -width 0  -font $gedFont -anchor e -width $largeur
combobox $w9.frame.clip \
    -borderwidth 1 \
    -highlightthickness 1 \
    -maxheight 0 \
    -width 8 \
    -textvariable curclipstate\
    -editable false \
    -command [list SelectClipState ] -font $gedFont
eval $w9.frame.clip list insert end [list "on" "off" "clipgrf"]

pack $w9.frame.cliplabel -in $w9.frame.clpstat   -side left
pack $w9.frame.clip -in $w9.frame.clpstat  -side left -pady 0 -padx $mediumPad

#clip box
frame $w9.frame.lb1 -borderwidth 0
pack $w9.frame.lb1  -in $w9.frame -side top   -fill x
label $w9.frame.labelul -text "Clip box : upper-left point coordinates" -font $gedFont
pack $w9.frame.labelul -in  $w9.frame.lb1 -side left

frame $w9.frame.lb2 -borderwidth 0
pack $w9.frame.lb2  -in $w9.frame -side top   -fill x

frame $w9.frame.lb21 -borderwidth 0
pack $w9.frame.lb21  -in $w9.frame -side top   -fill x

frame $w9.frame.lb22 -borderwidth 0
pack $w9.frame.lb22  -in $w9.frame -side top   -fill x

label $w9.frame.labelx -text "X:" -font $gedFont  -anchor e -width $largeur
entry $w9.frame.datax -relief sunken  -textvariable Xclipbox -width 10  -font $gedFont
label $w9.frame.labely -text "Y:" -font $gedFont -anchor e -width $largeur
entry $w9.frame.datay -relief sunken  -textvariable Yclipbox -width 10  -font $gedFont

pack $w9.frame.labelx  -in  $w9.frame.lb2 -side left 
pack $w9.frame.datax   -in  $w9.frame.lb2 -side left -pady 0 -padx $mediumPad
pack $w9.frame.labely  -in  $w9.frame.lb21 -side left 
pack $w9.frame.datay   -in  $w9.frame.lb21 -side left -pady 0 -padx $mediumPad
bind  $w9.frame.datax <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datay <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datax <KP_Enter> "SelectClipBox $w9.frame"
bind  $w9.frame.datay <KP_Enter> "SelectClipBox $w9.frame"

#----------------------------#
frame $w9.frame.lb3 -borderwidth 0
pack $w9.frame.lb3  -in $w9.frame -side top   -fill x
label $w9.frame.labelwh -text "Clip box : width and height" -font $gedFont
pack $w9.frame.labelwh -in  $w9.frame.lb3 -side left

frame $w9.frame.lb4 -borderwidth 0
pack $w9.frame.lb4  -in $w9.frame -side top   -fill x

frame $w9.frame.lb41 -borderwidth 0
pack $w9.frame.lb41  -in $w9.frame -side top   -fill x

label $w9.frame.labelw -text "W:" -font $gedFont -anchor e -width $largeur
entry $w9.frame.dataw -relief sunken  -textvariable Wclipbox -width 10  -font $gedFont
label $w9.frame.labelh -text "H:" -font $gedFont -anchor e -width $largeur
entry $w9.frame.datah -relief sunken  -textvariable Hclipbox -width 10  -font $gedFont

pack $w9.frame.labelw  -in  $w9.frame.lb4 -side left 
pack $w9.frame.dataw   -in  $w9.frame.lb4 -side left -pady 0 -padx $mediumPad
pack $w9.frame.labelh  -in  $w9.frame.lb41 -side left 
pack $w9.frame.datah   -in  $w9.frame.lb41 -side left -pady 0 -padx $mediumPad
bind  $w9.frame.dataw <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.datah <Return> "SelectClipBox $w9.frame"
bind  $w9.frame.dataw <KP_Enter> "SelectClipBox $w9.frame"
bind  $w9.frame.datah <KP_Enter> "SelectClipBox $w9.frame"


frame $w9.frame.warning
label $w9.frame.mesgwarning  -justify left -textvariable letext -font $gedFont
$w9.frame.mesgwarning config -foreground red
pack $w9.frame.mesgwarning -in $w9.frame.warning
pack $w9.frame.warning -in $w9.frame

#sep bar
frame $w9.sep -height 2 -borderwidth 1 -relief sunken
pack $w9.sep -fill both


#exit button
frame $w9.buttons
button $w9.b -text Quit -command "DestroyGlobals; destroy $ww" -font $gedFont
pack $w9.b -side bottom


pack $sw $pw1 -fill both -expand yes
pack $titf1 -padx 4 -side left -fill both -expand yes
pack $topf -fill both -pady 2 -expand yes


#proc associes
proc toggleVis { frame } {
    global curvis
    ScilabEval "global ged_handle;ged_handle.visible='$curvis'"

    OnOffForeground $frame $curvis
}

proc SelectArrowSize {} {
global curarrowsize
ScilabEval "global ged_handle;ged_handle.arrow_size=$curarrowsize"
}

proc SelectLineStyle {w args} {
global curlinestyle
ScilabEval "setLineStyle('$curlinestyle')"
}



proc setThickness {w thick} {
ScilabEval "global ged_handle;ged_handle.thickness=$thick;"
}

proc SelectMarkStyle {w args} {
global curmarkstyle
global curmarkmode
ScilabEval "setMarkStyle('$curmarkstyle')"

set curmarkmode "on"
#ScilabEval "global ged_handle;ged_handle.mark_mode='$curmarkmode'"

}

proc toggleMarkmode { frame } {
    global curmarkmode
    ScilabEval "global ged_handle;ged_handle.mark_mode='$curmarkmode'"

    OnOffForeground $frame $curmarkmode
}

#Added on the 21.01.05
proc toggleLinemode { frame } {
    global curlinemode
    ScilabEval "global ged_handle;ged_handle.line_mode='$curlinemode'"

    OnOffForeground $frame $curlinemode
}


proc setData { i j } {
global segsVAL
ScilabEval "execstr(\"global ged_handle; ged_handle.data($i,$j)=$segsVAL($i,$j);\",\'errcatch\',\'n\');"
}



proc setSegsColorData { i } {
    global segscolorVAL
    ScilabEval "execstr(\"global ged_handle; tmp = ged_handle.segs_color; tmp($i)=$segscolorVAL($i);ged_handle.segs_color = tmp;\",\'errcatch\',\'n\');"
}



#Clipping proc for all entities (clip box and clip state fields)
proc SelectClipBox { w } {
    global Xclipbox Yclipbox Wclipbox Hclipbox curclipstate letext
    if { ($Xclipbox == "") || ($Yclipbox == "") ||
	 ($Wclipbox == "") || ($Hclipbox == "") } {
	if { [info exists  text] } {
	    unset text
	}
	set letext "Note that all clip box fields must be filled."
	
    } else {
	if { [info exists  text] } {
	    unset text
	}
	set letext ""
	
	ScilabEval "global ged_handle;ged_handle.clip_box=\[$Xclipbox $Yclipbox $Wclipbox $Hclipbox\]"
	set curclipstate "on"
    }
}

proc SelectClipState {w args} {
global curclipstate Xclipbox Yclipbox Wclipbox Hclipbox
global old_Xclipbox old_Yclipbox old_Wclipbox old_Hclipbox
ScilabEval "global ged_handle;ged_handle.clip_state='$curclipstate';"
    if { $curclipstate == "off" } {
	set old_Xclipbox $Xclipbox
	set old_Yclipbox $Yclipbox
	set old_Wclipbox $Wclipbox
	set old_Hclipbox $Hclipbox
	set Xclipbox ""
	set Yclipbox ""
	set Wclipbox ""
	set Hclipbox ""
    } else {
	set Xclipbox $old_Xclipbox
	set Yclipbox $old_Yclipbox
	set Wclipbox $old_Wclipbox
	set Hclipbox $old_Hclipbox
    }
}


proc SelectData  {w args} {
    global curdata
    variable mycurdata
    set mycurdata $curdata
    set finddbarray -1
    set dbarray "double array"
    set finddbarray [expr [string first $dbarray $mycurdata]]
#    puts "finddbarray = $finddbarray"

    if { ($mycurdata == "----") || ($finddbarray != -1) } {
	#	puts "nothing to do"
    } else {
	if { $mycurdata ==  "Edit data..." } {
	    ScilabEval "global ged_handle;EditData(ged_handle.data,\"ged_handle.data\")" "seq"
	}
    }
}


proc SavePreferences { } {
    global ged_listofpref
    global envSCIHOME MAIN_WINDOW_POSITION TICK_WINDOW_POSITION
    global ww msdos
    
    ScilabEval "DestroyGlobals()" "seq"
        
    if { $msdos == "F" } {
#unix mandrake (at least) needs this offset
#test other unix distribution to see (red hat, suse...)
	set xoffset -5
	set yoffset -26
    } else {
	set xoffset 0
	set yoffset 0
    }
    
    set x [eval {winfo x $ww}]
    set y [eval {winfo y $ww}]
    set MAIN_WINDOW_POSITION "+[expr $x+$xoffset]+[expr $y+$yoffset]"
    
    #save preferences (position...)
    set preffilename [file join $envSCIHOME .GedPreferences.tcl]
    catch {
 	set preffile [open $preffilename w]
	foreach opt $ged_listofpref {
	    global $opt
	    puts $preffile [concat "set $opt " [set $opt]]
	    # 	    puts [concat "set $opt" [set $opt]]
	}
	close $preffile
    }
}

proc DestroyGlobals { } {
    ScilabEval "DestroyGlobals()" "seq"

    SavePreferences
}


proc setMarkForeground {w index} {   
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.mark_foreground <> $index then ged_handle.mark_foreground=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.mark_foreground <> $index then ged_handle.mark_foreground=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.mark_foreground <> $index then ged_handle.mark_foreground=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
		
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.mark_foreground <> $index then ged_handle.mark_foreground=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}



proc setMarkBackground {w index} {   
    global RED BLUE GREEN
    variable REDCOL 
    variable GRECOL 
    variable BLUCOL
    
    #ScilabEval "global ged_handle;"
    if { $index == -2 } {
	ScilabEval "global ged_handle; if ged_handle.mark_background <> $index then ged_handle.mark_background=$index; end;"
	#like $index==-2: display white color
	set color [format \#%02x%02x%02x 255 255 255]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == -1 } {
	ScilabEval "global ged_handle; if ged_handle.mark_background <> $index then ged_handle.mark_background=$index; end;"
	#like $index==-1: display black color
	set color [format \#%02x%02x%02x 0 0 0]
	$w config  -activebackground $color -troughcolor $color
    } elseif { $index == 0 } {
	ScilabEval "global ged_handle; if ged_handle.mark_background <> $index then ged_handle.mark_background=$index; end;"
	#like $index==1: display first color
	set REDCOL $RED(1) 
	set GRECOL $GREEN(1) 
	set BLUCOL $BLUE(1) 
		
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
    } else { 
	ScilabEval "global ged_handle; if ged_handle.mark_background <> $index then ged_handle.mark_background=$index; end;"
	
	set REDCOL $RED($index) 
	set GRECOL $GREEN($index) 
	set BLUCOL $BLUE($index) 
	
	set color [format \#%02x%02x%02x [expr int($REDCOL*255)]  [expr int($GRECOL*255)]  [expr int($BLUCOL*255)]]
	
	$w config  -activebackground $color -troughcolor $color
	
    }
}



proc SelectMarkSizeUnit {w args} {
    global curmarksizeunit
    ScilabEval "global ged_handle;ged_handle.mark_size_unit='$curmarksizeunit'"
}



proc SelectMarkSize {w args} {
    global curmarksize
    ScilabEval "global ged_handle;ged_handle.mark_size=$curmarksize"
}


proc SelectThickness {w args} {
    global curthick
    ScilabEval "global ged_handle;ged_handle.thickness=$curthick;"
}
