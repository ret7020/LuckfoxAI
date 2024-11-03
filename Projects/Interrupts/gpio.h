#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

int exportPin(uint8_t pin)
{
    FILE *exportFile = fopen("/sys/class/gpio/export", "w");
    if (exportFile == NULL)
        return -1;
    fprintf(exportFile, "%d", pin);
    fclose(exportFile);

    return 0;
}

int setDirPin(uint8_t pin, const char *dir)
{
    char directionPath[50];
    snprintf(directionPath, sizeof(directionPath), "/sys/class/gpio/gpio%d/direction", pin);
    FILE *directionFile = fopen(directionPath, "w");
    if (directionFile == NULL)
        return -1;
    fprintf(directionFile, dir);
    fclose(directionFile);

    return 0;
}

int setInterruptType(uint8_t pin, const char *type)
{
    char interruptPath[50];
    snprintf(interruptPath, sizeof(interruptPath), "/sys/class/gpio/gpio%d/edge", pin);
    FILE *interruptFile = fopen(interruptPath, "w");
    if (interruptFile == NULL)
        return -1;
    fprintf(interruptFile, type);
    fclose(interruptFile);

    return 0;
}

#endif