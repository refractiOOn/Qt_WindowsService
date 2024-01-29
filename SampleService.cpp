#include "SampleService.hpp"
#include "ThreadPool.hpp"
#include <QSaveFile>
#include <QStandardPaths>

SampleService::SampleService(std::wstring serviceName,
                             bool canStop,
                             bool canShutdown,
                             bool canPauseContinue) :
    ServiceBase(std::move(serviceName), canStop, canShutdown, canPauseContinue),
    m_stopping(false)
{
    m_stoppedEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (!m_stoppedEvent)
    {
        throw std::runtime_error("CreateEvent failed");
    }
}

SampleService::~SampleService()
{
    if (m_stoppedEvent)
    {
        CloseHandle(m_stoppedEvent);
        m_stoppedEvent = NULL;
    }
}

void SampleService::OnStart(DWORD argc, wchar_t **argv)
{
    WriteEventLogEntry(Name() + L" in OnStart", EVENTLOG_INFORMATION_TYPE);
    ThreadPool::QueueUserWorkItem(&SampleService::ServiceWorkerThread, this);
}

void SampleService::OnStop()
{
    WriteEventLogEntry(Name() + L" in OnStop", EVENTLOG_INFORMATION_TYPE);
    m_stopping = true;
    if (WaitForSingleObject(m_stoppedEvent, INFINITE) != WAIT_OBJECT_0)
    {
        throw std::runtime_error("WaitForSingleObject failed");
    }
}

void SampleService::ServiceWorkerThread()
{
    uint counter = 0;
    const QByteArray data("Hello, world!");

    while (!m_stopping)
    {
        const QString fileName("C:/file_" + QString::number(counter) + ".txt");
        QSaveFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            throw std::runtime_error("QSaveFile::open failed");
        }

        file.write(data);
        file.commit();

        ++counter;
        ::Sleep(5000);
    }
    SetEvent(m_stoppedEvent);
}