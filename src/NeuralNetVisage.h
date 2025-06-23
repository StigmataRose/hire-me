#pragma once

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

#include <vector>
#include <cmath>
#include <algorithm> // For std::min/max
#include <cstdlib>   // For rand() and srand()
#include <ctime>     // For time() to seed srand()
// NOTE: Please call srand(time(NULL)); once at the beginning of your main() function.
inline float random_float(float min, float max) {
    if (min >= max) return min;
    return min + (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) * (max - min);
}

inline float point_distance(visage::Point p1, visage::Point p2) {
    float dx = p1.x - p2.x;
    float dy = p1.y - p2.y;
    return std::sqrt(dx * dx + dy * dy);
}


// A simple structure to hold our animated points
struct Node {
    visage::Point pos;
    visage::Point vel;
    float unique_offset;
};


/**
 * @class SimplifiedWebFrame
 * @brief A performant frame that draws an animated web of interconnected points.
 */
class SimplifiedWebFrame : public visage::Frame {
public:
    // --- CONFIGURATION ---
    static constexpr int kNumPoints = 70;
    static constexpr float kMaxSpeed = 25.0f;
    static constexpr float kConnectionDist = 150.0f;

    SimplifiedWebFrame() : last_time_(0.0) {
        setIgnoresMouseEvents(true, false);
    }

    void resized() override {
        if (points_.empty() && width() > 0 && height() > 0) {
            initPoints();
        }
    }

    void draw(visage::Canvas& canvas) override {
        if (points_.empty()) {
            redraw();
            return;
        }

        // --- Calculate time delta manually ---
        double current_time = canvas.time();
        if (last_time_ == 0.0) { // First frame, prevent huge dt
             last_time_ = current_time;
        }
        float dt = static_cast<float>(current_time - last_time_);
        last_time_ = current_time;
        
        // --- Update and Draw ---
        updatePoints(dt);
        drawConnections(canvas);
        drawPoints(canvas);
        
        // THIS IS THE FIX: By placing redraw() here, the animating frame itself
        // tells the application it needs to be rendered again. This creates a
        // controlled animation loop tied to the browser's refresh rate.
        redraw();
    }

private:
    std::vector<Node> points_;
    double last_time_;

    void initPoints() {
        points_.clear();
        for (int i = 0; i < kNumPoints; ++i) {
            Node p;
            p.pos = visage::Point(random_float(0, width()), random_float(0, height()));
            
            float angle = random_float(0, 6.283f);
            float speed = random_float(kMaxSpeed * 0.5f, kMaxSpeed);
            p.vel = visage::Point(cos(angle) * speed, sin(angle) * speed);

            p.unique_offset = random_float(0, 100.0f);
            points_.push_back(p);
        }
    }

    void updatePoints(float dt) {
        // If no time has passed, do nothing.
        if (dt <= 0) return;
        
        // If the delta is too large (e.g. after pausing), clamp it to prevent jumps.
        if (dt > 0.1f) {
            dt = 1.0f / 60.0f;
        }
        
        for (auto& p : points_) {
            p.pos = p.pos + p.vel * dt;

            if (p.pos.x < 0) { p.pos.x = 0; p.vel.x *= -1; }
            if (p.pos.x > width()) { p.pos.x = width(); p.vel.x *= -1; }
            if (p.pos.y < 0) { p.pos.y = 0; p.vel.y *= -1; }
            if (p.pos.y > height()) { p.pos.y = height(); p.vel.y *= -1; }
        }
    }

    void drawThickLine(visage::Canvas& canvas, visage::Point p1, visage::Point p2, float thickness) const {
        visage::Point delta = p2 - p1;
        float length = std::sqrt(delta.x * delta.x + delta.y * delta.y);
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

    void drawConnections(visage::Canvas& canvas) const {
        visage::Color line_color = 0xff76b900;

        for (size_t i = 0; i < points_.size(); ++i) {
            for (size_t j = i + 1; j < points_.size(); ++j) {
                const auto& p1 = points_[i];
                const auto& p2 = points_[j];

                float dist = point_distance(p1.pos, p2.pos);

                if (dist < kConnectionDist) {
                    float alpha = (1.0f - dist / kConnectionDist);
                    
                    line_color.setAlpha(static_cast<uint8_t>(alpha * 100));
                    canvas.setColor(line_color);
                    
                    drawThickLine(canvas, p1.pos, p2.pos, 1.0f);
                }
            }
        }
    }
    
    void drawPoints(visage::Canvas& canvas) const {
        visage::Color point_color = 0xff76b900;
        double current_time = canvas.time();

        for (const auto& p : points_) {
            float pulse = sin(current_time * 2.0f + p.unique_offset);
            pulse = (pulse + 1.0f) * 0.5f;
            pulse = pow(pulse, 4.0f);
            
            point_color.setHdr(1.0f + pulse * 1.5f);
            point_color.setAlpha(255);

            canvas.setColor(point_color);
            canvas.circle(p.pos.x, p.pos.y, 2.0f);
        }
    }
};

/**
 * @class NeuralNetVisage
 * @brief A container Frame that applies a bloom post-processing effect.
 */
class NeuralNetVisage : public visage::Frame {
public:
    NeuralNetVisage() {
        // IMPORTANT: Seed the random number generator once in your main() function
        // by adding the line: srand(time(NULL));
        srand(time(NULL));

        bloom_.setBloomSize(40.0f);
        bloom_.setBloomIntensity(1.0f); 
        setPostEffect(&bloom_);

        addChild(&web_frame_);
        web_frame_.layout().setMargin(0);

        onDraw() = [&](visage::Canvas& canvas) {
            canvas.setColor(0xff101214);
            canvas.rectangle(0, 0, width(), height());
            // Redraw call was moved to the child frame which is actually animating.
        };
    }

    void resized() override {
        web_frame_.layout().setWidth(width());
        web_frame_.layout().setHeight(height());
    }

private:
    visage::BloomPostEffect bloom_;
    SimplifiedWebFrame web_frame_;
};