#include <stdio.h>
#include <stdlib.h>

int totalHammingDistance(int* nums, int numsSize) {
    int total_distance = 0;
    int i, j; // Declared outside the loops to support older C standards

    // Iterate through each of the 32 bit positions
    for (i = 0; i < 32; i++) {
        int count_ones = 0;
        int count_zeros;

        // Count how many numbers have the i-th bit set to 1
        for (j = 0; j < numsSize; j++) {
            if ((nums[j] >> i) & 1) {
                count_ones++;
            }
        }

        count_zeros = numsSize - count_ones;
        total_distance += count_ones * count_zeros;
    }

    return total_distance;
}

int main() {
    int size;
    int i; // Declared outside the loop
    int* nums;
    int result;

    printf("Enter the number of elements in the array: ");
    if (scanf("%d", &size) != 1 || size <= 0) {
        printf("Please enter a valid array size greater than 0.\n");
        return 1;
    }

    // Allocate memory dynamically for the array
    nums = (int*)malloc(size * sizeof(int));
    if (nums == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d integers separated by spaces: ", size);
    for (i = 0; i < size; i++) {
        if (scanf("%d", &nums[i]) != 1) {
            printf("Invalid input.\n");
            free(nums);
            return 1;
        }
    }

    // Call the function
    result = totalHammingDistance(nums, size);

    // Display the result
    printf("The total Hamming distance is: %d\n", result);

    // Clean up allocated memory
    free(nums);
    return 0;
}
