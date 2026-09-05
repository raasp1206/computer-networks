#include <stdio.h>

void stopAndWait(int n, int lost)
{
    int i, total = 0;

    printf("\n===== STOP-AND-WAIT ARQ =====\n");
    printf("Window Size = 1\n");

    for (i = 0; i < n; i++)
    {
        printf("\nFrame %d transmitted\n", i);
        total++;

        if (i == lost)
        {
            printf("Frame %d LOST/CORRUPTED\n", i);
            printf("Timeout occurred for Frame %d\n", i);

            printf("Frame %d retransmitted\n", i);
            total++;

            printf("Frame %d correctly received\n", i);
            printf("ACK %d received\n", i);

            lost = -1;
        }
        else
        {
            printf("Frame %d correctly received\n", i);
            printf("ACK %d received\n", i);
        }
    }

    printf("\nAll frames delivered successfully.\n");
    printf("Total transmissions = %d\n", total);
}


void goBackN(int n, int w, int lost)
{
    int i = 0, j, k;
    int total = 0;

    printf("\n===== GO-BACK-N ARQ =====\n");
    printf("Window Size = %d\n", w);

    while (i < n)
    {
        printf("\nTransmitting Window: ");

        for (j = i; j < i + w && j < n; j++)
            printf("%d ", j);

        printf("\n");

        for (j = i; j < i + w && j < n; j++)
        {
            printf("\nFrame %d transmitted\n", j);
            total++;

            if (j == lost)
            {
                printf("Frame %d LOST/CORRUPTED\n", j);
                printf("Timeout occurred for Frame %d\n", j);

                printf("\nGo-Back-N: Retransmitting from Frame %d\n", j);

                for (k = j; k < i + w && k < n; k++)
                {
                    printf("Frame %d retransmitted\n", k);
                    total++;

                    printf("ACK %d received\n", k);
                }

                lost = -1;
                break;
            }
            else
            {
                printf("Frame %d correctly received\n", j);
                printf("ACK %d received\n", j);
            }
        }

        i = i + w;
    }

    printf("\nAll frames delivered successfully.\n");
    printf("Total transmissions = %d\n", total);
}


void selectiveRepeat(int n, int w, int lost)
{
    int i, j;
    int total = 0;

    printf("\n===== SELECTIVE REPEAT ARQ =====\n");
    printf("Window Size = %d\n", w);

    for (i = 0; i < n; i += w)
    {
        printf("\nTransmitting Window: ");

        for (j = i; j < i + w && j < n; j++)
            printf("%d ", j);

        printf("\n");

        for (j = i; j < i + w && j < n; j++)
        {
            printf("\nFrame %d transmitted\n", j);
            total++;

            if (j == lost)
            {
                printf("Frame %d LOST/CORRUPTED\n", j);
                printf("No ACK received for Frame %d\n", j);
            }
            else
            {
                printf("Frame %d correctly received\n", j);
                printf("ACK %d received\n", j);
            }
        }

        /* Retransmit only lost frame */
        if (lost >= i && lost < i + w)
        {
            printf("\nTimeout occurred for Frame %d\n", lost);

            printf("Selective Repeat: Retransmitting only Frame %d\n",
                   lost);

            printf("Frame %d retransmitted\n", lost);
            total++;

            printf("Frame %d correctly received\n", lost);
            printf("ACK %d received\n", lost);

            lost = -1;
        }
    }

    printf("\nAll frames delivered successfully.\n");
    printf("Total transmissions = %d\n", total);
}


int main()
{
    int choice;
    int n, w, lost;

    printf("========================================\n");
    printf("       SLIDING WINDOW PROTOCOLS\n");
    printf("========================================\n");

    printf("\n1. Stop-and-Wait ARQ");
    printf("\n2. Go-Back-N ARQ");
    printf("\n3. Selective Repeat ARQ");
    printf("\n4. Exit");

    printf("\n\nEnter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            printf("\nEnter number of frames: ");
            scanf("%d", &n);

            printf("Enter frame to be lost/corrupted: ");
            scanf("%d", &lost);

            if (lost < 0 || lost >= n)
            {
                printf("Invalid frame number!\n");
                break;
            }

            stopAndWait(n, lost);
            break;


        case 2:
            printf("\nEnter number of frames: ");
            scanf("%d", &n);

            printf("Enter window size: ");
            scanf("%d", &w);

            printf("Enter frame to be lost/corrupted: ");
            scanf("%d", &lost);

            if (w <= 0 || w > n || lost < 0 || lost >= n)
            {
                printf("Invalid input!\n");
                break;
            }

            goBackN(n, w, lost);
            break;


        case 3:
            printf("\nEnter number of frames: ");
            scanf("%d", &n);

            printf("Enter window size: ");
            scanf("%d", &w);

            printf("Enter frame to be lost/corrupted: ");
            scanf("%d", &lost);

            if (w <= 0 || w > n || lost < 0 || lost >= n)
            {
                printf("Invalid input!\n");
                break;
            }

            selectiveRepeat(n, w, lost);
            break;


        case 4:
            printf("\nProgram terminated.\n");
            break;


        default:
            printf("\nInvalid choice!\n");
    }

    return 0;
}
