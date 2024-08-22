#include <SFML/Graphics.hpp>

#include <RmlUi/Core.h>
#include <RmlUi/Debugger.h>

#include "backend/RmlUi_Backend_SFML_GL2.h"

#include <iostream>
#include <filesystem>

struct ApplicationData {
    bool show_text = true;
    Rml::String animal = "dog";
} my_data;

int main()
{
    sf::RenderWindow win{ sf::VideoMode{ 1200, 800 }, "RmlUi in SFML", sf::Style::Close | sf::Style::Resize };
    win.setVerticalSyncEnabled(true);

    Backend::Initialize(win);

    Rml::SetRenderInterface(Backend::GetRenderInterface());
    Rml::SetSystemInterface(Backend::GetSystemInterface());

    Rml::Initialise();

    auto context = Rml::CreateContext("RmlUi in SFML",
        Rml::Vector2i{ static_cast<int>(win.getSize().x), static_cast<int>(win.getSize().y) });
    if (!context)
    {
        std::cerr << "Could not initialize RmlUI context\n";
        Rml::Shutdown();
        return EXIT_FAILURE;
    }

    const std::filesystem::path rml_path = "hello_world.rml";
    const std::filesystem::path rcss_path = "window.rcss";
    auto rml_lwt = std::filesystem::last_write_time(rml_path);
    auto rcss_lwt = std::filesystem::last_write_time(rcss_path);

    if (!Rml::LoadFontFace("Duran-Medium.ttf"))
    {
        std::cerr << "Could not load font face\n";
        Rml::Shutdown();
        return EXIT_FAILURE;
    }

    if (Rml::DataModelConstructor constructor = context->CreateDataModel("animals"))
    {
        constructor.Bind("show_text", &my_data.show_text);
        constructor.Bind("animal", &my_data.animal);
    }

    auto document = context->LoadDocument("hello_world.rml");
    if (!document)
    {
        std::cerr << "Could not load document\n";
        Rml::Shutdown();
        return EXIT_FAILURE;
    }
    document->Show();

    while (win.isOpen())
    {
        if (rml_lwt != std::filesystem::last_write_time(rml_path) || 
            rcss_lwt != std::filesystem::last_write_time(rcss_path))
        {
            document->Close();
            document = context->LoadDocument(rml_path.string());
            document->ReloadStyleSheet();
            document->Show();

            rml_lwt = std::filesystem::last_write_time(rml_path);
            rcss_lwt = std::filesystem::last_write_time(rcss_path);
        }

        for (sf::Event event{}; win.pollEvent(event);)
        {
            switch (event.type)
            {
            case sf::Event::Resized:
                UpdateWindowDimensions(win, data->render_interface, context);
                break;
            case sf::Event::Closed:
                win.close();
                break;
            default:
                RmlSFML::InputHandler(context, event);
                break;
            }
        }

        context->Update();

        win.clear();
        data->render_interface.BeginFrame();
        context->Render();
        win.display();
    }

    Rml::Shutdown();
    Backend::Shutdown();

    return EXIT_SUCCESS;
}