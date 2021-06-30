#ifdef _WIN32

#include "input.h"

#include <conio.h>

KeyCode GetInput()
{
    int key = getch();

    if(key == KC_NULL)
    {
        key = getch();

        // 59-69 -> F1-F10
        if(59 <= key && key <= 69)
        {
            return KC_F1 + (key-59);
        }

        key = KC_NULL;

    } else if(key == 224)
    {
        key = getch();

        // F11-F12 133-134
        if(133 <= key && key <= 134)
        {
            return KC_F11 + (key-133);
        }

        // Arrows
        // 72-Up, 75-Left, 77-Right, 80-Down
        switch(key)
        {
            case 72: { return KC_ARROW_UP; } break;
            case 75: { return KC_ARROW_LEFT; } break;
            case 77: { return KC_ARROW_RIGHT; } break;
            case 80: { return KC_ARROW_DOWN; } break;
            default: { } break;
        }

        key = KC_NULL;
    }

    return key;
}

#endif // WIN32