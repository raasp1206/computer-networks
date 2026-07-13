#include <stdio.h>
#include <string.h>

int main()
{
    char data[200], stuffed[300], received[300], destuffed[300];
    int i, j = 0;

    printf("Enter the message: ");
    scanf("%s", data);

    // Start of Frame
    stuffed[j++] = 'S';

    // Byte Stuffing
    for(i = 0; data[i] != '\0'; i++)
    {
        if(data[i] == 'S' || data[i] == 'E' || data[i] == 'X')
            stuffed[j++] = 'X';

        stuffed[j++] = data[i];
    }

    // End of Frame
    stuffed[j++] = 'E';
    stuffed[j] = '\0';

    printf("\nOriginal Message  : %s\n", data);
    printf("Stuffed Frame     : %s\n", stuffed);
    printf("Transmitted Frame : %s\n", stuffed);

    printf("\nEnter Received Frame: ");
    scanf("%s", received);

    printf("Received Frame    : %s\n", received);

    // Frame Check
    if(received[0] != 'S' || received[strlen(received)-1] != 'E')
    {
        printf("Status            : Frame Error Detected\n");
        return 0;
    }

    // De-stuffing
    j = 0;

    for(i = 1; i < strlen(received)-1; i++)
    {
        if(received[i] == 'X')
            i++;

        destuffed[j++] = received[i];
    }

    destuffed[j] = '\0';

    printf("Recovered Message : %s\n", destuffed);

    // Error Detection
    if(strcmp(destuffed, data) == 0)
        printf("Status            : No Error Detected\n");
    else
        printf("Status            : Error Detected\n");

    return 0;
}
