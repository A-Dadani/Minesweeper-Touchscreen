#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Array.h>
#include <stdint.h>
#include "Color_16.h"
#include "Vec2.h"

#define NUMBER_CELLS_H 11 //Number of HORIZONTAL cells, not referencing a header
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
		enum class Status
		{
			Covered,
			Uncovered
		};
	public:
		//Could've used Vec2<char> instead, but eh ¯\_(._.)_/¯ who cares
		void SetPos(const Vec2<unsigned short int> Pos) { myPos = Pos; }
		Vec2<unsigned short int> GetPos() const { return myPos; }
		void SetContent(Content c) { cellContent = c; }
		Content GetContent() const { return cellContent; }
		void SetnNeighboringBombs(unsigned char number) { nNeighboringBombs = number; }
		unsigned char GetnNeighboringBombs() const { return nNeighboringBombs; }
		void DrawBorders(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const;
		void Draw(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const;
	private:
		void DrawNumber(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const;
		void DrawBomb(Adafruit_ILI9341& scrn, unsigned short int boardBorderThiccness) const;
	private:
		Vec2<unsigned short int> myPos;
		Content cellContent = Content::Empty;
		Status cellStatus = Status::Covered;
		unsigned char nNeighboringBombs;
	private: //COLOR TIME!
		Color_16 lightBorderColor{ (uint16_t)0xFFFF };
		Color_16 darkBorderColor{ (uint32_t)0x808080 };
		Color_16 cellFillingColor{ (uint32_t)0xC6C6C6 };

		//Colors for the number of neighboring bombs
		Color_16 numberColors[7] = { { (uint32_t)0x0200FD }, { (uint32_t)0x017E02 }, { (uint32_t)0xFE0000 }, { (uint32_t)0x010180 },
									 { (uint32_t)0x7F0300 }, { (uint32_t)0x008080 }, { (uint16_t)0x0000 } };
	};
public:
	Board(Adafruit_ILI9341& scrn, unsigned short int ScreenWidth, unsigned short int ScreenHeight,
		unsigned short int BorderThiccness, unsigned short int nBombs,
		Color_16 BorderColor = Color_16{ (uint16_t)ILI9341_BLUE });
	void DrawBorders();
	void Setup(Vec2<unsigned short int>* bombCoords);
private:
	Adafruit_ILI9341& scrn;
	Color_16 borderColor;
	Cell cells[NUMBER_CELLS_H][NUMBER_CELLS_V];

	const unsigned short int ScreenWidth;
	const unsigned short int ScreenHeight;
	const unsigned short int BorderThiccness; //Thicc border o_o
	const unsigned short int nBombs;
};