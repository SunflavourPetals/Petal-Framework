#pragma once

#ifndef Petal_Header_RawInputKeyboardController
#define Petal_Header_RawInputKeyboardController

#include "Petal~KeyboardController.h"

namespace Petal::Keyboard
{
	class RawInputController final : public Abstract::Keyboard::Controller
	{
	protected:
		void QueryState() noexcept override;
	public:
		void UpdateKeyState(VirtualKey::Type vk_code, boolean pushed) noexcept;
	public:
		RawInputController() = default;
		~RawInputController() = default;
	};
}

#endif
