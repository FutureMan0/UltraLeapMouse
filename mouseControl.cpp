//
// Created by Luis Blüml on 14.11.2024.
//

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdbool.h>

int mouse() {
    // Rechteckbegrenzungen für die Bewegung
    int left = 100, top = 100;
    int right = 400, bottom = 500;

    // Startposition
    int x = left, y = top;

    // Bewegungsrichtung
    int xDirection = 1;  // 1 = rechts, -1 = links
    int yDirection = 0;  // 1 = unten, -1 = oben, 0 = keine Bewegung

    // Setze die Geschwindigkeit der Bewegung
    int speed = 10;

    while (true) {
        // Mausposition setzen
        SetCursorPos(x, y);

        // Rechteckbewegung prüfen und anpassen
        if (xDirection == 1 && x >= right) {
            xDirection = 0;
            yDirection = 1;  // Bewegung nach unten
        } else if (yDirection == 1 && y >= bottom) {
            xDirection = -1;
            yDirection = 0;  // Bewegung nach links
        } else if (xDirection == -1 && x <= left) {
            xDirection = 0;
            yDirection = -1;  // Bewegung nach oben
        } else if (yDirection == -1 && y <= top) {
            xDirection = 1;
            yDirection = 0;  // Bewegung nach rechts
        }

        // Position aktualisieren
        x += speed * xDirection;
        y += speed * yDirection;

        // Verzögerung für glattere Bewegung
        Sleep(1);
    }

    return 0;
}
