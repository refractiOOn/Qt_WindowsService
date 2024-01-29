#include <QCoreApplication>
#include <QCommandLineParser>
#include "ServiceInstaller.hpp"
#include "SampleService.hpp"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("QtService");
    parser.addHelpOption();

    QCommandLineOption actionOption({"a", "action"}, "Select the action.", "action");
    parser.addOption(actionOption);

    parser.process(app);

    const std::wstring serviceName = L"QtWindowsService";
    const std::wstring displayName = L"Qt Windows Service";
    const DWORD startType = SERVICE_DEMAND_START;
    
    if (!parser.isSet(actionOption))
    {
        qDebug() << "Parameters:\n"
                  << " -install to install the service\n"
                  << " -remove to remove the service";
        
        SampleService service(serviceName);
        if (!SampleService::Run(service))
        {
            wprintf(L"Service failed to run w/err 0x%08lx\n", GetLastError());
        }
        else qDebug() << "Service successfully started";
        return 0;
    }
    
    const QString action = parser.value(actionOption);

    const std::string exec(argv[0]);
    const std::wstring wideExec(exec.cbegin(), exec.cend());

    if (action == "install") installService(wideExec, serviceName, displayName, startType);
    else if (action == "uninstall") uninstallService(serviceName);
    else qDebug() << "Invalid action value. Print --help to get argument info";

    return 0;
}
