#include "States.h" 

int States::get_current_state() //returns the current program state
{
	return current_state;
}

int States::set_current_state(int new_state, sf::RenderWindow &window, sf::View &view) //changes the current state
{
	current_state = new_state; 
	switch (new_state) //change backgrounds dependant on state
	{
	case state_keys::main_menu:
		set_current_bg(sf::Color(107,107,107)); //this colour is one of the two main colours used in the UI mockup for the main menu
		mm.menu_layout(window, view); //handles positioning each button, window and text element
		break;
	case state_keys::editor:
		ed.construct_buttons(window, view);
	}
	return current_state;
}

sf::Color States::get_current_bg() //returns the sf::Color of the background
{
	return bg;
}

sf::Color States::set_current_bg(sf::Color new_bg) //sets the background's sf::Color
{
	bg = new_bg;
	return bg;
}

void States::update_state_events(sf::RenderWindow& window, sf::View& view, sf::Event &event) //carries out event loop functions associated with a certain state
{
	switch (current_state)
	{
	case state_keys::main_menu:
		mm.handle_events(window, view, event); //handles program operations that happen in SFML's event loop, such as mouse clicking
		if (mm.what_button == mm.button_pressed::go_to_editor) //mm.what_button will equal mm.button_pressed::go_to_editor when the "edit project" button is pressed
		{
			set_current_bg(sf::Color(0, 0, 0)); //changes from grey background to black background
			set_current_state(state_keys::editor, window, view);
		}
		break;
	case state_keys::editor:
		ed.editor_event_loop(window, view, event);
		if (ed.exit_flag) //ed.exit_flag will be true when the "exit" button is pressed in the editor
		{
			set_current_bg(sf::Color(107, 107, 107)); //switches from black background to grey background
			set_current_state(state_keys::main_menu, window, view);
			ed.exit_flag = false; //this flag needs to be reset so that the user can enter the editor again after one exit
			mm.what_button = -1; //puts the menu in it's default state
		}
		break;
	}
}

void States::update_state(sf::RenderWindow& window, sf::View& view) //carries out main loop functions associated with a certain state
{
	switch (current_state)
	{
	case state_keys::main_menu:
		mm.render_menu(window, view); //handles drawing menu elements and positioning that needs to happen in the main loop
		break;
	case state_keys::editor:
		ed.editor_main_loop(window, view);
		break; 
	}
}