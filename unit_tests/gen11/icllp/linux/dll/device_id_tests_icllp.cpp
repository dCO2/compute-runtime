/*
 * Copyright (C) 2018-2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "runtime/os_interface/linux/drm_neo.h"
#include "test.h"

#include "hw_cmds.h"

#include <array>

using namespace NEO;

TEST(IcllpDeviceIdTest, supportedDeviceId) {
    std::array<DeviceDescriptor, 9> expectedDescriptors = {{
        {IICL_LP_1x1x8_ULT_DEVICE_A0_ID, &ICLLP_1x1x8::hwInfo, &ICLLP_1x1x8::setupHardwareInfo, GTTYPE_GT1},

        {IICL_LP_1x4x8_ULT_DEVICE_F0_ID, &ICLLP_1x4x8::hwInfo, &ICLLP_1x4x8::setupHardwareInfo, GTTYPE_GT1},
        {IICL_LP_1x4x8_ULX_DEVICE_F0_ID, &ICLLP_1x4x8::hwInfo, &ICLLP_1x4x8::setupHardwareInfo, GTTYPE_GT1},
        {IICL_LP_GT1_MOB_DEVICE_F0_ID, &ICLLP_1x4x8::hwInfo, &ICLLP_1x4x8::setupHardwareInfo, GTTYPE_GT1},

        {IICL_LP_1x6x8_ULX_DEVICE_F0_ID, &ICLLP_1x6x8::hwInfo, &ICLLP_1x6x8::setupHardwareInfo, GTTYPE_GT1},
        {IICL_LP_1x6x8_ULT_DEVICE_F0_ID, &ICLLP_1x6x8::hwInfo, &ICLLP_1x6x8::setupHardwareInfo, GTTYPE_GT1},

        {IICL_LP_1x8x8_SUPERSKU_DEVICE_F0_ID, &ICLLP_1x8x8::hwInfo, &ICLLP_1x8x8::setupHardwareInfo, GTTYPE_GT2},
        {IICL_LP_1x8x8_ULT_DEVICE_F0_ID, &ICLLP_1x8x8::hwInfo, &ICLLP_1x8x8::setupHardwareInfo, GTTYPE_GT2},
        {IICL_LP_1x8x8_ULX_DEVICE_F0_ID, &ICLLP_1x8x8::hwInfo, &ICLLP_1x8x8::setupHardwareInfo, GTTYPE_GT2},
    }};

    auto compareStructs = [](const DeviceDescriptor *first, const DeviceDescriptor *second) {
        return first->deviceId == second->deviceId && first->pHwInfo == second->pHwInfo &&
               first->setupHardwareInfo == second->setupHardwareInfo && first->eGtType == second->eGtType;
    };

    size_t startIndex = 0;
    while (!compareStructs(&expectedDescriptors[0], &deviceDescriptorTable[startIndex]) &&
           deviceDescriptorTable[startIndex].deviceId != 0) {
        startIndex++;
    };
    EXPECT_NE(0u, deviceDescriptorTable[startIndex].deviceId);

    for (auto &expected : expectedDescriptors) {
        EXPECT_TRUE(compareStructs(&expected, &deviceDescriptorTable[startIndex]));
        startIndex++;
    }
}
