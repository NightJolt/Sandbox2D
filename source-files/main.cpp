#include <iostream>

#include "Input.h"
#include "R.h"
#include "FTime.h"
#include "Collider.h"
#include "ParticleSystem.h"

#include "UI/UI.h"
#include "UI/UISprite.h"
#include "UI/UIButton.h"

#include "globals.h"
#include "TileMap/TileMap.h"

using namespace std;
using namespace sf;

int main() {
    ios::sync_with_stdio(false);

    RenderWindow window(VideoMode(INIT_SCREEN_SIZE.x, INIT_SCREEN_SIZE.y), "Sandbox2D");
    View view(INIT_VIEW_ORIGIN, INIT_VIEW_SIZE);

    Vector2f screen_resolution = Vector2f(INIT_SCREEN_SIZE.x, INIT_SCREEN_SIZE.y);
    bool is_window_focused = false;

    Event event;

    R::LoadResources();

    TileMap tile_map;

    Color bg_color = Color(200, 255, 255, 255);

    UIText fps(sf::Vector2f(10, 10));
    fps.settings.stick_to_point = UI::StickPoint::TOP_RIGHT;
    fps.text.setFont(R::fonts[0]);
    fps.text.setCharacterSize(24);
    fps.text.setFillColor(Color::Black);

    while (window.isOpen()) {
        Input::Listen();
        FTime::Recalculate();

        while (window.pollEvent(event)) {
            switch (event.type) {
                case Event::Closed:
                    window.close();

                    break;
                case Event::GainedFocus:
                    is_window_focused = true;

                    break;
                case Event::LostFocus:
                    is_window_focused = false;

                    break;
                case Event::MouseWheelMoved:
                    view.zoom(event.mouseWheel.delta > 0 ? 0.9f : 1.1f);

                    break;
                case Event::Resized:
                    Vector2f ratio = (Vector2f)window.getSize() / screen_resolution;
                    screen_resolution = (Vector2f)window.getSize();
                    view.setSize(view.getSize() * ratio);

                    Vector2f scale_val = Math::Sqrt(ratio * Math::Swap(ratio));
                    fps.Rescale(scale_val);

                    break;
            }
        }


        /* ===== WORLD SPACE VIEW ===== */

        view.move(Input::K2D() * .3f * view.getSize() * Vector2f(1, -1) * FTime::DeltaTime());
        view.move(Input::J2D() * .3f * view.getSize() * FTime::DeltaTime());

        window.setView(view);

        /* ============================= */


        fps.text.setString(to_string(FTime::FPS()));

        Vector2i mouse_position = Mouse::getPosition(window);

        if (is_window_focused && Math::InBounds(mouse_position.x, sf::Vector2i(0, screen_resolution.x)) && Math::InBounds(mouse_position.y, sf::Vector2i(0, screen_resolution.y))) {
            Vector2f world_position = Math::ScreenToWorld(mouse_position, window);
            Vector2i grid_position = Math::WorldToGrid(world_position);

            if (Input::Hold(Mouse::Left)) {
                tile_map.SetTile(grid_position, static_cast<Tile::ID>(rand() % Tile::ID::Size));// Tile::Dirt);
            }

            if (Input::Hold(Mouse::Right)) {
                tile_map.SetTile(grid_position, Tile::Air, true);
            }
        }

        window.clear(bg_color);

        // world space
        window.draw(tile_map);

        // screen space
        window.setView(View(sf::FloatRect(0, 0, screen_resolution.x, screen_resolution.y)));
        window.draw(fps);

        window.display();
    }

    return 0;
}