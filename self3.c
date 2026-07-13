#include <stdio.h>
#include <string.h>
int main()
{
    char data[200], stuffed[300], received[300], destuffed[300];
    int i, j = 0, count = 0;
    int parity = 0, receivedParity, newParity = 0;
    printf("Enter the binary data: ");
    scanf("%s", data);
    for(i = 0; data[i] != '\0'; i++)
    {
        if(data[i] == '1')
            parity ^= 1;
    }
    printf("Generated Parity Bit : %d\n", parity);
    for(i = 0; data[i] != '\0'; i++)
    {
        stuffed[j++] = data[i];
        if(data[i] == '1')
            count++;
        else
            count = 0;
        if(count == 5)
        {
            stuffed[j++] = '0';
            count = 0;
        }
    }
    stuffed[j] = '\0';
    printf("\nOriginal Data     : %s\n", data);
    printf("Stuffed Frame     : %s\n", stuffed);
    printf("Transmitted Frame : %s\n", stuffed);
    printf("\nEnter Received Frame: ");
    scanf("%s", received);
    printf("Enter Received Parity Bit: ");
    scanf("%d", &receivedParity);
    printf("Received Frame    : %s\n", received);
    i = 0;
    j = 0;
    count = 0;
    while(received[i] != '\0')
    {
        destuffed[j++] = received[i];

        if(received[i] == '1')
            count++;
        else
            count = 0;
        if(count == 5)
        {
            i++;
            count = 0;
        }
        i++;
    }
    destuffed[j] = '\0';
    printf("De-stuffed Data   : %s\n", destuffed);
    for(i = 0; destuffed[i] != '\0'; i++)
    {
        if(destuffed[i] == '1')
            newParity ^= 1;
    }
    if(newParity == receivedParity)
        printf("Status            : No Error Detected\n");
    else
        printf("Status            : Error Detected\n");

    return 0;
}
