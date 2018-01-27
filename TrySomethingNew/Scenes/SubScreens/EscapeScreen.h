#ifndef _ESCAPESCREEN_H_
#define _ESCAPESCREEN_H_

#include <string>
#include <vector>
#include "Scenes\Scene.h"

class EscapeScreen {
protected:
	std::vector<ImageData*> EscapeImagesText;

	bool EscapeScreenVisible;

public:
	EscapeScreen();

	void AddEscapeText(Scene* _scene);
	void ShowEscapeText();
	void HideEscapeText();
};


#endif