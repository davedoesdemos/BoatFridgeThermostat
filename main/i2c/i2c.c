#include <stdio.h>
#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include "i2c.h"

extern i2c_config i2c_settings = {NULL, NULL}; 

// Function 1: Initializes the overall physical I2C Bus
esp_err_t init_i2c_bus(i2c_master_bus_handle_t *out_bus_handle)
{
   // i2c_config i2c_settings = {NULL, NULL};
    i2c_master_bus_config_t bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_PORT,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    // Pass the pointer through to allocate the global/app handle
    return i2c_new_master_bus(&bus_config, out_bus_handle);
}

// Function 2: Registers a unique peripheral device onto an initialized bus
esp_err_t init_i2c_device(int device_address, i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *out_dev_handle)
{
    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = device_address,
        .scl_speed_hz = 100000, 
    };

    // Link this component target to the existing bus
    return i2c_master_bus_add_device(bus_handle, &dev_config, out_dev_handle);
}