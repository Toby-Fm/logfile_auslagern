```Console
██╗          ██████╗      ██████╗     ███████╗    ██╗    ██╗        ███████╗       ██████╗     ██╗    ███████╗    ███╗   ██╗    ███████╗    ████████╗
██║         ██╔═══██╗    ██╔════╝     ██╔════╝    ██║    ██║        ██╔════╝       ██╔══██╗    ██║    ██╔════╝    ████╗  ██║    ██╔════╝    ╚══██╔══╝
██║         ██║   ██║    ██║  ███╗    █████╗      ██║    ██║        █████╗         ██║  ██║    ██║    █████╗      ██╔██╗ ██║    ███████╗       ██║   
██║         ██║   ██║    ██║   ██║    ██╔══╝      ██║    ██║        ██╔══╝         ██║  ██║    ██║    ██╔══╝      ██║╚██╗██║    ╚════██║       ██║   
███████╗    ╚██████╔╝    ╚██████╔╝    ██║         ██║    ███████╗   ███████╗       ██████╔╝    ██║    ███████╗    ██║ ╚████║    ███████║       ██║   
╚══════╝     ╚═════╝      ╚═════╝     ╚═╝         ╚═╝    ╚══════╝   ╚══════╝       ╚═════╝     ╚═╝    ╚══════╝    ╚═╝  ╚═══╝    ╚══════╝       ╚═╝
```

## Überblick

Dieses Projekt zielt darauf ab, ein Tool zu entwickeln, das die Größe der Logdatei eines Webservers auf einem Windows-Server verwaltet. Wenn die Logdatei eine bestimmte Größe überschreitet, wird sie in eine neue Datei geschrieben, und die ursprüngliche Logdatei wird geleert. Dieses Tool wird als Windows-Dienst implementiert, um im Hintergrund kontinuierlich zu laufen und die Logdatei zu überwachen.

## Installation

### Voraussetzungen

- Windows 11 (Wurde nur auf 11 getestet)
- Visual Studio / CLion oder ein anderes C++-Entwicklungstool
- Makefile-Unterstützung für Windows (z.B. durch MinGW oder Cygwin)

## Konfiguration

- **Auf sich selber Anpassen:** Mehrere Sachen sind noch konfigurierbar. Passen Sie diese Werte entsprechend Ihren Anforderungen an.

```cpp
//main.cpp
(Zeile: 54 -> Name der Datei)
    std::string vorZeitName = "Neue_Datei_";

(Zeile: 154 -> Verzeichnis wo die Datei liegt)
    std::string filePath = "C:/test";

(Zeile: 155 -> Name der Logdatei)
    std::string fileName = "test.txt";

(Zeile: 190 -> Maximale Dateigröße)
    std::streampos maxSize = 10;
    
(Zeile: 200 -> Pause zwischen der Prüfung)
    std::this_thread::sleep_for(std::chrono::seconds(10));
```

### Schritte zur Erstellung der ausführbaren Datei (.exe)

1. **Quellcode herunterladen:**
    - Laden Sie das Projekt-Repository auf Ihren Server herunter.

2. **In das Verzeichnis wechseln:**
    - Öffnen Sie die Eingabeaufforderung und navigieren Sie in das Projektverzeichnis:
      ```bash
      cd \pfad\zum\projektverzeichnis
      ```

3. **Erstellen der .exe-Datei:**
    - Geben Sie den folgenden Befehl ein, um die ausführbare Datei zu erstellen:
      ```bash
      make
      ```

    - Dies erstellt die ausführbare Datei im Projektverzeichnis. (logfile_auslagern.exe)

## Dienstinstallation

Um die erstellte .exe-Datei als Windows-Dienst hinzuzufügen, führen Sie die folgenden Schritte aus:

1. **Öffnen der Eingabeaufforderung als Administrator:**
    - Suchen Sie nach "cmd" im Startmenü, klicken Sie mit der rechten Maustaste darauf und wählen Sie "Als Administrator ausführen".

2. **Installieren des Dienstes:**
    - Verwenden Sie das Windows-Dienstprogramm `sc` (Service Control) zum Erstellen des Dienstes. Ersetzen Sie `<PfadZurExe>` durch den vollständigen Pfad der erstellten .exe-Datei:

      ```bash
      sc create logfile_auslagern binPath= "\"<PfadZurExe>\logfile_auslagern.exe\""
      ```

3. **Starten des Dienstes:**
    - Starten Sie den Dienst mit dem folgenden Befehl:

      ```bash
      sc start logfile_auslagern
      ```

4. **Überprüfen des Dienststatus:**
    - Um zu überprüfen, ob der Dienst korrekt läuft, verwenden Sie:

      ```bash
      sc query logfile_auslagern
      ```

5. **Dienst entfernen:**
    - Falls Sie den Dienst entfernen müssen, verwenden Sie:

      ```bash
      sc delete logfile_auslagern
      ```

## Funktionsweise

- Der Dienst überwacht kontinuierlich die Größe der Logdatei.
- Wenn die Logdatei die festgelegte Größe überschreitet, wird die aktuelle Logdatei in eine neue Datei verschoben, und die ursprüngliche Logdatei wird geleert.
- Dies hilft, die Speichernutzung des Servers zu verwalten und sicherzustellen, dass die Logdatei nicht unkontrolliert wächst.

## Fehlerbehebung

### Fehlende Bibliotheken
- Der Dienst nutzt in erster linie die normalen bibliotheken, die standardmäßig mitinstalliert werden. Wenn noch weitere fehlen, findet man unter `./include` die entsprechende Dateien.
  - [libgcc_s_seh-1.dll](include%2Flibgcc_s_seh-1.dll)
  - [libstdc++-6.dll](include%2Flibstdc%2B%2B-6.dll)
  - [libwinpthread-1.dll](include%2Flibwinpthread-1.dll)

1. Füge den Ordner zu den Umgebungsvariablen hinzu.

### Dienst startet nicht

Wenn der Dienst nicht startet, überprüfen Sie bitte Folgendes:

- **Dateipfad:** Stellen Sie sicher, dass die .exe-Datei im angegebenen Pfad vorhanden ist. Überprüfen Sie den Pfad in der `sc create`-Anweisung sorgfältig auf Tippfehler.

- **Administratorrechte:** Die Installation und der Start eines Dienstes erfordern Administratorrechte. Stellen Sie sicher, dass Sie die Eingabeaufforderung als Administrator ausführen.

### Wenn die Befehle nicht auf Anhieb funktionieren

Sollten die Befehle zur Dienstinstallation oder -verwaltung nicht sofort funktionieren, versuchen Sie die folgenden Schritte:

1. **Dienst manuell erstellen:**
   - Öffnen Sie die Eingabeaufforderung als Administrator und geben Sie den folgenden Befehl ein:
     ```bash
     sc create logfile_auslagern binPath= "\"<PfadZurExe>\logfile_auslagern.exe\""
     ```

2. **Dienste-Manager öffnen:**
   - Gehen Sie auf dem Server zum `Dienste-Manager`. Diesen finden Sie, indem Sie im Startmenü nach `Dienste` suchen oder drücken Sie `Win + R` und suchen `services.msc` in der Ausführen-Dialogbox.

3. **Dienst suchen:**
   - Suchen Sie den neu erstellten Dienst mit dem Namen `logfile_auslagern` in der Liste der Dienste.

4. **Dienst manuell starten:**
   - Klicken Sie mit der rechten Maustaste auf den Dienst und wählen Sie `Starten`, um ihn manuell zu starten.