#include "hvac_fujitsu.h"
#include <stdlib.h>
#include <string.h>

#define TAG "hvac_fujitsu"

typedef struct {
    uint8_t power;       // HvacFujitsuPower
    uint8_t mode;        // HvacFujitsuMode
    uint8_t fan;         // HvacFujitsuFanSpeed
    uint8_t temp;        // 16 to 30
    uint8_t vane;        // HvacFujitsuVane
    uint8_t send_power_on;
} HvacFujitsu;

uint8_t* hvac_fujitsu_init() {
    HvacFujitsu* hvac = malloc(sizeof(HvacFujitsu));
    hvac->power = HvacFujitsuPowerOn;
    hvac->mode = HvacFujitsuModeAuto;
    hvac->fan = HvacFujitsuFanSpeedAuto;
    hvac->temp = 21;
    hvac->vane = HvacFujitsuVaneAuto;
    hvac->send_power_on = 1; // Send PowerOn bit for the first frame
    return (uint8_t*)hvac;
}

void hvac_fujitsu_deinit(uint8_t* packet) {
    free(packet);
}

void hvac_fujitsu_power(uint8_t* packet, HvacFujitsuPower power) {
    HvacFujitsu* hvac = (HvacFujitsu*)packet;
    if(power == HvacFujitsuPowerOn && hvac->power == HvacFujitsuPowerOff) {
        hvac->send_power_on = 1;
    }
    hvac->power = power;
}

void hvac_fujitsu_set_mode(uint8_t* packet, HvacFujitsuMode mode) {
    HvacFujitsu* hvac = (HvacFujitsu*)packet;
    hvac->mode = mode;
}

void hvac_fujitsu_set_fan_speed(uint8_t* packet, HvacFujitsuFanSpeed speed) {
    HvacFujitsu* hvac = (HvacFujitsu*)packet;
    hvac->fan = speed;
}

void hvac_fujitsu_set_temperature(uint8_t* packet, uint8_t temp) {
    HvacFujitsu* hvac = (HvacFujitsu*)packet;
    if(temp < HVAC_FUJITSU_TEMPERATURE_FAHRENHEIT_MIN) {
        temp = HVAC_FUJITSU_TEMPERATURE_FAHRENHEIT_MIN;
    } else if(temp > HVAC_FUJITSU_TEMPERATURE_FAHRENHEIT_MAX) {
        temp = HVAC_FUJITSU_TEMPERATURE_FAHRENHEIT_MAX;
    }
    hvac->temp = 16 + (temp - 60) / 2;
}

void hvac_fujitsu_set_vane(uint8_t* packet, HvacFujitsuVane vane) {
    HvacFujitsu* hvac = (HvacFujitsu*)packet;
    hvac->vane = vane;
}

void hvac_fujitsu_send(uint8_t* packet) {
    HvacFujitsu* hvac = (HvacFujitsu*)packet;
    
    uint8_t data[16];
    size_t length = 0;
    
    if(hvac->power == HvacFujitsuPowerOff) {
        // Construct 7-byte short command for TurnOff
        length = 7;
        data[0] = 0x14;
        data[1] = 0x63;
        data[2] = 0x00;
        data[3] = 0x10;
        data[4] = 0x10;
        data[5] = 0x02; // TurnOff command
        data[6] = (uint8_t)(~data[5]); // Checksum (0xFD)
    } else {
        // Construct 16-byte long command
        length = 16;
        data[0] = 0x14;
        data[1] = 0x63;
        data[2] = 0x00;
        data[3] = 0x10;
        data[4] = 0x10;
        data[5] = 0xFE;
        data[6] = 0x09;
        data[7] = 0x30;
        
        // Byte 8: Temp (bits 2-7), Fahrenheit (bit 1, always 0), Power (bit 0)
        uint8_t temp_val = (hvac->temp - 16) * 16;
        uint8_t power_bit = hvac->send_power_on ? 1 : 0;
        data[8] = temp_val | power_bit;
        
        // Byte 9: Mode mapping
        // Heat=0 -> 4; Cool=1 -> 1; Dry=2 -> 2; Auto=3 -> 0
        uint8_t raw_mode = 0x00;
        switch(hvac->mode) {
            case HvacFujitsuModeHeat: raw_mode = 0x04; break;
            case HvacFujitsuModeCool: raw_mode = 0x01; break;
            case HvacFujitsuModeDry:  raw_mode = 0x02; break;
            case HvacFujitsuModeAuto: raw_mode = 0x00; break;
            case HvacFujitsuModeFan:  raw_mode = 0x03; break;
            default:                  raw_mode = 0x00; break;
        }
        data[9] = raw_mode;
        
        // Byte 10: Fan speed and Swing mapping
        // Auto=0 -> 0; Speed1=1 -> 4; Speed2=2 -> 3; Speed3=3 -> 2; Speed4=4 -> 1; Silent=5 -> 4
        uint8_t raw_fan = 0x00;
        switch(hvac->fan) {
            case HvacFujitsuFanSpeedAuto:   raw_fan = 0x00; break;
            case HvacFujitsuFanSpeed1:      raw_fan = 0x04; break;
            case HvacFujitsuFanSpeed2:      raw_fan = 0x03; break;
            case HvacFujitsuFanSpeed3:      raw_fan = 0x02; break;
            case HvacFujitsuFanSpeed4:      raw_fan = 0x01; break;
            case HvacFujitsuFanSpeedSilent: raw_fan = 0x04; break;
            default:                        raw_fan = 0x00; break;
        }
        
        // Swing mapping
        // VaneAuto=0 -> 1 (Swing Vert); VaneH1..H5 -> 0 (Swing Off); VaneAutoMove=6 -> 1
        uint8_t raw_swing = 0x00; // Swing Off
        if(hvac->vane == HvacFujitsuVaneAuto || hvac->vane == HvacFujitsuVaneAutoMove) {
            raw_swing = 0x01; // Vertical swing Vert
        }
        
        data[10] = raw_fan | (raw_swing << 4);
        
        // Bytes 11-13 are unused/stop timers
        data[11] = 0x00;
        data[12] = 0x00;
        data[13] = 0x00;
        
        // Byte 14: Default fixed bit (unknown / 0x20)
        data[14] = 0x20;
        
        // Byte 15: Checksum = (0 - sum of bytes 7..14)
        uint8_t sum = 0;
        for(size_t i = 7; i <= 14; i++) {
            sum += data[i];
        }
        data[15] = (uint8_t)(0 - sum);
        
        // Once sent, clear send_power_on
        hvac->send_power_on = 0;
    }
    
    // Construct IR raw timings
    uint32_t timings[300];
    size_t timings_size = 0;
    
    // Header
    timings[timings_size++] = HVAC_FUJITSU_HDR_MARK;
    timings[timings_size++] = HVAC_FUJITSU_HDR_SPACE;
    
    // Data bits (LSB-first)
    for(size_t i = 0; i < length; i++) {
        uint8_t byte_val = data[i];
        for(int bit = 0; bit < 8; bit++) {
            timings[timings_size++] = HVAC_FUJITSU_BIT_MARK;
            if(byte_val & (1 << bit)) {
                timings[timings_size++] = HVAC_FUJITSU_ONE_SPACE;
            } else {
                timings[timings_size++] = HVAC_FUJITSU_ZERO_SPACE;
            }
        }
    }
    
    // Trailing stop bit mark
    timings[timings_size++] = HVAC_FUJITSU_BIT_MARK;
    
    // Transmit over IR
    uint32_t frequency = 38000;
    float duty_cycle = 0.33;
    infrared_send_raw_ext(timings, timings_size, true, frequency, duty_cycle);
}
