#include "Editor.h"

void Editor::construct_buttons(sf::RenderWindow &window, sf::View &view)
{					
	std::string button_strs[6] = { "Maps", "Tiles", "Props", "NPCs", "Exit", "Save" }; //each string that each button will display
	for (int i=0; i!=sizeof(editor_buttons)/sizeof(editor_buttons[0]); i++)
	{
		if (i != 0) 
		{
			editor_buttons[i].button_full_construct(button_strs[i], sf::Vector2i(editor_buttons[i - 1].getPos().x + editor_buttons[i - 1].get_size().x - 1, editor_buttons[i - 1].getPos().y), gui_view, window);
		}
		else //only runs once (when i = 0), needed to place the first element of editor_buttons
		{
			editor_buttons[i].button_full_construct(button_strs[i], sf::Vector2i(2, 0), gui_view, window); menu_win.construct_button(window, gui_view);
			map_buttons[0].button_full_construct("Change map image", sf::Vector2i(menu_win.getPos().x + 5, menu_win.getPos().y + 15), gui_view, window); //using for loops for arrays less than 3 items long would use more or the same amount of lines than i for loop, so i opted to not use a for loop for construting the buttons of certain arrays
			map_buttons[1].button_full_construct("Select existing map", sf::Vector2i(menu_win.getPos().x + 5, map_buttons[0].getPos().y + 15), gui_view, window);
			node_buttons[0].button_full_construct("Deselect node", sf::Vector2i(menu_win.getPos().x + 5, menu_win.getPos().y + 15), gui_view, window);
			node_buttons[1].button_full_construct("Place collision node", sf::Vector2i(menu_win.getPos().x + 5, map_buttons[0].getPos().y + 15), gui_view, window);
			node_buttons[2].button_full_construct("Place map trigger", sf::Vector2i(menu_win.getPos().x + 5, map_buttons[0].getPos().y + 15), gui_view, window);
		}
	}
	std::string map_buttons_strs[5] = { "Change map image", "Select existing map", "Delete existing map", "Add new map", "back"};
	for (int i = 0; i != sizeof(map_buttons) / sizeof(map_buttons[0]); i++)
	{
		if (i != 0)
		{
			map_buttons[i].button_full_construct(map_buttons_strs[i], sf::Vector2i(menu_win.getPos().x + 5, map_buttons[i - 1].getPos().y + 15), gui_view, window);
		}
		else //only runs once (when i = 0), needed to place the first element of map_buttons
		{
			map_buttons[i].button_full_construct(map_buttons_strs[i], sf::Vector2i(menu_win.getPos().x + 5, menu_win.getPos().y + 15), gui_view, window);
		}
	}
	menu_win.construct_button(window, view); //constructs the enter button for menu_win
}

void Editor::editor_main_loop(sf::RenderWindow& window, sf::View& view)
{
	//THIS AREA IS USING render_view
	if (!maps_vec.empty()) //this check prevents an assertion error from occuring when trying to draw from an element in maps_vec that has an invalid index
	{
		maps_vec[current_map_vec_pos].render_map(window); //draws the map image
		if (!playing) //map vectors will not be drawn when the user is playing their project, but when editing, vectors will be visible
		{
			maps_vec[current_map_vec_pos].render_vectors(window); //draws all map collision nodes and map trigger
		}
	}
	window.setView(view);
	deltatime = frametime.restart().asSeconds(); //deltatime measures the time between each frame, as recorded by frametime.restart().asSeconds() (restart() restarts the clock's recording while also returning the last time recorded)
	if (!playing)
	{
		pan_thru_map(window, view); //allows the user to move the camera around the map with the arrow and WASD keys
		if (vectors_placed != 0) //if the user has not placed one node, we dont want to render the node that indicates the initial vector position
		{
			window.draw(first_pos_selected);
		}
		if (map_being_selected || vectors_placed == 2) //handle_node_placement() changes the value of map_being_selected and allows for the map selection menu to appear, however, currently sometimes the map selection window may not render, I have not been able to fix this bug due to time constraints
		{
			map_selection_main_loop(window, view);
		}
		if (setting_player_pos) //this makes the player sprite follow the mouse so that the user can click anywhere on the map to set the player's spawn coordinates and spawn map id/vector position
		{
			player_character.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
		}
	}
	else //when the user is not editing their project, they can take control of the player sprite
	{
		player_move(view, window);
	}
	if (current_map_id == player_spawn_details[spawn_details_indices::spawn_map_id]) //this ensures that the player is only drawn on their spawn map, while in the editor
	{
		window.draw(player_character);
	}
	if (!playing) //if the project is not being played, the user cannot place nodes, thus we shouldnt draw the icon for node placement
	{
		window.draw(mouse_node_icon);
	}
	//VIEW IS NOW USING gui_view, this is an sf::View object used specifically for drawing the user interface, it can be thought of as a layer, drawing on this view allows the defautlt view (or "map camera") to be moved while keeping all of the user interface elements stationary
	window.setView(gui_view);	
	sf::Vector2i view_bottom_right(window.mapPixelToCoords(sf::Vector2i(window.getSize().x, window.getSize().y))); //gets the bottom-right of the view, so that GUI elements such as the "play project" and "add player" icon can properly render when the window is resized
	if (!playing)
	{
		add_player_icon.setPosition(view_bottom_right.x - add_player_icon.getTextureRect().width, view_bottom_right.y - add_player_icon.getTextureRect().height); //we only want to draw this sprite while playing as the user isnt able to add the player while playing
		window.draw(add_player_icon);
		switch (what_button) //the appropriate function is selected depending on the menu dtate as given by "what_button"
		{
		case editor_button_labels::maps:
			handle_map_button_main_loop(window, view);
			break;
		case editor_button_labels::tiles:
			//this case is empty but present for future development
			break;
		case editor_button_labels::props:
			handle_props_button_main_loop(window, view);
			break;
		case editor_button_labels::NPCs:
			//this case is empty but present for future development
			break;
		case editor_button_labels::save:
			//this case is empty but present for future development
			break;
		default:
			window_page = 0;
			break;
		}
		window.draw(top_menu);
		for (int i = 0; i != sizeof(editor_buttons) / sizeof(editor_buttons[0]); i++)
		{ //this loop iterates through each button stored in "editor_buttons" and draws each of them
			editor_buttons[i].render_button(window, gui_view);
		}
	}
	play_project.setPosition(0, view_bottom_right.y - play_project.getGlobalBounds().height); //sets the position of the "play project" icon to the bottom left of the vew
	window.draw(play_project);
}

void Editor::make_all_buttons_functional(sf::RenderWindow& window, sf::View& view, sf::Event &event)
{	
	if (what_button == editor_button_labels::none && !playing) //the HUD buttons should only work when the project is being edited or a menu from one of the HUD buttons isnt already opened (which is determined by what_button being a value other than editor_button_labels::none)
	{
		menu_win.set_enter(false); //the enter button is not displayed for these windows
		if (window_page == map_button_selected::none_selected)
		{
			if (editor_buttons[editor_button_labels::maps].make_functional(window, view, event))
			{
				what_button = editor_button_labels::maps;
			}
			else if (editor_buttons[editor_button_labels::tiles].make_functional(window, view, event))
			{
				what_button = editor_button_labels::tiles;
				what_button = editor_button_labels::none; //this button's function has not yet been implemented, so to make it so that the button still changes colour when hovered, this if statement needs to be made, to make it so that pressing this button doesnt conflict with the other menu functions, what_button has to be set to editor_button_labels::none
			}
			else if (editor_buttons[editor_button_labels::props].make_functional(window, view, event))
			{
				menu_win.set_title("Select prop to place");
				what_button = editor_button_labels::props;
			}
			else if (editor_buttons[editor_button_labels::NPCs].make_functional(window, view, event))
			{
				what_button = editor_button_labels::NPCs;
				what_button = editor_button_labels::none;  //this button's function has not yet been implemented, so to make it so that the button still changes colour when hovered, this if statement needs to be made, to make it so that pressing this button doesnt conflict with the other menu functions, what_button has to be set to editor_button_labels::none
			}
			else if (editor_buttons[editor_button_labels::exit].make_functional(window, view, event))
			{
				map_ids.clear(); //clears all relevant project data/set variables to initial values when the editor is exited
				maps_vec.clear();
				map_list_elements.clear();
				current_map_id = 0;
				view = sf::View((sf::FloatRect(0, 0, 288, 216)));
				window.setView(view);
				exit_flag = true;
				node_type_selected = node_types::nonode;
				mouse_node_icon.setFillColor(sf::Color(0, 0, 0, 0));
				vectors_placed = 0;
			}
			else if (editor_buttons[editor_button_labels::save].make_functional(window, view, event))
			{
				what_button = editor_button_labels::save;
				what_button = editor_button_labels::none;  //this button's function has not yet been implemented, so to make it so that the button still changes colour when hovered, this if statement needs to be made, to make it so that pressing this button doesnt conflict with the other menu functions, what_button has to be set to editor_button_labels::none
			}
		}
	}
	if (drawable_calculations::chk_L_click(window, add_player_icon, event) == mouse_state::click)
	{ //checks if the add player icon has been clicked
		drawable_calculations::load_n_check_spr(player_character_texture, player_character, drawable_calculations::file_dialogue()); //opens a file explorer dialogue and loads from the directory that the file dialogue returns
		player_character.setTextureRect(sf::IntRect(0, 0, player_character_texture.getSize().x, player_character_texture.getSize().y)); //sets the player sprite's rectangle to the texture image's dimensions, this prevents the player sprite being rendered incorrectly when the player sprite used is switched with one that has larger dimensions
		setting_player_pos = true;
	}
}

void Editor::handle_props_button_main_loop(sf::RenderWindow& window, sf::View& view)
{
	menu_win.draw_ui_window(window, gui_view);
	for (int i=0; i != sizeof(node_buttons)/sizeof(node_buttons[i]); i++)
	{ //places each button in a sequences where there is equal amount of space between each button 
		if (i != 0)
		{
			node_buttons[i].setPos(menu_win.getPos().x + 5, node_buttons[i-1].getPos().y + 20);
		}
		else //the first button needs to be placed in a specific position
		{
			node_buttons[0].setPos(menu_win.getPos().x + 5, menu_win.getPos().y + 18);
		}
		node_buttons[i].render_button(window, view);
	}
}

void Editor::editor_event_loop(sf::RenderWindow& window, sf::View& view, sf::Event& event)
{
	window.setView(view);
	if (vectors_placed <= 1) //we only want the icon to follow the mouse when 1 or fewer nodes have been placed
	{
		mouse_node_icon.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
	}
	if (setting_player_pos)
	{
		if (event.type == sf::Event::MouseButtonReleased && maps_vec[current_map_vec_pos].map_sprite.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
		{ 
			setting_player_pos = false; //when the mouse is released we want the player sprite to be placed and for it to no longer follow the mouse
			int add_spawn_details[4] = { player_character.getPosition().x, player_character.getPosition().y, current_map_vec_pos, current_map_id }; //all relevant spawning details for the player are recorded, these are used when switching between the editor and the player
			for (int i=0; i!=sizeof(add_spawn_details)/sizeof(add_spawn_details[0]); i++)
			{ //this loop swaps values from the temporary array to the array that is an attribute of the editor class
				player_spawn_details[i] = add_spawn_details[i];
			}
		}
	}
	window.setView(gui_view); //the proceeding events are related to the user onterface, thus, we need to switch to the GUI view
	menu_win.handle_events(window, event, what_button);
	if (play_project.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) //drawable_calculations::chk_L_click() cant be used here as we dont want this icon to darken, it may have been more appropriate to implement this as a function in drawable_calculations
	{
		play_project.setTextureRect(sf::IntRect(0, 35, 38, 33)); //while the mouse is hovering over the "play project" icon sprite, the image needs to change from a "stationary" flag to a "waving" flag, this is achieved by changing the area of the texture image that the sprite reads from using sf::Sprite::setTextureRect()
		if (event.type == sf::Event::MouseButtonReleased) //if the mouse is hovering over the sprite and the mouse has been clicked, we know that this icon/sprite has been clicked
		{
			if (playing) //if the icon/sprite has been clicked and the project is being played, we need to return to the editor
			{
				player_character.setPosition(player_spawn_details[spawn_details_indices::spawn_point_x], player_spawn_details[spawn_details_indices::spawn_point_y]); //puts the player at their spawn coordinates
				playing = false;
			}
			else //if the icon/sprite has been clicked and the project is being edited, we need to set up the project to be played
			{
				view.setCenter(player_character.getPosition().x, player_character.getPosition().y); //takes the view (or "camera") to the player's spawn position
				current_map_vec_pos = player_spawn_details[spawn_details_indices::spawn_map_vec_pos];
				current_map_id = player_spawn_details[spawn_details_indices::spawn_map_id]; //sets the current map to the player's spawn map
				playing = true;
			}
		}
	}
	else
	{
		play_project.setTextureRect(sf::IntRect(0, 0, 28, 34)); //resets the TextureRect (the area on the texture image that the sprite reads from) so that the sprite shows the default "stationary" flag
	}
	switch (event.type)
	{
	case sf::Event::Resized:
		gui_view = sf::View(sf::FloatRect(0, 0, event.size.width / 3, event.size.height / 3)); //corrects the view when the window is resized
		break;
	case sf::Event::MouseWheelScrolled:
		if (!playing) //scrolls the view up/down when the mouse wheel is scrolled (this also works with gestures on a trackpad, but not on a touchscreen)
		{
			if (event.mouseWheelScroll.delta == 1) //scroll wheel up
			{
				view.zoom(0.66);
			}
			else if (event.mouseWheelScroll.delta == -1) //scroll wheel down
			{
				view.zoom(1.5);
			}
		}
		break;
	}
	make_all_buttons_functional(window, view, event);
	switch (what_button)
	{
	case editor_button_labels::none:
		map_id_field.clear(); //ensures that the field is cleared in preperation for the next time it is needed
		if (node_type_selected != node_types::nonode) //if no type of node is selected, we do not want this function to run
		{
			handle_node_placement(window, view, event);
		}
		break;
	case editor_button_labels::maps:
		handle_map_button_events(window, view, event);
		break;
	case editor_button_labels::tiles:
		//this case is empty but present for future development
		break;							      	// 	none:  |			collision: |			map triggers: |
	case editor_button_labels::props:	    	//		   |				       |						  |
	{ //scope has been created to allow					   |				       |						  |
	  //initilisation of								   |					   |						  |
	  //mouse_node_icon_colours[3]						   V					   V						  V
		sf::Color mouse_node_icon_colours[3] = { sf::Color(0, 0, 0, 0), sf::Color(205, 216, 0, 255), sf::Color(78, 0, 74, 255) };
		for (int i = 0; i != sizeof(node_buttons) / sizeof(node_buttons[0]); i++)
		{
			if (node_buttons[i].make_functional(window, view, event))
			{
				node_type_selected = i; //the indices of node_buttons and the values used to represent each node type (as seen in the node_type enum) are related, so we can use the index of the selected button as the value for node_type_selected
				mouse_node_icon.setFillColor(mouse_node_icon_colours[i]); //similarly, the indices of node_buttons and the indices of mouse_node_icon_colours are also related
				node_buffer[0] = sf::Vector2f(0, 0); //resets the position of the node buffers
				node_buffer[1] = sf::Vector2f(0, 0);
				what_button = editor_button_labels::none; //sets what_button to the default state, so the user has full view of the map for when they place their node
			}
		}
	}
		break;
	case editor_button_labels::NPCs:
		//this case is empty but present for future development
		break;
	case editor_button_labels::save:
		//this case is empty but present for future development
		break;
	}
}

void Editor::handle_node_placement(sf::RenderWindow &window, sf::View &view, sf::Event &event)
{ //this function's code is quite unclean and the map triggers do not properly work, this is because I was running low on time
	window.setView(view);
	if (event.type == sf::Event::MouseButtonReleased)
	{
		sf::Vector2f mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window)); //saving mouse position as it is referred to many times
		if (vectors_placed == 0) //if vectors_placed = 0, we only need to place the initial node (while the next node is being set)
		{
			node_buffer[0] = mouse_pos;
			first_pos_selected.setFillColor(mouse_node_icon.getFillColor());
			first_pos_selected.setPosition(mouse_pos);
			vectors_placed = 1;
		}
		
		else if (fabsf(node_buffer[0].y - mouse_pos.y) > fabsf(node_buffer[0].x - mouse_pos.x) && !map_being_selected) //compare x and y distances
		{ //y-axis
			vectors_placed = 2;
			if (node_buffer[0].y > mouse_pos.y) //checks which node is on the top
			{
				mouse_node_icon.setPosition(node_buffer[0]);
				if (node_type_selected == node_types::collision)
				{
					mouse_node_icon.setSize(sf::Vector2f(2, (node_buffer[0].y - mouse_pos.y) + 1)); //makes the sf::RectangleShape that is the size of the distance between both nodes
					maps_vec[current_map_vec_pos].collision_vectors.push_back(mouse_node_icon); //copies the final collision vector (sf::RectangleShape) into the map's std::vector for holding the sf::RectangleShape objects for vectors
					maps_vec[current_map_vec_pos].collision_vectors.back().setPosition(mouse_pos); //puts the position of the mouse (which is also the position of the vector) into the map's std::vector for holding the position for each vector
					mouse_node_icon = sf::RectangleShape(sf::Vector2f(2, 2)); //resets the size of the mouse icon
					mouse_node_icon.setFillColor(sf::Color(205, 216, 0, 255)); //sets the mouse node icon to it's appropriate colour
					vectors_placed = 0;
				}
				else if (node_type_selected == node_types::map_trigger)
				{
					map_being_selected = true;
					select_map_4_trigger(window, view, event, maps_vec[current_map_vec_pos]);
					mouse_node_icon.setSize(sf::Vector2f(2, (node_buffer[0].y - mouse_pos.y) + 1));
					maps_vec[current_map_vec_pos].map_triggers.push_back(mouse_node_icon);
					maps_vec[current_map_vec_pos].map_triggers.back().setPosition(mouse_pos);
					mouse_node_icon = sf::RectangleShape(sf::Vector2f(2, 2));
					mouse_node_icon.setFillColor(sf::Color(78, 0, 74, 255));
				}
				mouse_node_icon.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				node_buffer[0] = sf::Vector2f(mouse_pos.x + 99, 99);
				node_buffer[1] = sf::Vector2f(mouse_pos.x + 99, 99);
				first_pos_selected.setPosition(mouse_pos.x + 99, 99);
			}
			else
			{
				mouse_node_icon.setPosition(mouse_pos.x+1, mouse_pos.y);
				if (node_type_selected == node_types::collision)
				{
					mouse_node_icon.setSize(sf::Vector2f(2, (mouse_pos.y - node_buffer[0].y) + 1));
					maps_vec[current_map_vec_pos].collision_vectors.push_back(mouse_node_icon);
					maps_vec[current_map_vec_pos].collision_vectors.back().setPosition(node_buffer[0]);
					mouse_node_icon = sf::RectangleShape(sf::Vector2f(2, 2));
					mouse_node_icon.setFillColor(sf::Color(205, 216, 0, 255));
					vectors_placed = 0;
				}
				else if (node_type_selected == node_types::map_trigger)
				{
					map_being_selected = true;
					select_map_4_trigger(window, view, event, maps_vec[current_map_vec_pos]);
					mouse_node_icon.setSize(sf::Vector2f(2, (mouse_pos.y - node_buffer[0].y) + 1));
					maps_vec[current_map_vec_pos].map_triggers.push_back(mouse_node_icon);
					maps_vec[current_map_vec_pos].map_triggers.back().setPosition(node_buffer[0]);
					mouse_node_icon = sf::RectangleShape(sf::Vector2f(2, 2));
					mouse_node_icon.setFillColor(sf::Color(78, 0, 74, 255));
				}
				mouse_node_icon.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				node_buffer[0] = sf::Vector2f(mouse_pos.x + 99999, 0);
				node_buffer[1] = sf::Vector2f(mouse_pos.x + 99999, 0);
			}
		}
		else
		{
			vectors_placed = 2;
			if (node_buffer[0].x >= mouse_pos.x)
			{
				mouse_node_icon.setPosition(node_buffer[0]);
				if (node_type_selected == node_types::collision)
				{
					mouse_node_icon.setSize(sf::Vector2f((mouse_pos.x - node_buffer[0].x), 2));
					maps_vec[current_map_vec_pos].collision_vectors.push_back(mouse_node_icon);
					maps_vec[current_map_vec_pos].collision_vectors.back().setPosition(node_buffer[0]);
					mouse_node_icon = sf::RectangleShape(sf::Vector2f(2, 2));
					mouse_node_icon.setFillColor(sf::Color(205, 216, 0, 255));
					vectors_placed = 0;
				}
				else if (node_type_selected == node_types::map_trigger)
				{

					map_being_selected = true;
					select_map_4_trigger(window, view, event, maps_vec[current_map_vec_pos]);
					mouse_node_icon.setSize(sf::Vector2f((mouse_pos.x - node_buffer[0].x), 2));
					maps_vec[current_map_vec_pos].map_triggers.push_back(mouse_node_icon);
					maps_vec[current_map_vec_pos].map_triggers.back().setPosition(node_buffer[0]);
					mouse_node_icon = sf::RectangleShape(sf::Vector2f(2, 2));
					mouse_node_icon.setFillColor(sf::Color(78, 0, 74, 255));
				}
				mouse_node_icon.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				node_buffer[0] = sf::Vector2f(mouse_pos.x + 99999, 0);
				node_buffer[1] = sf::Vector2f(mouse_pos.x + 99999, 0);
				first_pos_selected.setPosition(mouse_pos.x + 99999, 0);
			}
			else
			{
				mouse_node_icon.setPosition(node_buffer[0]);
				if (node_type_selected == node_types::collision)
				{
					mouse_node_icon.setSize(sf::Vector2f((node_buffer[0].x - mouse_pos.x) + 1, 2));
					maps_vec[current_map_vec_pos].collision_vectors.push_back(mouse_node_icon);
					maps_vec[current_map_vec_pos].collision_vectors.back().setPosition(mouse_pos);
					mouse_node_icon = sf::RectangleShape(sf::Vector2f(2, 2));
					mouse_node_icon.setFillColor(sf::Color(205, 216, 0, 255));
					vectors_placed = 0;
				}
				else if (node_type_selected == node_types::map_trigger)
				{
					map_being_selected = true;
					select_map_4_trigger(window, view, event, maps_vec[current_map_vec_pos]);
					mouse_node_icon.setSize(sf::Vector2f((node_buffer[0].x - mouse_pos.x) + 1, 2));
					maps_vec[current_map_vec_pos].map_triggers.push_back(mouse_node_icon);
					maps_vec[current_map_vec_pos].map_triggers.back().setPosition(mouse_pos);
					mouse_node_icon = sf::RectangleShape(sf::Vector2f(2, 2));
					mouse_node_icon.setFillColor(sf::Color(78, 0, 74, 255));
				}
				mouse_node_icon.setPosition(window.mapPixelToCoords(sf::Mouse::getPosition(window)));
				node_buffer[0] = sf::Vector2f(mouse_pos.x + 99999, 0);
				node_buffer[1] = sf::Vector2f(mouse_pos.x + 99999, 0);
				first_pos_selected.setPosition(mouse_pos.x + 99999, 0);
			}
		}
	}
	if (!maps_vec[current_map_vec_pos].map_triggers.empty() && fabsf(maps_vec[current_map_vec_pos].map_triggers.back().getSize().x) > 9999 && fabsf(maps_vec[current_map_vec_pos].map_triggers.back().getSize().y) == 2)
	{ //prevents a bug where abnormally large map vectors appear after one has already been created, this temporary fix was only implemented due to time constraints
		maps_vec[current_map_vec_pos].map_triggers.pop_back();
	}
	window.setView(gui_view); //window view needs to be set back to gui_view to prevent rendering issues
}

void Editor::switch_map(sf::RenderWindow &window, sf::Event &event)
{
	if (event.type == sf::Event::KeyReleased)
	{ //changes the "page" of the map selection window when there are more than three map list elements (as only three map list elements can fit on a window) 
		if (event.key.code == sf::Keyboard::Right && (map_select_menu_page * 3 <= (map_list_elements.size() - 4)))
		{
			map_select_menu_page++;
		}
		else if (event.key.code == sf::Keyboard::Left && map_select_menu_page != 0)
		{
			map_select_menu_page--;
		}
	}
	for (int i = 0; i != map_list_elements.size(); i++)
	{
		if (map_list_elements[i].was_pressed(window, event) == mouse_state::click)
		{ //checks if a map list element was clicked, if it was the map switches based on the selected list element
			current_map_id = map_list_elements[i].map_el_id;
			current_map_vec_pos = map_list_elements[i].map_el_index;
			what_button = editor_button_labels::none; //returns to the default state
		}
	}
}

void Editor::select_map_4_trigger(sf::RenderWindow& window, sf::View &view, sf::Event& event, Map &map)
{
	window.setView(gui_view); //opening this window requires the window's view to be set to gui_view, this prevents the window moving when arrow or WASD keys are pressed
	vectors_placed = 2;
	menu_win.set_title("Select a map");
	if (event.type == sf::Event::KeyReleased)
	{//changes the "page" of the map selection window when there are more than three map list elements (as only three map list elements can fit on a window) 
		if (event.key.code == sf::Keyboard::Right && (map_select_menu_page * 3 <= (map_list_elements.size() - 4)))
		{
			map_select_menu_page++;
		}
		else if (event.key.code == sf::Keyboard::Left && map_select_menu_page != 0)
		{
			map_select_menu_page--;
		}
	}
	for (int i = 0; i != map_list_elements.size(); i++)
	{
		if (map_list_elements[i].was_pressed(window, event) == mouse_state::click)
		{ //checks if a map list element was clicked, if it was the map's vectors that store the trigger's corrsponding map id and vector position get appended to
			map.map_trigger_ids.push_back(map_list_elements[i].map_el_id);
			map.map_trigger_vec_pos.push_back(map_list_elements[i].map_el_index);
			map_being_selected = false; //these values need to be reset
			vectors_placed = 0;
		}
	}
	window.setView(view);
}

void Editor::map_selection_main_loop(sf::RenderWindow &window, sf::View &view)
{
	window.setView(gui_view); //need to switch the window's view to the GUI view, as we are handling GUI elements
	menu_win.draw_ui_window(window, gui_view);

	if (!map_list_elements.empty()) //need to check if this std::vector is empty in order 
	{
		int j = 0;
		for (int i = 0 + (map_select_menu_page * 3); i < map_list_elements.size(); i++)
		{
			if (j == 3) //makes sure that only three map list elements are displayed
			{
				break;
			}
			if (i % 3 != 0) //renders each list element and places them in a sequence so that there is equal space between each element sprite
			{
				map_list_elements[i].render_element(window, map_list_elements[i - 1].getPos().x,
					map_list_elements[i - 1].getPos().y + map_list_elements[i - 1].getGlobalBounds().height + 1);
			}
			else
			{
				map_list_elements[i].render_element(window, menu_win.getPos().x + 5, menu_win.getPos().y + 15);
			}
			j++;
		}
	}
}

void Editor::handle_map_button_events(sf::RenderWindow& window, sf::View& view, sf::Event &event)
{
	menu_win.set_title("Maps"); 
	switch (window_page)
	{
	case map_button_selected::none_selected:
		if (map_buttons[map_button_labels::upload].make_functional(window, view, event))
		{
			window_page = map_button_selected::upload_selected;
			std::string fd = drawable_calculations::file_dialogue(); //change_map_image() uses a pointer for its parameter (passed by reference) so we need to store drawable_calculations::file_dialogue() to use it in change_map_image()
			change_map_image(fd);
			what_button = editor_button_labels::none;
		}
		else if (map_buttons[map_button_labels::select].make_functional(window, view, event))
		{
			map_select_menu_page = 0; //make sure that the map selection screen starts at page 1 (technically 0)
			window_page = map_button_selected::select_selected;
		}
		else if (map_buttons[map_button_labels::del].make_functional(window, view, event))
		{
			//this if statement body is empty but present for future development
			what_button = editor_button_labels::none;
		}
		else if (map_buttons[map_button_labels::add].make_functional(window, view, event))
		{
			menu_win.set_enter(true);
			map_id_field.set_field_text("New ID:");
			window_page = map_button_selected::add_selected;
		}
		break;
	case map_button_selected::add_selected:
		map_id_field.allow_text_entering(window, event);
		map_id_field.set_field_position(menu_win.getPos().x + 5, menu_win.getPos().y + 15);
		if (menu_win.is_button_pressed(window, view, event)) //once the windows enter button has been clicked, the id entered in the text field needs to be retrieved and stored
		{ 
			if (drawable_calculations::chk_4_digit(map_id_field.getText()) //drawable_calculations::chk_4_digit() iterates through map_id_field.getText() and checks if any non-numeric characters exists in a given string, this makes sure that the ID entered meets validation rules (must be an integer) 
				&& map_id_field.getText() != "")
			{
				int from_field = std::stoi(map_id_field.getText()); //converts map_id_field.getText() to an integer
				bool is_in_vec_flag = false; //this will be true if an identical map ID exists in map_ids
				if (!maps_vec.empty())
				{
					for (int i = 0; i != map_ids.size(); i++)
					{
						if (map_ids[i] == from_field)
						{
							is_in_vec_flag = true;
						}
					}
				}
				if (is_in_vec_flag)
				{
					MessageBoxA(NULL, "A map with this ID already exits, please try again.", "Map Creation Failure", MB_ICONERROR);
				}
				else 
				{
					std::string fd = drawable_calculations::file_dialogue(); //if we know all validation rules are met, we can start to load our map background image texture from the file explorer dialogue
					add_new_map(fd, from_field);
					what_button = editor_button_labels::none;
				}
			}
			else
			{
				MessageBoxA(NULL, "Map IDs can only contain numbers", "Map Creation Failure", MB_ICONERROR);
			}	
		} 
		break; 
	case map_button_selected::select_selected:
		switch_map(window, event);
		break;
	}
}

void Editor::handle_map_button_main_loop(sf::RenderWindow &window, sf::View &view)
{
	menu_win.draw_ui_window(window, gui_view);
	if (what_button == editor_button_labels::maps)
	{
		for (int i = 0; i != (sizeof(map_buttons) / sizeof(map_buttons[0])) - 1; i++)
		{ //iterates and displays each map button, also places them in a way that ensures equal spacing between each button
			if (i != 0)
			{
				map_buttons[i].setPos(menu_win.getPos().x + 5, map_buttons[i - 1].getPos().y + 20);
			}
			else //places first map button in a specific position
			{
				map_buttons[i].setPos(menu_win.getPos().x + 5, menu_win.getPos().y + 18);
			}
		}
		map_buttons[map_button_labels::back].setPos(menu_win.getPos().x + menu_win.getGlobalBounds().width - map_buttons[map_button_labels::back].get_size().x - 2, 
			menu_win.getPos().y + menu_win.getGlobalBounds().height - map_buttons[map_button_labels::back].get_size().y - 2);
		switch (window_page) //renders different user interface elements based on the state of what_button
		{
		case map_button_selected::upload_selected:
			map_buttons[map_button_labels::upload].render_button(window, gui_view);
			map_buttons[map_button_labels::back].render_button(window, gui_view);
			break;
		case map_button_selected::add_selected:
			map_id_field.render_field(window);
			break;
		case map_button_selected::select_selected: 
			map_selection_main_loop(window, view);
			break;
		default:
			for (int i=0; i!=(sizeof(map_buttons) / sizeof(map_buttons[0]))-1; i++)
			{ //displays all elements in map_buttons
				map_buttons[i].render_button(window, gui_view);
			}
			break;
		}
	}
}

bool Editor::add_new_map(std::string& dir, int map_id)
{
	if (dir == "") //if the returned directory is "", we know that the user either closed the file explorer dialogue, or it somehow failed
	{
		return false;
	}
	else if (map_ids.empty()) 
	{
		map_ids.push_back(map_id); 
		Map new_map(map_ids[0]);
		maps_vec.push_back(new_map);
		current_map_id = map_ids[0];
		maps_vec[0].setID(map_id);
		maps_vec[0].set_vec_pos(0);
		current_map_vec_pos = 0;

		Map_List_Element new_element(current_map_vec_pos, current_map_id);
		map_list_elements.push_back(new_element);
	}
	else
	{
		map_ids.push_back(map_id); //the map id the user entered is pushed back into the map_ids std::vector
		Map new_map(map_ids[map_ids.size()-1]); //creates a new map, with the map id that the user entered
		maps_vec.push_back(new_map); //the new map we created on the line above is pushed back into the new_map std::vector
		current_map_id = map_ids[map_ids.size()-1]; //after creating the new map, we want to switch to it, the switching process is handled in the four lines below
		maps_vec[maps_vec.size()-1].setID(map_id);
		maps_vec[maps_vec.size()-1].set_vec_pos(maps_vec.size()-1);
		current_map_vec_pos = maps_vec[maps_vec.size() - 1].get_vec_pos();

		Map_List_Element new_element(current_map_vec_pos, current_map_id); //creates a new map list element that will be used later in the map switching menu and the menu used when two map trigger nodes have been placed
		map_list_elements.push_back(new_element); //this new map list element is pushed back into the map_list_elements std::vector
	} //all of the "new" objects created get deleted at the end of this scope
	maps_vec[maps_vec.size() - 1].map_sprite.setTextureRect(sf::IntRect(0, 0, maps_vec[maps_vec.size() - 1].map_texture.getSize().x, maps_vec[maps_vec.size() - 1].map_texture.getSize().y)); //prevents issues rendering new map background images that had larger dimensions than the last rendered one
	drawable_calculations::load_n_check_spr(maps_vec[maps_vec.size()-1].map_texture, maps_vec[maps_vec.size() - 1].map_sprite, dir); //loads the file selected from the file explorer dialogue, loads it into the sf::Texture maps_vec[maps_vec.size()-1].map_texture and then sets that texture to be used for maps_vec[maps_vec.size() - 1].map_sprite

	for (int i = 0; i != maps_vec.size(); i++) //after each insertion, std::vector::push_back() moves texture pointers, but the pointer the sf::Sprite uses is not updated this leads to the "white square problem" 
	{ //(https://www.sfml-dev.org/tutorials/2.6/graphics-sprite.php#the-white-square-problem), to fix this, i made it so textures reload after every insertion, which updates the neccessary pointers
		maps_vec[i].map_sprite.setTexture(maps_vec[i].map_texture); //sets all textures again, this is done to update the texture pointers as addition of new elements into an std::vector causes currently existing elements to move elsewhere in memory (in order to maintain the contiguos nature of the std::vector)
	}

	drawable_calculations::load_n_check_spr(map_list_el_texture, map_list_elements[map_list_elements.size() - 1].map_el_spr, "assets/images/Creator UI/map list element.png"); //sets the font used for the sf::Text used in this map list element

	map_list_elements[map_list_elements.size() - 1].details.setFont(menu_win.win_font); //sets the properties for the "details" sf::Text object
	map_list_elements[map_list_elements.size() - 1].details.setCharacterSize(90); 
	map_list_elements[map_list_elements.size() - 1].details.setScale(0.1, 0.1);
	map_list_elements[map_list_elements.size() - 1].details.setFillColor(sf::Color(43, 43, 43));
}

bool Editor::change_map_image(std::string& dir)
{
	if (dir == "") //if the returned directory is "", we know that the user either closed the file explorer dialogue, or it somehow failed
	{
		return false;
	}
	else if (map_ids.empty()) //if there are no maps, "changing the background image" will create a new map with an ID of 0
	{
		map_ids.push_back(0); //the map id the user entered is pushed back into the map_ids std::vector
		Map new_map(map_ids[0]); //creates a new map, with the map id 0
		maps_vec.push_back(new_map); //the new map we created on the line above is pushed back into the new_map std::vector
		current_map_id = map_ids[0]; //after creating the new map, we want to "switch" to it, the switching process is handled in the four lines below
		maps_vec[0].setID(0); 
		maps_vec[0].set_vec_pos(0);

		Map_List_Element new_element(current_map_vec_pos, current_map_id); //creates a new map list element that will be used later in the map switching menu and the menu used when two map trigger nodes have been placed
		map_list_elements.push_back(new_element); //this new map list element is pushed back into the map_list_elements std::vector
	}
	drawable_calculations::load_n_check_spr(maps_vec[maps_vec.size() - 1].map_texture, maps_vec[maps_vec.size() - 1].map_sprite, dir); //loads the file selected from the file explorer dialogue, loads it into the sf::Texture maps_vec[maps_vec.size()-1].map_texture and then sets that texture to be used for maps_vec[maps_vec.size() - 1].map_sprite
	maps_vec[current_map_vec_pos].map_sprite.setTextureRect(sf::IntRect(0, 0, maps_vec[current_map_vec_pos].map_texture.getSize().x, maps_vec[current_map_vec_pos].map_texture.getSize().y)); //prevents issues rendering new map background images that had larger dimensions than the last rendered one

	drawable_calculations::load_n_check_spr(map_list_el_texture, map_list_elements[map_list_elements.size() - 1].map_el_spr, "assets/images/Creator UI/map list element.png"); //sets the font used for the sf::Text used in this map list element
	map_list_elements[map_list_elements.size() - 1].details.setFont(menu_win.win_font); //sets the properties for the "details" sf::Text object
	map_list_elements[map_list_elements.size() - 1].details.setCharacterSize(90);
	map_list_elements[map_list_elements.size() - 1].details.setScale(0.1, 0.1);
	map_list_elements[map_list_elements.size() - 1].details.setFillColor(sf::Color(43, 43, 43));
	return true;
}

void Editor::pan_thru_map(sf::RenderWindow &window, sf::View &view)
{
	if (what_button == -1 && !(node_type_selected == node_types::map_trigger && vectors_placed == 2) && window.hasFocus()) //can only move through the map when what_button is in its default state and the map trigger map selection menu isnt open and when the window has focus (the window for this program is the last that has been clicked on, this prevents movement on this program occuring when the user uses their keyboard in another program)
	{
		window.setKeyRepeatEnabled(true);
		if (what_button != map_button_selected::none_selected && !(((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
			|| (((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)))))) //ensures that movement wont occur when two keys "in the opposite direction" are both pressed at the same time
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				view.move(100 * deltatime, 0); //movement is multiplied by deltatime (time between each frame) in order synchronise movements across framerates
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				view.move(-100 * deltatime, 0);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				view.move(0, -100 * deltatime);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				view.move(0, 100 * deltatime);
			}
		}
		window.setKeyRepeatEnabled(false);
	}
}

void Editor::player_move(sf::View& view, sf::RenderWindow &window)
{
	if (playing && window.hasFocus())
	{
		if (!(((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)))
			|| (((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))))))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				for (int i = 0; i != maps_vec[current_map_vec_pos].collision_vectors.size(); i++)
				{
					if (player_character.getGlobalBounds().intersects(maps_vec[current_map_vec_pos].collision_vectors[i].getGlobalBounds()))
					{
						player_character.move(-0.2, 0); //prevents player moving in a specific direction when they hit a collision vector
					}
				}
				player_character.move(100 * deltatime, 0);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				player_character.move(-100 * deltatime, 0);
				for (int i = 0; i != maps_vec[current_map_vec_pos].collision_vectors.size(); i++)
				{
					if (player_character.getGlobalBounds().intersects(maps_vec[current_map_vec_pos].collision_vectors[i].getGlobalBounds()))
					{
						player_character.move(0.2, 0);
					}
				}
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			{
				player_character.move(0, -100 * deltatime);
				for (int i = 0; i != maps_vec[current_map_vec_pos].collision_vectors.size(); i++)
				{
					if (player_character.getGlobalBounds().intersects(maps_vec[current_map_vec_pos].collision_vectors[i].getGlobalBounds()))
					{
						player_character.move(0, 0.2);
					}
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				player_character.move(0, 100 * deltatime);
				for (int i = 0; i != maps_vec[current_map_vec_pos].collision_vectors.size(); i++)
				{
					if (player_character.getGlobalBounds().intersects(maps_vec[current_map_vec_pos].collision_vectors[i].getGlobalBounds()))
					{
						player_character.move(0, -0.2);
					}
				}
			}
			view.setCenter(player_character.getPosition().x, player_character.getPosition().y); //moves the view which makes the "camera scroll" as the player moves, this has to be done after initial movement and collision logic in order to prevent the view/camera from shaking as the player moves back and forth due to the collision vector pushing them back

			for (int i = 0; i != maps_vec[current_map_vec_pos].map_triggers.size(); i++) //this is meant to change the map once the player collided with a map trigger, however, it currently doesnt work and i cannot fix it due to time constraints. Currently colliding with a map trigger will throw an assertion error at the line "current_map_vec_pos = maps_vec[current_map_vec_pos].map_trigger_vec_pos[i];" 
			{
				if (player_character.getGlobalBounds().intersects(maps_vec[current_map_vec_pos].map_triggers[i].getGlobalBounds()))
				{
					current_map_id = maps_vec[current_map_vec_pos].map_trigger_ids[i];
					current_map_vec_pos = maps_vec[current_map_vec_pos].map_trigger_vec_pos[i]; 
				}
			}
		}
	}
}