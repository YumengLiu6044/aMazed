#include "Game.h"
Game::Game() :
	mWindow(sf::VideoMode(WindowWidth, WindowHeight), "Maze Solver"),
	og_tile()
{
	mWindow.setFramerateLimit(30);
	std::string message;
	message = "Keys\tFunction\n";
	message += "B\tMake All Empty Blocks Wall\n";
	message += "C\tClear Screen\n";
	message += "SPACE\tStart Tracing\n";
	std::cout << message << std::endl;
	sf::Vector2u size = mWindow.getSize();
	og_tile = loadTexture("empty_tile.png");
	mWidth = (float)size.x;
	mHeight = (float)size.y;
	width = (unsigned int)(mWidth / 10.0f);
	height = (unsigned int)(mHeight / 10.0f);
	generateTiles();
}
//********************The original run function from the book "SFML Game Development"***********************************
/*
void Game::run()
{
while (mWindow.isOpen())
{
processEvents();
update();
render();
}
}
*/
void Game::run()
{
	while (mWindow.isOpen())
	{
		if (!(running_find || tracing))
		{
			processEvents();
		}
		else if (running_find)
		{
			find();
		}
		else
		{
			trace();
		}
		render();
	}
}
//********************The original processEvents function from the book "SFML Game Development"***********************************
/*
void Game::processEvents()
{
sf::Event event;
while (mWindow.pollEvent(event))
{
if (event.type == sf::Event::Closed)
mWindow.close();
}
}
*/
void Game::processEvents()
{
	sf::Event event;
	sf::Mouse mouse;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		
		case sf::Event::MouseButtonPressed:
			handleMouseEvent(event.mouseButton.button, true);
			break;
		case sf::Event::MouseButtonReleased:
			handleMouseEvent(event.mouseButton.button, false);
			break;
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			running_find = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
		{
			clear();
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					if (tiles[i][j].state == "empty")
					{
						set_state(tiles[i][j], "wall");
					}
				}
			}
		}
	}
	mouseX = mouse.getPosition(mWindow).x;
	mouseY = mouse.getPosition(mWindow).y;
	if (mouse.getPosition(mWindow).x < 0)
	{
		mouseX = 0;
	}
	if (mouse.getPosition(mWindow).y < 0)
	{
		mouseY = 0;
	}

	if (mouseX > mWidth)
	{
		mouseX = mWidth - 1;
	}
	if (mouseY > mHeight)
	{
		mouseY = mHeight - 1;
	}
	meta_tiles_listen_event();
}
void Game::update(sf::Time dt)
{
}
void Game::clear()
{
	tiles.clear();
	next_size = 0;
	next_tested_size = 0;
	tested_size = 0;
	generateTiles();


}
void Game::render()
{
	mWindow.clear(sf::Color(255, 255, 255, 255));
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			mWindow.draw(tiles[i][j]);
		}
	}
	mWindow.display();
}

void Game::find()
{
	running_find = true;
	if (!(end->state == "next"))
	{
		if (next_size == 0)
		{
			running_find = false;
			std::cout << "No solution" << std::endl;
			clear();
			return;
		}
		for (int i = 0; i < next_size; i++)
		{
			next_tested[i][0] = next[i][0];
			next_tested[i][1] = next[i][1];
			tiles[next_tested[i][0]][next_tested[i][1]].set_state("next_tested");
			tiles[next_tested[i][0]][next_tested[i][1]].setColor(sf::Color::Green);
			next_tested_size++;
		}
		next_size = 0;
		for (int i = 0; i < next_tested_size; i++)
		{
			around(tiles[next_tested[i][0]][next_tested[i][1]]);
			tested[tested_size][0] = next_tested[i][0];
			tested[tested_size][1] = next_tested[i][1];

			tiles[tested[tested_size][0]][tested[tested_size][1]].set_state("tested");
			tiles[tested[tested_size][0]][tested[tested_size][1]].setColor(sf::Color::Green);
			tested_size++;
		}
		next_tested_size = 0;
	}
	else
	{
		running_find = false;
		tracing = true;
	}
}

void Game::around(Tile &tile)
{
	unsigned int x = tile.x_pos;
	unsigned int y = tile.y_pos;

	if ((tiles[x + 1][y].state == "end" || tiles[x + 1][y].state == "empty"))
	{
		tiles[x + 1][y].set_state("next");
		tiles[x + 1][y].setColor(sf::Color::Yellow);
		tiles[x + 1][y].set_previous(&tile);
		next[next_size][0] = x + 1;
		next[next_size][1] = y;
		next_size++;
	}
	if ((tiles[x - 1][y].state == "end" || tiles[x - 1][y].state == "empty"))
	{
		tiles[x - 1][y].set_state("next");
		tiles[x - 1][y].setColor(sf::Color::Yellow);
		tiles[x - 1][y].set_previous(&tile);
		next[next_size][0] = x - 1;
		next[next_size][1] = y;
		next_size++;
	}

	if ((tiles[x][y + 1].state == "end" || tiles[x][y + 1].state == "empty"))
	{
		tiles[x][y + 1].set_state("next");
		tiles[x][y + 1].setColor(sf::Color::Yellow);
		tiles[x][y + 1].set_previous(&tile);
		next[next_size][0] = x;
		next[next_size][1] = y + 1;
		next_size++;
	}

	if ((tiles[x][y - 1].state == "end" || tiles[x][y - 1].state == "empty"))
	{
		tiles[x][y - 1].set_state("next");
		tiles[x][y - 1].setColor(sf::Color::Yellow);
		tiles[x][y - 1].set_previous(&tile);
		next[next_size][0] = x;
		next[next_size][1] = y - 1;
		next_size++;
	}
}

void Game::set_state(Tile& tile, std::string state)
{
	tile.set_state(state);
	if (state == "empty") tile.setColor(sf::Color::White);
	else if (state == "wall") tile.setColor(sf::Color::Black);
	else if (state == "begin")
	{
		tile.setColor(sf::Color::Blue);
		tile.set_begin();
		begin = &tile;
	}
	else if (state == "end")
	{
		tile.setColor(sf::Color::Red);
		tile.set_end();
		end = &tile;
	}
	else if (state == "tested") tile.setColor(sf::Color::Green);
	else if (state == "next_tested") tile.setColor(sf::Color::Green);
	else tile.setColor(sf::Color::Yellow);
}

void Game::handleMouseEvent(sf::Mouse::Button button, bool IsPressed)
{
	
	mouse_pressed = IsPressed;
	if (!(mouse_pressed))
	{
		mouse_button = "none";
	}
	else
	{
		if (button == sf::Mouse::Left)
		{
			mouse_button = "left";
		}
		else if (button == sf::Mouse::Right)
		{
			mouse_button = "right";
		}
	}
}

sf::Texture Game::loadTexture(std::string file_path)
{
	sf::Texture loaded_texture;
	if (!loaded_texture.loadFromFile(working_dir + file_path))
	{
		std::cout << "Can't load:\t" << file_path << std::endl;
	}
	return loaded_texture;
}

void Game::generateTiles()
{
	for (unsigned int i = 0; i < width; i++)
	{
		std::vector<Tile> these_tiles;
		for (unsigned int j = 0; j < height; j++)
		{

			Tile tile;
			tile.setTexture(og_tile);
			set_state(tile, "empty");
			tile.setPosition(i * 10, j * 10);
			tile.x_pos = i;
			tile.y_pos = j;
			if (i == 0 || i == (width - 1))
			{
				set_state(tile, "wall");
			}
			if (j == 0 || j == (height - 1))
			{
				set_state(tile, "wall");
			}
			these_tiles.push_back(tile);

		}
		tiles.push_back(these_tiles);
	}
	set_state(tiles[4][56], "begin");
	around(tiles[4][56]);
	set_state(tiles[76][4], "end");
}
void Game::meta_tiles_listen_event()
{
	unsigned int this_width = mouseX / 10;
	unsigned int this_height = mouseY / 10;
	if (this_width >= (unsigned int)(mWidth / 10.0f))
	{
		this_width = mWidth / 10.0f - 1;
	}
	if (this_height >= (unsigned int)(mHeight / 10.0f))
	{
		this_height = mHeight / 10.0f - 1;
	}
	if (mouse_button == "left")
	{
		if (!(tiles[this_width][this_height].state == "begin" || tiles[this_width][this_height].state == "end" || tiles[this_width][this_height].state == "next"))
			set_state(tiles[this_width][this_height], "wall");
	}
	else if (mouse_button == "right")
	{
		if (!(tiles[this_width][this_height].state == "begin" || tiles[this_width][this_height].state == "end" || tiles[this_width][this_height].state == "next"))
		{
			if (!(this_width == 0 || this_height == 0 || this_width == (width - 1) || this_height == (height - 1)))
			{
				set_state(tiles[this_width][this_height], "empty");
			}
		}
	}
}

void Game::trace()
{
	end->setColor(sf::Color::Blue);
	end = end->get_previous();
	if (end->state == "begin")
	{
		tracing = false;
	}
}