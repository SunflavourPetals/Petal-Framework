#include "Petal~RawInputKeyboardController.h"

namespace Petal::Keyboard
{
	void RawInputController::QueryState() noexcept
	{

	}
	void RawInputController::UpdateKeyState(VirtualKey::Type vk_code, boolean pushed) noexcept
	{
		this->last_state.Set(vk_code, this->state.Pushed(vk_code));
		this->state.Set(vk_code, pushed);
	}
}
