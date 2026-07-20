#include <stdio.h>
#include <string.h>

int main()
{
    char text[100];
    char data[800];
    char stuffed[1000], received[1000], destuffed[1000];
    char recovered_text[100];
    char flag[] = "01111110";

    int i, j = 0, b;
    int count = 0;
    int parity = 0, receivedParity, newParity = 0;
    int choice;

    // --- SENDER SIDE ---
    printf("Enter the string: ");
    scanf("%99s", text);

    int len = strlen(text);
    int bit_index = 0;

    // Convert input string characters to binary data bits
    for (i = 0; i < len; i++)
    {
        unsigned char ascii = text[i];
        for (b = 7; b >= 0; b--)
        {
            if ((ascii >> b) & 1)
                data[bit_index++] = '1';
            else
                data[bit_index++] = '0';
        }
    }
    data[bit_index] = '\0';

    // Calculate Even Parity Bit
    for (i = 0; data[i] != '\0'; i++)
    {
        if (data[i] == '1')
            parity ^= 1;
    }
   printf("\nGenerated Parity Bit: %d\n", parity);

    // Frame assembly: Copy opening flag
    strcpy(stuffed, flag);
    j = strlen(flag);
    count = 0;

    // Perform Bit Stuffing (Insert '0' after five consecutive '1's)
    for (i = 0; data[i] != '\0'; i++)
    {
        stuffed[j++] = data[i];
        if (data[i] == '1')
        {
            count++;
        }
        else
        {
            count = 0;
        }

        if (count == 5)
        {
            stuffed[j++] = '0';
            count = 0;
        }
    }
    stuffed[j] = '\0';

    // Append closing flag
    strcat(stuffed, flag);

    printf("\n========== Sender Side ==========\n");
    printf("Original Text      : %s\n", text);
    printf("Original Data Bits : %s\n", data);
    printf("Stuffed Frame      : %s\n", stuffed);
    printf("Transmitted Frame  : %s\n", stuffed);

    // --- CHANNEL SIMULATION ---
    strcpy(received, stuffed);
    receivedParity = parity;

    printf("\nDo you want to simulate an error? (1 or 0): ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        int errorPos = strlen(flag) + 4; // Introduces error inside payload
        if (received[errorPos] == '0')
            received[errorPos] = '1';
        else
            received[errorPos] = '0';
        printf("Error introduced at bit position: %d\n", errorPos);
    }

    // --- RECEIVER SIDE ---
    printf("\n========== Receiver Side ==========\n");
    printf("Received Frame     : %s\n", received);
    printf("Received Parity    : %d\n", receivedParity);

    int flag_len = strlen(flag);
    int received_len = strlen(received);
    i = flag_len;
    j = 0;
    count = 0;

    // Perform De-stuffing (Remove '0' that follows five consecutive '1's)
    while (i < (received_len - flag_len))
    {
        destuffed[j++] = received[i];
        if (received[i] == '1')
        {
            count++;
        }
        else
        {
            count = 0;
        }

        if (count == 5)
        {
            i++; // Skip the stuffed bit
            count = 0;
        }
        i++;
    }
    destuffed[j] = '\0';
    printf("De-stuffed Data    : %s\n", destuffed);

    // Recalculate Parity on De-stuffed Data
    newParity = 0;
    for (i = 0; destuffed[i] != '\0'; i++)
    {
        if (destuffed[i] == '1')
            newParity ^= 1;
    }
    printf("Calculated Parity  : %d\n", newParity);

    // --- ERROR CHECK & MESSAGE HANDLING ---
    if (newParity == receivedParity)
    {
        printf("Status             : No Error Detected\n");

        // Recover plain text from binary bits
        int char_index = 0;
        for (i = 0; i < strlen(destuffed); i += 8)
        {
            unsigned char current_char = 0;
            for (b = 0; b < 8; b++)
            {
                current_char <<= 1;
                if (destuffed[i + b] == '1')
                    current_char |= 1;
            }
            recovered_text[char_index++] = current_char;
        }
        recovered_text[char_index] = '\0';
        printf("Recovered Text     : %s\n", recovered_text);
    }
    else
    {
        printf("Status             : Error Detected\n");
        printf("Action             : Message Discarded due to corruption.\n");

        // Ensure character string remains completely blank
        recovered_text[0] = '\0';
    }

    return 0;
}
