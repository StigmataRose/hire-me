#pragma once

#include <visage/app.h>
#include "visage/widgets.h"
#include "visage/graphics.h"
#include "visage/ui.h"
#include "embedded/fonts.h"
#include <string>
#include <chrono>
#include "button.h"
#include <iostream>
#include <functional>
#include <chrono>
#include <vector>
#include <cmath>
#include <algorithm> // For std::max


class GreyBorder : public visage::Frame {
public:
    // --- Configuration Constants ---
    static constexpr float BORDER_SCALE = 0.80f; // Border is 80% of the frame's total size.
    static constexpr float BORDER_THICKNESS = 4.0f; // Thickness of the border line.
    const visage::Color BORDER_COLOR = 0xFFC0C0C0;  // Silver color for the border.

    /**
     * @brief Default constructor.
     */
    GreyBorder() {
        // No special initialization needed.
    }

    /**
     * @brief The main drawing method, overridden from visage::Frame.
     * @param canvas The canvas object to draw on.
     */
    void draw(visage::Canvas& canvas) override {
        // --- Get frame properties ---
        int render_height = height();
        int render_width = width();

        // Avoid drawing if the frame is too small.
        if (render_width < 10 || render_height < 10) {
            return;
        }

        // --- Geometry Setup ---
        // Calculate the dimensions and position of the inner rectangle based on BORDER_SCALE.
        float scaled_width = render_width * BORDER_SCALE;
        float scaled_height = render_height * BORDER_SCALE;
        float margin_x = (render_width - scaled_width) / 2.0f;
        float margin_y = (render_height - scaled_height) / 2.0f;

        // Define the four corners of the rectangular border.
        visage::Point p_tl(margin_x, margin_y);                               // Top-Left
        visage::Point p_tr(margin_x + scaled_width, margin_y);                // Top-Right
        visage::Point p_br(margin_x + scaled_width, margin_y + scaled_height); // Bottom-Right
        visage::Point p_bl(margin_x, margin_y + scaled_height);               // Bottom-Left

        // --- Drawing ---
        // Set the static color for the border.
        canvas.setColor(BORDER_COLOR);

        // Draw the four sides of the rectangle.
        drawThickLine(canvas, p_tl, p_tr, BORDER_THICKNESS); // Top
        drawThickLine(canvas, p_tr, p_br, BORDER_THICKNESS); // Right
        drawThickLine(canvas, p_br, p_bl, BORDER_THICKNESS); // Bottom
        drawThickLine(canvas, p_bl, p_tl, BORDER_THICKNESS); // Left
        
        // No redraw() call is needed as this is a static drawing.
    }

private:
    /**
     * @brief Draws a thick line by constructing a quadrilateral (as two triangles).
     * @param canvas The canvas to draw on.
     * @param p1 The starting point of the line.
     * @param p2 The ending point of the line.
     * @param thickness The desired thickness of the line.
     */
    void drawThickLine(visage::Canvas& canvas, visage::Point p1, visage::Point p2, float thickness) const {
        visage::Point delta = p2 - p1;
        float length = sqrt(delta.x * delta.x + delta.y * delta.y);

        // Don't draw if the line has no length.
        if (length < 1e-6f) return;

        // Calculate the perpendicular vector and scale it by half the thickness.
        visage::Point perp(-delta.y / length, delta.x / length);
        float half_thickness = thickness / 2.0f;

        // Calculate the 4 vertices of the thick line's rectangle.
        visage::Point v1 = p1 + perp * half_thickness;
        visage::Point v2 = p2 + perp * half_thickness;
        visage::Point v3 = p2 - perp * half_thickness;
        visage::Point v4 = p1 - perp * half_thickness;

        // Draw the rectangle as two triangles.
        canvas.triangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);
        canvas.triangle(v1.x, v1.y, v3.x, v3.y, v4.x, v4.y);
    }
};


class AnimatedBorder : public visage::Frame {
public:
    // --- Configuration Constants ---
    // You can easily tweak these values to change the border's appearance and behavior.
    static constexpr float BORDER_SCALE = 0.80f;             // Border is 80% of the frame's total size.
    static constexpr float BASE_THICKNESS = 4.0f;              // Base thickness of the border line.
    static constexpr float BOOSTED_THICKNESS_ADDITION = 2.5f;  // How much thickness the boost effect adds.
    static constexpr float BOOST_SPEED_MULTIPLIER = 0.3f;      // Controls the speed of the animation.
    static constexpr float BOOST_INTENSITY_MULTIPLIER = 2.0f;  // How much brighter the boosted section gets.
    const visage::Color BASE_COLOR = 0xFFC0C0C0;             // Silver color for the border.

    /**
     * @brief Default constructor.
     */
    AnimatedBorder() {
        // Initialization is handled within the draw method.
    }

    /**
     * @brief The main drawing method, overridden from visage::Frame.
     * @param canvas The canvas object to draw on.
     */
    void draw(visage::Canvas& canvas) override {
        // --- Get frame and time properties ---
        double render_time = canvas.time();
        int render_height = height();
        int render_width = width();

        // Avoid drawing if the frame is too small to prevent visual glitches.
        if (render_width < 10 || render_height < 10) {
            return;
        }

        // --- Animation Parameters for TWO boosts ---
        float boost_time = render_time * BOOST_SPEED_MULTIPLIER;
        // Phase 1 moves forward (0 to 1)
        float boost_phase_1 = boost_time - floor(boost_time);
        // Phase 2 moves backward (1 to 0)
        float boost_phase_2 = 1.0f - boost_phase_1;


        // --- Geometry Setup ---
        float scaled_width = render_width * BORDER_SCALE;
        float scaled_height = render_height * BORDER_SCALE;
        float margin_x = (render_width - scaled_width) / 2.0f;
        float margin_y = (render_height - scaled_height) / 2.0f;

        visage::Point p_tl(margin_x, margin_y);                               // Top-Left
        visage::Point p_tr(margin_x + scaled_width, margin_y);                // Top-Right
        visage::Point p_br(margin_x + scaled_width, margin_y + scaled_height); // Bottom-Right
        visage::Point p_bl(margin_x, margin_y + scaled_height);               // Bottom-Left

        // --- Perimeter Calculation for Animation ---
        float perimeter = (2.0f * scaled_width) + (2.0f * scaled_height);
        if (perimeter < 1.0f) {
            return; // Avoid division by zero on very small frames.
        }

        float top_prop = scaled_width / perimeter;
        float right_prop = scaled_height / perimeter;
        float bottom_prop = scaled_width / perimeter;
        // The left property is the remainder.
        
        // --- Set animation start point to top-center ---
        // The proportional distance to the middle of the top edge.
        float start_offset = top_prop / 2.0f;

        // --- Drawing Loop ---
        visage::Point last_point = p_tl;
        const int kNumSegments = 200; // More segments = smoother animation.

        for (int i = 0; i <= kNumSegments; ++i) { // Use <= to ensure the loop closes
            float t = static_cast<float>(i) / kNumSegments;
            visage::Point current_point;

            // Determine which edge the current point is on.
            if (t <= top_prop) {
                float local_t = t / top_prop;
                current_point = p_tl + (p_tr - p_tl) * local_t;
            } else if (t <= top_prop + right_prop) {
                float local_t = (t - top_prop) / right_prop;
                current_point = p_tr + (p_br - p_tr) * local_t;
            } else if (t <= top_prop + right_prop + bottom_prop) {
                float local_t = (t - (top_prop + right_prop)) / bottom_prop;
                current_point = p_br + (p_bl - p_br) * local_t;
            } else {
                float local_t = (t - (top_prop + right_prop + bottom_prop)) / (1.0f - (top_prop + right_prop + bottom_prop));
                current_point = p_bl + (p_tl - p_bl) * local_t;
            }
            
            // For the very first point, just set last_point and continue.
            if (i == 0) {
                last_point = current_point;
                continue;
            }

            // --- Apply COMBINED Boost Effect ---
            // Calculate boost from the first path (clockwise).
            float boost1 = compute_boost(t - fmod(boost_phase_1 + start_offset, 1.0f));
            // Calculate boost from the second path (counter-clockwise).
            float boost2 = compute_boost(t - fmod(boost_phase_2 + start_offset, 1.0f));
            // The final boost is the maximum of the two.
            float current_boost = std::max(boost1, boost2);

            visage::Color boostedColor = BASE_COLOR;
            boostedColor.setHdr(1.0f + current_boost * BOOST_INTENSITY_MULTIPLIER);
            canvas.setColor(boostedColor);

            float current_thickness = BASE_THICKNESS + current_boost * BOOSTED_THICKNESS_ADDITION;
            drawThickLine(canvas, last_point, current_point, current_thickness);

            last_point = current_point;
        }

        redraw();
    }

private:
    /**
     * @brief Computes the boost amount based on distance from the peak.
     * @param dist The normalized distance from the peak of the boost.
     * @return A value from 0.0 to 1.0 representing the boost intensity.
     */
    static float compute_boost(float dist) {
        // Wrap the distance to the range [-0.5, 0.5] to handle the loop correctly.
        dist = dist - floorf(dist);
        if (dist > 0.5f) {
            dist -= 1.0f;
        }
        // This creates a triangle wave that peaks at dist=0 and has a configurable width.
        // A larger multiplier (e.g., 20.0f) makes the boost shorter and more focused.
        // A multiplier of 20.0f gives the boost a total "width" of 0.1 on the 0-1 scale.
        return std::max(0.0f, 1.0f - std::abs(dist) * 20.0f);
    }

    /**
     * @brief Draws a thick line by constructing a quadrilateral (as two triangles).
     * @param canvas The canvas to draw on.
     * @param p1 The starting point of the line.
     * @param p2 The ending point of the line.
     * @param thickness The desired thickness of the line.
     */
    void drawThickLine(visage::Canvas& canvas, visage::Point p1, visage::Point p2, float thickness) const {
        visage::Point delta = p2 - p1;
        float length = sqrt(delta.x * delta.x + delta.y * delta.y);

        if (length < 1e-6f) return;

        visage::Point perp(-delta.y / length, delta.x / length);
        float half_thickness = thickness / 2.0f;

        visage::Point v1 = p1 + perp * half_thickness;
        visage::Point v2 = p2 + perp * half_thickness;
        visage::Point v3 = p2 - perp * half_thickness;
        visage::Point v4 = p1 - perp * half_thickness;

        canvas.triangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);
        canvas.triangle(v1.x, v1.y, v3.x, v3.y, v4.x, v4.y);
    }
};


class emptyframe : public visage::Frame { // Inherit directly from visage::Frame
public:
    emptyframe(int _width = 800, int _height = 600) {
        m_width = _width;
        m_height = _height;

        onDraw() = [&](visage::Canvas& canvas) {


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

};

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
            //canvas.setColor(0xFFFFFFFF); // White color (ARGB: Alpha, Red, Green, Blue)
            canvas.setColor(0x80000000);
            canvas.roundedRectangle(0, 0, width(), height(), 4); // No rounded corners for a simple rectangle
            
            // Draw the text
            visage::Font myFont(16, visage::fonts::Lato_Regular_ttf);
            canvas.setColor(0xFFFFFFFF); // Black color for the text
            // You can adjust the justification and position as needed
            // canvas.text(m_welcome_message, myFont, visage::Font::Justification::kCenter, 0, 0, width(), height());
                        // Draw the title "GPU Accelerated Website In C++"
            visage::Font titleFont(static_cast<int>(m_height * 0.05), visage::fonts::Lato_Regular_ttf);
            int fontHeight = static_cast<int>(m_height * 0.05);
            canvas.setColor(0xFFFFFFFF); // Black color for the text
            canvas.text("Welcome!", titleFont, visage::Font::Justification::kCenter, 0, m_height * 0.1f, width(), fontHeight);

            // // Draw the smaller font description "hello my name is Skyler crank and I am applying to nvidia"
            visage::Font descFont(static_cast<int>(m_height * 0.03), visage::fonts::Lato_Regular_ttf);
            int descfontHeight = static_cast<int>(m_height * 0.03);
            canvas.text("What you are seeing is a Hardware Accelerated Website In C++.", descFont, visage::Font::Justification::kCenter, 0, m_height * 0.2f, width(), descfontHeight);

            // Draw Job ID in bottom-left
            int idFontHeight = static_cast<int>(m_height * 0.025);
            visage::Font jobIdFont(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("Programmed directly from your GPU to your screen.", jobIdFont, visage::Font::Justification::kCenter, 0, m_height * 0.3f, width(), idFontHeight);

            visage::Font jobIdFont2(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("Page 1 / 4", jobIdFont2, visage::Font::Justification::kCenter, 0, m_height * 0.7f, width(), idFontHeight);


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

class MySimpleFrame1 : public visage::Frame { // Inherit directly from visage::Frame
public:
    MySimpleFrame1(int _width = 800, int _height = 600) {
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
            canvas.setColor(0x80000000); // White color (ARGB: Alpha, Red, Green, Blue)
            canvas.roundedRectangle(0, 0, width(), height(), 4); // No rounded corners for a simple rectangle
            
            // Draw the text
            visage::Font myFont(16, visage::fonts::Lato_Regular_ttf);
            canvas.setColor(0xFFFFFFFF); // Black color for the text
            // You can adjust the justification and position as needed
            // canvas.text(m_welcome_message, myFont, visage::Font::Justification::kCenter, 0, 0, width(), height());
                        // Draw the title "GPU Accelerated Website In C++"
            visage::Font titleFont(static_cast<int>(m_height * 0.05), visage::fonts::Lato_Regular_ttf);
            int fontHeight = static_cast<int>(m_height * 0.05);
            canvas.setColor(0xFFFFFFFF); // Black color for the text
            canvas.text("My name is Skyler Crank", titleFont, visage::Font::Justification::kCenter, 0, m_height * 0.1f, width(), fontHeight);

            // // Draw the smaller font description "hello my name is Skyler crank and I am applying to nvidia"
            visage::Font descFont(static_cast<int>(m_height * 0.03), visage::fonts::Lato_Regular_ttf);
            int descfontHeight = static_cast<int>(m_height * 0.03);
            canvas.text("I am applying for Senior DevOps Engineer", descFont, visage::Font::Justification::kCenter, 0, m_height * 0.2f, width(), descfontHeight);

            // Draw Job ID in bottom-left
            int idFontHeight = static_cast<int>(m_height * 0.025);
            visage::Font jobIdFont(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("Job ID JR1997172", jobIdFont, visage::Font::Justification::kCenter, 0, m_height * 0.3f, width(), idFontHeight);

            visage::Font jobIdFont2(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("Page 2 / 4", jobIdFont2, visage::Font::Justification::kCenter, 0, m_height * 0.7f, width(), idFontHeight);


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
            //canvas.setColor(0xFFFFFFFF); // White color (ARGB: Alpha, Red, Green, Blue)
            canvas.setColor(0x80000000);
            canvas.roundedRectangle(0, 0, width(), height(), 4); // No rounded corners for a simple rectangle
            
            // Draw the text
            visage::Font myFont(16, visage::fonts::Lato_Regular_ttf);
            canvas.setColor(0xFFFFFFFF); // Black color for the text
            // You can adjust the justification and position as needed
            // canvas.text(m_welcome_message, myFont, visage::Font::Justification::kCenter, 0, 0, width(), height());
                        // Draw the title "GPU Accelerated Website In C++"
            visage::Font titleFont(static_cast<int>(m_height * 0.05), visage::fonts::Lato_Regular_ttf);
            int fontHeight = static_cast<int>(m_height * 0.05);
            canvas.setColor(0xFFFFFFFF); // Black color for the text
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

            visage::Font jobIdFont2(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("Page 3 / 4", jobIdFont2, visage::Font::Justification::kCenter, 0, m_height * 0.7f, width(), idFontHeight);


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
            canvas.setColor(0x80000000); // White color (ARGB: Alpha, Red, Green, Blue)
            canvas.roundedRectangle(0, 0, width(), height(), 4); // No rounded corners for a simple rectangle
            
            // Draw the text
            visage::Font myFont(16, visage::fonts::Lato_Regular_ttf);
            canvas.setColor(0xFFFFFFFF); // Black color for the text
            // You can adjust the justification and position as needed
            // canvas.text(m_welcome_message, myFont, visage::Font::Justification::kCenter, 0, 0, width(), height());
                        // Draw the title "GPU Accelerated Website In C++"
            visage::Font titleFont(static_cast<int>(m_height * 0.05), visage::fonts::Lato_Regular_ttf);
            int fontHeight = static_cast<int>(m_height * 0.05);
            canvas.setColor(0xFFFFFFFF); // Black color for the text
            canvas.text("Thank you!", titleFont, visage::Font::Justification::kCenter, 0, m_height * 0.1f, width(), fontHeight);

            // // // Draw the smaller font description "hello my name is Skyler crank and I am applying to nvidia"
            visage::Font descFont(static_cast<int>(m_height * 0.03), visage::fonts::Lato_Regular_ttf);
            int descfontHeight = static_cast<int>(m_height * 0.03);
            canvas.text("If you want to see more of my work, check out", descFont, visage::Font::Justification::kCenter, 0, m_height * 0.2f, width(), descfontHeight);

            // // Draw Job ID in bottom-left
            int idFontHeight = static_cast<int>(m_height * 0.03);
            visage::Font jobIdFont(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("www.SkylerCrank.com", jobIdFont, visage::Font::Justification::kCenter, 0, m_height * 0.3f, width(), idFontHeight);

            visage::Font jobIdFont2(idFontHeight, visage::fonts::Lato_Regular_ttf);
            canvas.text("Page 4 / 4", jobIdFont2, visage::Font::Justification::kCenter, 0, m_height * 0.7f, width(), idFontHeight);

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