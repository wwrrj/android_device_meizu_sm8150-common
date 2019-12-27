/*
 * Copyright (C) 2019 The MoKee Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "android.hardware.light@2.0-service.meizu_sm8150"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>
#include <utils/Errors.h>

#include "Light.h"

// libhwbinder:
using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

// Generated HIDL files
using android::hardware::light::V2_0::ILight;
using android::hardware::light::V2_0::implementation::Light;

const static std::string kPanelBacklightPath = "/sys/class/backlight/panel0-backlight/brightness";
const static std::string kPanelMaxBacklightPath = "/sys/class/backlight/panel0-backlight/max_brightness";
const static std::string kMxLedPath = "/sys/class/leds/mx-led/brightness";
const static std::string kMxBlinkPath = "/sys/class/leds/mx-led/blink";

int main() {
    uint32_t panelMaxBrightness = 255;

    std::ofstream panelBacklight(kPanelBacklightPath);
    if (!panelBacklight) {
        LOG(ERROR) << "Failed to open " << kPanelBacklightPath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }

    std::ifstream panelMaxBacklight(kPanelMaxBacklightPath);
    if (!panelMaxBacklight) {
        LOG(ERROR) << "Failed to open " << kPanelMaxBacklightPath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    } else {
        panelMaxBacklight >> panelMaxBrightness;
    }

    std::ofstream mxLed(kMxLedPath);
    if (!mxLed) {
        LOG(ERROR) << "Failed to open " << kMxLedPath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }

    std::ofstream mxBlink(kMxBlinkPath);
    if (!mxBlink) {
        LOG(ERROR) << "Failed to open " << kMxBlinkPath << ", error=" << errno
                   << " (" << strerror(errno) << ")";
        return -errno;
    }

    android::sp<ILight> service = new Light(
            {std::move(panelBacklight), panelMaxBrightness},
            std::move(mxLed), std::move(mxBlink));

    configureRpcThreadpool(1, true);

    android::status_t status = service->registerAsService();

    if (status != android::OK) {
        LOG(ERROR) << "Cannot register Light HAL service";
        return 1;
    }

    LOG(INFO) << "Light HAL Ready.";
    joinRpcThreadpool();
    // Under normal cases, execution will not reach this line.
    LOG(ERROR) << "Light HAL failed to join thread pool.";
    return 1;
}
