/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 * === LICENSE_END ===
 *
 */

#ifndef __RETURNTYPE_H__
#define __RETURNTYPE_H__

enum _ReturnType_
{
    jni_string,
    jni_string_vector,
    jni_double,
    jni_double_vector,
    jni_int,
    jni_int_vector,
    jni_bool,
    jni_bool_vector
};

#endif /* !__RETURNTYPE_H__ */
