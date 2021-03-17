#include <functional>
#include <string>

#include "imgui/imgui.h"

#ifdef _WIN32
#include <intrin.h>
#include <Windows.h>
#include <Psapi.h>

#include "imgui/imgui_impl_dx9.h"
#include "imgui/imgui_impl_win32.h"

#include "MinHook/MinHook.h"
#elif __linux__
#include <sys/mman.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "imgui/GL/gl3w.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_opengl3.h"
#endif

#include "Config.h"
#include "EventListener.h"
#include "GameData.h"
#include "GUI.h"
#include "Hooks.h"
#include "Interfaces.h"
#include "Memory.h"

#include "Hacks/Aimbot.h"
#include "Hacks/AntiAim.h"
#include "Hacks/Backtrack.h"
#include "Hacks/Chams.h"
#include "Hacks/EnginePrediction.h"
#include "Hacks/StreamProofESP.h"
#include "Hacks/Glow.h"
#include "Hacks/Misc.h"
#include "Hacks/SkinChanger.h"
#include "Hacks/Triggerbot.h"
#include "Hacks/Visuals.h"

#include "SDK/Engine.h"
#include "SDK/Entity.h"
#include "SDK/EntityList.h"
#include "SDK/FrameStage.h"
#include "SDK/GameEvent.h"
#include "SDK/GameUI.h"
#include "SDK/GlobalVars.h"
#include "SDK/InputSystem.h"
#include "SDK/MaterialSystem.h"
#include "SDK/ModelRender.h"
#include "SDK/Panel.h"
#include "SDK/Platform.h"
#include "SDK/RenderContext.h"
#include "SDK/SoundInfo.h"
#include "SDK/SoundEmitter.h"
#include "SDK/StudioRender.h"
#include "SDK/Surface.h"
#include "SDK/UserCmd.h"
#include "SDK/ConVar.h"

namespace Sightdown {
	Vector initialOffset{ 0.0f,0.0f,0.0f };
	bool hasInitted{ false };
	struct SightData {
		Vector data;
		Vector cdata;
		bool isEqualToInitialOffset;

		SightData() {
			data = Vector{ 0.0f,0.0f,0.0f };
			cdata = Vector{ 0.0f,0.0f,0.0f };
			isEqualToInitialOffset = false;
		}

		SightData(Vector Offset,Vector COffset) {
			data = Offset;
			cdata = COffset;
			isEqualToInitialOffset = false;
		}

		SightData(bool isEqual) {
			data = Vector{ 0.0f,0.0f,0.0f };
			cdata = Vector{ 0.0f,0.0f,0.0f };
			isEqualToInitialOffset = isEqual;
		}

		void Init(Vector initialVector) {
			initialOffset = initialVector;
		}

		Vector getSightOffset(bool crouch)
		{
			if (isEqualToInitialOffset)
			{
				return initialOffset;
			}
			else
			{
				if (crouch) {
					return cdata;
				}
				else {
					return data;
				}
			}
		}
	};
	void init();
	void setSight(UserCmd* cmd);
}