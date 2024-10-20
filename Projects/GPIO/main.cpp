#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int bank = 1;
	char group = 2;
	int X = 0;
	int linuxPin = 0;
	char result[2];
	printf("Enter pin from pinout, like example: 1 B 2 stands for GPIO1_B2_d in pinout: ");
	scanf("%d %c %d", &bank, &group, &X);
	group -= 'A';
	linuxPin = bank * 32 + (group * 8 + X);
	printf("Set to (1 or 0)?:");
	scanf("%s", &result);
	printf("Pin %d will be set to %s\n", linuxPin, result);

	// Export pin to userspace
	FILE *exportFile = fopen("/sys/class/gpio/export", "w");
	if (exportFile == NULL)
	{
		perror("Failed to open GPIO export file, maybe it is alreay exported or invalid?");
		return -1;
	}
	fprintf(exportFile, "%d", linuxPin);
	fclose(exportFile);

	// Set to output
	char directionPath[50];
	snprintf(directionPath, sizeof(directionPath), "/sys/class/gpio/gpio%d/direction", linuxPin);
	FILE *directionFile = fopen(directionPath, "w");
	if (directionFile == NULL)
	{
		perror("Failed to open GPIO direction file, check export");
		return -1;
	}
	fprintf(directionFile, "out");
	fclose(directionFile);

	// Write value
    char valuePath[50];
    snprintf(valuePath, sizeof(valuePath), "/sys/class/gpio/gpio%d/value", linuxPin);
    FILE *valueFile = fopen(valuePath, "w");
    if (valueFile == NULL) {
        perror("Failed to open GPIO value file");
        return -1;
    }


	fprintf(valueFile, result);
    fflush(valueFile);


	// Release pin
    fclose(valueFile);
    FILE *unexportFile = fopen("/sys/class/gpio/unexport", "w");
    if (unexportFile == NULL) {
        perror("Failed to open GPIO unexport file");
        return -1;
    }
    fprintf(unexportFile, "%d", linuxPin);
    fclose(unexportFile);


	return 0;
}
