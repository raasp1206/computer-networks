#include <stdio.h>
#include <string.h>

// Helper function to convert a character array into a space-separated binary string
void stringToBinary(const char *input, char *output) {
    int outIdx = 0;
    int i,bit;
    for (i = 0; input[i] != '\0'; i++) {
        char ch = input[i];
        // Convert the 8 bits of each character from MSB to LSB
        for ( bit = 7; bit >= 0; bit--) {
            if ((ch >> bit) & 1) {
                output[outIdx++] = '1';
            } else {
                output[outIdx++] = '0';
            }
        }
        output[outIdx++] = ' '; // Spacer between bytes for clear reading
    }
    if (outIdx > 0) {
        output[outIdx - 1] = '\0'; // Clean trailing space
    } else {
        output[outIdx] = '\0';
    }
}

// Quick helper to output binary for an individual standalone character
void charToBinary(char ch, char *output) {
    int outIdx = 0;
    int bit;
    for (bit = 7; bit >= 0; bit--) {
        output[outIdx++] = ((ch >> bit) & 1) ? '1' : '0';
    }
    output[outIdx] = '\0';
}

int main()
{
    char data[100], stuffed[200], received[200], destuffed[100];
    char binaryBuffer[2000];
    char singleCharBin[10];

    char SOF = 'F';
    char EOFC = 'E';
    char ESC = '\\';
    int i, j = 0;

    /*/ --- Print Key Delimiters in Binary first ---
    printf("--- Frame Control Characters (Binary Reference) ---\n");
    charToBinary(SOF, singleCharBin);
    printf("SOF Flag ('%c')      : %s\n", SOF, singleCharBin);
    charToBinary(EOFC, singleCharBin);
    printf("EOFC Flag ('%c')     : %s\n", EOFC, singleCharBin);
    charToBinary(ESC, singleCharBin);
    printf("ESC Character ('%c') : %s\n", ESC, singleCharBin);
    printf("---------------------------------------------------\n\n");*/

    printf("Enter a string: ");
    scanf("%s", data);

    // --- Byte Stuffing Loop ---
    stuffed[j++] = SOF; // Start of frame flag included
    for (i = 0; data[i] != '\0'; i++)
    {
        if (data[i] == SOF || data[i] == EOFC || data[i] == ESC)
        {
            stuffed[j++] = ESC; // Escape stuffed when data matches markers
        }
        stuffed[j++] = data[i];
    }
    stuffed[j++] = EOFC; // End of frame flag included
    stuffed[j] = '\0';

    // --- Transmission Outputs ---
    printf("\nOriginal String : %s", data);
    stringToBinary(data, binaryBuffer);
    printf("\nOriginal (Binary): %s\n", binaryBuffer);

    printf("\nStuffed Frame    : %s", stuffed);
    stringToBinary(stuffed, binaryBuffer);
    printf("\nStuffed (Binary) : %s\n", binaryBuffer);

    strcpy(received, stuffed);
    printf("\nTransmitted Frame: %s", stuffed);
    printf("\nReceived Frame   : %s", received);

    // --- Simulate Error ---
    int choice, pos;
    printf("\n\nDo you want to introduce an error? (1 or 0): ");
    scanf("%d", &choice);
    if (choice == 1)
    {
        printf("Enter position to modify (0-%lu): ", strlen(received) - 1);
        scanf("%d", &pos);
        if (pos >= 0 && pos < strlen(received))
            received[pos] = '#';
    }
    printf("\nModified Received Frame: %s", received);
    stringToBinary(received, binaryBuffer);
    printf("\nModified (Binary)      : %s\n", binaryBuffer);

    // --- Error Check & De-stuffing ---
    if (strchr(received, '#'))
    {
        printf("\nStatus: Error Detected");
        printf("\nAction: Message Discarded due to corruption.");
        destuffed[0] = '\0';
    }
    else
    {
        printf("\nStatus: No Error");
        j = 0;
        // Unstuffing skips index 0 (SOF flag) and runs until EOFC flag is met
        for (i = 1; received[i] != EOFC && received[i] != '\0'; i++)
        {
            if (received[i] == ESC)
            {
                i++; // Skip the stuffed escape character
            }
            destuffed[j++] = received[i];
        }
        destuffed[j] = '\0';

        printf("\nRecovered Data   : %s", destuffed);
        stringToBinary(destuffed, binaryBuffer);
        printf("\nRecovered (Binary): %s", binaryBuffer);
    }

    printf("\n");
    return 0;
}
