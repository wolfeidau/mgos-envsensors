#include "mgos.h"

#include <mgos_i2c.h>
#include <mgos_spi.h>

#include "mgos_bme680.h"

#include "bme680.h"

const double MGOS_BME680_ERROR = -128.0;

bool mgos_bme680_init()
{
    return true;
}

int8_t user_spi_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    // int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    /*
     * The parameter dev_id can be used as a variable to select which Chip Select pin has
     * to be set low to activate the relevant device on the SPI bus
     */

    /*
     * Data on the bus should be like
     * |----------------+---------------------+-------------|
     * | MOSI           | MISO                | Chip Select |
     * |----------------+---------------------|-------------|
     * | (don't care)   | (don't care)        | HIGH        |
     * | (reg_addr)     | (don't care)        | LOW         |
     * | (don't care)   | (reg_data[0])       | LOW         |
     * | (....)         | (....)              | LOW         |
     * | (don't care)   | (reg_data[len - 1]) | LOW         |
     * | (don't care)   | (don't care)        | HIGH        |
     * |----------------+---------------------|-------------|
     */

    (void)dev_id; /* Not used. Using default spi.cs0_gpio */

    struct mgos_spi *spi = mgos_spi_get_global();
    if (NULL == spi)
    {
        LOG(LL_INFO, ("Could not get SPI global instance"));
        return -1;
    }

    struct mgos_spi_txn txn;
    memset(&txn, 0, sizeof(txn));
    txn.cs = 0;   /* Using default spi.cs0_gpio */
    txn.mode = 0; /* Mode 0 or 3*/
    txn.freq = 1000000;
    txn.hd.tx_data = &reg_addr;
    txn.hd.tx_len = 1;
    txn.hd.rx_data = reg_data;
    txn.hd.rx_len = len;

    if (!mgos_spi_run_txn(spi, false, &txn))
    {
        LOG(LL_INFO, ("SPI transaction failed"));
        return -1;
    }

    return BME680_OK;
}

int8_t user_spi_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    // int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    /*
     * The parameter dev_id can be used as a variable to select which Chip Select pin has
     * to be set low to activate the relevant device on the SPI bus
     */

    /*
     * Data on the bus should be like
     * |---------------------+--------------+-------------|
     * | MOSI                | MISO         | Chip Select |
     * |---------------------+--------------|-------------|
     * | (don't care)        | (don't care) | HIGH        |
     * | (reg_addr)          | (don't care) | LOW         |
     * | (reg_data[0])       | (don't care) | LOW         |
     * | (....)              | (....)       | LOW         |
     * | (reg_data[len - 1]) | (don't care) | LOW         |
     * | (don't care)        | (don't care) | HIGH        |
     * |---------------------+--------------|-------------|
     */

    (void)dev_id; /* Not used. Using default spi.cs0_gpio */

    struct mgos_spi *spi = mgos_spi_get_global();
    if (NULL == spi)
    {
        LOG(LL_INFO, ("user_spi_write: Could not get SPI global instance"));
        return -1;
    }

    struct mgos_spi_txn txn;
    memset(&txn, 0, sizeof(txn));
    txn.cs = 0;   /* Using default spi.cs0_gpio */
    txn.mode = 0; /* Mode 0 or 3*/
    txn.freq = 1000000;

    uint8_t temp_buff[20]; /* Typically not to write more than 10 registers */
    temp_buff[0] = reg_addr;
    temp_buff[1] = reg_data[0];
    if (len >= 2)
    {
        memcpy(temp_buff + 2, reg_data + 1, len - 1);
    }
    txn.hd.tx_data = temp_buff;
    txn.hd.tx_len = len + 1;

    if (!mgos_spi_run_txn(spi, false, &txn))
    {
        LOG(LL_INFO, ("user_spi_write: SPI transaction failed"));
        return -1;
    }

    return BME680_OK;
}

int8_t user_i2c_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    // int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Stop       | -                   |
     * | Start      | -                   |
     * | Read       | (reg_data[0])       |
     * | Read       | (....)              |
     * | Read       | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */

    struct mgos_i2c *i2c = mgos_i2c_get_global();
    if (NULL == i2c)
    {
        LOG(LL_INFO, ("Could not get i2c global instance"));
        return -1;
    }
    bool ok = mgos_i2c_read_reg_n(i2c, dev_id, reg_addr, len, reg_data);
    return ok ? 0 : -2;
}

int8_t user_i2c_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    // int8_t rslt = 0; /* Return 0 for Success, non-zero for failure */

    /*
     * The parameter dev_id can be used as a variable to store the I2C address of the device
     */

    /*
     * Data on the bus should be like
     * |------------+---------------------|
     * | I2C action | Data                |
     * |------------+---------------------|
     * | Start      | -                   |
     * | Write      | (reg_addr)          |
     * | Write      | (reg_data[0])       |
     * | Write      | (....)              |
     * | Write      | (reg_data[len - 1]) |
     * | Stop       | -                   |
     * |------------+---------------------|
     */

    struct mgos_i2c *i2c = mgos_i2c_get_global();
    if (NULL == i2c)
    {
        LOG(LL_INFO, ("Could not get i2c global instance"));
        return -1;
    }

    bool ok = mgos_i2c_write_reg_n(i2c, dev_id, reg_addr, len, reg_data);
    return ok ? 0 : -2;
}

struct mgos_bme680 {
    struct bme680_dev dev;
};

static int8_t mgos_common_init(struct mgos_bme680* bme) {

    int8_t rslt = bme680_init(&bme->dev);
    if (BME680_OK != rslt) {
        LOG(LL_INFO, ("BME680 device not found - %hhd", rslt));
        return rslt;
    }

        //
    // Configuring the sensor
    //
    uint8_t set_required_settings;

    /* Set the temperature, pressure and humidity settings */
    bme->dev.tph_sett.os_hum = BME680_OS_2X;
    bme->dev.tph_sett.os_pres = BME680_OS_4X;
    bme->dev.tph_sett.os_temp = BME680_OS_8X;
    bme->dev.tph_sett.filter = BME680_FILTER_SIZE_3;

    /* Set the remaining gas sensor settings and link the heating profile */
    bme->dev.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
    /* Create a ramp heat waveform in 3 steps */
    bme->dev.gas_sett.heatr_temp = 320; /* degree Celsius */
    bme->dev.gas_sett.heatr_dur = 150;  /* milliseconds */

    /* Select the power mode */
    /* Must be set before writing the sensor configuration */
    bme->dev.power_mode = BME680_FORCED_MODE;

    /* Set the required sensor settings needed */
    set_required_settings = BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

    /* Set the desired sensor configuration */
    rslt = bme680_set_sensor_settings(set_required_settings, &bme->dev);
    if (BME680_OK != rslt) {
        LOG(LL_ERROR, ("Could not set sensor settings"));
        return rslt;
    }

    return BME680_OK;
}

struct mgos_bme680* mgos_bme680_i2c_create(uint8_t addr)
{
    // Is I2C enabled?
    if (!mgos_sys_config_get_i2c_enable()) {
        LOG(LL_ERROR, ("I2C is disabled."));
        return NULL;
    }

    struct mgos_bme680* bme = calloc(1, sizeof (struct mgos_bme680));
    if (NULL == bme) {
        LOG(LL_ERROR, ("Could not allocate mgos_bme680 structure."));
        return NULL;
    }

    //initialize the structure
    bme->dev.dev_id = addr;
    bme->dev.intf = BME680_I2C_INTF;
    bme->dev.read = user_i2c_read;
    bme->dev.write = user_i2c_write;
    bme->dev.delay_ms = mgos_msleep;

    int8_t rslt = mgos_common_init(bme);
    if (BME680_OK != rslt) {
        free(bme);
        return NULL;
    }

    return bme;
}

int8_t mgos_bme680_read(struct mgos_bme680* bme, struct mgos_bme680_data* data)
{
    if (NULL == bme) {
        return -1;
    }
    
    int8_t rslt;

    /* Set the power mode */
    rslt = bme680_set_sensor_mode(&bme->dev); /* Trigger a measurement */
    if (BME680_OK != rslt) {
        LOG(LL_ERROR, ("Could not set sensor mode"));
        return rslt;
    }

    uint16_t meas_period;
    bme680_get_profile_dur(&meas_period, &bme->dev);

    mgos_msleep(meas_period * 2);

    struct bme680_field_data comp_data;
    rslt = bme680_get_sensor_data(&comp_data, &bme->dev);
    if (BME680_OK == rslt) {
#ifdef BME680_FLOAT_POINT_COMPENSATION
        data->temp = comp_data.temperature / 100.0;
        data->press = comp_data.pressure / 100.0;
        data->humid = comp_data.humidity / 1024.0;
        if(comp_data.status & BME680_GASM_VALID_MSK)
            data->gas = comp_data.gas_resistance / 100.0;
#else
        data->temp = comp_data.temperature;
        data->press = comp_data.pressure;
        data->humid = comp_data.humidity;
        data->valid_gas = (comp_data.status & BME680_GASM_VALID_MSK) ? true : false;
        if(data->valid_gas)
            data->gas = comp_data.gas_resistance;
#endif /* BME680_FLOAT_POINT_COMPENSATION */
    }
    return rslt;
}

void mgos_bme680_delete(struct mgos_bme680* bme)
{
    if (NULL != bme) {
        free(bme);
    }
}

struct mgos_bme680_data* mgos_bme680_data_create()
{
    return calloc(1, sizeof (struct mgos_bme680_data));
}

void mgos_bme680_data_delete(struct mgos_bme680_data* data)
{
    if (NULL != data) {
        free(data);
    }
}

double mgos_bme680_data_get_temp(const struct mgos_bme680_data* data)
{
    return (NULL != data) ? data->temp : MGOS_BME680_ERROR;
}

double mgos_bme680_data_get_press(const struct mgos_bme680_data* data)
{
    return (NULL != data) ? data->press : MGOS_BME680_ERROR;
}

double mgos_bme680_data_get_humid(const struct mgos_bme680_data* data)
{
    return (NULL != data) ? data->humid : MGOS_BME680_ERROR;
}

double mgos_bme680_data_get_gas(const struct mgos_bme680_data* data)
{
    return (NULL != data) ? data->gas : MGOS_BME680_ERROR;
}

bool mgos_bme680_data_get_valid_gas(const struct mgos_bme680_data* data)
{
    return (NULL != data) ? data->valid_gas : MGOS_BME680_ERROR;
}