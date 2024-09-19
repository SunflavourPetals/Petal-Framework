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
		void UpdateState() noexcept
		{
			this->last_state = this->state;
		}
		void UpdateState(VirtualKey::Type vk_code, boolean pushed) noexcept
		{
			UpdateState();
			this->state.Set(vk_code, pushed);
		}
	public:
		RawInputController() = default;
		~RawInputController() = default;
	};
}

#endif
