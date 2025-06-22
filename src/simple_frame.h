#pragma once

#include "visage/ui.h"
#include "visage/graphics.h"
#include "embedded/fonts.h"
#include <string>

#include "button.h"

class MySimpleFrame : public visage::Frame { // Inherit directly from visage::Frame
public:
    MySimpleFrame(int _width = 800, int _height = 600) {
        m_width = _width;
        m_height = _height;
        // setIgnoresMouseEvents(false, false); // Allow mouse events to be processed
        // setFlexLayout(true);
        // layout().setPadding(10);
        // layout().setFlexGap(10);

        // addChild(&previous_button_);
        // addChild(&next_button_);

        // // Positioning for Previous button (bottom-left)
        // // Set a small margin from the left edge
        // previous_button_.layout().setMarginLeft(m_width * 0.05f);
        // previous_button_.layout().setMarginTop(m_height * 0.6f);
        // // Set its width and height
        // previous_button_.layout().setWidth(m_width * 0.15f);
        // previous_button_.layout().setHeight(m_height * 0.07f);


        // // Positioning for Next button (bottom-right)
        // // Use marginLeft with a large value to push it to the right.
        // // This effectively creates a space between the previous button and itself.
        // next_button_.layout().setMarginLeft(m_width * 0.1f); // Adjust this value to control spacing from left button
        // next_button_.layout().setMarginTop(m_height * 0.3f);
        // // Set its width and height
        // next_button_.layout().setWidth(m_width * 0.15f);
        // next_button_.layout().setHeight(m_height * 0.07f);

        // Set the background color to white
        onDraw() = [&](visage::Canvas& canvas) {
            canvas.setColor(0xFFFFFFFF); // White color (ARGB: Alpha, Red, Green, Blue)
            canvas.roundedRectangle(0, 0, width(), height(), 4); // No rounded corners for a simple rectangle
            
            // Draw the text
            visage::Font myFont(16, visage::fonts::Lato_Regular_ttf);
            canvas.setColor(0xFF000000); // Black color for the text
            // You can adjust the justification and position as needed
            // canvas.text(m_welcome_message, myFont, visage::Font::Justification::kCenter, 0, 0, width(), height());
                        // Draw the title "GPU Accelerated Website In C++"
            visage::Font titleFont(static_cast<int>(m_height * 0.05), visage::fonts::Lato_Regular_ttf);
            int fontHeight = static_cast<int>(m_height * 0.05);
            canvas.setColor(0xFF000000); // Black color for the text
            canvas.text("GPU Accelerated Website In C++", titleFont, visage::Font::Justification::kCenter, 0, m_height * 0.1f, width(), fontHeight);

            // // Draw the smaller font description "hello my name is Skyler crank and I am applying to nvidia"
            visage::Font descFont(static_cast<int>(m_height * 0.03), visage::fonts::Lato_Regular_ttf);
            int descfontHeight = static_cast<int>(m_height * 0.03);
            canvas.text("Skyler Crank, I am applying to Nvidia", descFont, visage::Font::Justification::kCenter, 0, m_height * 0.2f, width(), descfontHeight);

            // Draw Job ID in bottom-left
            int idFontHeight = static_cast<int>(m_height * 0.025);
            visage::Font jobIdFont(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("Senior DevOps Engineer - Job ID JR1997172", jobIdFont, visage::Font::Justification::kCenter, 0, m_height * 0.3f, width(), idFontHeight);


        };
    }

    void set_frames(int width_, int height_) {
        layout().setWidth(width_);
        layout().setHeight(height_);
        redraw(); // Request a redraw to apply the new size
    }

    int m_width = 800;
    int m_height = 600;
private:
    std::string m_welcome_message = "Built entirely with C++, this website utilizes a hardware-accelerated GPU to program straight to your browser.";
    Button previous_button_;
    Button next_button_;
};



class MySimpleFrame2 : public visage::Frame { // Inherit directly from visage::Frame
public:
    MySimpleFrame2(int _width = 800, int _height = 600) {
        m_width = _width;
        m_height = _height;
        // setIgnoresMouseEvents(false, false); // Allow mouse events to be processed
        // setFlexLayout(true);
        // layout().setPadding(10);
        // layout().setFlexGap(10);

        // addChild(&previous_button_);
        // addChild(&next_button_);

        // // Positioning for Previous button (bottom-left)
        // // Set a small margin from the left edge
        // previous_button_.layout().setMarginLeft(m_width * 0.05f);
        // previous_button_.layout().setMarginTop(m_height * 0.6f);
        // // Set its width and height
        // previous_button_.layout().setWidth(m_width * 0.15f);
        // previous_button_.layout().setHeight(m_height * 0.07f);


        // // Positioning for Next button (bottom-right)
        // // Use marginLeft with a large value to push it to the right.
        // // This effectively creates a space between the previous button and itself.
        // next_button_.layout().setMarginLeft(m_width * 0.1f); // Adjust this value to control spacing from left button
        // next_button_.layout().setMarginTop(m_height * 0.3f);
        // // Set its width and height
        // next_button_.layout().setWidth(m_width * 0.15f);
        // next_button_.layout().setHeight(m_height * 0.07f);

        // Set the background color to white
        onDraw() = [&](visage::Canvas& canvas) {
            canvas.setColor(0xFFFFFFFF); // White color (ARGB: Alpha, Red, Green, Blue)
            canvas.roundedRectangle(0, 0, width(), height(), 4); // No rounded corners for a simple rectangle
            
            // Draw the text
            visage::Font myFont(16, visage::fonts::Lato_Regular_ttf);
            canvas.setColor(0xFF000000); // Black color for the text
            // You can adjust the justification and position as needed
            // canvas.text(m_welcome_message, myFont, visage::Font::Justification::kCenter, 0, 0, width(), height());
                        // Draw the title "GPU Accelerated Website In C++"
            visage::Font titleFont(static_cast<int>(m_height * 0.05), visage::fonts::Lato_Regular_ttf);
            int fontHeight = static_cast<int>(m_height * 0.05);
            canvas.setColor(0xFF000000); // Black color for the text
            canvas.text("Experience", titleFont, visage::Font::Justification::kCenter, 0, m_height * 0.1f, width(), fontHeight);

            // // Draw the smaller font description "hello my name is Skyler crank and I am applying to nvidia"
            visage::Font descFont(static_cast<int>(m_height * 0.03), visage::fonts::Lato_Regular_ttf);
            int descfontHeight = static_cast<int>(m_height * 0.03);
            canvas.text("10 Years C++ (CMake)", descFont, visage::Font::Justification::kCenter, 0, m_height * 0.2f, width(), descfontHeight);

            // Draw Job ID in bottom-left
            int idFontHeight = static_cast<int>(m_height * 0.03);
            visage::Font jobIdFont(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("3 Years Python", jobIdFont, visage::Font::Justification::kCenter, 0, m_height * 0.3f, width(), idFontHeight);

                        // Draw Job ID in bottom-left
            // int idFontHeight = static_cast<int>(m_height * 0.025);
            visage::Font jobIdFont1(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("7 Years Linux (Ubuntu) (Red-Hat)", jobIdFont1, visage::Font::Justification::kCenter, 0, m_height * 0.4f, width(), idFontHeight);


        };
    }

    void set_frames(int width_, int height_) {
        layout().setWidth(width_);
        layout().setHeight(height_);
        redraw(); // Request a redraw to apply the new size
    }

    int m_width = 800;
    int m_height = 600;
private:
    std::string m_welcome_message = "Built entirely with C++, this website utilizes a hardware-accelerated GPU to program straight to your browser.";
    Button previous_button_;
    Button next_button_;
};


class MySimpleFrame3 : public visage::Frame { // Inherit directly from visage::Frame
public:
    MySimpleFrame3(int _width = 800, int _height = 600) {
        m_width = _width;
        m_height = _height;
        // setIgnoresMouseEvents(false, false); // Allow mouse events to be processed
        // setFlexLayout(true);
        // layout().setPadding(10);
        // layout().setFlexGap(10);

        // addChild(&previous_button_);
        // addChild(&next_button_);

        // // Positioning for Previous button (bottom-left)
        // // Set a small margin from the left edge
        // previous_button_.layout().setMarginLeft(m_width * 0.05f);
        // previous_button_.layout().setMarginTop(m_height * 0.6f);
        // // Set its width and height
        // previous_button_.layout().setWidth(m_width * 0.15f);
        // previous_button_.layout().setHeight(m_height * 0.07f);


        // // Positioning for Next button (bottom-right)
        // // Use marginLeft with a large value to push it to the right.
        // // This effectively creates a space between the previous button and itself.
        // next_button_.layout().setMarginLeft(m_width * 0.1f); // Adjust this value to control spacing from left button
        // next_button_.layout().setMarginTop(m_height * 0.3f);
        // // Set its width and height
        // next_button_.layout().setWidth(m_width * 0.15f);
        // next_button_.layout().setHeight(m_height * 0.07f);

        // Set the background color to white
        onDraw() = [&](visage::Canvas& canvas) {
            canvas.setColor(0xFFFFFFFF); // White color (ARGB: Alpha, Red, Green, Blue)
            canvas.roundedRectangle(0, 0, width(), height(), 4); // No rounded corners for a simple rectangle
            
            // Draw the text
            visage::Font myFont(16, visage::fonts::Lato_Regular_ttf);
            canvas.setColor(0xFF000000); // Black color for the text
            // You can adjust the justification and position as needed
            // canvas.text(m_welcome_message, myFont, visage::Font::Justification::kCenter, 0, 0, width(), height());
                        // Draw the title "GPU Accelerated Website In C++"
            visage::Font titleFont(static_cast<int>(m_height * 0.05), visage::fonts::Lato_Regular_ttf);
            int fontHeight = static_cast<int>(m_height * 0.05);
            canvas.setColor(0xFF000000); // Black color for the text
            canvas.text("Portfolios", titleFont, visage::Font::Justification::kCenter, 0, m_height * 0.1f, width(), fontHeight);

            // // Draw the smaller font description "hello my name is Skyler crank and I am applying to nvidia"
            visage::Font descFont(static_cast<int>(m_height * 0.03), visage::fonts::Lato_Regular_ttf);
            int descfontHeight = static_cast<int>(m_height * 0.03);
            canvas.text("skylercrank.com - project details in 3D", descFont, visage::Font::Justification::kCenter, 0, m_height * 0.2f, width(), descfontHeight);

            // Draw Job ID in bottom-left
            int idFontHeight = static_cast<int>(m_height * 0.03);
            visage::Font jobIdFont(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("skycrank.com - tech specs & code", jobIdFont, visage::Font::Justification::kCenter, 0, m_height * 0.3f, width(), idFontHeight);


        };
    }

    void set_frames(int width_, int height_) {
        layout().setWidth(width_);
        layout().setHeight(height_);
        redraw(); // Request a redraw to apply the new size
    }

    int m_width = 800;
    int m_height = 600;
private:
    std::string m_welcome_message = "Built entirely with C++, this website utilizes a hardware-accelerated GPU to program straight to your browser.";
    Button previous_button_;
    Button next_button_;
};