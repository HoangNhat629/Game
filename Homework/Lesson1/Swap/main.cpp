#include <stdio.h>

void swap_by_ref(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

void swap_by_pointer(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int a;
    int b;

    printf_s("Enter 2 numbers (a, b): ");
    scanf_s("%d%d", &a, &b);

    printf_s("Before swapping: %d, %d\n", a, b);

    int a1 = a;
    int b1 = b;
    swap_by_ref(a1, b1);
    printf_s("After reference swap: %d, %d\n", a1, b1);

    int a2 = a;
    int b2 = b;
    swap_by_pointer(&a2, &b2);
    printf_s("After pointer swap: %d, %d\n", a2, b2);

    return 0;
}