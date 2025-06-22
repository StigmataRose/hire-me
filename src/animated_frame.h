/* Copyright Vital Audio, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include <visage/app.h>
#include <visage/app.h>
#include "visage/widgets.h"
#include "visage/graphics.h"
#include "visage/ui.h"
#include "embedded/fonts.h"
#include <string>

#include "visage/windowing.h"
#include "visage/graphics.h"
#include "visage/ui.h"
#include <vector>
#include <cmath>
#include <algorithm> // For std::max and std::min

class AnimationLineLeft : public visage::Frame {
public:
  static constexpr int kNumPoints = 3; // Number of base vertices for the triangle
  static constexpr float kDotRadius = 4.0f; // No longer used, but constant can remain.
  static constexpr float TAU = 6.28318530718f; // 2 * PI

  static inline float quickSin1(float phase) {
    phase = 0.5f - phase;
    return phase * (8.0f - 16.0f * fabsf(phase));
  }

  static inline float sin1(float phase) {
    float approx = quickSin1(phase - floorf(phase));
    return approx * (0.776f + 0.224f * fabsf(approx));
  }

  AnimationLineLeft() {
    // If you want this to behave like a clickable button, ensure mouse events are enabled.
    // By default, visage::Frame might enable them, or you might need `setIgnoresMouseEvents(false, false);`
  }

  void resized() override {
    // No specific resized logic needed here as drawing is responsive to width/height
  }

  void draw(visage::Canvas& canvas) override {
    static constexpr int kNumSplineSegments = 50; // More segments for smoother spline curves

    double render_time = canvas.time();
    int render_height = height();
    int render_width = width();

    // --- Animation Parameters for Boosting (Direction Reversed) ---
    float boost_time = render_time * 0.2f;
    float boost_phase = (boost_time - floor(boost_time)) * 1.5f - 0.25f; // Normalized phase for boost

    auto compute_boost = [](float dist) {
        // This function outputs a value from 0.0 to 1.0 (or slightly more)
        return std::max(0.0f, 1.0f - 8.0f * std::abs(dist));
    };

    const float BOOST_INTENSITY_MULTIPLIER = 2.0f; // How much the color brightens


    // --- Button Background ---
    float corner_radius = 6.0f;
    float border_thickness_base = 1.5f;

    // 1. Draw the black background (filled rounded rectangle)
    canvas.setColor(0xFF000000); // Black color (ARGB: Opaque Black)
    canvas.roundedRectangle(0, 0, (float)render_width, (float)render_height, corner_radius);

    // --- Draw the Outer Rounded Border using `canvas.roundedRectangle` (Overall Boost) ---
    visage::Color outerBorderBaseColor = 0xFFC0C0C0; // Silver color
    // For overall boost, we still use boost_phase-0.5f as it's a general brightness.
    // The "direction" only applies to the traveling boost.
    float overall_border_boost = compute_boost(boost_phase - 0.5f); // Use same direction for static boost

    visage::Color boostedOuterBorderColor = outerBorderBaseColor;
    boostedOuterBorderColor.setHdr(1.0f + overall_border_boost * BOOST_INTENSITY_MULTIPLIER);

    canvas.setColor(boostedOuterBorderColor);
    // canvas.setLineWidth(border_thickness_base + overall_border_boost * 1.0f);
    // canvas.roundedRectangle(0, 0, (float)render_width, (float)render_height, corner_radius);


    // --- Animated, Boosted Inner Rounded Border (Spline-driven with REVERSED BOOST) ---
    visage::Color innerBorderBaseColor = 0xFF808080; // Medium Grey color for inner border
    float inner_border_thickness_base = 1.0f; // Slightly thinner for the inner border

    const float inner_scale_factor = 0.80f; // 80% scale
    float center_x_frame = render_width / 2.0f;
    float center_y_frame = render_height / 2.0f;

    auto scaleAndCenterPoint = [&](visage::Point p) {
        float scaled_x = (p.x - center_x_frame) * inner_scale_factor + center_x_frame;
        float scaled_y = (p.y - center_y_frame) * inner_scale_factor + center_y_frame;
        return visage::Point(scaled_x, scaled_y);
    };

    std::vector<visage::Point> inner_border_spline_control_points;
    inner_border_spline_control_points.reserve(8 + 3);

// Define the 8 key points on the perimeter, going counter-clockwise:
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(0, corner_radius)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(0, render_height - corner_radius)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(corner_radius, render_height)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(render_width - corner_radius, render_height)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(render_width, render_height - corner_radius)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(render_width, corner_radius)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(render_width - corner_radius, 0)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(corner_radius, 0)));

    // Add the first three points to close the spline loop
    inner_border_spline_control_points.push_back(inner_border_spline_control_points[0]);
    inner_border_spline_control_points.push_back(inner_border_spline_control_points[1]);
    inner_border_spline_control_points.push_back(inner_border_spline_control_points[2]);

    const int kNumInnerBorderSplineSegments = 8;
    for (int i = 0; i < kNumInnerBorderSplineSegments; ++i) {
        visage::Point p0 = inner_border_spline_control_points[i];
        visage::Point p1 = inner_border_spline_control_points[i + 1];
        visage::Point p2 = inner_border_spline_control_points[i + 2];
        visage::Point p3 = inner_border_spline_control_points[i + 3];

        visage::Point current_spline_point = p1;

        for (int j = 1; j <= kNumSplineSegments; ++j) {
            float t = static_cast<float>(j) / kNumSplineSegments;
            float border_total_t = (static_cast<float>(i) + t) / kNumInnerBorderSplineSegments;

            // REVERSED BOOST DIRECTION: Use 'border_total_t - boost_phase' for opposite
            float current_boost = compute_boost(border_total_t - boost_phase);

            visage::Point next_spline_point = splineInterpolation(p0, p1, p2, p3, t);
            visage::Color boostedInnerBorderColor = innerBorderBaseColor;
            boostedInnerBorderColor.setHdr(1.0f + current_boost * BOOST_INTENSITY_MULTIPLIER);

            canvas.setColor(boostedInnerBorderColor);
            drawThickLine(canvas, current_spline_point, next_spline_point, inner_border_thickness_base + current_boost * 1.0f);

            current_spline_point = next_spline_point;
        }
    }


    // --- Define base triangle properties ---
    float padding = 10.0f;
    float available_width = render_width - 2 * padding;
    float available_height = render_height - 2 * padding;

    float triangle_center_x = render_width * 0.5f;
    float triangle_center_y = render_height * 0.5f;
    float triangle_base_height = available_height * 0.6f;
    float triangle_width = available_width * 0.5f;

    // TRIANGLE POINTING LEFT: Redefine base points
    // P1 (leftmost point - the tip)
    visage::Point base_p1(triangle_center_x - triangle_width / 2.0f, triangle_center_y);
    // P2 (top-right point of the base)
    visage::Point base_p2(triangle_center_x + triangle_width / 2.0f, triangle_center_y - triangle_base_height / 2.0f);
    // P3 (bottom-right point of the base)
    visage::Point base_p3(triangle_center_x + triangle_width / 2.0f, triangle_center_y + triangle_base_height / 2.0f);

    // --- Generate Deformed Triangle Points ---
    std::vector<visage::Point> deformed_triangle_points(kNumPoints);
    float deformation_magnitude = 0.03f * std::min(triangle_width, triangle_base_height);
    float current_deformation_offset = sin(render_time * 2.0f) * deformation_magnitude;
    deformed_triangle_points[0] = base_p1 + visage::Point(current_deformation_offset * cos(render_time * 2.1f), current_deformation_offset * sin(render_time * 1.8f));
    deformed_triangle_points[1] = base_p2 + visage::Point(current_deformation_offset * sin(render_time * 2.3f), current_deformation_offset * cos(render_time * 2.7f));
    deformed_triangle_points[2] = base_p3 + visage::Point(current_deformation_offset * cos(render_time * 1.5f), current_deformation_offset * sin(render_time * 2.5f));

    // --- Prepare points for spline interpolation (wrap around for closed loop) ---
    std::vector<visage::Point> triangle_spline_control_points;
    triangle_spline_control_points.reserve(kNumPoints + 3);
    for(const auto& p : deformed_triangle_points) {
        triangle_spline_control_points.push_back(p);
    }
    triangle_spline_control_points.push_back(deformed_triangle_points[0]);
    triangle_spline_control_points.push_back(deformed_triangle_points[1]);
    triangle_spline_control_points.push_back(deformed_triangle_points[2]);


    // --- Drawing the Spline Triangle (with REVERSED BOOST) ---
    visage::Color triangleColor = 0xff76b900; // NVIDIA green
    float triangleBorderWidth_base = 3.0f;

    for (int i = 0; i < kNumPoints; ++i) {
        visage::Point p0 = triangle_spline_control_points[i];
        visage::Point p1 = triangle_spline_control_points[i + 1];
        visage::Point p2 = triangle_spline_control_points[i + 2];
        visage::Point p3 = triangle_spline_control_points[i + 3];

        visage::Point current_spline_point = p1;

        for (int j = 1; j <= kNumSplineSegments; ++j) {
            float t = static_cast<float>(j) / kNumSplineSegments;
            float segment_t = (static_cast<float>(i) + t) / kNumPoints;

            // REVERSED BOOST DIRECTION: 'segment_t - boost_phase' for opposite
            float current_boost = compute_boost(segment_t - boost_phase);

            visage::Point next_spline_point = splineInterpolation(p0, p1, p2, p3, t);
            visage::Color boostedColor = triangleColor;
            boostedColor.setHdr(1.0f + current_boost * BOOST_INTENSITY_MULTIPLIER);

            canvas.setColor(boostedColor);
            drawThickLine(canvas, current_spline_point, next_spline_point, triangleBorderWidth_base + current_boost * 1.5f);

            current_spline_point = next_spline_point;
        }
    }

    redraw();
  }

private:
    void drawThickLine(visage::Canvas& canvas, visage::Point p1, visage::Point p2, float thickness) const {
        visage::Point delta = p2 - p1;
        float length = sqrt(delta.x * delta.x + delta.y * delta.y);
        if (length < 1e-6f) return;

        visage::Point perp( -delta.y * (1.0f/length), delta.x * (1.0f/length) );
        float half_thickness = thickness / 2.0f;

        visage::Point v1 = p1 + perp * half_thickness;
        visage::Point v2 = p2 + perp * half_thickness;
        visage::Point v3 = p2 - perp * half_thickness;
        visage::Point v4 = p1 - perp * half_thickness;

        canvas.triangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);
        canvas.triangle(v1.x, v1.y, v3.x, v3.y, v4.x, v4.y);
    }

    visage::Point splineInterpolation(visage::Point p0, visage::Point p1, visage::Point p2, visage::Point p3, float t, float tension = 0.0f, float alpha = 0.5f) const {
        auto distance = [](visage::Point a, visage::Point b) {
            return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
        };

        float t01 = pow(distance(p0, p1), alpha);
        float t12 = pow(distance(p1, p2), alpha);
        float t23 = pow(distance(p2, p3), alpha);

        if (t01 < 1e-6f) t01 = 1.0f;
        if (t12 < 1e-6f) t12 = 1.0f;
        if (t23 < 1e-6f) t23 = 1.0f;

        float inv_t01 = 1.0f / t01;
        float inv_t23 = 1.0f / t23;
        float inv_t01_t12 = 1.0f / (t01 + t12);
        float inv_t12_t23 = 1.0f / (t12 + t23);

        visage::Point m1 = (p2 - p1 + (p1 - p0) * inv_t01 * t12 - (p2 - p0) * inv_t01_t12 * t12) * (1.0f - tension);
        visage::Point m2 = (p2 - p1 + (p3 - p2) * inv_t23 * t12 - (p3 - p1) * inv_t12_t23 * t12) * (1.0f - tension);

        visage::Point a = (p1 - p2) * 2.0f + m1 + m2;
        visage::Point b = (p1 - p2) * -3.0f - m1 - m1 - m2;
        visage::Point c = m1;
        visage::Point d = p1;

        return a * t * t * t + b * t * t + c * t + d;
    }
};

class AnimatedLine : public visage::Frame {
public:
  static constexpr int kNumPoints = 3; // Number of base vertices for the triangle
  static constexpr float kDotRadius = 4.0f; // No longer used.
  static constexpr float TAU = 6.28318530718f; // 2 * PI

  static inline float quickSin1(float phase) {
    phase = 0.5f - phase;
    return phase * (8.0f - 16.0f * fabsf(phase));
  }

  static inline float sin1(float phase) {
    float approx = quickSin1(phase - floorf(phase));
    return approx * (0.776f + 0.224f * fabsf(approx));
  }

  AnimatedLine() {
    // Keep this commented if default is interactive, uncomment if you want to explicitly ignore mouse.
    // setIgnoresMouseEvents(false, false);
  }

  void resized() override {
    // No specific resized logic needed here as drawing is responsive to width/height
  }

  void draw(visage::Canvas& canvas) override {
    static constexpr int kNumSplineSegments = 50; // More segments for smoother triangle spline curves

    double render_time = canvas.time();
    int render_height = height();
    int render_width = width();

    // --- Animation Parameters for Boosting ---
    float boost_time = render_time * 0.2f;
    float boost_phase = (boost_time - floor(boost_time)) * 1.5f - 0.25f;

    auto compute_boost = [](float dist) {
        return std::max(0.0f, 1.0f - 8.0f * std::abs(dist));
    };

    const float BOOST_INTENSITY_MULTIPLIER = 2.0f; // Used for how much the color brightens


    // --- Button Background ---
    float corner_radius = 6.0f; // Slightly smaller corner radius for a sleeker look
    float border_thickness_base = 1.5f; // Base thickness for the border lines

    // 1. Draw the black background (filled rounded rectangle)
    canvas.setColor(0xFF000000); // Black color (ARGB: Opaque Black)
    canvas.roundedRectangle(0, 0, (float)render_width, (float)render_height, corner_radius);

    // --- Draw the Outer Rounded Border using `canvas.roundedRectangle` (Overall Boost) ---
    visage::Color outerBorderBaseColor = 0xFFC0C0C0; // Silver color
    float overall_border_boost = compute_boost(boost_phase - 0.5f); // Use a central phase for overall boost

    visage::Color boostedOuterBorderColor = outerBorderBaseColor;
    boostedOuterBorderColor.setHdr(1.0f + overall_border_boost * BOOST_INTENSITY_MULTIPLIER);


    // canvas.setLineWidth(border_thickness_base + overall_border_boost * 1.0f);

    // This is the function you indicated you made for the border
    // canvas.roundedRectangle(0, 0, (float)render_width, (float)render_height, corner_radius);


    // --- Animated, Boosted Inner Rounded Border (Spline-driven) ---
    visage::Color innerBorderBaseColor = 0xFF808080; // Medium Grey color for inner border
    float inner_border_thickness_base = 1.0f; // Slightly thinner for the inner border

    // canvas.setColor(0xFF000000); // Black color (ARGB: Opaque Black)
    // canvas.roundedRectangle(render_width*0.1, render_height*0.1, (float)render_width*0.8, (float)render_height*0.8, corner_radius);

    canvas.setColor(boostedOuterBorderColor);
    // Scaling and centering parameters for the inner border
    const float inner_scale_factor = 0.80f; // 80% scale
    float center_x_frame = render_width / 2.0f;
    float center_y_frame = render_height / 2.0f;

    // Helper lambda for scaling and centering a point
    auto scaleAndCenterPoint = [&](visage::Point p) {
        float scaled_x = (p.x - center_x_frame) * inner_scale_factor + center_x_frame;
        float scaled_y = (p.y - center_y_frame) * inner_scale_factor + center_y_frame;
        return visage::Point(scaled_x, scaled_y);
    };

    std::vector<visage::Point> inner_border_spline_control_points;
    inner_border_spline_control_points.reserve(8 + 3);

    // Define the 8 key points on the perimeter, going clockwise:
    // Scale and center these points
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(corner_radius, 0)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(render_width - corner_radius, 0)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(render_width, corner_radius)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(render_width, render_height - corner_radius)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(render_width - corner_radius, render_height)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(corner_radius, render_height)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(0, render_height - corner_radius)));
    inner_border_spline_control_points.push_back(scaleAndCenterPoint(visage::Point(0, corner_radius)));

    // Add the first three points to close the spline loop
    inner_border_spline_control_points.push_back(inner_border_spline_control_points[0]);
    inner_border_spline_control_points.push_back(inner_border_spline_control_points[1]);
    inner_border_spline_control_points.push_back(inner_border_spline_control_points[2]);

    const int kNumInnerBorderSplineSegments = 8;
    for (int i = 0; i < kNumInnerBorderSplineSegments; ++i) {
        visage::Point p0 = inner_border_spline_control_points[i];
        visage::Point p1 = inner_border_spline_control_points[i + 1];
        visage::Point p2 = inner_border_spline_control_points[i + 2];
        visage::Point p3 = inner_border_spline_control_points[i + 3];

        visage::Point current_spline_point = p1;

        for (int j = 1; j <= kNumSplineSegments; ++j) {
            float t = static_cast<float>(j) / kNumSplineSegments;
            visage::Point next_spline_point = splineInterpolation(p0, p1, p2, p3, t);

            float border_total_t = (static_cast<float>(i) + t) / kNumInnerBorderSplineSegments;
            float current_boost = compute_boost(boost_phase - border_total_t);

            visage::Color boostedInnerBorderColor = innerBorderBaseColor;
            boostedInnerBorderColor.setHdr(1.0f + current_boost * BOOST_INTENSITY_MULTIPLIER);

            canvas.setColor(boostedInnerBorderColor);
            drawThickLine(canvas, current_spline_point, next_spline_point, inner_border_thickness_base + current_boost * 1.0f); // Boost thickness scaled for inner border

            current_spline_point = next_spline_point;
        }
    }


    // --- Define base triangle properties ---
    float padding = 10.0f;
    float available_width = render_width - 2 * padding;
    float available_height = render_height - 2 * padding;

    float triangle_center_x = render_width * 0.5f;
    float triangle_center_y = render_height * 0.5f;
    float triangle_base_height = available_height * 0.6f;
    float triangle_width = available_width * 0.5f;

    visage::Point base_p1(triangle_center_x + triangle_width / 2.0f, triangle_center_y);
    visage::Point base_p2(triangle_center_x - triangle_width / 2.0f, triangle_center_y - triangle_base_height / 2.0f);
    visage::Point base_p3(triangle_center_x - triangle_width / 2.0f, triangle_center_y + triangle_base_height / 2.0f);

    // --- Generate Deformed Triangle Points ---
    std::vector<visage::Point> deformed_triangle_points(kNumPoints);
    float deformation_magnitude = 0.03f * std::min(triangle_width, triangle_base_height);
    float current_deformation_offset = sin(render_time * 2.0f) * deformation_magnitude;
    deformed_triangle_points[0] = base_p1 + visage::Point(current_deformation_offset * cos(render_time * 2.1f), current_deformation_offset * sin(render_time * 1.8f));
    deformed_triangle_points[1] = base_p2 + visage::Point(current_deformation_offset * sin(render_time * 2.3f), current_deformation_offset * cos(render_time * 2.7f));
    deformed_triangle_points[2] = base_p3 + visage::Point(current_deformation_offset * cos(render_time * 1.5f), current_deformation_offset * sin(render_time * 2.5f));

    // --- Prepare points for spline interpolation (wrap around for closed loop) ---
    std::vector<visage::Point> triangle_spline_control_points;
    triangle_spline_control_points.reserve(kNumPoints + 3);
    for(const auto& p : deformed_triangle_points) {
        triangle_spline_control_points.push_back(p);
    }
    triangle_spline_control_points.push_back(deformed_triangle_points[0]);
    triangle_spline_control_points.push_back(deformed_triangle_points[1]);
    triangle_spline_control_points.push_back(deformed_triangle_points[2]);


    // --- Drawing the Spline Triangle ---
    visage::Color triangleColor = 0xff76b900; // NVIDIA green
    float triangleBorderWidth_base = 3.0f;

    for (int i = 0; i < kNumPoints; ++i) {
        visage::Point p0 = triangle_spline_control_points[i];
        visage::Point p1 = triangle_spline_control_points[i + 1];
        visage::Point p2 = triangle_spline_control_points[i + 2];
        visage::Point p3 = triangle_spline_control_points[i + 3];

        visage::Point current_spline_point = p1;

        for (int j = 1; j <= kNumSplineSegments; ++j) {
            float t = static_cast<float>(j) / kNumSplineSegments;
            visage::Point next_spline_point = splineInterpolation(p0, p1, p2, p3, t);

            float segment_t = (static_cast<float>(i) + t) / kNumPoints;
            float current_boost = compute_boost(boost_phase - segment_t);

            visage::Color boostedColor = triangleColor;
            boostedColor.setHdr(1.0f + current_boost * BOOST_INTENSITY_MULTIPLIER);

            canvas.setColor(boostedColor);
            drawThickLine(canvas, current_spline_point, next_spline_point, triangleBorderWidth_base + current_boost * 1.5f);

            current_spline_point = next_spline_point;
        }
    }

    redraw();
  }

private:
    void drawThickLine(visage::Canvas& canvas, visage::Point p1, visage::Point p2, float thickness) const {
        visage::Point delta = p2 - p1;
        float length = sqrt(delta.x * delta.x + delta.y * delta.y);
        if (length < 1e-6f) return;

        visage::Point perp( -delta.y * (1.0f/length), delta.x * (1.0f/length) );
        float half_thickness = thickness / 2.0f;

        visage::Point v1 = p1 + perp * half_thickness;
        visage::Point v2 = p2 + perp * half_thickness;
        visage::Point v3 = p2 - perp * half_thickness;
        visage::Point v4 = p1 - perp * half_thickness;

        // Draws a rectangle by using two triangles to form the thick line
        canvas.triangle(v1.x, v1.y, v2.x, v2.y, v3.x, v3.y);
        canvas.triangle(v1.x, v1.y, v3.x, v3.y, v4.x, v4.y);
    }

    visage::Point splineInterpolation(visage::Point p0, visage::Point p1, visage::Point p2, visage::Point p3, float t, float tension = 0.0f, float alpha = 0.5f) const {
        auto distance = [](visage::Point a, visage::Point b) {
            return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
        };

        float t01 = pow(distance(p0, p1), alpha);
        float t12 = pow(distance(p1, p2), alpha);
        float t23 = pow(distance(p2, p3), alpha);

        if (t01 < 1e-6f) t01 = 1.0f;
        if (t12 < 1e-6f) t12 = 1.0f;
        if (t23 < 1e-6f) t23 = 1.0f;

        float inv_t01 = 1.0f / t01;
        float inv_t23 = 1.0f / t23;
        float inv_t01_t12 = 1.0f / (t01 + t12);
        float inv_t12_t23 = 1.0f / (t12 + t23);

        visage::Point m1 = (p2 - p1 + (p1 - p0) * inv_t01 * t12 - (p2 - p0) * inv_t01_t12 * t12) * (1.0f - tension);
        visage::Point m2 = (p2 - p1 + (p3 - p2) * inv_t23 * t12 - (p3 - p1) * inv_t12_t23 * t12) * (1.0f - tension);

        visage::Point a = (p1 - p2) * 2.0f + m1 + m2;
        visage::Point b = (p1 - p2) * -3.0f - m1 - m1 - m2;
        visage::Point c = m1;
        visage::Point d = p1;

        return a * t * t * t + b * t * t + c * t + d;
    }
};
class AnimatedFrame : public visage::Frame { // Inherit directly from visage::Frame
public:
    AnimatedFrame(int _width = 800, int _height = 600) : animated_line_() {
        m_width = _width;
        m_height = _height;

        bloom_.setBloomSize(40.0f);
        bloom_.setBloomIntensity(1.0f);
        setPostEffect(&bloom_);

        addChild(&animated_line_);
        animated_line_.layout().setMargin(0);

        setPalette(&palette_);
        visage::Brush rainbow = visage::Brush::horizontal(visage::Gradient(0xffff6666, 0xffffff66,
                                                                           0xff66ff66, 0xff66ffff, 0xff6666ff,
                                                                           0xffff66ff, 0xffff6666));
        palette_.setColor(visage::GraphLine::LineColor, rainbow);
        palette_.setValue(visage::GraphLine::LineWidth, 3.0f);
        palette_.setValue(visage::GraphLine::LineColorBoost, 0.8f);

        onDraw() = [&](visage::Canvas& canvas) {
            canvas.setColor(0xff22282d); // Dark background for the animated line
            canvas.roundedRectangle(0, 0, width(), height(), 4); // No rounded corners for a simple rectangle

            // // Draw the title "GPU Accelerated Website In C++"
            // visage::Font titleFont(static_cast<int>(m_height * 0.05), visage::fonts::Lato_Regular_ttf);
            // int fontHeight = static_cast<int>(m_height * 0.05);
            // canvas.setColor(0xFFFFFFFF); // White color for the text
            // canvas.text("GPU Accelerated Website In C++", titleFont, visage::Font::Justification::kCenter, 0, m_height * 0.1f, width(), fontHeight);

            // // Draw the smaller font description "Skyler Crank, I am applying to Nvidia"
            // visage::Font descFont(static_cast<int>(m_height * 0.03), visage::fonts::Lato_Regular_ttf);
            // int descfontHeight = static_cast<int>(m_height * 0.03);
            // canvas.text("Skyler Crank, I am applying to Nvidia", descFont, visage::Font::Justification::kCenter, 0, m_height * 0.2f, width(), descfontHeight);

            // // Draw Job ID in bottom-left
            // int idFontHeight = static_cast<int>(m_height * 0.025);
            // visage::Font jobIdFont(idFontHeight, visage::fonts::Lato_Regular_ttf);
            // canvas.text("Job ID JR1997172", jobIdFont, visage::Font::Justification::kCenter, 0, m_height * 0.5f, width(), idFontHeight);
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
    visage::Palette palette_;
    visage::BloomPostEffect bloom_;
    AnimatedLine animated_line_;
    // Button previous_button_; // Uncomment if you have button.h
    // Button next_button_;     // Uncomment if you have button.h
};


class AnimatedFrameLeft : public visage::Frame { // Inherit directly from visage::Frame
public:
    AnimatedFrameLeft(int _width = 800, int _height = 600) : animated_line_() {
        m_width = _width;
        m_height = _height;

        bloom_.setBloomSize(40.0f);
        bloom_.setBloomIntensity(1.0f);
        setPostEffect(&bloom_);

        addChild(&animated_line_);
        animated_line_.layout().setMargin(0);

        setPalette(&palette_);
        visage::Brush rainbow = visage::Brush::horizontal(visage::Gradient(0xffff6666, 0xffffff66,
                                                                           0xff66ff66, 0xff66ffff, 0xff6666ff,
                                                                           0xffff66ff, 0xffff6666));
        palette_.setColor(visage::GraphLine::LineColor, rainbow);
        palette_.setValue(visage::GraphLine::LineWidth, 3.0f);
        palette_.setValue(visage::GraphLine::LineColorBoost, 0.8f);

        onDraw() = [&](visage::Canvas& canvas) {
            canvas.setColor(0xff22282d); // Dark background for the animated line
            canvas.roundedRectangle(0, 0, width(), height(), 4); // No rounded corners for a simple rectangle

            // // Draw the title "GPU Accelerated Website In C++"
            // visage::Font titleFont(static_cast<int>(m_height * 0.05), visage::fonts::Lato_Regular_ttf);
            // int fontHeight = static_cast<int>(m_height * 0.05);
            // canvas.setColor(0xFFFFFFFF); // White color for the text
            // canvas.text("GPU Accelerated Website In C++", titleFont, visage::Font::Justification::kCenter, 0, m_height * 0.1f, width(), fontHeight);

            // // Draw the smaller font description "Skyler Crank, I am applying to Nvidia"
            // visage::Font descFont(static_cast<int>(m_height * 0.03), visage::fonts::Lato_Regular_ttf);
            // int descfontHeight = static_cast<int>(m_height * 0.03);
            // canvas.text("Skyler Crank, I am applying to Nvidia", descFont, visage::Font::Justification::kCenter, 0, m_height * 0.2f, width(), descfontHeight);

            // // Draw Job ID in bottom-left
            // int idFontHeight = static_cast<int>(m_height * 0.025);
            // visage::Font jobIdFont(idFontHeight, visage::fonts::Lato_Regular_ttf);
            // canvas.text("Job ID JR1997172", jobIdFont, visage::Font::Justification::kCenter, 0, m_height * 0.5f, width(), idFontHeight);
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
    visage::Palette palette_;
    visage::BloomPostEffect bloom_;
    AnimationLineLeft animated_line_;
    // Button previous_button_; // Uncomment if you have button.h
    // Button next_button_;     // Uncomment if you have button.h
};
// No runExample function