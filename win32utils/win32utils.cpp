#include "win32utils.h"
#include <VersionHelpers.h>
#include <shellapi.h>
#include <shlobj.h>
#include <strsafe.h>
#include <tchar.h>
#include <windowsx.h>

bool win32utils::hasUAC() {
    // Windows Vista and later
    OSVERSIONINFO osver = {sizeof(osver)};
    if (IsWindowsVistaOrGreater()) {

        return true;
    } else {
        // You need at least Windows Vista, Version Not Supported
        return false;
    }
}

bool win32utils::isRunAsAdmin() {
    BOOL  fIsRunAsAdmin        = FALSE;
    DWORD dwError              = ERROR_SUCCESS;
    PSID  pAdministratorsGroup = NULL;

    // Allocate and initialize a SID of the administrators group.
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
    if (!AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0,
                                  &pAdministratorsGroup)) {
        dwError = GetLastError();
    }

    if (ERROR_SUCCESS == dwError) {
        // Determine whether the SID of administrators group is enabled in
        // the primary access token of the process.
        if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin)) {
            dwError = GetLastError();
        }
    }

    // Centralized cleanup for all allocated resources.
    if (pAdministratorsGroup) {
        FreeSid(pAdministratorsGroup);
        pAdministratorsGroup = NULL;
    }

    if (ERROR_SUCCESS != dwError) {
        return false;
    }

    return (fIsRunAsAdmin != 0);
}

/**
 * Restart the current process as admin.
 *
 * This method asks the user to run the current process with administrator
 * rights. If the user allows, the process is restarted with admin rights, the
 * method returns true. In that case, immediately close the program. If the user
 * does not allow, the method returns false.
 */
bool win32utils::runAsAdmin(std::wstring pFile, std::wstring cmdLine, int nShow) {

    if (pFile.empty()) {
        return false;
    }

    LPCWSTR lpFile = pFile.c_str();
    LPCWSTR lsw    = cmdLine.c_str();

    // Launch itself as administrator.
    SHELLEXECUTEINFO sei = {sizeof(sei)};
    sei.lpVerb           = L"runas";
    sei.lpFile           = lpFile;
    if (!cmdLine.empty()) {
        sei.lpParameters = lsw;
    }
    sei.nShow = nShow;

    bool isOK = false;
    if (!ShellExecuteEx(&sei)) {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED) {
            // The user refused the elevation.
            // Do nothing ...
        }
    } else {
        // EndDialog(hWnd, TRUE);  // Quit itself
        isOK = true;
    }

    return isOK;
}

bool win32utils::runAsUser(std::wstring pFile, std::wstring cmdLine, int nShow) {
    if (pFile.empty()) {
        return false;
    }

    LPCWSTR lpFile = pFile.c_str();
    LPCWSTR lsw    = cmdLine.c_str();

    // Launch itself as user.
    SHELLEXECUTEINFO sei = {sizeof(sei)};
    sei.lpFile           = lpFile;

    if (!cmdLine.empty()) {
        sei.lpParameters = lsw;
    }

    sei.nShow = nShow;
    sei.fMask = 0;

    bool isOK = false;
    if (!ShellExecuteEx(&sei)) {
        DWORD dwError = GetLastError();
        if (dwError == ERROR_CANCELLED) {
            // The user refused the elevation.
            // Do nothing ...
        }
    } else {
        // EndDialog(hWnd, TRUE);  // Quit itself
        isOK = true;
    }

    return isOK;
}
