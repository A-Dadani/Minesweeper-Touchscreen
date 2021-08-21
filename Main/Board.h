#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

class Board
{
private:
	class Cell
	{
	public:
	private:
	};
public:
	Board(Adafruit_ILI9341& scrn);
private:
	Adafruit_ILI9341& scrn;
};