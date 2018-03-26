
let BME680Data = {
    //data
    _crt_data: ffi('void* mgos_bme680_data_create()'),
    _dlt_data: ffi('void mgos_bme680_data_delete(void*)'),
    _temp: ffi('double mgos_bme680_data_get_temp(void*)'),
    _press: ffi('double mgos_bme680_data_get_press(void*)'),
    _humid: ffi('double mgos_bme680_data_get_humid(void*)'),
    _gas: ffi('double mgos_bme680_data_get_gas(void*)'),

    // ## **`BME680Data.create()`**
    // Creates a BME680Data instance to be used for reading data from BME680.
    // Return value: an object with the methods described below.
    create: function () {
        let obj = Object.create(BME680Data._proto);
        obj.data = BME680Data._crt_data();
        return obj;
    },

    _proto: {
        // ## **`bmeData.free()`**
        // Frees a BME680Data instance.
        // No methods can be called on this instance after that.
        // Return value: none.
        free: function () {
            BME680Data._dlt_data(this.data);
        },

        // ## **`bmeData.temp()`**
        // Gets the temperature component of the BME680Data structure.
        temp: function () {
            return BME680Data._temp(this.data);
        },

        // ## **`bmeData.press()`**
        // Gets the pressure component of the BME680Data structure.
        press: function () {
            return BME680Data._press(this.data);
        },

        // ## **`bmeData.humid()`**
        // Gets the humidity component of the BME680Data structure.
        humid: function () {
            return BME680Data._humid(this.data);
        },

        // ## **`bmeData.gas()`**
        // Gets the gas component of the BME680Data structure.
        gas: function () {
            return BME680Data._gas(this.data);
        },
    },
};

let BME680 = {
    _crt_i2c: ffi('void* mgos_bme680_i2c_create(int)'),
    _dlt: ffi('void mgos_bme680_delete(void*)'),
    _read: ffi('int mgos_bme680_read(void*, void*)'),
    MGOS_BME280_ERROR: -128.0,

    // ## **`BME680.createI2C(address)`**
    // Creates a BME680 instance on the I2C bus with the given address `address`.
    // Return value: an object with the methods described below.    
    createI2C: function (address) {
        let obj = Object.create(BME680._proto);
        obj.bme = BME680._crt_i2c(address);
        return obj;
    },

    _proto: {
        // ## **`bme.free()`**
        // Frees the  BME680 instance.
        // No methods can be called on this instance after that.
        // Return value: none.
        free: function () {
            BME680._dlt(this.bme);
        },

        // ## **`bme.readAll()`**
        // Reads all data into an instance of BME680Data
        // Returns zero if success, otherwise a negative value.
        readAll: function (data) {
            return BME680._read(this.bme, data.data);
        },
    },
};