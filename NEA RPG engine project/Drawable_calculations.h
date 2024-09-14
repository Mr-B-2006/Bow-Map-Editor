#pragma once
#ifndef DRAWABLE_CALCULATIONS
#define DRAWABLE_CALCULATIONS
#define NOMINMAX //used so that windows.h doesnt break std::min() and std::max() (which is used in SFML's library code) https://en.sfml-dev.org/forums/index.php?topic=26401.0 
#endif
#include <iostream>
#include <Windows.h>
#include <WinUser.h>
#include <string>
#include <commdlg.h>
#include <cstring>
#include "SFML/Graphics.hpp"

enum mouse_state //allows us to label a number with a corresponding 'state' of the mouse
{
	untouched = 0, hovering = 1, click = 2
};

namespace drawable_calculations //this namespace is intended to be used to easily use common functions that will be required by the program, such as centering sprites or opening file dialogues
{
	const int darkening_factor = 191; //darkening_factor is the amount by which a sprite is darkened by when chk_L_click_spr() or chk_L_click_txt() are called
	void center_text(sf::Text& txt, sf::View& view); //centers the sprite to the window's view
	void center_sprite(sf::Sprite& spr, sf::View& view); //the tidiness of this code could be improved by using templates, this would mean there wouldnt have to be seperate functions for sf::Texts and sf::Sprites
	sf::Vector2i center_sprite_return(sf::Sprite& spr, sf::View& view); //returns the vector value that corresponds with the center position of a sprite
	sf::Vector2i center_text_return(sf::Text& txt, sf::View& view);
	void center_n_offset_text(sf::Text& txt, int x, int y, sf::View& view); //centres and applies an offset to a sf::Sprite or sf::Text
	void center_n_offset_sprite(sf::Sprite& spr, int x, int y, sf::View& view);
	int chk_L_click(sf::RenderWindow& window, sf::Sprite& spr, sf::Event &event); //checks if left click is pressed and if the mouse is inbetween the globalbounds of the sprite/text, darkens the sprite when the mouse is hovering over it
	bool chk_L_click_txt(sf::RenderWindow& window, sf::Text& txt, sf::Event &event);
	bool load_n_check_spr(sf::Texture &texture, sf::Sprite& spr, std::string path); //loads a texture and sets a sprite the given texture, also handles errors if the sprite cannot be loaded
	bool load_n_check_txt(sf::Font &font, sf::Text& txt, std::string path);
	std::string file_dialogue(); //opens a windows file explorer dialogue and returns the directory of the selected file
	bool chk_4_digit(std::string str); //checks for a digit in an std::string
}