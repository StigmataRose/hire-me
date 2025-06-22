#pragma once

#include "visage/ui.h"
#include "visage/graphics.h"
#include "embedded/fonts.h"
#include <string>
class MyScrollableContent : public visage::ScrollableFrame {
public:
    static constexpr int kNumFrames = 3;
    MyScrollableContent() {
        setFlexLayout(true);
        layout().setPadding(10);
        layout().setFlexGap(10);
        layout().setFlexWrap(true);
        layout().setFlexReverseDirection(true);
        layout().setFlexWrapReverse(true);
    uint32_t colors[] = {
        0xFFF1FAEE, // Off-white
        0xFF808080, // Grey
        0x80000000  // Transparent Black (50% opaque)
    };
        const int num_colors = sizeof(colors) / sizeof(uint32_t);

        for (int i = 0; i < kNumFrames; ++i) {
            // It's safer to use the indexed operator since the order is guaranteed
            Frame& frame = frames_[i]; 
            addScrolledChild(&frame);
            frame.layout().setHeight(100);
            frame.layout().setWidth(100);
            frame.layout().setFlexGrow(1.0f);
            frame.layout().setMarginTop(i*100);

            // Directly capture the color value for this specific frame.
            // Using the modulo operator (%) prevents reading past the end of the colors array.
            const uint32_t frame_color = colors[i % num_colors];
            int index = i;
            // Add frame_color to the capture list. It will be copied by value.
            frame.onDraw() = [this, &frame, frame_color, index](visage::Canvas& canvas) {
                int height_ = frame.height(); // Call height()
                int width_ = frame.width();   // Call width()
                canvas.setColor(frame_color);
                int actual_height = int(height_ * 0.9f); // Call height()
                int actual_width = int(width_ *0.5f);   // Call width()
                int x_pad = 0;//width_ * 0.10f;               // Call width()
                int y_pad = 0;//height_ * 0.10f;              // Call height()
                canvas.roundedRectangle(x_pad, y_pad, actual_width, actual_height, 16);
                visage::Font myFont(16, visage::fonts::Lato_Regular_ttf);

                canvas.setColor(0xff000000);
                //canvas.roundedRectangle(0, 0, frame.width(), frame.height(), 16);

                // Corrected call to canvas.text()
                // You need to provide the string, a font, a justification, and then the coordinates.
                canvas.text(welcome, myFont, visage::Font::Justification::kCenter, x_pad, y_pad, actual_width, actual_height);
            };
        }
        setScrollableHeight(1000.0f,0.0f);
        setYPosition(300.0f); // Set initial scroll position
        redraw();
    }

    void set_frames(int width_, int height_) {
        int m_width_ = width_;
        int m_height_ = height_;
        int all_height = 0;
        for (int i = 0; i < kNumFrames; ++i) {
            frames_[i].layout().setHeight(m_height_);
            frames_[i].layout().setWidth(m_width_);
            frames_[i].layout().setMarginTop(i * m_height_);
        }
    }
    // The resized() method is called by the framework when the frame has a size.
    void resized() override {
        // First, let the base class set up the scrollbar's size and position.
        visage::ScrollableFrame::resized();
    }



private:
    bool m_content_created = false;
    Frame frames_[kNumFrames];
    visage::Text text_;

    std::string welcome = "Built entirely with C++, this website utilizes a hardware-accelerated GPU to program straight to your browser.";
};