#include "BrowserImpl.h"

#include <fstream>
#include <sstream>

#include <wrapper/cef_helpers.h>
#include <wrapper/cef_stream_resource_handler.h>

namespace {

int sfKeycodeToWindowsKeycode(sf::Keyboard::Key sfKeycode) {
	int vkey = 0;

	switch (sfKeycode)
	{
	default:                       vkey = 0;             break;
	case sf::Keyboard::A:          vkey = 'A';           break;
	case sf::Keyboard::B:          vkey = 'B';           break;
	case sf::Keyboard::C:          vkey = 'C';           break;
	case sf::Keyboard::D:          vkey = 'D';           break;
	case sf::Keyboard::E:          vkey = 'E';           break;
	case sf::Keyboard::F:          vkey = 'F';           break;
	case sf::Keyboard::G:          vkey = 'G';           break;
	case sf::Keyboard::H:          vkey = 'H';           break;
	case sf::Keyboard::I:          vkey = 'I';           break;
	case sf::Keyboard::J:          vkey = 'J';           break;
	case sf::Keyboard::K:          vkey = 'K';           break;
	case sf::Keyboard::L:          vkey = 'L';           break;
	case sf::Keyboard::M:          vkey = 'M';           break;
	case sf::Keyboard::N:          vkey = 'N';           break;
	case sf::Keyboard::O:          vkey = 'O';           break;
	case sf::Keyboard::P:          vkey = 'P';           break;
	case sf::Keyboard::Q:          vkey = 'Q';           break;
	case sf::Keyboard::R:          vkey = 'R';           break;
	case sf::Keyboard::S:          vkey = 'S';           break;
	case sf::Keyboard::T:          vkey = 'T';           break;
	case sf::Keyboard::U:          vkey = 'U';           break;
	case sf::Keyboard::V:          vkey = 'V';           break;
	case sf::Keyboard::W:          vkey = 'W';           break;
	case sf::Keyboard::X:          vkey = 'X';           break;
	case sf::Keyboard::Y:          vkey = 'Y';           break;
	case sf::Keyboard::Z:          vkey = 'Z';           break;
	case sf::Keyboard::Num0:       vkey = '0';           break;
	case sf::Keyboard::Num1:       vkey = '1';           break;
	case sf::Keyboard::Num2:       vkey = '2';           break;
	case sf::Keyboard::Num3:       vkey = '3';           break;
	case sf::Keyboard::Num4:       vkey = '4';           break;
	case sf::Keyboard::Num5:       vkey = '5';           break;
	case sf::Keyboard::Num6:       vkey = '6';           break;
	case sf::Keyboard::Num7:       vkey = '7';           break;
	case sf::Keyboard::Num8:       vkey = '8';           break;
	case sf::Keyboard::Num9:       vkey = '9';           break;
	case sf::Keyboard::Escape:     vkey = VK_ESCAPE;     break;
	case sf::Keyboard::LControl:   vkey = VK_LCONTROL;   break;
	case sf::Keyboard::LShift:     vkey = VK_LSHIFT;     break;
	case sf::Keyboard::LAlt:       vkey = VK_LMENU;      break;
	case sf::Keyboard::LSystem:    vkey = VK_LWIN;       break;
	case sf::Keyboard::RControl:   vkey = VK_RCONTROL;   break;
	case sf::Keyboard::RShift:     vkey = VK_RSHIFT;     break;
	case sf::Keyboard::RAlt:       vkey = VK_RMENU;      break;
	case sf::Keyboard::RSystem:    vkey = VK_RWIN;       break;
	case sf::Keyboard::Menu:       vkey = VK_APPS;       break;
	case sf::Keyboard::LBracket:   vkey = VK_OEM_4;      break;
	case sf::Keyboard::RBracket:   vkey = VK_OEM_6;      break;
	case sf::Keyboard::Semicolon:  vkey = VK_OEM_1;      break;
	case sf::Keyboard::Comma:      vkey = VK_OEM_COMMA;  break;
	case sf::Keyboard::Period:     vkey = VK_OEM_PERIOD; break;
	case sf::Keyboard::Quote:      vkey = VK_OEM_7;      break;
	case sf::Keyboard::Slash:      vkey = VK_OEM_2;      break;
	case sf::Keyboard::Backslash:  vkey = VK_OEM_5;      break;
	case sf::Keyboard::Tilde:      vkey = VK_OEM_3;      break;
	case sf::Keyboard::Equal:      vkey = VK_OEM_PLUS;   break;
	case sf::Keyboard::Hyphen:     vkey = VK_OEM_MINUS;  break;
	case sf::Keyboard::Space:      vkey = VK_SPACE;      break;
	case sf::Keyboard::Enter:      vkey = VK_RETURN;     break;
	case sf::Keyboard::Backspace:  vkey = VK_BACK;       break;
	case sf::Keyboard::Tab:        vkey = VK_TAB;        break;
	case sf::Keyboard::PageUp:     vkey = VK_PRIOR;      break;
	case sf::Keyboard::PageDown:   vkey = VK_NEXT;       break;
	case sf::Keyboard::End:        vkey = VK_END;        break;
	case sf::Keyboard::Home:       vkey = VK_HOME;       break;
	case sf::Keyboard::Insert:     vkey = VK_INSERT;     break;
	case sf::Keyboard::Delete:     vkey = VK_DELETE;     break;
	case sf::Keyboard::Add:        vkey = VK_ADD;        break;
	case sf::Keyboard::Subtract:   vkey = VK_SUBTRACT;   break;
	case sf::Keyboard::Multiply:   vkey = VK_MULTIPLY;   break;
	case sf::Keyboard::Divide:     vkey = VK_DIVIDE;     break;
	case sf::Keyboard::Left:       vkey = VK_LEFT;       break;
	case sf::Keyboard::Right:      vkey = VK_RIGHT;      break;
	case sf::Keyboard::Up:         vkey = VK_UP;         break;
	case sf::Keyboard::Down:       vkey = VK_DOWN;       break;
	case sf::Keyboard::Numpad0:    vkey = VK_NUMPAD0;    break;
	case sf::Keyboard::Numpad1:    vkey = VK_NUMPAD1;    break;
	case sf::Keyboard::Numpad2:    vkey = VK_NUMPAD2;    break;
	case sf::Keyboard::Numpad3:    vkey = VK_NUMPAD3;    break;
	case sf::Keyboard::Numpad4:    vkey = VK_NUMPAD4;    break;
	case sf::Keyboard::Numpad5:    vkey = VK_NUMPAD5;    break;
	case sf::Keyboard::Numpad6:    vkey = VK_NUMPAD6;    break;
	case sf::Keyboard::Numpad7:    vkey = VK_NUMPAD7;    break;
	case sf::Keyboard::Numpad8:    vkey = VK_NUMPAD8;    break;
	case sf::Keyboard::Numpad9:    vkey = VK_NUMPAD9;    break;
	case sf::Keyboard::F1:         vkey = VK_F1;         break;
	case sf::Keyboard::F2:         vkey = VK_F2;         break;
	case sf::Keyboard::F3:         vkey = VK_F3;         break;
	case sf::Keyboard::F4:         vkey = VK_F4;         break;
	case sf::Keyboard::F5:         vkey = VK_F5;         break;
	case sf::Keyboard::F6:         vkey = VK_F6;         break;
	case sf::Keyboard::F7:         vkey = VK_F7;         break;
	case sf::Keyboard::F8:         vkey = VK_F8;         break;
	case sf::Keyboard::F9:         vkey = VK_F9;         break;
	case sf::Keyboard::F10:        vkey = VK_F10;        break;
	case sf::Keyboard::F11:        vkey = VK_F11;        break;
	case sf::Keyboard::F12:        vkey = VK_F12;        break;
	case sf::Keyboard::F13:        vkey = VK_F13;        break;
	case sf::Keyboard::F14:        vkey = VK_F14;        break;
	case sf::Keyboard::F15:        vkey = VK_F15;        break;
	case sf::Keyboard::Pause:      vkey = VK_PAUSE;      break;
	}

	return vkey;
}

void UpdateEventModifiers(uint32_t &eventModifiers) {
	eventModifiers = 0;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::RShift))
		eventModifiers |= EVENTFLAG_SHIFT_DOWN;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt) || sf::Keyboard::isKeyPressed(sf::Keyboard::RAlt))
		eventModifiers |= EVENTFLAG_ALT_DOWN;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
		eventModifiers |= EVENTFLAG_CONTROL_DOWN;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		eventModifiers |= EVENTFLAG_LEFT_MOUSE_BUTTON;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		eventModifiers |= EVENTFLAG_RIGHT_MOUSE_BUTTON;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Middle))
		eventModifiers |= EVENTFLAG_MIDDLE_MOUSE_BUTTON;
}

class BrowserContentProvider : public CefResourceManager::Provider
{
public:
	explicit BrowserContentProvider(const std::shared_ptr<BrowserContent> &content) :
		contentWeakPointer(content)
	{ }

	bool OnRequest(scoped_refptr<CefResourceManager::Request> request)
	{
		auto content = contentWeakPointer.lock();
		if (!content || content->html.empty())
			return false;

		const std::string urlRoot = "http://content";

		auto founded_iter = request->url().find(urlRoot);
		if (founded_iter == std::string::npos)
			return false;

		std::string resourceRelativePath = request->url().erase(0, urlRoot.size());
		CefRefPtr<CefStreamReader> stream;
		CefString contentType;
		if (resourceRelativePath == "/") {
			stream = CefStreamReader::CreateForData(static_cast<void*>(const_cast<char*>(content->html.c_str())), content->html.size());
			contentType = "text/html";
		} else {
			std::ifstream inputStream(content->resourcesPath + resourceRelativePath);
			std::stringstream buffer;
			buffer << inputStream.rdbuf();
			stream = CefStreamReader::CreateForData(static_cast<void*>(const_cast<char*>(buffer.str().c_str())), buffer.str().size());
			contentType = "text/css";
		}
		DCHECK(stream.get());
		request->Continue(new CefStreamResourceHandler(contentType, stream));
		return true;
	}

	std::weak_ptr<BrowserContent> contentWeakPointer;
};

void AddResourceProvider(CefRefPtr<CefResourceManager> resource_manager, std::shared_ptr<BrowserContent> content) {
	if (!CefCurrentlyOn(TID_IO)) {
		// Execute on the browser IO thread.
		CefPostTask(TID_IO, base::Bind(AddResourceProvider, resource_manager, content));
		return;
	}

	resource_manager->AddProvider(new BrowserContentProvider(content), 0, std::string());
	resource_manager->AddDirectoryProvider("http://content", "Resources/Templates", 0, std::string());
}

}  // namespaces

BrowserImpl::BrowserImpl(CefRefPtr<CefBrowser> cefBrowser, CefRefPtr<BrowserHandlerImpl> handler) :
	cefBrowser(cefBrowser),
	cefBrowserHost(cefBrowser->GetHost()),
	handler(handler),
	content(std::make_shared<BrowserContent>())
{
	content->resourcesPath = "Resources/Templates";
	AddResourceProvider(handler->GetResourceManager(), content);
}

BrowserImpl::~BrowserImpl()
{ }

void BrowserImpl::SetContent(const std::string &content) {
	this->content->html = content;
	cefBrowser->GetMainFrame()->LoadURL("http://content");
	cefBrowserHost->SetFocus(true);
}

bool BrowserImpl::OnMouseButtonPressed(sf::Mouse::Button button, uf::vec2i position) {
	UpdateEventModifiers(eventModifiers);

	CefBrowserHost::MouseButtonType cefButtonType{};
	switch (button) {
		case sf::Mouse::Button::Left:
			cefButtonType = CefBrowserHost::MouseButtonType::MBT_LEFT;
			break;
		case sf::Mouse::Button::Right:
			cefButtonType = CefBrowserHost::MouseButtonType::MBT_RIGHT;
			break;
		case sf::Mouse::Button::Middle:
			cefButtonType = CefBrowserHost::MouseButtonType::MBT_MIDDLE;
			break;
		default:
			break;
	}

	CefMouseEvent mouseEvent{};
	mouseEvent.modifiers = eventModifiers;
	mouseEvent.x = position.x;
	mouseEvent.y = position.y;

	cefBrowserHost->SendMouseClickEvent(mouseEvent, cefButtonType, false, 1);
	return true;
}

bool BrowserImpl::OnMouseButtonReleased(sf::Mouse::Button button, uf::vec2i position) {
	UpdateEventModifiers(eventModifiers);

	CefBrowserHost::MouseButtonType cefButtonType{};
	switch (button) {
		case sf::Mouse::Button::Left:
			cefButtonType = CefBrowserHost::MouseButtonType::MBT_LEFT;
			break;
		case sf::Mouse::Button::Right:
			cefButtonType = CefBrowserHost::MouseButtonType::MBT_RIGHT;
			break;
		case sf::Mouse::Button::Middle:
			cefButtonType = CefBrowserHost::MouseButtonType::MBT_MIDDLE;
			break;
		default:
			break;
	}
	
	CefMouseEvent mouseEvent{};
	mouseEvent.modifiers = eventModifiers;
	mouseEvent.x = position.x;
	mouseEvent.y = position.y;

	cefBrowserHost->SendMouseClickEvent(mouseEvent, cefButtonType, true, 1);
	return true;
}

bool BrowserImpl::OnMouseMoved(uf::vec2i position) {
	UpdateEventModifiers(eventModifiers);

	CefMouseEvent mouseEvent{};
	mouseEvent.modifiers = eventModifiers;
	mouseEvent.x = position.x;
	mouseEvent.y = position.y;

	cefBrowserHost->SendMouseMoveEvent(mouseEvent, false);
	return true;
}

bool BrowserImpl::OnMouseLeft() {
	UpdateEventModifiers(eventModifiers);

	CefMouseEvent mouseEvent{};
	mouseEvent.modifiers = eventModifiers;
	cefBrowserHost->SendMouseMoveEvent(mouseEvent, true);
	return true;
}

bool BrowserImpl::OnMouseWheelScrolled(float delta, uf::vec2i position) {
	UpdateEventModifiers(eventModifiers);

	CefMouseEvent mouseEvent{};
	mouseEvent.modifiers = eventModifiers;
	mouseEvent.x = position.x;
	mouseEvent.y = position.y;

	delta *= 120; // delta is adjusted by SFML. TODO: Check on Linux

	cefBrowserHost->SendMouseWheelEvent(mouseEvent, 0, static_cast<int>(delta));
	return true;
}

bool BrowserImpl::OnKeyPressed(sf::Event::KeyEvent keyEvent) {
	UpdateEventModifiers(eventModifiers);

	auto windowsVirtualKey = sfKeycodeToWindowsKeycode(keyEvent.code);
	UINT windowsScanCode = MapVirtualKey(windowsVirtualKey, MAPVK_VK_TO_VSC);

	CefKeyEvent cefKeyEvent{};
	cefKeyEvent.modifiers = eventModifiers;
	cefKeyEvent.type = KEYEVENT_RAWKEYDOWN;
	cefKeyEvent.windows_key_code = windowsVirtualKey;
	cefKeyEvent.native_key_code = (windowsScanCode << 16) | 1;
	cefKeyEvent.is_system_key = keyEvent.system;

	cefBrowserHost->SendKeyEvent(cefKeyEvent);
	return true;
}

bool BrowserImpl::OnKeyReleased(sf::Event::KeyEvent keyEvent) {
	UpdateEventModifiers(eventModifiers);

	auto windowsVirtualKey = sfKeycodeToWindowsKeycode(keyEvent.code);
	UINT windowsScanCode = MapVirtualKey(windowsVirtualKey, MAPVK_VK_TO_VSC);

	CefKeyEvent cefKeyEvent{};
	cefKeyEvent.modifiers = eventModifiers;
	cefKeyEvent.type = KEYEVENT_KEYUP;
	cefKeyEvent.windows_key_code = windowsVirtualKey;
	cefKeyEvent.native_key_code = (windowsScanCode << 16) | 1;
	cefKeyEvent.native_key_code |= 0xC0000000;  // bits 30 and 31 should be always 1 for WM_KEYUP
	cefKeyEvent.is_system_key = keyEvent.system;

	cefBrowserHost->SendKeyEvent(cefKeyEvent);
	return true;
}

bool BrowserImpl::OnTextEntered(uint32_t unicodeChar) {
	UpdateEventModifiers(eventModifiers);

	SHORT vkey_and_modifiers = ::VkKeyScanW(unicodeChar); // use here VkKeyScanExW if a specific HKL is needed
	BYTE vk_code = LOBYTE(vkey_and_modifiers);
	UINT scanCode = MapVirtualKey(vk_code, MAPVK_VK_TO_VSC);
	int native_key_code = (scanCode << 16) | 1;

	CefKeyEvent cefKeyEvent;
	cefKeyEvent.modifiers = eventModifiers;
	cefKeyEvent.type = KEYEVENT_CHAR;
	cefKeyEvent.windows_key_code = unicodeChar;
	cefKeyEvent.native_key_code = native_key_code;
	cefKeyEvent.is_system_key = false;

	cefBrowserHost->SendKeyEvent(cefKeyEvent);
	return true;
}

std::future<void> BrowserImpl::Close() {
	cefBrowserHost->CloseBrowser(true);
	return handler->GetCloseFuture();
}
