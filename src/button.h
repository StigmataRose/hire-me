#pragma once
#include <visage/app.h>
#include "visage/windowing.h"
#include "visage/graphics.h"
#include "visage/ui.h"
#include <vector>
#include <cmath>
#include <iostream> // For console output to demonstrate event handling
#include "embedded/shaders.h"
#include "animated_frame.h"

class Button : public visage::Frame {
public:
    Button(int width_ = 100, int height_ = 100) : m_width(width_), m_height(height_) {
        // Set ignoresMouseEvents to false to receive mouse events.
        // The second parameter 'false' means mouse events will not be passed to children if this frame consumes them.
        setIgnoresMouseEvents(false, false);
        animated = std::make_unique<AnimatedFrameLeft>();
        addChild(*animated.get());
        animated->layout().setMargin(0); // Set margin to 0 for the animated frame
        animated->layout().setWidth(width_); // Set a default width for the animated frame
        animated->layout().setHeight(height_); // Set a default height for the animated frame
        bloom_.setBloomSize(0.0f);
        setPostEffect(&bloom_);

    }

    void set_bloom(float size) {
        bloom_.setBloomSize(size);
    }
    void draw(visage::Canvas& canvas) override {
        // Example: Change color based on mouse state
        // if (is_mouse_down_) {
        //     canvas.setColor(0xff0000ff); // Blue when pressed
        // } else if (is_mouse_over_) {
        //     canvas.setColor(0xff00ff00); // Green when hovered
        // } else {
        //     canvas.setColor(0xffff0000); // Red normally
        // }
        // canvas.roundedRectangle(0, 0, width(), height(), 10.0f); // Draw rounded rectangle
        // redraw();
    }

    // Override mouseDown to handle mouse press events
    void mouseDown(const visage::MouseEvent& e) override {
        if (e.isLeftButton()) {
            // Corrected: Access x and y directly from e.position
            std::cout << "Button Mouse Down at (" << e.position.x << ", " << e.position.y << ")" << std::endl;
            is_mouse_down_ = true;
            redraw(); // Request a redraw to update the button's appearance
        }
        // Call the base class implementation if you want to allow further processing
        visage::Frame::mouseDown(e);
    }

    // Override mouseUp to handle mouse release events
    void mouseUp(const visage::MouseEvent& e) override {
        // When mouseUp is called, e.isLeftButton() will be false if the button was released.
        // The check `&& is_mouse_down_` is still important to ensure it was previously pressed on this button.
        if (!e.isLeftButton() && is_mouse_down_) {
            // Corrected: Access x and y directly from e.position
            std::cout << "Button Mouse Up at (" << e.position.x << ", " << e.position.y << ")" << std::endl;
            is_mouse_down_ = false;
            redraw(); // Request a redraw to update the button's appearance
        }
        // Call the base class implementation if you want to allow further processing
        visage::Frame::mouseUp(e);
    }

    // Optional: Override mouseEnter and mouseExit for hover effects
    void mouseEnter(const visage::MouseEvent& e) override {
        std::cout << "Mouse Entered Button" << std::endl;
        is_mouse_over_ = true;
        redraw();
        visage::Frame::mouseEnter(e);
    }

    void mouseExit(const visage::MouseEvent& e) override {
        std::cout << "Mouse Exited Button" << std::endl;
        is_mouse_over_ = false;
        is_mouse_down_ = false; // Reset mouse down state if mouse exits while still down
        redraw();
        visage::Frame::mouseExit(e);
    }

        bool is_mouse_down_ = false;
    bool is_mouse_over_ = false;

    int m_width = 100; // Default width
    int m_height = 100; // Default height
private:
std::unique_ptr<AnimatedFrameLeft> animated;
visage::BloomPostEffect bloom_;
};


class ButtonRight : public visage::Frame {
public:
    ButtonRight(int width_ = 100, int height_ = 100) : m_width(width_), m_height(height_) {
        // Set ignoresMouseEvents to false to receive mouse events.
        // The second parameter 'false' means mouse events will not be passed to children if this frame consumes them.
        setIgnoresMouseEvents(false, false);
        animated = std::make_unique<AnimatedFrame>();
        addChild(*animated.get());
        animated->layout().setMargin(0); // Set margin to 0 for the animated frame
        animated->layout().setWidth(width_); // Set a default width for the animated frame
        animated->layout().setHeight(height_); // Set a default height for the animated frame

        bloom_.setBloomSize(0.0f);
        setPostEffect(&bloom_);

    }

    void set_bloom(float size) {
        bloom_.setBloomSize(size);
    }
    void draw(visage::Canvas& canvas) override {
        // Example: Change color based on mouse state
        // if (is_mouse_down_) {
        //     canvas.setColor(0xff0000ff); // Blue when pressed
        // } else if (is_mouse_over_) {
        //     canvas.setColor(0xff00ff00); // Green when hovered
        // } else {
        //     canvas.setColor(0xffff0000); // Red normally
        // }
        // canvas.roundedRectangle(0, 0, width(), height(), 10.0f); // Draw rounded rectangle
        // redraw();
    }

    // Override mouseDown to handle mouse press events
    void mouseDown(const visage::MouseEvent& e) override {
        if (e.isLeftButton()) {
            // Corrected: Access x and y directly from e.position
            std::cout << "Button Mouse Down at (" << e.position.x << ", " << e.position.y << ")" << std::endl;
            is_mouse_down_ = true;
            redraw(); // Request a redraw to update the button's appearance
        }
        // Call the base class implementation if you want to allow further processing
        visage::Frame::mouseDown(e);
    }

    // Override mouseUp to handle mouse release events
    void mouseUp(const visage::MouseEvent& e) override {
        // When mouseUp is called, e.isLeftButton() will be false if the button was released.
        // The check `&& is_mouse_down_` is still important to ensure it was previously pressed on this button.
        if (!e.isLeftButton() && is_mouse_down_) {
            // Corrected: Access x and y directly from e.position
            std::cout << "Button Mouse Up at (" << e.position.x << ", " << e.position.y << ")" << std::endl;
            is_mouse_down_ = false;
            redraw(); // Request a redraw to update the button's appearance
        }
        // Call the base class implementation if you want to allow further processing
        visage::Frame::mouseUp(e);
    }

    // Optional: Override mouseEnter and mouseExit for hover effects
    void mouseEnter(const visage::MouseEvent& e) override {
        std::cout << "Mouse Entered Button" << std::endl;
        is_mouse_over_ = true;
        redraw();
        visage::Frame::mouseEnter(e);
    }

    void mouseExit(const visage::MouseEvent& e) override {
        std::cout << "Mouse Exited Button" << std::endl;
        is_mouse_over_ = false;
        is_mouse_down_ = false; // Reset mouse down state if mouse exits while still down
        redraw();
        visage::Frame::mouseExit(e);
    }

        bool is_mouse_down_ = false;
    bool is_mouse_over_ = false;

    int m_width = 100; // Default width
    int m_height = 100; // Default height
private:
std::unique_ptr<AnimatedFrame> animated;
visage::BloomPostEffect bloom_;
};