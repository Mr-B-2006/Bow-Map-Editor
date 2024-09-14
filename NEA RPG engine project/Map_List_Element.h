#pragma once
#pragma once
#ifndef MAP_LIST_ELEMENT_H
#define MAP_LIST_ELEMENT_H
#define NOMINMAX //used so that windows.h doesnt break std::min() and std::max() (which is used in SFML's library code) https://en.sfml-dev.org/forums/index.php?topic=26401.0 
#endif
#include "SFML/Graphics.hpp"
#include "Drawable_calculations.h"
#include "UI_Window.h"

class Map_List_Element
{
public:	
	sf::Sprite map_el_spr; //sprite used for a map list element, rendered underneat text
	sf::Text details; //sf::Text object used to display the map id
	int map_el_index = -1; //index for the std::vector Editor::maps_vec associated with this map list element
	int map_el_id = -1; //map id associated with this map list element
	Map_List_Element(int &index, int &id) //constructor for this class takes in map index (for std::vector) and map id and sets the private members to these values
	{
		map_el_index = index;
		map_el_id = id;
	}
	void render_element(sf::RenderWindow &window, int x, int y); //renders the map list element sprite and its sf::Text object, also handles positioning
	int was_pressed(sf::RenderWindow &window, sf::Event &event); //checks if the map list element sprite was clicked/hovered
	void setPos(int x, int y); //sets the position of the map sprite
	sf::Vector2f getPos(); //returns the position of the map list element sprite
	sf::FloatRect getGlobalBounds(); //returns the boundaries of the map list element sprite
};