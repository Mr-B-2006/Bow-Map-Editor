#pragma once
#ifndef EDITOR_H //Makes sure to define the header if it is not defined already
#define EDITOR_H
#endif
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Drawable_calculations.h"
#include "Button.h"
#include "UI_Window.h"
#include "Text_Field.h"
#include "map.h"
#include "Map_List_Element.h"

class Editor
{
private:
	sf::Texture top_menu_texture; //top_menu refers to the heads-up-display (HUD) at the top of the editor user interface
	sf::Sprite top_menu;
	sf::Texture map_list_el_texture; 
	sf::View gui_view; //this is a seperate view that is rendered on top of the default view in main.cpp, this is used so we move through our maps without movng the HUD and other user interface elements
	sf::Clock frametime; //records the time between each frame, this is used in combination with deltatime to sync movements with framerate, so that higher/lower framerates dont cause faster/slower movement speeds
	float deltatime = 0.0;
	Button editor_buttons[6]; //refers to the buttons on the HUD
	Button map_buttons[5]; //refers to the buttons that are displayd on the window that appears when you select the "maps" button upload, select, delete, add, back
	UI_Window menu_win; //this is the window used for when certain buttons like "maps" are pressed
	Text_Field map_id_field; //text field to enter new ID for a map
	std::vector<int> map_ids; //holds all maC:\Users\mrb20\Desktop\!C++ projects\!!!!!!!!NEA project program\SFML-2.6.1\libp ids for each map stored in maps_vec
	std::vector<Map> maps_vec; //holds each map object
	std::vector<Map_List_Element> map_list_elements; //holds each map list element object, list elements are the boxes that appear when the user is prompted to select from a list of currently loaded maps
	sf::RectangleShape mouse_node_icon = sf::RectangleShape(sf::Vector2f(2,2)); //this icon follows the mouse when the user is about to place either a map trigger or collision node
	sf::RectangleShape first_pos_selected = sf::RectangleShape(sf::Vector2f(2, 2)); //this rectangle shows the initial node position
	int vectors_placed = 0; //counts the stages of vector placement for map triggers and collision nodes (0 = nothing placed, 1 = initial position chosen, 2 = final position chosen (vector can be created from both nodes now))
	int node_type_selected = 0; //identifies what type of node the user wants to place (map trigger or collision)
	Button node_buttons[3]; //this array holds each button that is displayed when the "props" button is pressed in the HUD
	sf::Vector2f node_buffer[2]; //holds each mouse position when creating a collision or map trigger vector
	int current_map_id = 0; 
	int current_map_vec_pos = 0; //indicates the index location in maps_vec for a given map
	int window_page = 0; //responsible for updating information in a window
	int map_select_menu_page = 0; //handles switching "pages" in the map selection window when the amount of maps is greater than 3 (maximum number of list elements that can be displayed on 1 page)
	bool map_being_selected = false; //handles opening the map selectn window after a map trigger has been placed
	sf::Texture add_player_icon_texture;
	sf::Sprite add_player_icon; //this sprite is a GUI element that acts like a button, when it is pressed, a file dialogue opens, where the user can select an image for their player character's sprite
	sf::Texture player_character_texture;
	sf::Sprite player_character; 
	bool setting_player_pos = false; //when this is true (after the player's sprite has been picked from a file dialogue)
	int player_spawn_details[4]{ 0, 0, 0, 0 }; //these hold details about where the player spawns, this includes: their spawn coordinates (x and y), the id of the map the player is meant to spawn at and the vector psition for the map that the player is meant to spawn on
	sf::Texture play_project_texture;
	sf::Sprite play_project; //when this sprite is clicked, the program switches from "edit" mode to "play" mode (via the toggling of the Boolean value "playing"), when this is done collision vectors, map vectors, the user interface (apart from the "play_project" sprite) are no longer rendered, the user also cannot pan through the map, but they can now move the player character with the arrow or WASD keys (this also moves the view, so that "camera" scrolls through the map)
	bool playing = false;
public:
	enum spawn_details_indices //labels each index for player_spawn_details
	{
		spawn_point_x = 0, spawn_point_y = 1, spawn_map_vec_pos = 2, spawn_map_id = 3
	};
	bool exit_flag = false; //handles switching between the editor and the main menu
	enum node_types //correlates an integer with the type of node the user has selected
	{
		nonode = 0, collision = 1, map_trigger = 2
	};
	enum node_arr_types //for handling indexes used regarding nodes
	{
		collision_ = 0, map_trigger_ = 1
	};
	enum editor_button_labels //correlates the indices of edit_buttons and what button is stored in that index
	{ //in this enum, none represents when no button's menu/function is active															    
		none = -1, maps = 0, tiles = 1, props = 2, NPCs = 3, exit = 4, save = 5
	};
	enum map_button_labels //correlates the indices of editor_buttons and what button is stored in that index
	{
		upload = 0, select = 1, del = 2, add = 3, back = 4
	};
	enum map_button_selected //correlates pages in map window with what buttons are selected from the initial menu
	{
		none_selected = 0, upload_selected = 1, select_selected = 2, delete_selected = 3, add_selected = 4
	};
	int what_button = -1; //stores what button the user has pressed and allows for the appropriate menus to be rendered and handled
	Editor()
	{
		gui_view = sf::View(sf::FloatRect(0, 0, 288, 216));
		drawable_calculations::load_n_check_spr(top_menu_texture, top_menu, "assets/images/Creator UI/top menu.png");
		menu_win.set_enter(false); //handles whether the enter button should be drawn or not
		mouse_node_icon.setFillColor(sf::Color(0,0,0,0)); //makes the icon invisible
		drawable_calculations::load_n_check_spr(add_player_icon_texture, add_player_icon, "assets/images/Creator UI/add player character.png");
		drawable_calculations::load_n_check_spr(play_project_texture, play_project, "assets/images/Creator UI/editor play flag.png");
		play_project.setTextureRect(sf::IntRect(0, 0, 28, 34)); //crops the texture used for the "play_project" so that the "stationary" flag is being drawn
	}
	void construct_buttons(sf::RenderWindow &window, sf::View &view); //constructs all buttons, including those in menu_win
	void editor_main_loop(sf::RenderWindow &window, sf::View &view); //handles drawing functions that require the main loop
	void editor_event_loop(sf::RenderWindow& window, sf::View& view, sf::Event &event); //handles functions that are more appropriate to be executed in the event loop
	bool add_new_map(std::string &dir, int map_id); //handles the addition of maps through maps > add new map > new id > enter
	void handle_map_button_events(sf::RenderWindow& window, sf::View& view, sf::Event &event); //handles events that occur when the map menu is opened (when the "maps" button is pressed)
	void handle_map_button_main_loop(sf::RenderWindow& window, sf::View& view); //handles drawing and other instructions that need to be executed in the main loop
	void make_all_buttons_functional(sf::RenderWindow& window, sf::View& view, sf::Event &event); //allows all buttons to be able to change colour when hovered over and return when they are clicked
	bool change_map_image(std::string &dir); //allows the user to change a map's background image
	void pan_thru_map(sf::RenderWindow &window, sf::View &view); //allows the user to move through the map while in the editor
	void handle_props_button_main_loop(sf::RenderWindow &window, sf::View &view); //displays the menu that appears when the "props" button is pressed
	void handle_node_placement(sf::RenderWindow &window, sf::View &view, sf::Event &event); //handles the placement of map trigger and collision nodes
	void switch_map(sf::RenderWindow &window, sf::Event &event); //handles switching between maps in "maps" > "select existing map"
	void select_map_4_trigger(sf::RenderWindow& window, sf::View& view, sf::Event& event, Map &map); //displays the menu with all exiting maps that is also used for switch_map()
	void map_selection_main_loop(sf::RenderWindow &window, sf::View &view); //displays the menu for switching between maps in the editor, this function is used for both switch_map() and select_map_4_trigger()
	void player_move(sf::View& view, sf::RenderWindow &window); //handles player movement and collisions when in "play" mode
};