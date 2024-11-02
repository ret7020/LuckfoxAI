#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <poll.h>
#include <string.h>


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

int setDirInterrupt(uint8_t pin, const char *type)
{
    char interruptPath[50];
    snprintf(interruptPath, sizeof(interruptPath), "/sys/class/gpio/gpio%d/edge", pin);
    FILE *interruptFile = fopen(interruptPath, "w");
    if (interruptFile == NULL)
        return -1;
    fprintf(interruptFile, dir);
    fclose(interruptFile);

    return 0;
}


int main() {
    exportPin(57);
    setDirPin(57, "in");
    setDirPin(57, "rising"); // rising/falling/both

    int fd = open("/sys/class/gpio/gpio57/value", O_RDONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("Unable to open GPIO value file");
        return 1;
    } 

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLPRI;

    while (1) {
        int ret = poll(&pfd, 1, -1);
        if (ret > 0) {
            char buf[3];
            lseek(fd, 0, SEEK_SET);
            read(fd, buf, sizeof(buf));
	    printf("Val: %s\n", buf);
            printf("Interrupt detected!\n");
        }
    }

    close(fd);
    return 0;
}

