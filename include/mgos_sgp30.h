#pragma once
#include <stdint.h>
#include <mgos_i2c.h>

#ifdef __cplusplus
extern "C"
{
#endif

/*
 * `tvoc`  - tVOC concentration ppb
 * `co2`   - CO2eq concentration ppm
 */
struct mgos_sgp30_data
{
    int tvoc;
    int co2;
};

extern const int MGOS_sgp30_ERROR;

bool mgos_sgp30_init(void);
bool mgos_sgp30_setup(void);
int8_t mgos_sgp30_read(struct mgos_sgp30_data* data);
int mgos_sgp30_read_tvoc(void);
int mgos_sgp30_read_co2(void);
struct mgos_sgp30_data* mgos_sgp30_data_create();

#ifdef __cplusplus
}
#endif