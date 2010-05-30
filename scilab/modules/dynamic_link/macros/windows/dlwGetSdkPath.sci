// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function SDKpath = dlwGetSdkPath()
  SDKpath = [];
  // We always use last version of MS SDK
  try
    // Vista & Seven SDK
    SDKpath = winqueryreg('HKEY_LOCAL_MACHINE', ..
                          'Software\Microsoft\Microsoft SDKs\Windows', ..
                          'CurrentInstallFolder');
    // remove last file separator if it exists
    if SDKpath <> [] then
      SDKpath = pathconvert(SDKpath, %f, %t);
    end
    return;
  catch
    try
      // Windows 2003 R2 SDK
      SDKpath = winqueryreg('HKEY_LOCAL_MACHINE', ..
                'Software\Microsoft\MicrosoftSDK\InstalledSDKs\D2FF9F89-8AA2-4373-8A31-C838BF4DBBE1', ..
                'Install Dir');
      // remove last file separator if it exists
    if SDKpath <> [] then
      SDKpath = pathconvert(SDKpath, %f, %t);
    end
      return;
    catch
      try
        // Windows 2003 SDK
        SDKpath = winqueryreg('HKEY_LOCAL_MACHINE', ..
                  'Software\Microsoft\MicrosoftSDK\InstalledSDKs\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3', ..
                  'Install Dir');
        // remove last file separator if it exists
        if SDKpath <> [] then
          SDKpath = pathconvert(SDKpath, %f, %t);
        end
        return;
      catch
        SDKpath = [];
      end
    end
  end
endfunction
//=============================================================================
