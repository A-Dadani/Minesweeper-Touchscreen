#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Array.h>
#include "Color_16.h"
#include "Vec2.h"

#define NUMBER_CELLS_H 11
#define NUMBER_CELLS_V 15
#define CELL_DIMENTIONS 20

class Board
{
private:
	class Cell
	{
	public:
		enum class Content
		{
			Empty,
			Bomb
		};
	public:
		Cell() = default;
		//Could've used Vec2<char> instead, but it's too late
		void SetPos(const Vec2<unsigned short int> Pos) { myPos = Pos; }
		Vec2<unsigned short int> GetPos() const { return myPos; }
		void SetContent(Content c) { cellContent = c; }
		Content GetContent() const { return cellContent; }
		void SetnNeighboringBombs(unsigned char number) { nNeighboringBombs = number; }
		unsigned char GetnNeighboringBombs() const { return nNeighboringBombs; }
	private:
		Vec2<unsigned short int> myPos;
		Content cellContent = Content::Empty;
		unsigned char nNeighboringBombs;
	};
public:
	Board(Adafruit_ILI9341& scrn, unsigned short int ScreenWidth, unsigned short int ScreenHeight,
		unsigned short int BorderThiccness, unsigned short int nBombs,
		Color_16 BorderColor = Color_16{ (uint16_t)ILI9341_BLUE });
	void DrawBorders();
	void Setup();
private:
	Adafruit_ILI9341& scrn;
	Color_16 borderColor;
	Cell cells[NUMBER_CELLS_H][NUMBER_CELLS_V];

	const unsigned short int ScreenWidth;
	const unsigned short int ScreenHeight;
	const unsigned short int BorderThiccness;
	const unsigned short int nBombs;
};