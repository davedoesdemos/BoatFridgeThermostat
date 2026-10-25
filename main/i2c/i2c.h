#ifndef I2C_H
#define I2C_H

// Master I2C Settings
#define I2C_MASTER_SDA_IO           8       // Hardware Pin SDA
#define I2C_MASTER_SCL_IO           9       // Hardware Pin SCL
#define I2C_MASTER_PORT             I2C_NUM_0

#include <driver/i2c_master.h>

typedef struct {
    i2c_master_bus_handle_t global_bus_handle;
    i2c_master_dev_handle_t lcd_dev_handle;
}  i2c_config;

extern i2c_config i2c_settings;

esp_err_t init_i2c_bus(i2c_master_bus_handle_t *out_bus_handle);
esp_err_t init_i2c_device(int device_address, i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *out_dev_handle);

#endif // I2C_H