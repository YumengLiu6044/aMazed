#ifndef GAME_H
#define GAME_H
#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include "Tile.h"
#define WindowWidth 800
#define WindowHeight 600
// The base structure of the Game class was from the book "SFML Game Development"
/*
* class Game
{
public:
Game();
void run();
private:
void processEvents();
void update();
void render();
private:
sf::RenderWindow mWindow;
sf::CircleShape mPlayer;
};
*/
// The rest are my own code

class Game
{
public:
	Game();
	void run();
private:
	void meta_tiles_listen_event();
	void generateTiles();
	void processEvents();
	void update(sf::Time dt);
	void clear();
	void render();
	void find();
	void around(Tile &tile);
	void trace();
	void set_state(Tile &tile, std::string state);
	void handleMouseEvent(sf::Mouse::Button button, bool IsPressed);
	sf::Texture loadTexture(std::string file_path);
private:
	sf::RenderWindow mWindow;
	sf::Texture og_tile;
	std::vector<std::vector<Tile>> tiles;
	bool mouse_pressed = false, running_find = false, tracing = false;
	std::string mouse_button = "none";
	std::string working_dir = "";
	float mWidth, mHeight;
	unsigned int width, height;
	unsigned int mouseX = 0, mouseY = 0;
	int tested[WindowWidth * WindowHeight / 100][2], next_tested[WindowWidth * WindowHeight / 100][2], next[WindowWidth * WindowHeight / 100][2];
	int next_size = 0, next_tested_size = 0, tested_size = 0;
	Tile* begin, * end;
};
#endif

