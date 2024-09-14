#include "Map.h"

void Map::change_image(std::string& dir)
{ //handles the changing of the map image texture
	drawable_calculations::load_n_check_spr(map_texture, map_sprite, drawable_calculations::file_dialogue());
}

void Map::render_map(sf::RenderWindow &window)
{ //draws the map image sprite
	window.draw(map_sprite);
}

void Map::setID(int set_map_id)
{ //sets the private member map_id
	map_id = set_map_id;
}

int Map::get_map_id()
{ //returns the private member map_id
	return map_id;
}

int Map::get_vec_pos()
{ //returns the private member vec_pos
	return vec_pos;
}

void Map::set_map_id(int new_id)
{ //sets the private member map_id
	map_id = new_id;
}

void Map::set_vec_pos(int new_pos)
{ //sets the private member vec_pos
	vec_pos = new_pos;
}

void Map::render_vectors(sf::RenderWindow &window)
{
	if (!collision_vectors.empty()) //we check that the vectors are empty in order to prevent assertion errors
	{
		for (int i=0; i!=collision_vectors.size(); i++)
		{
			window.draw(collision_vectors[i]);
		}
	}
	if (!map_triggers.empty())
	{
		for (int i = 0; i != map_triggers.size(); i++)
		{
			window.draw(map_triggers[i]);
		}
	}
}