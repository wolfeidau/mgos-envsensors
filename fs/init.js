load('api_bme680.js');
load('api_sgp30.js');
load('api_shadow.js');
load('api_config.js');
load('api_timer.js');
load('api_sys.js');

let bme = BME680.createI2C(0x77);
let bmeData = BME680Data.create();
let sgpData = SGP30Data.create();

SGP30.setup();

// let state = {};

// // Set up Shadow handler to synchronise device state with the shadow state
// Shadow.addHandler(function(event, obj) {
//     if (event === 'CONNECTED') {
//       // Connected to shadow - report our current state.
//       Shadow.update(0, {reported: state});
//     } else if (event === 'UPDATE_DELTA') {
//       // Got delta. Iterate over the delta keys, handle those we know about.
      
//       // Once we've done synchronising with the shadow, report our state.
//       // Shadow.update(0, {reported: state});
//     }
// });

Timer.set(15000, Timer.REPEAT, function() {

    bme.readAll(bmeData);
    SGP30.readAll(sgpData);

    Shadow.update(0, {reported: {
        temp: bmeData.temp()/100,
        press: bmeData.press()/100,
        humid: bmeData.humid()/1024,
        gas: bmeData.gas()/100,
        tvoc: sgpData.tvoc(),
        co2: sgpData.co2(),
        uptime: Sys.uptime()
    }});
}, null);