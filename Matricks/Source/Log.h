/* Start Header -------------------------------------------------------
Copyright (C) 2018-2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
Author: Sidhant Tumma
- End Header --------------------------------------------------------*/
#pragma once

#define LogError(x) std::cout << "Error:" << __FILE__ << ":Line " << __LINE__ << ": " <<  x << std::endl; __debugbreak()
#define LogInfo(x) std::cout << "Info:" << x << std::endl;
#ifdef DEBUG
#define LogWarning(x) std::cout << "Warning:" << __FILE__ << ":Line " << __LINE__ << ": " <<  x << std::endl; __debugbreak()
#endif