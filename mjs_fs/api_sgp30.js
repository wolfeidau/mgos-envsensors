let SGP30Data = {
    //data
    _crt_data: ffi('void* mgos_sgp30_data_create()'),
    _dlt_data: ffi('void mgos_sgp30_data_delete(void*)'),
    _tvoc: ffi('int mgos_sgp30_data_get_tvoc(void*)'),
    _co2: ffi('int mgos_sgp30_data_get_co2(void*)'),

    // ## **`SGP30Data.create()`**
    // Creates a SGP30Data instance to be used for reading data from SGP30.
    // Return value: an object with the methods described below.
    create: function () {
        let obj = Object.create(SGP30Data._proto);
        obj.data = SGP30Data._crt_data();
        return obj;
    },

    _proto: {
        // ## **`sgpData.free()`**
        // Frees a SGP30Data instance.
        // No methods can be called on this instance after that.
        // Return value: none.
        free: function () {
            SGP30Data._dlt_data(this.data);
        },

        // ## **`sgpData.tvoc()`**
        // Gets the tvoc component of the SGP30Data structure.
        tvoc: function () {
            return SGP30Data._tvoc(this.data);
        },

        // ## **`sgpData.co2()`**
        // Gets the co2 component of the SGP30Data structure.
        co2: function () {
            return SGP30Data._co2(this.data);
        },
    },
};

let SGP30 = {
    MGOS_SGP30_ERROR: -128,
    setup: ffi('bool mgos_sgp30_setup()'),
    _read: ffi('int mgos_sgp30_read(void*)'),

    // ## **`SGP30.readAll()`**
    // Reads all data into an instance of SGP30Data
    // Returns zero if success, otherwise a negative value.
    readAll: function (data) {
        return SGP30._read(data.data);
    },
};