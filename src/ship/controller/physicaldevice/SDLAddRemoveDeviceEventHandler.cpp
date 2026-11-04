#include "ship/controller/physicaldevice/SDLAddRemoveDeviceEventHandler.h"
#include <SDL2/SDL.h>
#include "ship/Context.h"
#include "ship/controller/controldeck/ControlDeck.h"
#include "ship/window/Window.h"
#include "ship/window/gui/Gui.h"

namespace Ship {

SDLAddRemoveDeviceEventHandler::~SDLAddRemoveDeviceEventHandler() {
}

void SDLAddRemoveDeviceEventHandler::InitElement() {
}

void SDLAddRemoveDeviceEventHandler::DrawElement() {
}

void SDLAddRemoveDeviceEventHandler::UpdateElement() {
    SDL_PumpEvents();
    SDL_Event event;
    bool changed = false;
    while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_CONTROLLERDEVICEADDED, SDL_CONTROLLERDEVICEADDED) > 0) {
        // from https://wiki.libsdl.org/SDL2/SDL_ControllerDeviceEvent: which - the joystick device index for
        // the SDL_CONTROLLERDEVICEADDED event
        Context::GetInstance()->GetControlDeck()->GetConnectedPhysicalDeviceManager()->HandlePhysicalDeviceConnect(
            event.cdevice.which);
        changed = true;
    }

    while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_CONTROLLERDEVICEREMOVED, SDL_CONTROLLERDEVICEREMOVED) > 0) {
        // from https://wiki.libsdl.org/SDL2/SDL_ControllerDeviceEvent: which - the [...] instance id for the
        // SDL_CONTROLLERDEVICEREMOVED [...] event
        Context::GetInstance()->GetControlDeck()->GetConnectedPhysicalDeviceManager()->HandlePhysicalDeviceDisconnect(
            event.cdevice.which);
        changed = true;
    }

    // These events never reach ImGui, so tell its gamepad backend the controller set changed; otherwise a
    // controller connected after launch cannot navigate the menus.
    if (changed) {
        auto window = Context::GetInstance()->GetWindow();
        if (window != nullptr && window->GetGui() != nullptr) {
            window->GetGui()->RefreshImGuiGamepads();
        }
    }
}
} // namespace Ship
