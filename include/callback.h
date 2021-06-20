#include <homekit/characteristics.h>
#include <homekit/homekit.h>

// Identify accessory
void identify(homekit_value_t _value);

// Get/set active status
homekit_value_t active_get();
void active_set(homekit_value_t value);

// Get/set speed
homekit_value_t speed_get();
void speed_set(homekit_value_t value);