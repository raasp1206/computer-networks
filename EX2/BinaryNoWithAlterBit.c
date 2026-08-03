#include <stdbool.h>

bool hasAlternatingBits(int n) {
    // Get the status of the very first bit (rightmost bit)
    int lastBit = n % 2;
    n /= 2; // Shift right to check the next bit

    while (n > 0) {
        int currentBit = n % 2;
        
        // If two adjacent bits match, it fails the condition
        if (currentBit == lastBit) {
            return false;
        }
        
        // Update the marker for the next iteration
        lastBit = currentBit;
        n /= 2;
    }
    
    return true;
}
