#pragma once
#include "OpenGUI/Style2/Selector.h"
#include <functional>
#include <filesystem>
#include <memory>
#include <string_view>
#include <string.h>
#include <unordered_map>
#include <vector>
#include <iostream>
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Core/Types.h"
#include "OpenGUI/Style2/VisualStyleSystem.h"
#include "OpenGUI/VisualElement.h"
#include "OpenGUI/Style2/Parse.h"
#include "OpenGUI/XmlParser/XmlParser.h"
#include "OpenGUI/VisualWindow.h"
#include "OpenGUI/Core/Utilities/ipair.hpp"
#include "OpenGUI/Core/open_string.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Event/KeyEvent.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "efsw/efsw.hpp"

using namespace OGUI;

#define SDL_MAIN_NEEDED
#include <SDL2/SDL_main.h>

#include "OpenGUI/Managers/RenderTextureManager.h"
#include "OpenGUI/Core/AsyncRenderTexture.h"
#if defined(_WIN32) || defined(_WIN64)
#define SDL_MAIN_HANDLED
#ifndef __WIN32__
#define __WIN32__
#endif // __WIN32__
#endif
#include "utils.h"

#ifdef USE_TRACY
	#define TRACY_IMPORTS
	#define TRACY_ENABLE
	#include "tracy/Tracy.hpp"
#endif

class UpdateListener : public efsw::FileWatchListener
{
public:
	std::function<void(efsw::WatchID, const std::string&, const std::string&, efsw::Action, std::string)> handle;

	UpdateListener() {};

	FORCEINLINE void handleFileAction(efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "")
	{
		handle(watchid, dir, filename, action, oldFilename);
	}
};

class Timer
{
public:
	void Reset()
	{
		prev = std::chrono::high_resolution_clock::now();
	}
	float Tick()
	{
		auto& ctx = OGUI::Context::Get();
		std::chrono::time_point now = std::chrono::high_resolution_clock::now();
		using namespace ostr::literal;
		float deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(now - prev).count();
		prev = now;
		return deltaTime;
	}
	std::chrono::time_point<std::chrono::steady_clock> prev = std::chrono::high_resolution_clock::now();
};

class AppWindow : public WindowInterface
{
private:
	int width, height;
public:
	WindowContext* cWnd;
	SDL_Window* window;
	SDL_SysWMinfo wmInfo;
	window::Handle hWnd;
	Timer UpdateTimer;

	FORCEINLINE AppWindow(int width, int height, const char *title)
		:width(width), height(height)
    {
		window = SDL_CreateWindow(title,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height, 0
		);

		SDL_VERSION(&wmInfo.version);
		SDL_GetWindowWMInfo(window, &wmInfo);

#if defined(_WIN32) || defined(_WIN64)
		hWnd = (window::Handle)wmInfo.info.win.window;
		auto &ctx = Context::Get();
        cWnd = &ctx.Create(this);
#endif
		UpdateTimer.Reset();
    }
	void * GetNativeHandle() override
	{
		return window;
	}

	int GetWidth() const override {return width;}
	int GetHeight() const override {return height;}

    virtual ~AppWindow()
    {
		auto& ctx = OGUI::Context::Get();
		ctx.Remove(this);

		SDL_DestroyWindow(window);
    }

	virtual bool Update() override
    {
		const auto deltaTime = UpdateTimer.Tick();
		auto& ctx = OGUI::Context::Get();
		{
			ZoneScopedN("Elements Update");
			ctx.Update(this, deltaTime);
		}
		{
			ZoneScopedN("Prepare Primitives");
			ctx.PreparePrimitives(this);
		}
		{
			ZoneScopedN("Primitives Render");
			ctx.Render(this);
		}
		return true;
    }
};

class CSSWindow : public AppWindow
{
public:
	efsw::FileWatcher fileWatcher;
	UpdateListener listener;
	std::string mainXmlFile;
	std::vector<std::string> allCssFile;
    std::vector<std::string> allXmlFile;
	std::function<void(OGUI::VisualElement*)> onReloadedEvent;

	FORCEINLINE CSSWindow(int width, int height, const char *title, const char *xmlFile, std::function<void(OGUI::VisualElement*)> onReloadedEvent)
        :AppWindow(width, height, title)
    {
		this->onReloadedEvent = std::move(onReloadedEvent);
        LoadResource(xmlFile);
    }

    virtual ~CSSWindow()
    {

    }
private:
	FORCEINLINE void LoadResource(const char *xmlFile)
	{
		using namespace OGUI;
		auto& ctx = Context::Get();
		ParseXmlState xmlState;
		auto asset = LoadXmlFile(xmlFile, xmlState);
		InstantiateXmlState InstantState;
		auto ve = asset->Instantiate(InstantState);

		mainXmlFile = xmlFile;
		allCssFile = xmlState.allCssFile;
		allXmlFile = xmlState.allXmlFile;

		for(auto child : cWnd->GetWindowUI()->_children)
		{
			cWnd->GetWindowUI()->RemoveChild(child);
			VisualElement::DestoryTree(child);
		}
		cWnd->GetWindowUI()->PushChild(ve);
		OnReloaded();

		listener.handle = [this](efsw::WatchID watchid, const std::string& dir, const std::string& filename, efsw::Action action, std::string oldFilename = "")
		{
			auto& ctx = Context::Get();
			switch (action)
			{
				case efsw::Actions::Add:
					std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Added" << std::endl;
					break;
				case efsw::Actions::Delete:
					std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Delete" << std::endl;
					break;
				case efsw::Actions::Modified:
				{
					auto path = std::filesystem::path(dir.substr(0, dir.length() - 1) + filename);
					if (find(allXmlFile.begin(), allXmlFile.end(), path) != allXmlFile.end())
					{
						std::chrono::time_point begin = std::chrono::high_resolution_clock::now();
						ParseXmlState xmlState;
						xmlState.useFileCache = false;
						auto asset = LoadXmlFile(mainXmlFile.c_str(), xmlState);

						if(asset)
						{
							InstantiateXmlState InstantState;
							auto newVe = asset->Instantiate(InstantState);
							if (newVe)
							{
								for(auto child : cWnd->GetWindowUI()->_children)
								{
									cWnd->GetWindowUI()->RemoveChild(child);
									VisualElement::DestoryTree(child);
								}
								cWnd->GetWindowUI()->PushChild(newVe);
								OnReloaded();
								ctx._layoutDirty = true;
							}
						}
						olog::Info(u"xml reload completed, time used: {}"_o.format(std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - begin).count()));
					}
					else if (find(allCssFile.begin(), allCssFile.end(), path) != allCssFile.end())
					{
						std::chrono::time_point begin = std::chrono::high_resolution_clock::now();
						auto asset = ParseCSSFile(path.string());
						if (asset)
						{
							std::vector<VisualElement*> current {cWnd->GetWindowUI()->_children};
							std::vector<VisualElement*> next;
							while (current.size() > 0) 
							{
								for (auto element : current)
								{
									for(auto& styleSheet : element->_styleSheets)
									{
										if(styleSheet->path == path)
										{
											*styleSheet = asset.value();
											styleSheet->Initialize();
											element->_selectorDirty = true;
										}
									}

									next.insert(next.end(), element->_children.begin(), element->_children.end());
								}
								current.clear();
								std::swap(current, next);
							}
							ctx._layoutDirty = true;
							ctx.styleSystem.InvalidateCache();
						}
						olog::Info(u"css reload completed, time used: {}"_o.format(std::chrono::duration_cast<std::chrono::duration<float>>(std::chrono::high_resolution_clock::now() - begin).count()));
					}
					std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Modified" << std::endl;
					break;
				}
				case efsw::Actions::Moved:
					std::cout << "DIR (" << dir << ") FILE (" << filename << ") has event Moved from (" << oldFilename << ")" << std::endl;
					break;
				default:
					std::cout << "Should never happen!" << std::endl;
			}
		};

		for(auto dir : fileWatcher.directories())
			fileWatcher.removeWatch(dir);
		for(auto filePath : allCssFile)
			fileWatcher.addWatch(std::filesystem::path(filePath).remove_filename().string(), &listener, true);
		for(auto filePath : allXmlFile)
			fileWatcher.addWatch(std::filesystem::path(filePath).remove_filename().string(), &listener, true);

		fileWatcher.watch();
	}

    void OnReloaded()
	{
		auto ve = cWnd->GetWindowUI()->_children[0];
		onReloadedEvent(ve);
		ve->_pseudoMask |= PseudoStates::Root;
	}
};

