#include "UI_Window.h"

void UI_Window::set_title(std::string new_title) //sets the window title
{
	title.setString(new_title);
}

void UI_Window::draw_ui_window(sf::RenderWindow &window, sf::View &view) //draws the window and its components, also handles positioning that needs to be done in the main loop
{
	drawable_calculations::center_sprite(window_sprite, view);
	close_button.setPosition(window_sprite.getPosition().x + window_sprite.getGlobalBounds().width - close_button.getGlobalBounds().width - 1, window_sprite.getPosition().y + 1); //sets the exit button at the top right of the window
	window.draw(window_sprite);
	window.draw(close_button);
	title.setPosition(window_sprite.getPosition().x + 2, window_sprite.getPosition().y + 1); //sets the title to the top left of the window
	window.draw(title);
	if (show_enter)
	{
		enter_button.setPos(window_sprite.getPosition().x + window_sprite.getGlobalBounds().width - enter_button.get_size().x - 3, window_sprite.getPosition().y + window_sprite.getGlobalBounds().height - enter_button.get_size().y - 2); //places the entr button in the bottom right of the window																																																	
		enter_button.render_button(window, view);
		enter_icon.setPosition(enter_button.getPos().x, enter_button.getPos().y - 1 + enter_icon.getGlobalBounds().height); //places the eneter icon in the middle of the enter button
		window.draw(enter_icon);
	}
}

void UI_Window::construct_button(sf::RenderWindow& window, sf::View& view) //constructs the enter button and handles sizing it
{
	enter_button.button_full_construct("", sf::Vector2i(0, 0), view, window); //we only want the enter icon in this button, so we use an empty string for the button text
	enter_button.override_size(enter_icon.getGlobalBounds().width); //sets the button's width to the enter icon's width
}

void UI_Window::set_enter(bool toggle)
{
	show_enter = toggle;
}

bool UI_Window::get_enter()
{
	return show_enter;
}

void UI_Window::handle_events(sf::RenderWindow &window, sf::Event &event, int &change_button) //handles parts of the program that need to occur in the event loop
{
	if (drawable_calculations::chk_L_click(window, close_button, event) == mouse_state::click) //checks if the close button is clicked, if so it will change the variable change_button (in main_menu) to 0
	{
		change_button = -1;
	}
}

bool UI_Window::is_button_pressed(sf::RenderWindow& window, sf::View &view, sf::Event &event)
{ 
	if (enter_button.make_functional(window, view, event))
	{
		return true;
	}
	return false;
}

sf::Vector2f UI_Window::getPos()
{
	return window_sprite.getPosition();
}

sf::FloatRect UI_Window::getGlobalBounds()
{
	return window_sprite.getGlobalBounds();
}