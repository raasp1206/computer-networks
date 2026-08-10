#include <stdio.h>
#include <string.h>
#define MAX 1000

char message[100];
char binary[MAX];
char frame[MAX];
char receivedChecksum[MAX];
char destuffed[MAX]; // Kept for variable compatibility

int blockSize = 0;

// Convert character to standard 8-bit binary
void charToBinary(char ch, char bin[])
{
    int i;
    int val = (int)ch;
    for(i = 7; i >= 0; i--)
    {
        bin[i] = (val % 2) + '0';
        val /= 2;
    }
    bin[8] = '\0';
}

// Convert input string message to binary
void messageToBinary(char msg[], char output[])
{
    int i;
    char temp[9];
    output[0] = '\0';

    for(i = 0; msg[i] != '\0'; i++)
    {
        charToBinary(msg[i], temp);
        strcat(output, temp);
    }
}

// Convert a binary string chunk back to text characters
void binaryToMessage(char binStr[], char msg[])
{
    int i = 0, j = 0;
    int len = strlen(binStr);

    while(i < len)
    {
        int value = 0;
        int k;
        for(k = 0; k < 8; k++)
        {
            if (i + k < len)
            {
                value = value * 2 + (binStr[i+k] - '0');
            }
        }
        msg[j++] = (char)value;
        i += 8;
    }
    msg[j] = '\0';
}

// Adds two binary strings of fixed block size together
void addBinaryStrings(char a[], char b[], char result[])
{
    int carry = 0;
    int i;
    char temp[MAX];
    temp[blockSize] = '\0';

    for (i = blockSize - 1; i >= 0; i--)
    {
        int sum = (a[i] - '0') + (b[i] - '0') + carry;
        if (sum == 0) { temp[i] = '0'; carry = 0; }
        else if (sum == 1) { temp[i] = '1'; carry = 0; }
        else if (sum == 2) { temp[i] = '0'; carry = 1; }
        else if (sum == 3) { temp[i] = '1'; carry = 1; }
    }

    // If there is an end carry, handle the wrap-around carry addition
    if (carry == 1)
    {
        char one[MAX];
        for (i = 0; i < blockSize - 1; i++) one[i] = '0';
        one[blockSize - 1] = '1';
        one[blockSize] = '\0';

        // Add carry '1' back to the sum result
        int secondaryCarry = 0;
        for (i = blockSize - 1; i >= 0; i--)
        {
            int sum = (temp[i] - '0') + (one[i] - '0') + secondaryCarry;
            if (sum == 0) { result[i] = '0'; secondaryCarry = 0; }
            else if (sum == 1) { result[i] = '1'; secondaryCarry = 0; }
            else if (sum == 2) { result[i] = '0'; secondaryCarry = 1; }
            else if (sum == 3) { result[i] = '1'; secondaryCarry = 1; }
        }
    }
    else
    {
        strcpy(result, temp);
    }
    result[blockSize] = '\0';
}

// Groups binary data into blocks, performs additions, and returns 1's complement checksum
void computeChecksumBits(char bitStream[], char checksumOut[])
{
    char currentSum[MAX];
    char nextBlock[MAX];
    int len = strlen(bitStream);
    int i, j;

    // Initialize first block sum with all zeroes
    for (i = 0; i < blockSize; i++) currentSum[i] = '0';
    currentSum[blockSize] = '\0';

    // Loop through the data in chunks of blockSize
    for (i = 0; i < len; i += blockSize)
    {
        // Extract block chunk safely
        for (j = 0; j < blockSize; j++)
        {
            if (i + j < len)
                nextBlock[j] = bitStream[i + j];
            else
                nextBlock[j] = '0'; // Pad trailing empty spaces with 0
        }
        nextBlock[blockSize] = '\0';

        char tempResult[MAX];
        addBinaryStrings(currentSum, nextBlock, tempResult);
        strcpy(currentSum, tempResult);
    }

    // Invert bits to calculate One's Complement Checksum
    for (i = 0; i < blockSize; i++)
    {
        checksumOut[i] = (currentSum[i] == '0') ? '1' : '0';
    }
    checksumOut[blockSize] = '\0';
}

// Sender Module
void sender()
{
    char generatedChecksum[MAX];

    printf("Enter plaintext message: ");
    fgets(message, 100, stdin);
    message[strcspn(message, "\n")] = '\0';

    messageToBinary(message, binary);

    printf("\n-------------------------------------------------\n");
    printf("Binary Transformed Data: %s\n", binary);
    printf("-------------------------------------------------\n");

    printf("\nEnter bit block size: ");
    scanf("%d", &blockSize);
    getchar();

    computeChecksumBits(binary, generatedChecksum);
    strcpy(receivedChecksum, generatedChecksum);

    // Frame layout assembly: [Payload Data] + [Checksum Bits]
    strcpy(frame, binary);
    strcat(frame, generatedChecksum);

    printf("\n-------------------------------------------------\n");
    printf("Calculated Checksum Bits : %s\n", generatedChecksum);
    printf("Final Transmitted Frame  : %s\n", frame);
    printf("-------------------------------------------------\n");
}

// Receiver Module
void receiver()
{
    char dataPayload[MAX];
    char extractedChecksum[MAX];
    char combinedReceiverStream[MAX];
    char finalCheckResult[MAX];
    int choice;

    int frameLen = strlen(frame);
    int payloadLen = frameLen - blockSize;

    // Isolate payload from trailing bits
    strncpy(dataPayload, frame, payloadLen);
    dataPayload[payloadLen] = '\0';

    strncpy(extractedChecksum, &frame[payloadLen], blockSize);
    extractedChecksum[blockSize] = '\0';

    printf("\n\n----------------- RECEIVER SIDE -----------------\n");
    printf("Received Frame Stream: %s\n", frame);
    printf("Extracted Data Block : %s\n", dataPayload);
    printf("Extracted Checksum   : %s\n", extractedChecksum);
    printf("-------------------------------------------------\n");

    // Configured error simulation menu layout
    printf("\nError Simulation Menu:\n");
    printf("1. No Error\n");
    printf("2. Single Bit Error\n");
    printf("3. Multi-Bit Error\n");
    printf("Enter choice layout: ");
    scanf("%d", &choice);

    if (choice == 2)
    {
        int pos;
        printf("Enter bit index to flip (0-%d): ", payloadLen - 1);
        scanf("%d", &pos);
        if (pos >= 0 && pos < payloadLen)
        {
            dataPayload[pos] = (dataPayload[pos] == '0') ? '1' : '0';
        }
    }
    else if (choice == 3)
    {
        int n, i, pos;
        printf("How many bits to corrupt? ");
        scanf("%d", &n);
        for (i = 0; i < n; i++)
        {
            printf("Enter bit index position: ");
            scanf("%d", &pos);
            if (pos >= 0 && pos < payloadLen)
            {
                dataPayload[pos] = (dataPayload[pos] == '0') ? '1' : '0';
            }
        }
    }

    if (choice == 2 || choice == 3)
    {
        printf("\nModified Corrupted Payload Data: %s\n", dataPayload);
    }

    // Combine payload + original checksum to check for zero matching values
    strcpy(combinedReceiverStream, dataPayload);
    strcat(combinedReceiverStream, extractedChecksum);

    computeChecksumBits(combinedReceiverStream, finalCheckResult);

    // Scan the final complement bitstream for errors
    int errorDetected = 0,i;
    for (i = 0; i < blockSize; i++)
    {
        if (finalCheckResult[i] != '0')
        {
            errorDetected = 1;
            break;
        }
    }

    printf("\n-------------------------------------------------\n");
    printf("Receiver Checksum Inversion Result: %s\n", finalCheckResult);

    if (errorDetected == 0)
    {
        char decodedMessage[100];
        printf("\n---> RESULT: NO ERROR DETECTED (VALID FRAME) <---\n");
        binaryToMessage(dataPayload, decodedMessage);
        printf("Decoded Text: %s\n", decodedMessage);
    }
    else
    {
        printf("\n---> RESULT: ERROR DETECTED (INVALID FRAME) <---\n");
        printf("Checksum check failed! Packet data discarded.\n");
    }
    printf("-------------------------------------------------\n");
}

// Program Core
int main()
{
    printf("================ SENDER PROCESS ================\n");
    sender();

    printf("\n================ RECEIVER PROCESS ================\n");
    receiver();

    return 0;
}
