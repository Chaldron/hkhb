#include "fan.h"

#include <homekit/characteristics.h>
#include <homekit/homekit.h>

#include "callback.h"

// Active characteristic [0, 1]
homekit_characteristic_t ch_active = HOMEKIT_CHARACTERISTIC_(
    ACTIVE, false, .getter = active_get, .setter = active_set);

// Rotation speed characteristic [0%, 33%, 66%, 100%]
homekit_characteristic_t ch_speed = HOMEKIT_CHARACTERISTIC_(
    ROTATION_SPEED, 1, .min_value = (float[]){0}, .max_value = (float[]){3},
    .getter = speed_get, .setter = speed_set);

// HomeKit fan definition
homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(
            .id = 1, .category = homekit_accessory_category_fan,
            .services =
                (homekit_service_t *[]){
                    HOMEKIT_SERVICE(
                        ACCESSORY_INFORMATION,
                        .characteristics =
                            (homekit_characteristic_t *[]){
                                HOMEKIT_CHARACTERISTIC(NAME, FAN_NAME),
                                HOMEKIT_CHARACTERISTIC(MANUFACTURER,
                                                       FAN_MANUFACTURER),
                                HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER,
                                                       FAN_SERIAL_NUMBER),
                                HOMEKIT_CHARACTERISTIC(MODEL, FAN_MODEL),
                                HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION,
                                                       FAN_FIRMWARE_REVISION),
                                HOMEKIT_CHARACTERISTIC(IDENTIFY, identify),
                                NULL}),
                    HOMEKIT_SERVICE(FAN2, .primary = true,
                                    .characteristics =
                                        (homekit_characteristic_t *[]){
                                            HOMEKIT_CHARACTERISTIC(NAME,
                                                                   FAN_NAME),
                                            &ch_active, &ch_speed, NULL}),

                    NULL}),
    NULL};

homekit_server_config_t config = {.accessories = accessories,
                                  .password = FAN_PASSWORD};
