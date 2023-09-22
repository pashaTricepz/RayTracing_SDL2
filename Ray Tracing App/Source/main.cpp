#include "CApp.h"

int main(int argc, char* argv[]) {
	const int w = 1280;
	const int h = 720;

	CApp app(w ,h);
	
	return app.OnExecute();
}