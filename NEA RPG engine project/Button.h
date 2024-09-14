#pragma once
#ifndef BUTTON_H
#define BUTTON_H
#define NOMINMAX //used so that windows.h doesnt break std::min() and std::max() (which is used in SFML's library code) https://en.sfml-dev.org/forums/index.php?topic=26401.0 
#endif
#include "SFML/Graphics.hpp"
#include "Drawable_calculations.h"

enum button_sprites //enum and and class attribute have the same name, address this in a later iteration
{
	beginning = 0, middle = 1, end = 2
};

class Button
{
private:
	sf::Texture button_textures[2];
	sf::Font button_font; //Using the "Press Start K" https://www.1001fonts.com/press-start-font.html font which is licensed under the "Open Font License" https://openfontlicense.org/
public:
	sf::Text button_txt; //this is the text overlayed on top of each button, that describes the buttons function. The use of text instead of icons both communicates the button's function clearer than with an icon and means less time has to be spent creating icons, if needed (for example, if using words will not allow the button to fit into a window), icons can be added to a button as seen in UI_Window.cpp, however, currently this cannot be done natively in the Button class
	sf::Sprite button_sprites[3]; //This array holds each sprite that makes up a button 
	Button() {} //Buttons can be constructed with the default constructor, (and then calling the full_construct() function later) or you can construct it straight away using the overloaded constructor
	Button(std::string b_text, sf::Vector2i position, sf::View view, sf::RenderWindow &window) //the default constructor allows a Button to be easily constructed as another class' member
	{
		drawable_calculations::load_n_check_spr(button_textures[button_sprites::beginning], button_sprites[button_sprites::beginning], "assets/images/Main Menu UI/button (beginning and end).png");
		drawable_calculations::load_n_check_spr(button_textures[button_sprites::middle], button_sprites[button_sprites::middle], "assets/images/Main Menu UI/button (middle).png");
		drawable_calculations::load_n_check_spr(button_textures[button_sprites::beginning], button_sprites[button_sprites::end], "assets/images/Main Menu UI/button (beginning and end).png");
		drawable_calculations::load_n_check_txt(button_font, button_txt, "assets/fonts/prstartk.ttf"); 
																		
		button_txt.setString(b_text);
		button_txt.setCharacterSize(110); //in SFML, anti-ailiasing is used by default, this ailiases the text so it doesnt look blurry
		button_txt.setScale(0.1, 0.1); //since we set a high character size on the above to stop extreme ailiasing while keeping the character size we want, we can set the scale to 0.1, thus making the character size actually 8 pixels large, this means that our text will not be ailiased which will thereby not make the text so blurry
		button_txt.setFillColor(sf::Color(43, 43, 43));

		button_sprites[button_sprites::beginning].setTexture(button_textures[button_sprites::beginning]);
		button_sprites[button_sprites::middle].setTexture(button_textures[button_sprites::middle]);
		button_sprites[button_sprites::end].setTexture(button_textures[button_sprites::beginning]); //we can flip the first sprite to get an ending component to the button without needing a third texture
		button_sprites[button_sprites::beginning].setPosition(position.x, position.y);
		button_sprites[button_sprites::end].setScale(-1,1); //flips the last end of the button, thus giving us an ending component to the button
		button_sprites[button_sprites::end].setOrigin(button_sprites[button_sprites::end].getTextureRect().width, button_sprites[button_sprites::end].getOrigin().y); //puts the sprites origin to top-left of the flipped sprite (as the sprite origin does not change after flipping)
		button_textures[button_sprites::middle].setRepeated(true); //This repeats the texture when the size of the sprite is (or it's textureRect) is greater than the original source texture image. This lets us make a button of any size by repeating the texture for button_sprites[button_sprites::middle]
		button_txt.setPosition(button_sprites[button_sprites::beginning].getPosition().x + 2, 
			button_sprites[button_sprites::beginning].getPosition().y + 3);
		button_sprites[button_sprites::middle].setTextureRect(sf::IntRect(0, 0, button_txt.getGlobalBounds().width - 3, button_sprites[button_sprites::middle].getGlobalBounds().height)); //sets the width of the button to a value proportional to the width of the text
		button_sprites[button_sprites::middle].setPosition(button_sprites[button_sprites::beginning].getPosition().x + 4, button_sprites[button_sprites::beginning].getPosition().y); //puts the middle portion of the sprite in front of the beginning portion
		button_sprites[button_sprites::end].setPosition(button_sprites[button_sprites::middle].getPosition().x + button_sprites[button_sprites::middle].getTextureRect().width, button_sprites[button_sprites::middle].getPosition().y); //puts the end portion of the sprite in front of the middle portion
	}
	void render_button(sf::RenderWindow &window, sf::View &view); //allows all buttons to be rendered in the correct updated position in one line
	bool make_functional(sf::RenderWindow& window, sf::View& view, sf::Event &event); //makes the button return a bool when clicked and darkens the button when the mouse hovers over it
	void override_size(int override); //allows a custom size to be used for a button
	void button_full_construct(std::string b_text, sf::Vector2i position, sf::View view, sf::RenderWindow& window); //allows a button to be fully constructed after it's constructor is called, which allows a button to easily be constructed as a member of a class 
	sf::Vector2f get_size(); //returns the total size of each button
	void setPos(int x, int y); //sets the position of the button
	sf::Vector2f getPos(); //returns the position of the button
};