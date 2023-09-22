#include "CApp.h"

CApp::CApp(const int width, const int height)
{
	isRunning = true;
	pWindow = NULL;
	pRenderer = NULL;
	m_Height = height;
	m_Width = width;

	m_threadCounter = new std::atomic<int>(0);
}

int CApp::OnExecute()
{
	SDL_Event event;

	if (OnInit() == false) {
		return -1;
	}

	while (isRunning) {
		while (SDL_PollEvent(&event) != 0) {
			OnEvent(&event);
		}
		OnLoop();
		OnRenderer();

		SDL_Delay(1);
	}
	OnExit();
	return 0;
}

bool CApp::OnInit()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return false;
	}

	pWindow = SDL_CreateWindow("Ray Tracing App", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_Width, m_Height, SDL_WINDOW_SHOWN);

	if (pWindow != NULL) {
		pRenderer = SDL_CreateRenderer(pWindow, -1, 0);

		m_scene.m_xSize = m_Width;
		m_scene.m_ySize = m_Height;

		if (!GenerateTileGrid(128, 90)) {
			std::cout << "failed to generate tile grid\n";
			return false;
		}

		m_threadCounter->store(0, std::memory_order_release);

		SDL_SetRenderDrawColor(pRenderer, 255, 0, 255, 255);
		SDL_RenderClear(pRenderer);

		SDL_RenderPresent(pRenderer);

	}
	else {
		return false;
	}

	return true;
}

void CApp::OnEvent(SDL_Event* event)
{
	if (event->type == SDL_QUIT) {
		isRunning = false;
	}
}

void CApp::OnLoop()
{
	for (int i = 0; i < m_tiles.size(); ++i) {
		if (m_tileFlags[i]->load(std::memory_order_acquire) == 0) {
			if (m_threadCounter->load(std::memory_order_acquire) < m_maxThreads) {
				int numActiveThreads = m_threadCounter->load(std::memory_order_acquire);
				m_threadCounter->store(numActiveThreads + 1, std::memory_order_release);
				std::thread renderThread(&CApp::RenderTile, this, &m_tiles[i], m_threadCounter, m_tileFlags[i]);
				renderThread.detach();
			}
		}
	}
}

void CApp::OnRenderer()
{
	double widthFactor = 1.0;
	double heightFactor = 1.0;

	for (int i = 0; i < m_tiles.size(); ++i) {
		if (m_tileFlags[i]->load(std::memory_order_acquire) == 2) {
			SDL_Rect srcRect, dstRect;
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.w = m_tiles[i].xSize;
			srcRect.h = m_tiles[i].ySize;

			dstRect.x = static_cast<int>(std::round(static_cast<double>(m_tiles[i].x) * widthFactor));
			dstRect.y = static_cast<int>(std::round(static_cast<double>(m_tiles[i].y) * heightFactor));
			dstRect.w = static_cast<int>(std::round(static_cast<double>(m_tiles[i].xSize) * widthFactor));
			dstRect.h = static_cast<int>(std::round(static_cast<double>(m_tiles[i].ySize) * heightFactor));

			if (!m_tiles[i].textureComplete) {
				ConvertImageToTexture(m_tiles[i]);
				m_tiles[i].textureComplete = true;
				SDL_RenderCopy(pRenderer, m_tiles[i].pTexture, &srcRect, &dstRect);
			}
		}
	}
	SDL_RenderPresent(pRenderer);
	
}

void CApp::OnExit()
{
	bool result = DestroyTileGrid();

	delete m_threadCounter;
	for (int i = 0; i < m_tileFlags.size(); ++i) {
		delete m_tileFlags[i];
	}

	SDL_DestroyRenderer(pRenderer);
	SDL_DestroyWindow(pWindow);
	pWindow = NULL;
	SDL_Quit();
}

void CApp::RenderTile(RT::DATA::tile* tile, std::atomic<int>* threadCounter, std::atomic<int>* tileFlag)
{
	tileFlag->store(1, std::memory_order_release);
	m_scene.RenderTile(tile);
	int numActiveThreads = threadCounter->load(std::memory_order_acquire);
	threadCounter->store(numActiveThreads - 1, std::memory_order_release);
	tileFlag->store(2, std::memory_order_release);
}

void CApp::PrintVector(const glm::dvec3& inputVec)
{
	std::cout << std::fixed << std::setprecision(3) << inputVec.x << std::endl;
	std::cout << std::fixed << std::setprecision(3) << inputVec.y << std::endl;
	std::cout << std::fixed << std::setprecision(3) << inputVec.z << std::endl;
	std::cout << std::endl;
}

bool CApp::GenerateTileGrid(int tileSizeX, int tileSizeY)
{
	int numTilesX = std::floor(m_Width / tileSizeX);
	m_numTilesX = numTilesX;
	
	int numTilesY = std::floor(m_Height / tileSizeY);
	m_numTilesY = numTilesY;

	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
	amask = 0x000000ff;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = 0xff000000;
#endif

	SDL_Surface* tempSurface = SDL_CreateRGBSurface(0, tileSizeX, tileSizeY, 32, rmask, gmask, bmask, amask);

	for (int y = 0; y < numTilesY; ++y) {
		for (int x = 0; x < numTilesX; ++x) {
			RT::DATA::tile tempTile;
			tempTile.x = x * tileSizeX;
			tempTile.y = y * tileSizeY;

			tempTile.xSize = tileSizeX;
			tempTile.ySize = tileSizeY;

			tempTile.renderComplete = 0;
			tempTile.pTexture = SDL_CreateTextureFromSurface(pRenderer, tempSurface);

			tempTile.rgbData.resize(tempTile.xSize * tempTile.ySize);
			m_tiles.push_back(tempTile);
		}
	}

	for (int i = 0; i < m_tiles.size(); ++i) {
		m_tileFlags.push_back(new std::atomic<int>(0));
	}

	SDL_FreeSurface(tempSurface);
	return true;
}

bool CApp::DestroyTileGrid()
{
	for (int i = 0; i < m_tiles.size(); ++i) {
		if (m_tiles[i].pTexture != NULL) {
			SDL_DestroyTexture(m_tiles[i].pTexture);
		}
	}
	return true;
}

void CApp::ConvertImageToTexture(RT::DATA::tile& tile)
{
	Uint32* tempPixels = new Uint32[tile.xSize * tile.ySize];

	memset(tempPixels, 0, tile.xSize * tile.ySize * sizeof(Uint32));

	for (int i = 0; i < tile.xSize * tile.ySize; ++i) {
		tempPixels[i] = ConvertColor(tile.rgbData[i].x, tile.rgbData[i].y, tile.rgbData[i].z);
	}

	SDL_UpdateTexture(tile.pTexture, NULL, tempPixels, tile.xSize* sizeof(Uint32));

	delete[] tempPixels;
}

Uint32 CApp::ConvertColor(const double red, const double green, const double blue)
{
	double newRed	= std::max(std::min(std::pow(red, m_maxLevel), 1.0), 0.0);
	double newGreen = std::max(std::min(std::pow(green, m_maxLevel), 1.0), 0.0);
	double newBlue	= std::max(std::min(std::pow(blue, m_maxLevel), 1.0), 0.0);

	unsigned char r = static_cast<unsigned char>(newRed * 255.0);
	unsigned char g = static_cast<unsigned char>(newGreen * 255.0);
	unsigned char b = static_cast<unsigned char>(newBlue * 255.0);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	Uint32 pixelColor = (r << 24) + (g << 16) + (b << 8) + 255;
#else
	Uint32 pixelColor = (255 << 24) + (r << 16) + (g << 8) + b;
#endif
	
	return pixelColor;
}

void CApp::ResetTileFlags()
{
	for (int i = 0; i < m_tileFlags.size(); ++i) {
		m_tileFlags[i]->store(0, std::memory_order_release);
		m_tiles[i].textureComplete = false;
	}
}
