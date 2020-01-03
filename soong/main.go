package sm8150

import (
    "android/soong/android"
)

func init() {
    android.RegisterModuleType("meizu_sm8150_fod_hal_binary", fodHalBinaryFactory)
    android.RegisterModuleType("meizu_sm8150_light_hal_binary", lightHalBinaryFactory)
}
