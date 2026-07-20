#include <stdio.h>

void solve() {
    char s[1005];
    int sum = 0;
    int i = 0;

    printf("Enter string: ");
    if (scanf("%s", s) != 1) {
        return;
    }

    // Loop through each character until the string ends ('\0')
    while (s[i] != '\0') {
        // Check if the current character is a digit between '0' and '9'
        if (s[i] >= '0' && s[i] <= '9') {
            // Convert character to integer and add it to the sum
            sum += (s[i] - '0');
        }
        i++;
    }

    // Print the final calculated sum
    printf("Sum of digits: %d\n\n", sum);
}

int main() {
    int t;

    printf("Enter number of test cases (T): ");
    if (scanf("%d", &t) == 1) {
        while (t--) {
            solve();
        }
    }
    return 0;
}
