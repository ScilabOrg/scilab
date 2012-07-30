#
# Return MacOSX version using system_profile tool.
#
AC_DEFUN([AC_GET_MACOSX_VERSION],[
    AC_MSG_CHECKING([Mac OS X Version])
    [macosx_version=`system_profiler SPSoftwareDataType 2>&1 | grep 'System Version' | sed -e 's/.*\([0-9][0-9]\.[0-9]*\).*/\1/'`]
    echo $macosx_version
    case "$macosx_version" in
         "10.8")
              AC_MSG_RESULT([Mac OS X 10.8 - Mountain Lion.])
         ;;
         "10.7")
              AC_MSG_RESULT([Mac OS X 10.7 - Lion.])
         ;;
         "10.6")
              AC_MSG_RESULT([Mac OS X 10.6 - Snow Leopard.])
         ;;
         "10.5")
              AC_MSG_RESULT([Mac OS X 10.5 - Leopard.])
         ;;
         *)
              AC_MSG_ERROR([MacOSX 10.5, 10.6, 10.7 or 10.8 are needed. Found $macosx_version])
         ;;
	 esac
])
