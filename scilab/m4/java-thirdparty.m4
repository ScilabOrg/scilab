dnl
dnl Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
dnl Copyright (C) DIGITEO - 2010 - Sylvestre Ledru
dnl Copyright (C) Scilab Enterprises - 2015 - Clement David
dnl 
dnl This file must be used under the terms of the CeCILL.
dnl This source file is licensed as described in the file COPYING, which
dnl you should have received as part of this distribution.  The terms
dnl are also available at    
dnl http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
dnl

#------------------------------------------------------------------------
# AC_JAVA_CHECK_VERSION_PACKAGE(NAME, IMPORTS, JAR, MIN_VERSION, CURRENT_VERSION, [PRE_PROCESSING], [GREATER_OR_EQUALS])
#
# Check if the minimal version of a software/package is available or not.
# Note that since java does not provide an universal mechanism to detect
# the version of a package, some work is left to the user.
# I will update this work when OSGI is more supported.
#
# Arguments:
#    1. The name of the package (only used in the display of feedbacks)
#    2. Which imports are necessary in the check
#    3. The name of the jar files used to build against
#    4. What is the minimal version expected
#    5. The Java code to get the version ... This can be a serie of java
#       instructions
#   6. Pre processing to reformat the values
#   7. Specify if we want the exact version or greater. (equals or greater by
#      default).
#
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_CHECK_VERSION_PACKAGE], [
    AC_MSG_CHECKING([minimal version ($4) of $1])
   saved_ac_java_classpath=$ac_java_classpath
   export ac_java_classpath="$3:$ac_java_classpath"
   if test "x$7" == "x"; then
   AC_JAVA_TRY_COMPILE($2, [String minVersion="$4";
                            $6
                            System.out.println($5);
                                  if (compare(minVersion, $5) > 0) {
                                   System.exit(-1);
                                } ]
                            , "yes", echo "yes" , AC_MSG_ERROR([Wrong version of $1. Expected at least $4. Found $STDOUT]))
   else
   AC_JAVA_TRY_COMPILE($2, [String minVersion="$4";
                            $6
                            System.out.println($5);
                                  if (compare(minVersion, $5) != 0) {
                                   System.exit(-1);
                                } ]
                            , "yes", echo "yes" , AC_MSG_ERROR([Wrong version of $1. Expected exact version $4. Found $STDOUT]))
    fi
   ac_java_classpath=$saved_ac_java_classpath
])

#------------------------------------------------------------------------
# AC_JAVA_CHECK_VERSION_MANIFEST(NAME, JAR, MIN_VERSION, [PRE_PROCESSING], [GREATER_OR_EQUALS])
#
# Check if the minimal version of a software/package is available or not.
# Note that since java does not provide an universal mechanism to detect
# the version of a package, we assume that the "Specification-Version" of
# the MANIFEST.MF is correct.
#
# Arguments:
#    1. The name of the package (only used in the display of feedbacks)
#    2. The name of the jar files used to build against
#    3. What is the minimal version expected
#    4. Manifest attribute name
#    5. Specify if we want the exact version or greater. (equals or greater by
#      default).
#
#------------------------------------------------------------------------

AC_DEFUN([AC_JAVA_CHECK_VERSION_MANIFEST], [
    AC_MSG_CHECKING([minimal version ($4 $3) of $1])
    export JARFILE=$2;
    if test "x$5" == "x"; then
    AC_JAVA_TRY_COMPILE([import java.io.IOException;
import java.util.jar.JarFile;], [String minVersion="$3";
        try {
            String version = new JarFile(System.getenv("JARFILE")).getManifest().getMainAttributes().getValue("$4");
            System.out.println(version);
            if (compare(minVersion, version) > 0) {
                System.exit(-1);
            }
        } catch (IOException e) {
            System.err.println(e.getMessage());
            System.exit(1);
        } ], "yes", echo "yes" , AC_MSG_ERROR([Wrong version of $1. Expected at least $3. Found $STDOUT]))
    else
    AC_JAVA_TRY_COMPILE([import java.io.IOException;
import java.util.jar.JarFile;], [String minVersion="$3";
        try {
            String version = new JarFile(System.getenv("JARFILE")).getManifest().getMainAttributes().getValue("$4");
            System.out.println("$4" + ": " + version);
            if (compare(minVersion, version) != 0) {
                System.exit(-1);
            }
        } catch (IOException e) {
            System.err.println(e.getMessage());
            System.exit(1);
        } ], "yes", echo "yes" , AC_MSG_ERROR([Wrong version of $1. Expected exact version $4. Found $STDOUT]))
    fi
    unset JARFILE
])

