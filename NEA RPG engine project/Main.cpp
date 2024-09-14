#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <WinUser.h>
#include <iostream>
#include <string.h>
#include "states.h"
#include "Button.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(288 * 3, 216 * 3), "Point Engine");
    sf::View render_view(sf::FloatRect(0, 0, 288, 216)); //allows a 228x216 view to be put on a window much larger

    window.setKeyRepeatEnabled(false);
    States states(window, render_view);
    states.set_current_state(state_keys::main_menu, window, render_view); //starts the program in the main menu state

    while (window.isOpen()) //main loop
    {
        sf::Event event;

        while (window.pollEvent(event)) //event loop, only executes when an event (such as mouse movement or keyboard presses) occurs
        {
            if (window.hasFocus())
            {
                states.update_state_events(window, render_view, event);
            }
            switch (event.type)
            {
            case sf::Event::Resized:
                render_view = sf::View(sf::FloatRect(0, 0, event.size.width / 3, event.size.height / 3)); //extends the window, currently resizing is not handled correctly and causes some graphical/layout errors

                break;
            case sf::Event::Closed:
                window.close();
            }
        }
        window.clear(states.get_current_bg()); //clears the screen with a color given by states.get_current_bg() this is drawn before anything else
        window.setView(render_view); //we will be dealing with multiple views, so now we need to restore this default view at the end of the window loop
        states.update_state(window, render_view); //handles eveything in a program state that needs to be in the main loop
        window.display(); //allows the window to show graphics
    }
    return 0;
}