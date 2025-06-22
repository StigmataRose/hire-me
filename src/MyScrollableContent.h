#pragma once

#include "visage/ui.h"
#include "visage/graphics.h"
#include "embedded/fonts.h"

class MyScrollableContent : public visage::ScrollableFrame {
public:
    static constexpr int kNumFrames = 10;
    MyScrollableContent() {
        setFlexLayout(true);
        layout().setPadding(10);
        layout().setFlexGap(10);
        layout().setFlexWrap(true);
        layout().setFlexReverseDirection(true);
        layout().setFlexWrapReverse(true);

        uint32_t colors[] = {
            0xffe63946, // Red
            0xfff1faee, // Off-white
            0xffa8dadc, // Light Blue
            0xff457b9d, // Darker Blue
            0xffe63946, // Red
            0xfff1faee, // Off-white
            0xffa8dadc, // Light Blue
            0xff457b9d, // Darker Blue
            0xffe63946, // Red
            0xfff1faee, // Off-white
            0xffa8dadc, // Light Blue
            0xff457b9d, // Darker Blue
            0xffe63946, // Red
            0xfff1faee, // Off-white
            0xffa8dadc, // Light Blue
            0xff457b9d, // Darker Blue
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

            // Add frame_color to the capture list. It will be copied by value.
            frame.onDraw() = [this, &frame, frame_color](visage::Canvas& canvas) {
                canvas.setColor(frame_color);
                canvas.roundedRectangle(0, 0, frame.width(), frame.height(), 16);
                visage::Font myFont(16, visage::fonts::Lato_Regular_ttf);

                canvas.setColor(0xffffffff);
                //canvas.roundedRectangle(0, 0, frame.width(), frame.height(), 16);

                // Corrected call to canvas.text()
                // You need to provide the string, a font, a justification, and then the coordinates.
                canvas.text("Frame ", myFont, visage::Font::Justification::kCenter, 0, 0, 50, 50);
            };
        }
        setScrollableHeight(100.0f*kNumFrames,400.0f);
        setYPosition(100.0f); // Set initial scroll position
        redraw();
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
};