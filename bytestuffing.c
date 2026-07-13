#include <stdio.h>
#include <string.h>

int main() {
    char word[100];
    int data[100][8], stuffed_output[200][8], destuffed_output[100][8];
    char recovered_word[100];

    int flag[8] = {0,1,1,1,1,1,1,0};
    int esc[8]  = {0,1,1,1,1,1,0,1};

    int i, k;
    int stuffed_idx = 0;
    int destuffed_idx = 0;
    int stuffing_counter = 0;

    printf("Enter a word: ");
    scanf("%99s", word);
    int n = strlen(word);
    for (i = 0; i < n; i++) {
        unsigned char ch = word[i];
        for (k = 7; k >= 0; k--) {
            data[i][k] = ch % 2;
            ch /= 2;
        }
    }

    for (k = 0; k < 8; k++) {
        stuffed_output[stuffed_idx][k] = flag[k];
    }
    stuffed_idx++;

    for (i = 0; i < n; i++) {
        int isFlag = 1;
        int isEsc = 1;
        for (k = 0; k < 8; k++) {
            if (data[i][k] != flag[k]) isFlag = 0;
            if (data[i][k] != esc[k])  isEsc = 0;
        }

        if (isFlag || isEsc) {
            for (k = 0; k < 8; k++) {
                stuffed_output[stuffed_idx][k] = esc[k];
            }
            stuffed_idx++;
            stuffing_counter++;
        }

        for (k = 0; k < 8; k++) {
            stuffed_output[stuffed_idx][k] = data[i][k];
        }
        stuffed_idx++;
    }

    for (k = 0; k < 8; k++) {
        stuffed_output[stuffed_idx][k] = flag[k];
    }
    stuffed_idx++;

    for (i = 1; i < stuffed_idx - 1; i++) {
        int isEsc = 1;
        for (k = 0; k < 8; k++) {
            if (stuffed_output[i][k] != esc[k]) {
                isEsc = 0;
                break;
            }
        }

        if (isEsc) {

           i++;

           for (k = 0; k < 8; k++) {
              destuffed_output[destuffed_idx][k] = stuffed_output[i][k];
            }

            destuffed_idx++;

        } else {

           for (k = 0; k < 8; k++) {
                destuffed_output[destuffed_idx][k] = stuffed_output[i][k];
            }

           destuffed_idx++;
        }
    }

    for (i = 0; i < destuffed_idx; i++) {
        int ascii_val = 0;
        for (k = 0; k < 8; k++) {
            ascii_val = (ascii_val << 1) | destuffed_output[i][k];
        }
        recovered_word[i] = (char)ascii_val;
    }
    recovered_word[destuffed_idx] = '\0';
    printf("\nOriginal Word: %s\n", word);
    printf("Original Binary: \n");
    for (i = 0; i < n; i++) {
        for (k = 0; k < 8; k++) printf("%d", data[i][k]);
        printf(" ");
    }
    printf("\n\nTransmitted Frame (with Flags & Stuffing):\n");
    for (i = 0; i < stuffed_idx; i++) {
        for (k = 0; k < 8; k++) printf("%d", stuffed_output[i][k]);
        printf(" ");
    }

    printf("\n\nByte Stuffing Status: %s\n", stuffing_counter ? "Performed" : "Not Required");

    printf("\nDestuffed Binary at Receiver:\n");
    for (i = 0; i < destuffed_idx; i++) {
        for (k = 0; k < 8; k++) printf("%d", destuffed_output[i][k]);
        printf(" ");
    }

    printf("\n\nRecovered Word at Receiver: %s\n", recovered_word);

    return 0;
}
