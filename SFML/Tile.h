#ifndef TILE_H
#define TILE_H
#include <SFML\Graphics.hpp>
class Tile : public sf::Sprite
{
public:
	void set_state(std::string state);
	void set_previous(Tile* tile);
	Tile* get_previous();
	void set_begin();
	void set_end();
	void set_next();
	std::string state = "empty";
	unsigned int x_pos, y_pos;
private:
	Tile* previous;
	bool begin = false, end = false;
};
#endif