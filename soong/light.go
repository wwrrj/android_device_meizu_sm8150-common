package sm8150

import (
    "android/soong/android"
    "android/soong/cc"
    "strings"
)

func lightFlags(ctx android.BaseContext) []string {
    var cflags []string

    var config = ctx.AConfig().VendorConfig("MEIZU_SM8150_LIGHT")
    var mxLedPath = strings.TrimSpace(config.String("MX_LED_PATH"))

    cflags = append(cflags, "-DLIGHT_MX_LED_PATH=\"" + mxLedPath + "\"")
    return cflags
}

func lightHalBinary(ctx android.LoadHookContext) {
    type props struct {
        Target struct {
            Android struct {
                Cflags []string
            }
        }
    }

    p := &props{}
    p.Target.Android.Cflags = lightFlags(ctx)
    ctx.AppendProperties(p)
}

func lightHalBinaryFactory() android.Module {
    module, _ := cc.NewBinary(android.HostAndDeviceSupported)
    newMod := module.Init()
    android.AddLoadHook(newMod, lightHalBinary)
    return newMod
}
