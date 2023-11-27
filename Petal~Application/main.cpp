#include "Petal~Main.h"
#include "Petal~Window.h"
#include "Petal~VSDebugOutput.h"
#include "Petal~FrequencyController.h"

#include <format>

class Main
{
public:
	static int main()
	{
		using namespace Petal;
		class MyWindow : public Window
		{
			void CreateEvent(CreateMessage& e) noexcept override
			{
				dout + std::format("Create: {}", e.continue_creation) + ln;
			}
			void PaintEvent(PaintMessage& e) noexcept override
			{
				dout + "repaint event" + ln;
				this->DefaultDraw(e);
			}
		} w{};
		class Proc : public Abstract::Process<>
		{
			FrequencyController f;
			class IProc : public Abstract::Process<typename FrequencyController::ResourceDelta>
			{
				Window& w;
			public:
				IProc(Window& w_) : w{ w_ } {}
				void Execution(const FrequencyController::ResourceDelta&) override
				{
					dout + "call repaint" + ln;
					w.Repaint();
				}
			} ip;
		public:
			Proc(Window& w) : f{}, ip{ w } { f.UpdateFrequency(1.0); }
			void Execution() override
			{
				f.LimitedDo(ip);
			}
		} p{ w };
		w.Create(WindowClassArgs{ Petal_TStr("my window class") }.Register().class_atom);
		w.Show();
		return MessageLoop(p);
	}
};

Petal_SetMainClass(Main);
