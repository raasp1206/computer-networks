#include <stdio.h>
#include <string.h>
int main()
{
    char str[100];
    int binary_stream[800];
    int stuffed_stream[1200];
    int destuffed_stream[800];
    int flag[8] = {0,1,1,1,1,1,1,0};
    int bit_idx = 0;
    int stuffed_idx = 0;
    int destuffed_idx = 0;
    int count = 0;
    int stuffing_performed = 0;
    int len, i, b;
    char ch;
    printf("Enter word: ");
    fgets(str, sizeof(str), stdin);
    str[strcspn(str, "\n")] = '\0';
    len = strlen(str);
    for(i = 0; i < len; i++)
    {
        ch = str[i];
        for(b = 7; b >= 0; b--)
        {
            binary_stream[bit_idx++] = (ch >> b) & 1;
        }
    }
    for(i = 0; i < bit_idx; i++)
    {
        stuffed_stream[stuffed_idx++] = binary_stream[i];

        if(binary_stream[i] == 1)
            count++;
        else
            count = 0;
        if(count == 5)
        {
            stuffed_stream[stuffed_idx++] = 0;
            stuffing_performed = 1;
            count = 0;
        }
    }
    printf("\nOriginal Binary data: ");
    for(i = 0; i < bit_idx; i++)
        printf("%d", binary_stream[i]);

    printf("\nTransmitted data : ");
    for(i = 0; i < 8; i++) printf("%d", flag[i]);
    for(i = 0; i < stuffed_idx; i++) printf("%d", stuffed_stream[i]);
    for(i = 0; i < 8; i++) printf("%d", flag[i]);
    if(stuffing_performed)
        printf("\n status : Bit Stuffing Performed.");
    else
        printf("\nstatus : No Bit Stuffing Required.");
    count = 0;
    for(i = 0; i < stuffed_idx; i++)
    {
        destuffed_stream[destuffed_idx++] = stuffed_stream[i];
        if(stuffed_stream[i] == 1)
            count++;
        else
            count = 0;
        if(count == 5)
        {
            i++;
            count = 0;
        }
    }
    printf("\n\ndestuffed data: ");
    for(i = 0; i < destuffed_idx; i++)
        printf("%d", destuffed_stream[i]);
    printf("\noriginal data  : ");
    for(i = 0; i < destuffed_idx; i += 8)
    {
        ch = 0;
        for(b = 0; b < 8; b++)
        {
            ch = (ch << 1) | destuffed_stream[i + b];
        }
        printf("%c", ch);
    }
    printf("\n");
    return 0;
}
