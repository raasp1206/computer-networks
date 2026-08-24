#include <stdio.h>

int main()
{
    int config, choice, pos, i, errorPos;
    printf("\nSelect Hamming Code:\n");
    printf("1. Hamming Code (7,4)\n");
    printf("2. Hamming Code (12,8)\n");
    printf("Enter your choice: ");
    scanf("%d", &config);

    if(config == 1) {
        int data[4];
        int h[8], s1, s2, s4;
        printf("Enter 4 Data Bits: ");
        for(i = 0; i < 4; i++)
        {
            scanf("%1d", &data[i]);
            if(data[i] != 0 && data[i] != 1)
            {
                printf("Invalid input! Enter only 0 or 1.\n");
                return 0;
            }
        }
        h[7] = data[0];
        h[6] = data[1];
        h[5] = data[2];
        h[3] = data[3];

        h[1] = h[3] ^ h[5] ^ h[7];
        h[2] = h[3] ^ h[6] ^ h[7];
        h[4] = h[5] ^ h[6] ^ h[7];

        printf("\nGenerated Hamming Code: ");
        for(i = 1; i <= 7; i++)
            printf("%d", h[i]);

        printf("\n\nDo you want to generate an error?");
        printf("\n1. Yes\n2. No\nEnter choice: ");
        scanf("%d", &choice);

        if(choice == 1){
            printf("Enter error position (1-7): ");
            scanf("%d", &pos);
            if(pos < 1 || pos > 7)
            {
                printf("Invalid position!\n");
                return 0;
            }
            h[pos] = h[pos] ^ 1;
            printf("Error introduced at position %d.\n", pos);
        }
        else if(choice != 2)
        {
            printf("Invalid choice!\n");
            return 0;
        }

        printf("\nReceived Code: ");
        for(i = 1; i <= 7; i++)
            printf("%d", h[i]);

        s1 = h[1] ^ h[3] ^ h[5] ^ h[7];
        s2 = h[2] ^ h[3] ^ h[6] ^ h[7];
        s4 = h[4] ^ h[5] ^ h[6] ^ h[7];

        errorPos = s1 + (s2 * 2) + (s4 * 4);

        if(errorPos == 0){
            printf("\n\nNo Error Detected.\n");
        }
        else
        {
            printf("\n\nError Detected at position: %d", errorPos);
            h[errorPos] = h[errorPos] ^ 1;
            printf("\nCorrected Hamming Code: ");
            for(i = 1; i <= 7; i++)
                printf("%d", h[i]);
            printf("\n");
        }
        printf("Original Data Recovered: %d%d%d%d\n", h[7], h[6], h[5], h[3]);
    }
    else if(config == 2) {
        int data[8];
        int h[13], s1, s2, s4, s8;
        printf("Enter 8 Data Bits (consecutive or space-separated): ");
        for(i = 0; i < 8; i++)
        {
            scanf("%1d", &data[i]);
            if(data[i] != 0 && data[i] != 1)
            {
                printf("Invalid input! Enter only 0 or 1.\n");
                return 0;
            }
        }
        h[12] = data[0];
        h[11] = data[1];
        h[10] = data[2];
        h[9]  = data[3];
        h[7]  = data[4];
        h[6]  = data[5];
        h[5]  = data[6];
        h[3]  = data[7];

        h[1] = h[3] ^ h[5] ^ h[7] ^ h[9] ^ h[11];
        h[2] = h[3] ^ h[6] ^ h[7] ^ h[10] ^ h[11];
        h[4] = h[5] ^ h[6] ^ h[7] ^ h[12];
        h[8] = h[9] ^ h[10] ^ h[11] ^ h[12];

        printf("\nGenerated Hamming Code: ");
        for(i = 1; i <= 12; i++)
            printf("%d", h[i]);

        printf("\n\nDo you want to generate an error?");
        printf("\n1. Yes\n2. No\nEnter choice: ");
        scanf("%d", &choice);

        if(choice == 1){
            printf("Enter error position (1-12): ");
            scanf("%d", &pos);
            if(pos < 1 || pos > 12)
            {
                printf("Invalid position!\n");
                return 0;
            }
            h[pos] = h[pos] ^ 1;
            printf("Error introduced at position %d.\n", pos);
        }
        else if(choice != 2)
        {
            printf("Invalid choice!\n");
            return 0;
        }

        printf("\nReceived Code: ");
        for(i = 1; i <= 12; i++)
            printf("%d", h[i]);

        // Syndrome calculations for 12-bit code
        s1 = h[1] ^ h[3] ^ h[5] ^ h[7] ^ h[9] ^ h[11];
        s2 = h[2] ^ h[3] ^ h[6] ^ h[7] ^ h[10] ^ h[11];
        s4 = h[4] ^ h[5] ^ h[6] ^ h[7] ^ h[12];
        s8 = h[8] ^ h[9] ^ h[10] ^ h[11] ^ h[12];

        errorPos = s1 + (s2 * 2) + (s4 * 4) + (s8 * 8);

        if(errorPos == 0){
            printf("\n\nNo Error Detected.\n");
        }
        else
        {
            printf("\n\nError Detected at position: %d", errorPos);
            h[errorPos] = h[errorPos] ^ 1; // Correct the bit
            printf("\nCorrected Hamming Code: ");
            for(i = 1; i <= 12; i++)
                printf("%d", h[i]);
            printf("\n");
        }
        printf("Original Data Recovered: %d%d%d%d%d%d%d%d\n",
               h[12], h[11], h[10], h[9], h[7], h[6], h[5], h[3]);
    }
    else {
        printf("Invalid choice selection!\n");
    }
    return 0;
}
