#include <stdio.h>
#include <string.h>

#define MAX 1000

char message[100];
char binary[MAX];
char frame[MAX];
char parityType[10];


// Convert character to 8-bit binary
void charToBinary(char ch, char bin[])
{
    int i;

    for(i = 7; i >= 0; i--)
    {
        bin[7-i] = ((ch >> i) & 1) ? '1' : '0';
    }

    bin[8] = '\0';
}


// Convert message to binary
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


// Count number of 1s
int countOnes(char data[])
{
    int i, count = 0;

    for(i = 0; data[i] != '\0'; i++)
    {
        if(data[i] == '1')
            count++;
    }

    return count;
}


// Generate parity bit
char computeParityBit(char data[], char type[])
{
    int ones = countOnes(data);


    if(strcmp(type,"EVEN") == 0)
    {
        if(ones % 2 == 0)
            return '0';
        else
            return '1';
    }

    else
    {
        if(ones % 2 == 0)
            return '1';
        else
            return '0';
    }
}


// Binary to message
void binaryToMessage(char data[], char msg[])
{
    int i = 0;
    int j = 0;


    while(data[i] != '\0')
    {
        int value = 0;
        int k;


        for(k = 0; k < 8; k++)
        {
            value = value * 2 + (data[i+k]-'0');
        }


        msg[j++] = (char)value;

        i += 8;
    }


    msg[j] = '\0';
}



// ---------------- GENERATOR ----------------

void generator()
{
    int choice;
    char parityBit;


    printf("Enter message: ");
    fgets(message,100,stdin);

    message[strcspn(message,"\n")] = '\0';



    printf("\nChoose Parity Type\n");
    printf("1. Even Parity\n");
    printf("2. Odd Parity\n");

    printf("Enter choice: ");
    scanf("%d",&choice);


    getchar();



    if(choice == 2)
        strcpy(parityType,"ODD");
    else
        strcpy(parityType,"EVEN");



    messageToBinary(message,binary);



    parityBit = computeParityBit(binary,parityType);



    strcpy(frame,binary);

    frame[strlen(binary)] = parityBit;

    frame[strlen(binary)+1] = '\0';



    printf("\n---------- GENERATOR ----------\n");

    printf("Original Message : %s\n",message);

    printf("Binary Data : %s\n",binary);

    printf("Parity Type : %s\n",parityType);

    printf("Parity Bit : %c\n",parityBit);

    printf("Transmitted Frame: %s\n",frame);

}



// ---------------- CHECKER ----------------

void checker()
{
    char received[MAX];

    char data[MAX];

    char receivedParity;

    char calculatedParity;

    char decoded[100];

    int choice;

    int len;



    strcpy(received,frame);



    printf("\n---------- CHECKER ----------\n");

    printf("Received Frame : %s\n",received);



    printf("\nError Simulation\n");

    printf("0. No Error\n");

    printf("1. Single Bit Error\n");

    printf("2. Two Bit Error\n");

    printf("3. Custom Error\n");

    printf("Enter choice: ");

    scanf("%d",&choice);



    if(choice == 1)
    {
        int pos;

        printf("Enter bit position (0-%d): ",
               strlen(received)-1);

        scanf("%d",&pos);


        if(pos >=0 && pos < strlen(received))
        {
            received[pos] =
            (received[pos]=='0')?'1':'0';
        }
    }


    else if(choice == 2)
    {
        if(strlen(received)>=2)
        {
            received[0]=(received[0]=='0')?'1':'0';

            received[1]=(received[1]=='0')?'1':'0';
        }
    }


    else if(choice == 3)
    {
        int n,i,pos;


        printf("Number of bits to flip: ");
        scanf("%d",&n);


        for(i=0;i<n;i++)
        {
            printf("Enter position: ");
            scanf("%d",&pos);


            if(pos>=0 && pos<strlen(received))
            {
                received[pos] =
                (received[pos]=='0')?'1':'0';
            }
        }
    }



    printf("\nFrame Used For Checking:\n%s\n",
            received);



    len=strlen(received);



    strncpy(data,received,len-1);

    data[len-1]='\0';



    receivedParity = received[len-1];



    calculatedParity =
        computeParityBit(data,parityType);



    printf("\nData Bits : %s\n",data);

    printf("Received Parity : %c\n",receivedParity);

    printf("Calculated Parity : %c\n",calculatedParity);



    if(receivedParity == calculatedParity)
    {
        printf("\n>>> NO ERROR DETECTED <<<\n");


        binaryToMessage(data,decoded);


        printf("Decoded Message : %s\n",decoded);
    }

    else
    {
        printf("\n>>> ERROR DETECTED <<<\n");

        printf("Data discarded\n");
    }

}



// ---------------- MAIN ----------------

int main()
{

    printf("========== PARITY GENERATOR ==========\n");

    generator();



    printf("\n========== PARITY CHECKER ==========\n");

    checker();



    return 0;
}
