// Start Header -------------------------------------------------------
//Copyright (C) 2018-2019 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior
//written consent of DigiPen Institute of Technology is prohibited.
//Author: Sidhant Tumma
//- End Header --------------------------------------------------------
#include "stdafx.h"
//#include "Visual Leak Detecter/include/vld.h"

Engine *engine;

int main(int argc, char* args[])
{
	engine = new Engine();
	engine->Init();
	engine->Run();
	engine->Close();
	delete engine;
	return 0;
}