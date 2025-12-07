# MyShell – einfache Windows-Shell in C++

MyShell ist eine kleine, selbst geschriebene Kommandozeilen-Shell für Windows in C++.  
Ziel: zeigen, wie eine Shell intern arbeitet (Prompt → Eingabe → Parsing → Ausführung) und wie man über die Windows-API Prozesse startet.

---

## Features

**Interne Befehle**

- `help` – Übersicht der internen Befehle
- `exit` – Shell beenden
- `cd` – aktuelles Verzeichnis anzeigen oder wechseln
- `echo` – Text auf der Konsole ausgeben

**Externe Befehle**

- Starten externer Programme über die Windows-API `CreateProcessA`
- Warten auf das Ende des externen Prozesses (`WaitForSingleObject`)
- Einfache Behandlung von Argumenten mit Leerzeichen (z. B. `"C:\Pfad mit Leerzeichen\datei.txt"`)

---

## Projektstruktur

```text
Shell/
├── include/
│   └── shell.h        # Deklaration der shell-Klasse
├── src/
│   ├── main.cpp       # Einstiegspunkt (main-Funktion)
│   └── shell.cpp      # Implementierung der Shell-Logik
└── README.md
