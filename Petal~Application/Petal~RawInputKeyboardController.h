#pragma once

#ifndef Petal_Header_RawInputKeyboardController
#define Petal_Header_RawInputKeyboardController

#include "Petal~KeyboardController.h"

namespace Petal::Keyboard
{
	class RawInputController final : public BasicController
	{
	protected:
		void QueryState() noexcept override;
	public:
		void UpdateKeyState(VirtualKey::Type vk_code, boolean pushed) noexcept;
	public:
		RawInputController() = default;
		RawInputController(const RawInputController&) = default;
		RawInputController(RawInputController&&) noexcept = default;
		~RawInputController() = default;
		RawInputController& operator = (const RawInputController&) = default;
		RawInputController& operator = (RawInputController&&) noexcept = default;
	};
}

#endif
