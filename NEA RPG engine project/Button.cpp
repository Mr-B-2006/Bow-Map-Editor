#include "Button.h"
#define NOMINMAX //used so that windows.h doesnt break std::min() and std::max() (which is used in SFML's library code) https://en.sfml-dev.org/forums/index.php?topic=26401.0 

void Button::render_button(sf::RenderWindow& window, sf::View& view) //allows all buttons to be rendered in the correct updated position in one line
{
	for (int i=0; i != sizeof(button_sprites)/sizeof(button_sprites[0]); i++) //draws each component of the button
	{
		window.draw(button_sprites[i]);
	}
	button_txt.setPosition(button_txt.getPosition().x,
		button_sprites[button_sprites::beginning].getPosition().y +3); //Places the sf::Text element in front of the first button part and a little bit below the top of the button
	button_sprites[button_sprites::middle].setPosition(button_sprites[button_sprites::beginning].getPosition().x + 4, button_sprites[button_sprites::beginning].getPosition().y); //puts the middle part of the button in front of the first part
	button_sprites[button_sprites::end].setPosition(button_sprites[button_sprites::middle].getPosition().x + button_sprites[button_sprites::middle].getTextureRect().width, button_sprites[button_sprites::middle].getPosition().y); //puts the end part of the button in front of the middle part
	window.draw(button_txt);
}

bool Button::make_functional(sf::RenderWindow& window, sf::View& view, sf::Event &event) //makes the button return a bool when clicked and darkens the button when the mouse hovers over it
{
	for (int i=0; i != sizeof(button_sprites) / sizeof(button_sprites[0]); i++) //checks each button and its relationship with the mouse state
	{
		if (drawable_calculations::chk_L_click(window, button_sprites[i], event) == mouse_state::hovering) //darkens each component of the button
		{
			button_sprites[button_sprites::beginning].setColor(sf::Color(drawable_calculations::darkening_factor, drawable_calculations::darkening_factor, drawable_calculations::darkening_factor));
			button_sprites[button_sprites::middle].setColor(sf::Color(drawable_calculations::darkening_factor, drawable_calculations::darkening_factor, drawable_calculations::darkening_factor));
			button_sprites[button_sprites::end].setColor(sf::Color(drawable_calculations::darkening_factor, drawable_calculations::darkening_factor, drawable_calculations::darkening_factor));
			button_txt.setFillColor(sf::Color(drawable_calculations::darkening_factor, drawable_calculations::darkening_factor, drawable_calculations::darkening_factor));
			break;
		}
		else if (drawable_calculations::chk_L_click(window, button_sprites[i], event) == mouse_state::untouched) //sets each button back to it's normal default colour
		{
			button_sprites[button_sprites::beginning].setColor(sf::Color(255,255,255));
			button_sprites[button_sprites::middle].setColor(sf::Color(255,255,255));
			button_sprites[button_sprites::end].setColor(sf::Color(255, 255,255));
			button_txt.setFillColor(sf::Color(43, 43, 43));
		}
		else if (drawable_calculations::chk_L_click(window, button_sprites[i], event) == mouse_state::click) //keeps the button darkened when the mouse is clicked, and then returns true 
		{
			button_sprites[button_sprites::beginning].setColor(sf::Color(drawable_calculations::darkening_factor, drawable_calculations::darkening_factor, drawable_calculations::darkening_factor));
			button_sprites[button_sprites::middle].setColor(sf::Color(drawable_calculations::darkening_factor, drawable_calculations::darkening_factor, drawable_calculations::darkening_factor));
			button_sprites[button_sprites::end].setColor(sf::Color(drawable_calculations::darkening_factor, drawable_calculations::darkening_factor, drawable_calculations::darkening_factor));
			button_txt.setFillColor(sf::Color(drawable_calculations::darkening_factor, drawable_calculations::darkening_factor, drawable_calculations::darkening_factor));
			return true;
		}
	}
	return false;
}

void Button::button_full_construct(std::string b_text, sf::Vector2i position, sf::View view, sf::RenderWindow& window) //allows a button to be fully constructed after it's constructor is called, which allows a button to easily be constructed as a member of a class 
{
	drawable_calculations::load_n_check_spr(button_textures[button_sprites::beginning], button_sprites[button_sprites::beginning], "assets/images/Main Menu UI/button (beginning and end).png");
	drawable_calculations::load_n_check_spr(button_textures[button_sprites::middle], button_sprites[button_sprites::middle], "assets/images/Main Menu UI/button (middle).png");
	drawable_calculations::load_n_check_spr(button_textures[button_sprites::beginning], button_sprites[button_sprites::end], "assets/images/Main Menu UI/button (beginning and end).png");
	drawable_calculations::load_n_check_txt(button_font, button_txt, "assets/fonts/prstartk.ttf");						   
																   
	button_txt.setString(b_text);
	button_txt.setCharacterSize(90); //in SFML, anti-ailiasing is used by default, this ailiases the text so it doesnt look blurry
	button_txt.setScale(0.1, 0.1); //since we set a high character size on the above to stop extreme ailiasing while keeping the character size we want, we can set the scale to 0.1, thus making the character size actually 8 pixels large, this means that our text will not be ailiased which will thereby not make the text so blurry
	button_txt.setFillColor(sf::Color(43, 43, 43));

	button_sprites[button_sprites::beginning].setTexture(button_textures[button_sprites::beginning]);
	button_sprites[button_sprites::middle].setTexture(button_textures[button_sprites::middle]);
	button_sprites[button_sprites::end].setTexture(button_textures[button_sprites::beginning]); //we can flip the first sprite to get an ending component to the button without needing a third texture
	button_sprites[button_sprites::beginning].setPosition(position.x, position.y);
	button_sprites[button_sprites::end].setScale(-1, 1); //flips the last end of the button, thus giving us an ending component to the button
	button_sprites[button_sprites::end].setOrigin(button_sprites[button_sprites::end].getTextureRect().width, button_sprites[button_sprites::end].getOrigin().y); //puts the sprites origin to top-left of the flipped sprite (as the sprite origin does not change after flipping)
	button_textures[button_sprites::middle].setRepeated(true); //This repeats the texture when the size of the sprite is (or it's textureRect) is greater than the original source texture image. This lets us make a button of any size by repeating the texture for button_sprites[button_sprites::middle]
	button_txt.setPosition(button_sprites[button_sprites::beginning].getPosition().x + 2,
		button_sprites[button_sprites::beginning].getPosition().y + 1);
	button_sprites[button_sprites::middle].setTextureRect(sf::IntRect(0, 0, button_txt.getGlobalBounds().width - 3, button_sprites[button_sprites::middle].getGlobalBounds().height)); //sets the width of the button to a value proportional to the width of the text
	button_sprites[button_sprites::middle].setPosition(button_sprites[button_sprites::beginning].getPosition().x + 4, button_sprites[button_sprites::beginning].getPosition().y); //puts the middle portion of the sprite in front of the beginning portion
	button_sprites[button_sprites::end].setPosition(button_sprites[button_sprites::middle].getPosition().x + button_sprites[button_sprites::middle].getTextureRect().width, button_sprites[button_sprites::middle].getPosition().y); //puts the end portion of the sprite in front of the middle portion
}

void Button::override_size(int override)  //allows a custom size to be used for a button
{
	button_sprites[button_sprites::middle].setTextureRect(sf::IntRect(0, 0, override, button_sprites[button_sprites::middle].getGlobalBounds().height)); //extends the middle component's size so the texture will repeat across it
	button_txt.setPosition(button_sprites[button_sprites::middle].getPosition().x + (override / 2) - (button_txt.getGlobalBounds().width / 2), 
		button_txt.getPosition().y); //sets the button text in the correct position after the button has been resized
}

sf::Vector2f Button::get_size()
{
	sf::Vector2f size; //temporary vector that will be destroyed after it is out of scope
	for (int i = 0; i != sizeof(button_sprites) / sizeof(button_sprites[0]); i++)
	{
		size.x += button_sprites[i].getGlobalBounds().width; //adds each button component's width
	}
	size.y += button_sprites[button_sprites::beginning].getGlobalBounds().height; //we dont need to add height because each button component is of the same height
	return size;
}

void Button::setPos(int x, int y) //sets the position of the button
{
	button_sprites[button_sprites::beginning].setPosition(x, y);
	button_txt.setPosition(button_sprites[button_sprites::beginning].getPosition().x + 2,
		button_sprites[button_sprites::beginning].getPosition().y + 1);
}

sf::Vector2f Button::getPos() //returns the position of the button
{
	return button_sprites[button_sprites::middle].getPosition();
}