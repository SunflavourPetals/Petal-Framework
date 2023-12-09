#include "Petal~RawInputKeyboardController.h"

namespace Petal::Keyboard
{
	void RawInputController::QueryState() noexcept
	{

	};
	void RawInputController::UpdateKeyState(VirtualKey::Type vk_code, boolean pushed) noexcept
	{
		this->pt_last_state.Set(vk_code, this->pt_state.Pushed(vk_code));
		this->pt_state.Set(vk_code, pushed);
	}
}
