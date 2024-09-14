#include "Text_Field.h"

void Text_Field::set_field_text(std::string new_text) //changes the text displayed next to the actual field
{
	field_text.setString(new_text);
}

void Text_Field::set_field_position(int x, int y) //changes the position of the field and it's components
{
	field_text.setPosition(x, y + 2);
	field.setPosition(x + field_text.getGlobalBounds().width + 2, y); //puts the field in front of the field text
	entered_text.setPosition(field.getPosition().x + 1, field.getPosition().y +2); //entered text is placed within the field
	caret.setPosition(entered_text.getPosition().x + entered_text.getGlobalBounds().width + 2, entered_text.getPosition().y ); //caret is placed in front of the entered text (the caret's position will update as the size of entered_text grows)
}

void Text_Field::render_field(sf::RenderWindow &window) //draws the field text, the field itself, the entered text and the caret (if the field is selected)
{
	window.draw(field);
	window.draw(field_text);
	if (selected) //we only need to draw the caret if the field is selected
	{
		window.draw(caret);
	}
	window.draw(entered_text);
}

sf::Vector2f Text_Field::getPos() //returns the position of the field (the field text, specifically)
{
	return field_text.getPosition();
}

sf::FloatRect Text_Field::getGlobalBounds() //returns the global bounds of the field text
{
	return field_text.getGlobalBounds();
}

void Text_Field::allow_text_entering(sf::RenderWindow &window, sf::Event &event) //allows text to be entered into the field
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) //i didnt use drawable_calculations::chk_L_click() as i did not want the field to darken
	{
		if (field.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) //gets the mouse position relative to the SFML view instead of the window
		{
			selected = true;
		}
		else
		{
			selected = false;
		}
	}
	std::string crop_buffer; //this string will be displayed when the width of entered_text is greater than the width of the field
	if (selected && event.type == sf::Event::TextEntered)
	{ //the line below will only allow characters past 31 on the character set shown in the project document to be entered, this avoids erroneous characters such as 'backspace' being added to the string, it also prevents commas being added, which can interfere with the database (as the database is a csv file)
		if (event.text.unicode != 44 && event.text.unicode > 31 && event.text.unicode < 127)
		{
			entered_string += static_cast<char>(event.text.unicode); //takes the unicode value of whatever key was pressed, converts it, and then adds it to entered_string
			entered_text.setString(entered_string);
		}
		else if (event.text.unicode == 8 && entered_string.length() != 0) //handles backspace
		{
			entered_string.erase(entered_string.length() - 1);
			if (crop_buffer.length() != 0) //prevents std::abort() being called due to erasing a string with 0 characters
			{
				crop_buffer.erase(crop_buffer.length() - 1);
			}
			entered_text.setString(entered_string);
		}
	}
	if (entered_text.getGlobalBounds().width > field.getGlobalBounds().width - 7) //handles cropping the entered_string when it becomes wider than the field
	{
		//11 "#"s (one of the chars that is the longest in width) can be displayed
		for (int i=entered_string.length() - 12; i != entered_string.length(); i++)
		{
			crop_buffer += entered_string[i];
		}
		entered_text.setString("..." + crop_buffer);
	}
}

void Text_Field::allow_censored_text_entering(sf::RenderWindow& window, sf::Event& event)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) //i didnt use drawable_calculations::chk_L_click() as i did not want the field to darken
	{
		if (field.getGlobalBounds().contains(window.mapPixelToCoords(sf::Mouse::getPosition(window)))) //gets the mouse position relative to the SFML view instead of the window
		{
			selected = true;
		}
		else
		{
			selected = false;
		}//^FOR TESTING PURPOSES, WILL ALLOW US TO SEE CENSORED PASSWORD IN CONSOLE
	}
	std::string crop_buffer; //this string will be displayed when the width of entered_text is greater than the width of the field
	if (selected && event.type == sf::Event::TextEntered)
	{ //the line below will only allow characters past 31 on the character set shown in the project document to be entered, this avoids erroneous characters such as 'backspace' being added to the string, it also prevents commas being added, which can interfere with the database (as the database is a csv file)
		if (event.text.unicode != 44 && event.text.unicode > 31 && event.text.unicode < 127) //get password xensorship working, if its too much hassle just dont make last typed character visible
		{
			censored_string = "";
			entered_string += static_cast<char>(event.text.unicode); //takes the unicode value of whatever key was pressed, converts it, and then adds it to entered_string
			for (int i=0; i != entered_string.length() - 1 && entered_string.length() - 2 != -1; i++) //adds * for each letter in entered_string takeaway one (because we need the extra character for the last character entered)
			{
				censored_string += "*";
			}
			censored_string += static_cast<char>(event.text.unicode); //adds the last entered character to the end of censored_string
			entered_text.setString(censored_string);
		}
		else if (event.text.unicode == 8 && entered_string.length() != 0) //handles backspace
		{
			entered_string.erase(entered_string.length() - 1);
			if (censored_string.length()-1 == 0)
			{
				censored_string = "";
			}
			else
			{
				censored_string.erase(censored_string.length() - 2); //erases both the last character entered and the "*" before it
				censored_string += entered_string[entered_string.length()-1]; //this adds back the last character of entered_string, so that after backspace is pressed, stars will be followed by the last letter of entered_string
			}
			if (crop_buffer.length() != 0) //prevents std::abort() being called due to erasing a string with 0 characters
			{
				crop_buffer.erase(crop_buffer.length() - 1);
			}
			entered_text.setString(censored_string);
		}
	}
	if (entered_text.getGlobalBounds().width > field.getGlobalBounds().width - 7) //handles cropping the entered_string when it becomes wider than the field
	{
		//11 "#"s (one of the chars that is the longest in width) can be displayed
		for (int i = censored_string.length() - 10; i != censored_string.length(); i++)
		{
			crop_buffer += censored_string[i];
		}
		entered_text.setString("..." + crop_buffer);
	}
}

void Text_Field::clear() //clears the entered text from a fields (the caret's positioning is handled automatically by another function)
{
	entered_string = "";
	entered_text.setString("");
}

std::string Text_Field::getText() //returns the entered text from a field
{
	return entered_string;
}