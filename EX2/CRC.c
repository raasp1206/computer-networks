#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function to perform modulo-2 binary division (XOR)
void xorDivision(char *dividend, const char *generator, char *remainder) {
    int genLen = strlen(generator);
    int divLen = strlen(dividend);
    int i,j;
    char *temp = (char *)malloc((divLen + 1) * sizeof(char));
    strcpy(temp, dividend);

    for (i = 0; i <= divLen - genLen; i++) {
        if (temp[i] == '1') {
            for (j = 0; j < genLen; j++) {
                temp[i + j] = (temp[i + j] == generator[j]) ? '0' : '1';
            }
        }
    }

    strncpy(remainder, temp + divLen - genLen + 1, genLen - 1);
    remainder[genLen - 1] = '\0';
    free(temp);
}

// Function to check if a binary string consists entirely of '0's
int isZero(const char *str) {
    while (*str) {
        if (*str != '0') return 0;
        str++;
    }
    return 1;
}

// Receiver verification engine
void receiverVerify(const char *receivedCodeword, const char *generator) {
    int genLen = strlen(generator);
    char *remainder = (char *)malloc(genLen * sizeof(char));

    xorDivision((char *)receivedCodeword, generator, remainder);

    printf("Received Codeword : %s\n", receivedCodeword);
    printf("Receiver Remainder: %s\n", remainder);

    if (isZero(remainder)) {
        printf("Result            : ✅ DATA VALID (No errors detected)\n\n");
    } else {
        printf("Result            : ❌ ERROR DETECTED (Data is corrupted)\n\n");
    }
    free(remainder);
}

int main() {
    char textInput[256];
    char generator[64];
    char binaryData[2048] = "";
    char appendedData[2112];
    char remainder[64];
    char codeword[2112];
    char corruptedCodeword[2112];
    char simChoice, errorType;
    int bitIndex, numErrors;
    int i,j;
    printf("--- Text-Based Interactive CRC Simulation ---\n");

    printf("Enter Sender Text String (e.g., Hi): ");
    scanf("%255s", textInput);
    printf("Enter Generator Polynomial (e.g., 1011): ");
    scanf("%63s", generator);

    // Convert text characters into an 8-bit binary string safely
    int textLen = strlen(textInput);
    for (i = 0; i < textLen; i++) {
        char ch = textInput[i];
        for (j = 7; j >= 0; j--) {
            if ((ch >> j) & 1) {
                strcat(binaryData, "1");
            } else {
                strcat(binaryData, "0");
            }
        }
    }

    int genLen = strlen(generator);

    // Prepare appended data string
    strcpy(appendedData, binaryData);
    for (i = 0; i < genLen - 1; i++) {
        strcat(appendedData, "0");
    }

    // Generate CRC Remainder
    xorDivision(appendedData, generator, remainder);

    // Construct the final transmitted Codeword
    strcpy(codeword, binaryData);
    strcat(codeword, remainder);
    int cwLen = strlen(codeword);

    printf("\n================ TRANSMITTER SIDE ================\n");
    printf("Sender Text Input   : %s\n", textInput);
    printf("Converted Binary    : %s\n", binaryData);
    printf("Generator Polynomial: %s\n", generator);
    printf("Generated Remainder : %s\n", remainder);
    printf("Transmitted Codeword: %s (Length: %d bits, Valid indices: 0 to %d)\n", codeword, cwLen, cwLen - 1);
    printf("==================================================\n\n");

    // NEW: Automatic check before prompting user for errors
    printf("--- Initial Check (Before Error Simulation) ---\n");
    receiverVerify(codeword, generator);

    // Dynamic Choice Prompt: Simulate error?
    printf("Do you want to simulate a transmission error? (y/n): ");
    scanf(" %c", &simChoice);

    strcpy(corruptedCodeword, codeword);

    if (simChoice == 'y' || simChoice == 'Y') {
        // Prompt for Single or Multi-bit
        printf("Enter error type - Single bit (s) or Multi-bit (m): ");
        scanf(" %c", &errorType);

        if (errorType == 's' || errorType == 'S') {
            printf("Enter the bit index to flip (0 to %d): ", cwLen - 1);
            scanf("%d", &bitIndex);

            if (bitIndex >= 0 && bitIndex < cwLen) {
                corruptedCodeword[bitIndex] = (corruptedCodeword[bitIndex] == '1') ? '0' : '1';
                printf("\n--- TRANSMISSION LINK (Single-Bit Error Injected at Index %d) ---\n", bitIndex);
            } else {
                printf("\n⚠️ Invalid index! Proceeding with clean codeword.\n");
            }
        }
        else if (errorType == 'm' || errorType == 'M') {
            printf("How many bits do you want to flip? ");
            scanf("%d", &numErrors);

            printf("Enter the %d bit indices separated by spaces (e.g., 0 2 5): ", numErrors);
            for (i = 0; i < numErrors; i++) {
                scanf("%d", &bitIndex);
                if (bitIndex >= 0 && bitIndex < cwLen) {
                    corruptedCodeword[bitIndex] = (corruptedCodeword[bitIndex] == '1') ? '0' : '1';
                }
            }
            printf("\n--- TRANSMISSION LINK (Multi-Bit Errors Injected) ---\n");
        }
        else {
            printf("\n⚠️ Invalid choice! Running without modifying bits.\n");
        }

        // Verify the corrupted data
        receiverVerify(corruptedCodeword, generator);

    } else {
        printf("\n--- TRANSMISSION LINK (Perfect Channel Chosen) ---\n");
        receiverVerify(codeword, generator);
    }

    return 0;
}
