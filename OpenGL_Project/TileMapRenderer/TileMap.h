#pragma once

class TileMap 
{
public:
	int Width;
	int Height;
	unsigned char* Tiles;
	int TileSetHandle;

	TileMap(){}
	TileMap(int width, int height) 
	{
		Width = width;
		Height = height;
		Tiles = new unsigned char[width * height];
	}

	unsigned char GetTile(int x, int y) 
	{
		return Tiles[x + y * Width];
	}

	int GetSize() {
		return (Width * Height);
	}

};