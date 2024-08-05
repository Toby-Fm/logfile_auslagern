//
// Created by toby.wichmann on 26.07.2024.
//

#include "include/main.hpp"

#include <windows.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <thread>
#include <chrono>
#include <ctime>
#include <sstream>

// Konstruktor zur Initialisierung des Dateipfads und Dateinamens
Datei::Datei(const std::string& filePath, const std::string& fileName)
    : filePath(filePath), fileName(fileName)
{
    if (!this->filePath.empty() && this->filePath.back() != '/')
    {
        this->filePath += '/';
    }
}

// Zeitstempel
std::string Datei::getZeitStempel() const
{
    std::time_t now = std::time(nullptr);
    std::tm* now_tm = std::localtime(&now);

    std::ostringstream oss;
    oss << (now_tm->tm_year + 1900) << '-'
        << (now_tm->tm_mon + 1) << '-'
        << now_tm->tm_mday << '_'
        << now_tm->tm_hour << '-'
        << now_tm->tm_min << '-'
        << now_tm->tm_sec;

    return oss.str();
}

// Methode zum Lesen der Datei und Ausgeben des Inhalts
void Datei::readAndTransferFile() const
{
    std::ifstream file(filePath + fileName);
    if (file.is_open())
    {
        std::cout << "\n" << file.rdbuf() << "\n\n";

        file.clear();
        file.seekg(0);

        std::string vorZeitName = "Neue_Datei_"; // Dateiname ggf. aendern
        std::string newFileName = filePath + vorZeitName + getZeitStempel() + ".txt"; // aendung ggf aendern
        std::ofstream newFile(newFileName);
        if (newFile.is_open())
        {
            newFile << file.rdbuf();
            newFile.close();
        } else
        {
            std::cerr << "Fehler beim Öffnen der neuen Datei!" << std::endl;
        }
        std::cout << "Neue Datei wurde erstellt.\n" << "Name und Verzeichnis: " << newFileName << "\n\n";
        file.close();

        std::ofstream clearFile(filePath + fileName, std::ofstream::out | std::ofstream::trunc);
        if (clearFile.is_open())
        {
            std::cout << "Originale Datei wurde geleert und steht wieder zur Verfügung" << std::endl;
            clearFile.close();
        } else
        {
            std::cerr << "Fehler beim Leeren der alten Datei!" << std::endl;
        }
    } else
    {
        std::cerr << "Fehler beim Öffnen der Datei!" << std::endl;
    }
}

// Methode zur Ermittlung der Dateigröße
std::streampos Datei::getFileSize() const
{
    std::ifstream in(filePath + fileName, std::ifstream::ate | std::ifstream::binary);
    return in.tellg();
}

// Methode zur Überprüfung, ob die Dateigröße eine Maxgröße überschreitet
void Datei::overFileSize(std::streampos maxSize) const
{
    std::streampos fileSize = getFileSize();
    if (fileSize > maxSize)
    {
        std::cout << "Die Datei überschreitet die Mindestgröße von " << maxSize << " Bytes.\n";
        std::cout << "Inhalt wird ausgelesen und an eine neue Datei übergeben bzw. die Datei wird wieder geleert.\n\n";

        for (int i = 5; i > 0; i--)
        {
            std::cout << i;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        readAndTransferFile();
    } else
    {
        std::cout << "Die Datei überschreitet die Mindestgröße nicht.\n\n";
    }
}

// Dienststeuerungsfunktionen
SERVICE_STATUS ServiceStatus; // Struktur zur Beschreibung des Dienststatus
SERVICE_STATUS_HANDLE hStatus; // Handle zum Registrieren des Dienstes

// Deklaration der Hauptfunktion des Dienstes
void ServiceMain(int argc, char** argv);

// Deklaration der Steuerungsfunktion für Dienststeuerungsanforderungen
void ControlHandler(DWORD request);

std::ofstream logFile("service.log", std::ios::app);

void Log(const std::string& message)
{
    logFile << message << std::endl;
}

int main()
{
    // Tabelle der Dienststeuerungseinträge, die dem Dienststeuerungs-Dispatcher übergeben wird
    SERVICE_TABLE_ENTRY ServiceTable[] =
    {
        { (LPSTR)"Logfile_auslagern", (LPSERVICE_MAIN_FUNCTION)ServiceMain }, // Eintrag für den Dienst
        { NULL, NULL } // Ende der Tabelle markieren
    };

    // Starten des Dienststeuerungs-Dispatchers
    // Der Dispatcher ruft die in der Tabelle registrierte Dienstmainfunktion auf
    if (!StartServiceCtrlDispatcher(ServiceTable))
    {
        Log("Fehler beim Starten des Dispatchers");
        return 1; // Fehler beim Starten des Dispatchers
    }

    return 0;
}

void ServiceMain(int argc, char** argv)
{
    Log("ServiceMain started");

    // Dateioperationen
    std::string filePath = "C:/test"; // Verzeichnis wo die Logdatei liegt
    std::string fileName = "test.txt"; // Name der Logdatei

    Datei datei(filePath, fileName);

    // Registrieren der Steuerungsfunktion für den Dienst
    hStatus = RegisterServiceCtrlHandler("Logfile_auslagern", (LPHANDLER_FUNCTION)ControlHandler);
    if (hStatus == (SERVICE_STATUS_HANDLE)0)
    {
        Log("RegisterServiceCtrlHandler failed");
        return;
    }

    Log("RegisterServiceCtrlHandler succeeded");

    // Initialisieren der Dienststatusstruktur
    ServiceStatus.dwServiceType = SERVICE_WIN32;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 30000; // Längere Wartezeit einfügen, um dem Dienst mehr Zeit für die Initialisierung zu geben (30 Sekunden) (fehlercode 1053)

    // Aktualisieren des Dienststatus
    SetServiceStatus(hStatus, &ServiceStatus);
    Log("Service status set to SERVICE_START_PENDING");

    // Dienst ist jetzt in Betrieb
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &ServiceStatus);
    Log("Service status set to SERVICE_RUNNING");

    // Hauptschleife des Dienstes
    while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
    {
        std::streampos maxSize = 10; // Sind 10 Bytes - ggf. ändern.
        /*
         * 1 Kilobyte (KB) = 1024
         * 1 Megabyte (MB) = 1048576
         * 1 Gigabyte (GB) = 1073741824
         * 1 Terabyte (TB) = 1099511627776
         */

        datei.overFileSize(maxSize);

        std::this_thread::sleep_for(std::chrono::seconds(10)); // Pause zwischen der nächsten "überwachung" (hier 10sek)
        /*
         * 1 Tag = 86400
         * 1 Woche = 604800
         */
    }

    // Dienst wird gestoppt
    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(hStatus, &ServiceStatus);
    Log("Service status set to SERVICE_STOPPED");
}

void ControlHandler(DWORD request)
{
    switch (request)
    {
    case SERVICE_CONTROL_STOP:
        Log("SERVICE_CONTROL_STOP received");
        // Anforderung zum Stoppen des Dienstes
        ServiceStatus.dwWin32ExitCode = 0;
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;

    case SERVICE_CONTROL_SHUTDOWN:
        Log("SERVICE_CONTROL_SHUTDOWN received");
        // Anforderung zum Herunterfahren des Dienstes
        ServiceStatus.dwWin32ExitCode = 0;
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;

    default:
        break;
    }

    // Aktualisieren des Dienststatus für andere Steuerungsanforderungen
    SetServiceStatus(hStatus, &ServiceStatus);
}