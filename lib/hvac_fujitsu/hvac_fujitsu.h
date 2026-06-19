#pragma once

#include <infrared_transmit.h>
#include <infrared_worker.h>
#include "furi_hal.h"

#define HVAC_FUJITSU_HDR_MARK 3300
#define HVAC_FUJITSU_HDR_SPACE 1600
#define HVAC_FUJITSU_BIT_MARK 400
#define HVAC_FUJITSU_ONE_SPACE 1200
#define HVAC_FUJITSU_ZERO_SPACE 400

#define HVAC_FUJITSU_TEMPERATURE_MIN 16
#define HVAC_FUJITSU_TEMPERATURE_MAX 30
#define HVAC_FUJITSU_TEMPERATURE_FAHRENHEIT_MIN 60
#define HVAC_FUJITSU_TEMPERATURE_FAHRENHEIT_MAX 88

typedef enum {
    HvacFujitsuPowerOn,
    HvacFujitsuPowerOff,
} HvacFujitsuPower;

typedef enum {
    HvacFujitsuModeHeat = 0,
    HvacFujitsuModeCold = 1,
    HvacFujitsuModeCool = 1,
    HvacFujitsuModeDry = 2,
    HvacFujitsuModeAuto = 3,
} HvacFujitsuMode;

typedef enum {
    HvacFujitsuFanSpeedAuto = 0,
    HvacFujitsuFanSpeed1 = 1,
    HvacFujitsuFanSpeed2 = 2,
    HvacFujitsuFanSpeed3 = 3,
    HvacFujitsuFanSpeed4 = 4,
    HvacFujitsuFanSpeedSilent = 5,
} HvacFujitsuFanSpeed;

typedef enum {
    HvacFujitsuVaneAuto = 0,
    HvacFujitsuVaneH1 = 1,
    HvacFujitsuVaneH2 = 2,
    HvacFujitsuVaneH3 = 3,
    HvacFujitsuVaneH4 = 4,
    HvacFujitsuVaneH5 = 5,
    HvacFujitsuVaneAutoMove = 6,
} HvacFujitsuVane;

uint8_t* hvac_fujitsu_init();
void hvac_fujitsu_deinit(uint8_t* packet);
void hvac_fujitsu_power(uint8_t* packet, HvacFujitsuPower power);
void hvac_fujitsu_set_mode(uint8_t* packet, HvacFujitsuMode mode);
void hvac_fujitsu_set_fan_speed(uint8_t* packet, HvacFujitsuFanSpeed speed);
void hvac_fujitsu_set_temperature(uint8_t* packet, uint8_t temp);
void hvac_fujitsu_set_vane(uint8_t* packet, HvacFujitsuVane vane);
void hvac_fujitsu_send(uint8_t* packet);