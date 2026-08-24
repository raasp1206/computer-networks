#include <stdio.h>

int hammingDistance(int x, int y) {
    int xor_result = x ^ y;
    int distance = 0;

    while (xor_result > 0) {
        xor_result &= (xor_result - 1);
        distance++;
    }

    return distance;
}

int main() {
    int num1, num2;
    int result;

    // Ask user for the first integer
    printf("Enter the first integer (x): ");
    if (scanf("%d", &num1) != 1) {
        printf("Invalid input. Please enter an integer.\n");
        return 1;
    }

    // Ask user for the second integer
    printf("Enter the second integer (y): ");
    if (scanf("%d", &num2) != 1) {
        printf("Invalid input. Please enter an integer.\n");
        return 1;
    }

    // Calculate the Hamming distance
    result = hammingDistance(num1, num2);

    // Print the result
    printf("The Hamming distance between %d and %d is: %d\n", num1, num2, result);

    return 0;
}
