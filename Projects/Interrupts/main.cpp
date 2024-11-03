#include "gpio.h"
#include <poll.h>
#include <pthread.h>

#define TEST_PIN 57

int main()
{
    exportPin(TEST_PIN);
    setDirPin(TEST_PIN, "in");
    setInterruptType(TEST_PIN, "rising"); // rising/falling/both


    char valuePath[50];
    snprintf(valuePath, sizeof(valuePath), "/sys/class/gpio/gpio%d/value", TEST_PIN);
    int fd = open(valuePath, O_RDONLY | O_NONBLOCK);
    if (fd < 0)
    {
        perror("Unable to open GPIO value file");
        return 1;
    }

    struct pollfd pfd;
    pfd.fd = fd;
    pfd.events = POLLPRI;

    while (1)
    {
        int ret = poll(&pfd, 1, -1);
        if (ret > 0)
        {
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
