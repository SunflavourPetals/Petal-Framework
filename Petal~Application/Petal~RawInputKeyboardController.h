#pragma once

#ifndef Petal_Header_RawInputKeyboardController
#define Petal_Header_RawInputKeyboardController

#include "Petal~KeyboardController.h"

namespace Petal::Keyboard
{
	class RawInputController final : public Abstract::Keyboard::Controller
	{
	protected:
		void QueryState() noexcept override {};
	public:
		void UpdateKeyState(VirtualKey::Type vk_code, boolean pushed) noexcept
		{
			this->last_state.Set(vk_code, this->state.Pushed(vk_code));
			this->state.Set(vk_code, pushed);
		}
	public:
		RawInputController() = default;
		~RawInputController() = default;
	};
}

#endif
