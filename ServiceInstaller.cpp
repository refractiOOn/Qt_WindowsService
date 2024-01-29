#include "ServiceInstaller.hpp"
#include <QScopeGuard>
#include <iostream>

void installService(const std::wstring &execPath,
                    const std::wstring &serviceName,
                    const std::wstring &displayName,
                    DWORD startType)
{
    SC_HANDLE manager = NULL, service = NULL;
    auto cleanup = qScopeGuard([&manager, &service]
    {
        if (manager)
        {
            CloseServiceHandle(manager);
            manager = NULL;
        }
        if (service)
        {
            CloseServiceHandle(service);
            service = NULL;
        }
    });

    manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
    if (!manager)
    {
        std::wcerr << "OpenSCManager failed" << std::endl;
        return;
    }

    service = CreateServiceW(manager, serviceName.c_str(), displayName.c_str(),
                             SERVICE_QUERY_STATUS, SERVICE_WIN32_OWN_PROCESS,
                             startType, SERVICE_ERROR_NORMAL, execPath.c_str(),
                             NULL, NULL, NULL, NULL, NULL);
    if (!service)
    {
        std::wcerr << "CreateServiceW failed" << std::endl;
        return;
    }

    std::wcout << serviceName << " is installed" << std::endl;
}

void uninstallService(const std::wstring &serviceName)
{
    SC_HANDLE manager = NULL, service = NULL;
    auto cleanup = qScopeGuard([&manager, &service]
    {
        if (manager)
        {
            CloseServiceHandle(manager);
            manager = NULL;
        }
        if (service)
        {
            CloseServiceHandle(service);
            service = NULL;
        }
    });

    SERVICE_STATUS status = {};

    manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!manager)
    {
        std::wcerr << "OpenSCManager failed" << std::endl;
        return;
    }

    service = OpenServiceW(manager, serviceName.c_str(), SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE);
    if (!service)
    {
        std::wcerr << "OpenServiceW failed" << std::endl;
        return;
    }

    if (ControlService(service, SERVICE_CONTROL_STOP, &status))
    {
        std::wcout << "Stopping " << serviceName << std::endl;
        Sleep(1000);

        while (QueryServiceStatus(service, &status))
        {
            if (status.dwCurrentState == SERVICE_STOP_PENDING)
            {
                std::wcout << "." << std::endl;
                Sleep(1000);
            }
            else break;
        }

        if (status.dwCurrentState == SERVICE_STOPPED)
            std::wcout << serviceName << " is stopped" << std::endl;
        else
            std::wcerr << serviceName << " failed to stop" << std::endl;
    }

    if (!DeleteService(service))
    {
        std::wcerr << "DeleteService failed" << std::endl;
        return;
    }

    std::wcout << serviceName << " is uninstalled" << std::endl;
}