#include <string.h>
#include <stdlib.h>

char* addBinary(char* a, char* b) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    
    // Max length is the larger string size + 1 for an extra carry bit
    int maxLen = (lenA > lenB ? lenA : lenB) + 1;
    
    // Allocate memory (+1 for the null terminator '\0')
    char* result = (char*)malloc((maxLen + 1) * sizeof(char));
    result[maxLen] = '\0';
    
    int i = lenA - 1;
    int j = lenB - 1;
    int k = maxLen - 1;
    int carry = 0;
    
    // Process both strings from right to left
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        
        if (i >= 0) {
            sum += a[i] - '0';
            i--;
        }
        if (j >= 0) {
            sum += b[j] - '0';
            j--;
        }
        
        result[k] = (sum % 2) + '0'; 
        carry = sum / 2;             
        k--;
    }
    
    // Skip the unused first slot if there was no final overflow carry
    return &result[k + 1];
}
