#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <algorithm> // Required for std::max

#include <visage/app.h>
#include "visage/windowing.h"
#include "visage/graphics.h"
#include "visage/ui.h"

#include <emscripten.h>
#include <emscripten/html5.h>
#include "spline.h"
#include "button.h"

#include "MyScrollableContent.h"
#include "simple_frame.h"

EM_JS(void, get_canvas_size, (int* width_ptr, int* height_ptr), {
  const canvas = document.getElementById('canvas');
  if (canvas) {
    const dpr = window.devicePixelRatio || 1;
    const displayWidth = canvas.clientWidth;
    const displayHeight = canvas.clientHeight;
    // const physicalWidth = Math.floor(displayWidth * dpr);
    // const physicalHeight = Math.floor(displayHeight * dpr);
    const physicalWidth = displayWidth;
    const physicalHeight = displayHeight;
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
        setIgnoresMouseEvents(false, false);
        // Add the SplineDeformation as a child of this Frame.
        addChild(&spline_deformation_);

        spline_deformation_.layout().setMargin(0); 
        int width_ = 800;
        int height_ = 600;
        get_canvas_size(&width_, &height_);
        spline_deformation_.layout().setHeight(height_*1.5f);
        spline_deformation_.layout().setWidth(width_*1.5f);

        int button_size = width_ * 0.10f; // Set button size to 15% of the width.
        previous_button = std::make_unique<Button>(button_size, button_size);
        addChild(*previous_button.get()); // Add the button to the application editor.
        previous_button->layout().setMarginLeft(width_ * 0.15f);
        previous_button->layout().setMarginTop(height_ * 0.72f);
        // Set its width and height
        previous_button->layout().setWidth(button_size);
        previous_button->layout().setHeight(button_size);

        next_button = std::make_unique<ButtonRight>(button_size, button_size);
        addChild(*next_button.get()); // Add the button to the application editor.
        next_button->layout().setMarginLeft((width_ * 0.85f)-button_size);
        next_button->layout().setMarginTop(height_ * 0.72f);
        // Set its width and height
        next_button->layout().setWidth(button_size);
        next_button->layout().setHeight(button_size);
    
        float h_width = width_ * 0.5f;
        float h_height = height_ * 0.5f;
        simple_frame = std::make_unique<MySimpleFrame>(width_, height_);
        addChild(*simple_frame.get());
        simple_frame->set_frames(h_width*1.5f, h_height*1.5f);
        simple_frame->layout().setMarginLeft(h_width*0.25f);
        simple_frame->layout().setMarginRight(h_width*0.25f);
        simple_frame->layout().setMarginTop(h_height*0.25f);
        simple_frame->layout().setMarginBottom(h_height*0.25f);
        simple_frame->layout().setWidth(h_width*1.5f);
        simple_frame->layout().setHeight(h_height*1.5f);

        startTimer(20);
    }

    ~MyApp() {
        std::cout << "MyApp destructor called." << std::endl;
        // The destructor will automatically clean up children.
        removeChild(&spline_deformation_);
        simple_frame = nullptr; // Reset the unique_ptr to ensure it cleans up.
        previous_button = nullptr; // Reset the unique_ptr to ensure it cleans up.
        next_button = nullptr; // Reset the unique_ptr to ensure it cleans up.
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
    int width_ = 800;
    int height_ = 600;
    get_canvas_size(&width_, &height_);
    spline_deformation_.layout().setHeight(height_*1.5f);
    spline_deformation_.layout().setWidth(width_*1.5f);

        // float h_width = width_ * 0.5f;
        // float h_height = height_ * 0.5f;
        // simple_frame = std::make_unique<MySimpleFrame>(width_, height_);
        // // addChild(*simple_frame.get());
        // // simple_frame->set_frames(width_*1.5f, height_*1.5f);
        // simple_frame->layout().setMarginLeft(h_width*0.25f);
        // simple_frame->layout().setMarginRight(h_width*0.25f);
        // simple_frame->layout().setMarginTop(h_height*0.25f);
        // simple_frame->layout().setMarginBottom(h_height*0.25f);
        // simple_frame->layout().setWidth(h_width*1.5f);
        // simple_frame->layout().setHeight(h_height*1.5f);
std::cout << "currently getting " << height_ << " " << width_ << std::endl;
    // float h_width = width * 0.25f;
    // float h_height = height * 0.25f;
    // simple_frame.layout().setMarginLeft(h_width*0.25f);
    // simple_frame.layout().setMarginRight(h_width*0.25f);
    // simple_frame.layout().setMarginTop(h_height*0.25f);
    // simple_frame.layout().setMarginBottom(h_height*0.25f);
    // simple_frame.layout().setWidth(h_width);
    // simple_frame.layout().setHeight(h_height);
    }
    
    void mouseDown(const visage::MouseEvent& e) override {
        int mouse_x = e.position.x;
        int mouse_y = e.position.y;

        int previous_x = previous_button->x();
        int previous_y = previous_button->y();
        int previous_x_end = previous_x + previous_button->width();
        int previous_y_end = previous_y + previous_button->height();
        if(mouse_x > previous_x && mouse_x < previous_x_end &&
           mouse_y > previous_y && mouse_y < previous_y_end) {
        previous_button->setVisible(!previous_button->isVisible());
        }

        int next_x = next_button->x();
        int next_y = next_button->y();
        int next_x_end = next_x + next_button->width();
        int next_y_end = next_y + next_button->height();
        if(mouse_x > next_x && mouse_x < next_x_end &&
           mouse_y > next_y && mouse_y < next_y_end) {
        next_button->setVisible(!next_button->isVisible());
           }
    }
private:
    // Our child component.
    SplineDeformation spline_deformation_;
    std::unique_ptr<Button> previous_button; // Example button, can be used later.
    std::unique_ptr<ButtonRight> next_button; // Example button, can be used later.
    int count = 0 ;
    std::unique_ptr<MySimpleFrame> simple_frame; // Example scrollable frame, can be used later.

};


// --- Application Entry Point ---
int main() {
    std::cout << "Visage Application Starting for Web..." << std::endl;

    int width_ = 800;
    int height_ = 600;
    get_canvas_size(&width_, &height_);

    // Create our main application object.
    MyApp app;

    // Set the dimensions of our root frame.
    app.setNativeDimensions(width_, height_);
    
    // *** STEP 4: Create the OS Window and link it to the ApplicationEditor ***
    // The ApplicationEditor manages the UI, but it still needs a native window to live in.
    auto window = visage::createWindow({(float)width_}, {(float)height_});
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