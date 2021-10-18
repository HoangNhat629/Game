#include "state_context.h"

#include "door_action.h"
#include "closed_state.h"

#include <stdio.h>
#include <array>

constexpr std::array<door_action, 4> actions =
{
    door_action::Close,
    door_action::Open,
    door_action::Lock,
    door_action::Unlock
};

void show_action_menu()
{
    printf_s("---------------------------\n");
    printf_s("1. Close\n");
    printf_s("2. Open\n");
    printf_s("3. Lock\n");
    printf_s("4. Unlock\n");
    printf("\n");
    printf_s("0. Exit");
    printf("\n");
    printf_s("Enter an action: ");
}

int get_input()
{
    while (true)
    {
        show_action_menu();

        int input;
        if (scanf_s("%d", &input) == 1 && input >= 0 && input <= 4)
        {
            return input;
        }

        // Clear stdin before trying to read again
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {};

        printf_s("Invalid input!\n");
    }
}

int main()
{
    state_context context(std::make_unique<closed_state>());

    while (true)
    {
        int choice = get_input();
        if (choice == 0)
        {
            break;
        }
        else
        {
            context.apply_action(actions[choice - 1]);
            printf_s("\n");
        }
    }


    return 0;
}