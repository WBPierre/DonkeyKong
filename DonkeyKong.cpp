// DonkeyKong.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Game.h"

int main()
{
	sf::Music music;
	music.openFromFile("Media/Sound/Music.wav");
	music.setLoop(true);
	music.play();
	Game game;
	game.run();
}
