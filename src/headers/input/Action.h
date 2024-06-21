#ifndef ACTION_H
#define ACTION_H

enum Action
{
    // Axis Actions
    MOVE,
    MENU_MOVE,
    AXIS_END,   // <- this must stay at the end of axis actions

    // Button Actions
    CAST_SPELL,
    MENU_CLICK,
    BUTTON_END, // <- this must stay at the end of button actions

    // Custom
    PULL_ROPE
};

#endif // !ACTION_H