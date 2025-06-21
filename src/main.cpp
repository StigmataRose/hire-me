#include <iostream>
#include <memory>
#include <vector>
#include <functional>

#include "visage/windowing.h"
#include "visage/graphics.h"
#include "visage/ui.h"

// No global g_mainCanvas anymore

class MyApp {
public:
    // Constructor takes initial window dimensions.
    // These are passed to visage::createWindow, cast to float for Dimension objects.
    MyApp(int initial_width, int initial_height)
        : window_(visage::createWindow({static_cast<float>(initial_width)}, {static_cast<float>(initial_height)}))
    {
        std::cout << "MyApp constructor started." << std::endl;

        // Basic error check for window creation
        if (!window_) {
            std::cerr << "Error: Failed to create window!" << std::endl;
            // In a production app, you might throw an exception or exit here.
            return;
        }
        std::cout << "Window created successfully." << std::endl;

        // Set the window title
        window_->setWindowTitle("Visage Simple Background");

        // *** CRITICAL FIX: Initialize the Renderer BEFORE the Canvas ***
        // The Canvas relies on the rendering backend (BGFX via Renderer) being set up.
        // This was the likely cause of your 'canvas.h Assertion failed' error.
        visage::Renderer::instance().checkInitialization(window_->nativeHandle(), window_->globalDisplay());
        std::cout << "Renderer initialized." << std::endl;

        // Initialize the canvas and pair it to the created window
        mainCanvas_ = std::make_unique<visage::Canvas>(); // Now a member variable
        if (mainCanvas_) {
            // Use clientWidth() and clientHeight() methods to get current dimensions
            mainCanvas_->pairToWindow(window_.get(), window_->clientWidth(), window_->clientHeight());
        } else {
            std::cerr << "Error: Failed to create main canvas!" << std::endl;
            return;
        }
        std::cout << "Canvas paired." << std::endl;

        // Set the main drawing callback for the window
        window_->setDrawCallback([this](double time) {
            // std::cout << "Entering draw callback." << std::endl; // Uncomment for heavy debugging
            
            if (!mainCanvas_ || !window_) { // Use mainCanvas_ here
                // std::cerr << "Draw callback: Canvas or Window not valid!" << std::endl;
                return;
            }

            // Update time for canvas/animations (important for any dynamic elements later)
            mainCanvas_->updateTime(time); // Use mainCanvas_ here

            // --- Draw a solid background color ---
            // Set the color to a distinct blue (RGBA: 0.2, 0.3, 0.8, 1.0)
            // Hex format (0xAARRGGBB): 0xFF3366CC
            mainCanvas_->setColor(0xFF3366CC); // Use mainCanvas_ here
            visage::Brush rainbow = visage::Brush::horizontal(visage::Gradient(0xffff6666, 0xffffff66,
                                                                       0xff66ff66, 0xff66ffff, 0xff6666ff,
                                                                       0xffff66ff, 0xffff6666));
            mainCanvas_->setBrush(rainbow); // Use mainCanvas_ here
            // Fill the entire window with this color using window's current client dimensions
            mainCanvas_->fill(0, 0, window_->clientWidth(), window_->clientHeight()); // Use mainCanvas_ here

            // Submit the canvas for rendering to display the frame
            mainCanvas_->submit(); // Use mainCanvas_ here
        });

        // Removed setMouseEventCallback as it's not directly exposed by visage::Window
        // and is not needed for a simple background. If you need mouse events later,
        // you'll need to use the visage library's intended event handling mechanism
        // (e.g., through Frames or a higher-level App class's event routing).
        std::cout << "MyApp setup complete." << std::endl;
    }

    // Call this to show the window and start the event loop
    void launch() {
        std::cout << "Launching application (showing window and starting event loop)." << std::endl;
        window_->show();       // Show the window
        window_->runEventLoop(); // Start the main event loop (blocks until app closes)
    }

private:
    std::unique_ptr<visage::Window> window_;    // MyApp owns the actual window
    std::unique_ptr<visage::Canvas> mainCanvas_; // Now a private member of MyApp
    bool ran = false;
    // No other members (frames, effects, etc.) are needed for just a background.
};

// --- Application Entry Point ---
int main() {
    std::cout << "Visage Application Starting for Web..." << std::endl;

    // Create an instance of MyApp with desired initial window dimensions
    // These will be passed to the MyApp constructor and then to visage::createWindow.
    MyApp app(800, 600); // Using 400x720 as you provided, but any initial size works.

    // Launch the application (shows window and starts the event loop)
    app.launch();

    std::cout << "Visage Application Exited." << std::endl;
    return 0; // The event loop will block here until the app is closed.
}