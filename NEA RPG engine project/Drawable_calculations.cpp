#include "Drawable_calculations.h"
#define NOMINMAX //used so that windows.h doesnt break std::min() and std::max() (which is used in SFML's library code) https://en.sfml-dev.org/forums/index.php?topic=26401.0 

void drawable_calculations::center_text(sf::Text& txt, sf::View& view) //centers the sprite to the window's view
{
	txt.setPosition(view.getCenter().x - (txt.getGlobalBounds().width / 2), view.getCenter().y - (txt.getGlobalBounds().height / 2));
}

void drawable_calculations::center_sprite(sf::Sprite& spr, sf::View& view) //the tidiness of this code could be improved by using templates, this would mean there wouldnt have to be seperate functions for sf::Texts and sf::Sprites
{
	spr.setPosition(view.getCenter().x - (spr.getTextureRect().width / 2), view.getCenter().y - (spr.getTextureRect().height / 2));
}

sf::Vector2i drawable_calculations::center_sprite_return(sf::Sprite& spr, sf::View& view) //returns the vector value that corresponds with the center position of a sprite
{
	return sf::Vector2i(view.getCenter().x - (spr.getTextureRect().width / 2), view.getCenter().y - (spr.getTextureRect().height / 2));
}

sf::Vector2i drawable_calculations::center_text_return(sf::Text& txt, sf::View& view)
{
	return sf::Vector2i(view.getCenter().x - (txt.getGlobalBounds().width / 2), view.getCenter().y - (txt.getGlobalBounds().height / 2));
}

void drawable_calculations::center_n_offset_text(sf::Text& txt, int x, int y, sf::View& view) //centres and applies an offset to a sf::Sprite or sf::Text
{
	txt.setPosition(view.getCenter().x - (txt.getGlobalBounds().width / 2) + x, view.getCenter().y - (txt.getGlobalBounds().height / 2) + y);
}

void drawable_calculations::center_n_offset_sprite(sf::Sprite& spr, int x, int y, sf::View& view)
{
	spr.setPosition(view.getCenter().x - (spr.getTextureRect().width / 2) + x, view.getCenter().y - (spr.getTextureRect().height / 2) + y);
}

int drawable_calculations::chk_L_click(sf::RenderWindow& window, sf::Sprite& spr, sf::Event &event) //checks if left click is pressed and if the mouse is inbetween the globalbounds of the sprite/text, darkens the sprite when the mouse is hovering over it
{
	if (spr.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) //gets the mouse position relative to the SFML view instead of the window
	{
		spr.setColor(sf::Color(darkening_factor, darkening_factor, darkening_factor)); //darkens the sprite when the mouse hovers over the sprite
		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				return mouse_state::click;
			}
		}
		return mouse_state::hovering;
	}
	else if (spr.getColor() == sf::Color(darkening_factor, darkening_factor, darkening_factor)) //returns the sprite to it's normal brightness if the mouse not hovering over the sprite
	{
		spr.setColor(sf::Color(255, 255, 255));
	}
	return mouse_state::untouched;
}

bool drawable_calculations::chk_L_click_txt(sf::RenderWindow& window, sf::Text& txt, sf::Event &event)
{
	if (txt.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window))))
	{
		txt.setFillColor(sf::Color(darkening_factor, darkening_factor, darkening_factor));
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			return true;
		}
	}
	else if (txt.getFillColor() == sf::Color(darkening_factor, darkening_factor, darkening_factor))
	{
		txt.setFillColor(sf::Color(255, 255, 255));
	}
	return false;
}

bool drawable_calculations::load_n_check_spr(sf::Texture & texture, sf::Sprite& spr, std::string path) //loads a texture and sets a sprite the given texture, also handles errors if the sprite cannot be loaded
{
	if (!(texture.loadFromFile(path)))
	{
		MessageBoxA(NULL, ("Unable to locate image file in " + path).c_str(), "Unable to find image file.", MB_OK | MB_ICONSTOP); //shows error messagebox with the title "Unable to find image file." and message "Unable to locate image file in " + path (where path is a file directory parameter)
		return false;
	}
	spr.setTexture(texture); 
	return true;
}

bool drawable_calculations::load_n_check_txt(sf::Font &font, sf::Text& txt, std::string path)
{
	if (!(font.loadFromFile(path)))
	{
		MessageBoxA(NULL, ("Unable to locate font file in " + path).c_str(), "Unable to find font file.", MB_OK | MB_ICONSTOP);
		return false;
	}
	txt.setFont(font);
	return true;
}

std::string drawable_calculations::file_dialogue()
{
	TCHAR default_dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, default_dir);
	OPENFILENAME ofn;
	TCHAR szFile[MAX_PATH] = { 0 };

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT("All Files\0*.*\0");
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = TEXT("Select a File");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box
	if (GetOpenFileName(&ofn) == TRUE) {
		// User selected a file, `szFile` contains the selected file path
		std::wstring ws_buffer(ofn.lpstrFile);
		SetCurrentDirectory(default_dir);
		return std::string(ws_buffer.begin(), ws_buffer.end());
	}	
	SetCurrentDirectory(default_dir);
	return "";
}

bool drawable_calculations::chk_4_digit(std::string str)
{
	for (int i=0; i != str.length(); i++)
	{
		if (!isdigit(str[i]))
		{
			return false;
		}
	}
	return true;
}