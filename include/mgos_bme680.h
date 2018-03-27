#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif
struct mgos_bme680;

/*
 * `temp`  - temperature in degrees Celsius
 * `press` - pressure in Pa
 * `humid` - humidity in %
 * `gas`   - gas resistance in Ohms
 */
struct mgos_bme680_data
{
    double temp;
    double press;
    double humid;
    bool valid_gas;
    double gas;
};

extern const double MGOS_BME680_ERROR;

struct mgos_bme680* mgos_bme680_i2c_create(uint8_t addr);
int8_t mgos_bme680_read(struct mgos_bme680* bme, struct mgos_bme680_data* data);
double mgos_bme680_read_temperature(struct mgos_bme680* bme);
double mgos_bme680_read_pressure(struct mgos_bme680* bme);
double mgos_bme680_read_humidity(struct mgos_bme680* bme);
void mgos_bme680_delete(struct mgos_bme680* bme);
struct mgos_bme680_data* mgos_bme680_data_create();
double mgos_bme680_data_get_temp(const struct mgos_bme680_data* data);
double mgos_bme680_data_get_press(const struct mgos_bme680_data* data);
double mgos_bme680_data_get_humid(const struct mgos_bme680_data* data);
double mgos_bme680_data_get_gas(const struct mgos_bme680_data* data);

#ifdef __cplusplus
}
#endif