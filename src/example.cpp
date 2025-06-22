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

#include "example.h"

#include <visage/app.h>
#include "visage/windowing.h"
#include "visage/graphics.h"
#include "visage/ui.h"
#include "visage/widgets.h"
#include <filesystem>
// #include "visage_graphics/VisageEmbeddedFonts_generated/embedded/fonts.h"
// #include "visage_graphics/VisageEmbeddedIcons_generated/embedded/icons.h"
// #include "visage_graphics/VisageEmbeddedShaders_generated/embedded/shaders.h"

ExamplesFrame::ExamplesFrame() {

}

ExamplesFrame::~ExamplesFrame() = default;

void ExamplesFrame::resized() {
  ScrollableFrame::resized();
  setScrollableHeight(sections_.back()->bottom() + 8.0f);
}

void ExamplesFrame::setupBars() {
  bar_list_ = std::make_unique<visage::BarList>(kNumBars);

  bar_list_->onDraw() = [this](visage::Canvas& canvas) {
    canvas.setNativePixelScale();

    double render_time = canvas.time();
    float space = 1;
    float bar_width = (bar_list_->nativeWidth() + space) / kNumBars;
    int bar_height = bar_list_->nativeHeight();
    for (int i = 0; i < kNumBars; ++i) {
      float x = bar_width * i;
      float x_pos = std::round(x);
      float right = std::round(x + bar_width - space);
      float current_height = (sin1((render_time * 60.0 + i * 30) / 600.0f) + 1.0f) * 0.5f * bar_height;
      bar_list_->positionBar(i, x_pos, current_height, right - x_pos, bar_height - current_height);
    }
    bar_list_->draw(canvas);
  };
}

void ExamplesFrame::setupButtons() {
  visage::Font font(20, resources::fonts::Lato_Regular_ttf);

  icon_button_ = std::make_unique<visage::ToggleIconButton>(resources::icons::check_circle_svg.data,
                                                            resources::icons::check_circle_svg.size, true);

  
}

void ExamplesFrame::setupShapes() {
  
  };
}

void ExamplesFrame::setupTextEditors() {
  
}