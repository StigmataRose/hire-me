#include <iostream>
#include <memory>
#include <vector>
#include <functional>

#include <visage/app.h>
#include "visage/windowing.h"
#include "visage/graphics.h"
#include "visage/ui.h"

#include <emscripten.h>
#include <emscripten/html5.h>
#include "spline.h"
#include "button.h"

#include "MyScrollableContent.h"

EM_JS(void, get_canvas_size, (int* width_ptr, int* height_ptr), {
  const canvas = document.getElementById('canvas');
  if (canvas) {
    const dpr = window.devicePixelRatio || 1;
    const displayWidth = canvas.clientWidth;
    const displayHeight = canvas.clientHeight;
    const physicalWidth = Math.floor(displayWidth * dpr);
    const physicalHeight = Math.floor(displayHeight * dpr);
    setValue(width_ptr, physicalWidth, 'i32');
    setValue(height_ptr, physicalHeight, 'i32');
  } else {
    console.error("Canvas element with id 'canvas' not found.");
    setValue(width_ptr, 0, 'i32');
    setValue(height_ptr, 0, 'i32');
  }
});

// *** STEP 1: Inherit from visage::ApplicationEditor ***
class MyApp : public visage::ApplicationEditor,  visage::EventTimer {
public:
    MyApp() {
        std::cout << "MyApp constructor started." << std::endl;
        
        // Add the SplineDeformation as a child of this Frame.
        addChild(&spline_deformation_);

        // Make the spline deformation fill the entire parent frame.
        spline_deformation_.layout().setMargin(0); 
        // The on_draw_.connect(...) line is now removed.
        // We will override the draw() method instead.
        spline_deformation_.layout().setHeight(1000);
        spline_deformation_.layout().setWidth(1000);
        // addChild(&button_);
        // button_.layout().setHeight(100);
        // button_.layout().setWidth(100);
        // To this:
        addChild(&scrollable_frame_);
        scrollable_frame_.layout().setMargin(0);
        scrollable_frame_.layout().setWidth(400);
        scrollable_frame_.layout().setHeight(400);

        std::cout << "MyApp setup complete." << std::endl;
        startTimer(20);
    }

    // This is the CORRECT way to provide drawing logic for a Frame subclass.
    // We override the virtual 'draw' method from the base class.
    void draw(visage::Canvas& canvas) override {

        canvas.setColor(0xff000000);
        
        // Fill the entire area of this frame (which is the whole window).
        canvas.fill(0, 0, width(), height());
    }

    void timerCallback() override {
        // This method is called periodically by the EventTimer.
        // You can implement any periodic logic here if needed.
        // For now, we will just redraw the spline deformation.
        // ++count;
        // std::cout << "called" << std::endl;
        // if (count == 100) {
        //     std::cout << "Hiding spline deformation after 1000 timer callbacks." << std::endl;
        //     // button_.setVisible(false);
        //     // removeChild(&spline_deformation_);
        //     // redraw();
        // }

    }
private:
    // Our child component.
    SplineDeformation spline_deformation_;
    Button button_; // Example button, can be used later.
    int count = 0 ;
    MyScrollableContent scrollable_frame_; // Example scrollable frame, can be used later.

};


// --- Application Entry Point ---
int main() {
    std::cout << "Visage Application Starting for Web..." << std::endl;

    int width = 800;
    int height = 600;
    get_canvas_size(&width, &height);

    // Create our main application object.
    MyApp app;

    // Set the dimensions of our root frame.
    app.setNativeDimensions(width, height);
    
    // *** STEP 4: Create the OS Window and link it to the ApplicationEditor ***
    // The ApplicationEditor manages the UI, but it still needs a native window to live in.
    auto window = visage::createWindow({(float)width}, {(float)height});
    if (!window) {
        std::cerr << "Fatal: Could not create window." << std::endl;
        return 1;
    }
    window->setWindowTitle("Visage with ApplicationEditor");
    
    // This crucial step tells the editor which window to use.
    // It handles renderer initialization and canvas pairing internally.
    app.addToWindow(window.get());

    // Launch the application (shows window and starts the event loop).
    // You can also create a custom game loop here if needed for Emscripten.
    window->show();
    window->runEventLoop(); // For desktop, or use emscripten_set_main_loop for web.

    std::cout << "Visage Application Exited." << std::endl;
    return 0;
}