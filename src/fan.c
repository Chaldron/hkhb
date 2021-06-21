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

homekit_accessory_t *accessories[] = {
    HOMEKIT_ACCESSORY(
            .id = 1, .category = homekit_accessory_category_fan,
            .services =
                (homekit_service_t *[]){
                    HOMEKIT_SERVICE(
                        ACCESSORY_INFORMATION,
                        .characteristics =
                            (homekit_characteristic_t *[]){
                                HOMEKIT_CHARACTERISTIC(NAME, "Fan"),
                                HOMEKIT_CHARACTERISTIC(MANUFACTURER,
                                                       "Harbor Breeze"),
                                HOMEKIT_CHARACTERISTIC(SERIAL_NUMBER, "1289"),
                                HOMEKIT_CHARACTERISTIC(MODEL, "CHQ7030T"),
                                HOMEKIT_CHARACTERISTIC(FIRMWARE_REVISION,
                                                       "1.0"),
                                HOMEKIT_CHARACTERISTIC(IDENTIFY, identify),
                                NULL}),
                    HOMEKIT_SERVICE(FAN2, .primary = true,
                                    .characteristics =
                                        (homekit_characteristic_t *[]){
                                            HOMEKIT_CHARACTERISTIC(NAME, "Fan"),
                                            &ch_active, &ch_speed, NULL}),

                    NULL}),
    NULL};

homekit_server_config_t config = {.accessories = accessories,
                                  .password = "111-11-111"};
