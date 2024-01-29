#pragma once

#include <string>
#include <Windows.h>

void installService(const std::wstring &serviceName,
                    const std::wstring &displayName,
                    DWORD startType);
void uninstallService(const std::wstring &serviceName);