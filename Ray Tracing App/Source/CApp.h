#pragma once

#include <iostream>
#include <iomanip>
#include <thread>
#include <atomic>

#include <SDL.h>

#include "RayTrace/image.h"
#include "RayTrace/Scenes/example_scene.h"
#include "RayTrace/camera.h"

#include "RayTrace/Textures/checker.h"
#include "RayTrace/Textures/flat.h"


class CApp {
public:
	CApp(const int width, const int height);

	int OnExecute();
	bool OnInit();
	void OnEvent(SDL_Event* event);
	void OnLoop();
	void OnRenderer();
	void OnExit();

	void RenderTile(RT::DATA::tile* tile, std::atomic<int>* threadCounter, std::atomic<int>* tileFlag);

private:
	void PrintVector(const glm::dvec3& inputVec);

	bool GenerateTileGrid(int tileSizeX, int tileSizeY);

	bool DestroyTileGrid();

	void ConvertImageToTexture(RT::DATA::tile& tile);

	Uint32 ConvertColor(const double red, const double green, const double blue);

	void ResetTileFlags();

private:
	Image							m_image;
	RT::ExampleScene				m_scene;

	bool							isRunning;

	std::vector<RT::DATA::tile>		m_tiles;
	std::vector<std::atomic<int>*>	m_tileFlags;

	int								m_maxThreads = 8;
	int								m_numCurrentThreads = 0;

	std::vector<int>				m_tilesCurrentlyRendering;
	std::vector<std::thread>		m_threads;
	std::atomic<int>*				m_threadCounter;

	int								m_numTilesX;
	int								m_numTilesY;

	int								m_Width;
	int								m_Height;

	SDL_Window*						pWindow;
	SDL_Renderer*					pRenderer;

	double							m_maxLevel = 0.8;
};