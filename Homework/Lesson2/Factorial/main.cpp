#include <stdio.h>
#include <cstdint>

// Calculate up to n = 20
// Return true for valid input within the range
// of [0, 20]
bool calculate_factorial(uint64_t n, uint64_t& result)
{
    if (n > 20)
    {
        return false;
    }

    if (n == 0 || n == 1)
    {
        result = 1;
        return true;
    }

    result = 2;
    for (uint64_t i = 3; i <= n; i++)
    {
        result *= i;
    }

    return true;
}

int main()
{
    int input;
    while (true)
    {
        printf_s("Enter a positive integer within the range of [1, 20]: ");

        if (scanf_s("%d", &input) == 1 && input > 0 && input <= 20)
        {
            break;
        }

        // Clear stdin before trying to read again
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {};

        printf_s("Invalid input!\n");
    }

    uint64_t result;
    if (calculate_factorial(input, result))
    {
        printf("%d! = %llu\n", input, result);
    }

    return 0;
}