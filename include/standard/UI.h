#pragma once

#include <WinUser.h>

namespace UI {

	unsigned int BaseStyle = WS_CHILD | WS_VISIBLE;

	struct {
		const unsigned int Width = 100;
		const unsigned int Height = 20;
	} Button;
}