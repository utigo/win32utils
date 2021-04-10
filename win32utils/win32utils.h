/**
Copyright (c) Microsoft Corporation.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/en-us/openness/resources/licenses.aspx#MPL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
*/


#include <Windows.h>
#include <string>
#ifndef WIN32UTILS_H
#define WIN32UTILS_H

class win32utils {
public:
    static bool hasUAC();
    static bool isRunAsAdmin();
    static bool runAsAdmin(std::wstring file, std::wstring cmd_line, int nShow = SW_SHOW);
    static bool runAsUser(std::wstring file, std::wstring cmd_line, int nShow = SW_SHOW);
};

#endif
