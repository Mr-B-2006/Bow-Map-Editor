#include "Map_List_Element.h"

void Map_List_Element::render_element(sf::RenderWindow& window, int x, int y)
{ //renders the map list element sprite and its sf::Text object, also handles positioning
	map_el_spr.setPosition(x, y);
	
	details.setString("Map ID: " + std::to_string(map_el_id)); //basic string concatenation doesnt work in this function, so std::to_string() is needed
	details.setPosition(x + 27, y + 3); 
	window.draw(map_el_spr);
	window.draw(details); 
}

int Map_List_Element::was_pressed(sf::RenderWindow &window, sf::Event &event)
{ //checks if the map list element sprite was clicked/hovered
	return drawable_calculations::chk_L_click(window, map_el_spr, event);
}

void Map_List_Element::setPos(int x, int y)
{ //sets the position of the map sprite
	map_el_spr.setPosition(x, y);
}

sf::Vector2f Map_List_Element::getPos()
{ //returns the position of the map list element sprite
	return map_el_spr.getPosition();
}

sf::FloatRect Map_List_Element::getGlobalBounds()
{ //returns the boundaries of the map list element sprite
	return map_el_spr.getGlobalBounds();
}