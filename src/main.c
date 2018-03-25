#include "mgos.h"

#include <mgos_i2c.h>
#include <mgos_spi.h>
#include "mgos_shadow.h"

#include "mgos_bme680.h"
#include "mgos_sgp30.h"
#include "bme680.h"

static struct mgos_bme680 *s_bme680 = NULL;

// static void aws_shadow_state_handler(void *arg, enum mgos_aws_shadow_event ev,
//                                      uint64_t version,
//                                      const struct mg_str reported,
//                                      const struct mg_str desired,
//                                      const struct mg_str reported_md,
//                                      const struct mg_str desired_md) 
// {
//     LOG(LL_INFO, ("== Event: %d (%s), version: %llu", ev,
//                 mgos_aws_shadow_event_name(ev), version));

//     if (ev == MGOS_AWS_SHADOW_CONNECTED) {
//         // report_state();
//         return;
//     }
    
//     if (ev != MGOS_AWS_SHADOW_GET_ACCEPTED &&
//         ev != MGOS_AWS_SHADOW_UPDATE_DELTA) {
//         return;
//     }

//     LOG(LL_INFO, ("Reported state: %.*s\n", (int) reported.len, reported.p));
//     LOG(LL_INFO, ("Desired state : %.*s\n", (int) desired.len, desired.p));
//     LOG(LL_INFO, ("Reported metadata: %.*s\n", (int) reported_md.len, reported_md.p));
//     LOG(LL_INFO, ("Desired metadata : %.*s\n", (int) desired_md.len, desired_md.p));
// }

static void shadow_event_cb(int ev, void *ev_data, void *userdata)
{
    LOG(LL_INFO, ("Got system event %s", mgos_shadow_event_name(ev)));
}

static void iot_timer_cb(void *arg) 
{
    int8_t rslt = 0;
    struct mgos_bme680_data bme680_data;
    rslt = mgos_bme680_read(s_bme680, &bme680_data);

    if (BME680_OK != rslt) {
        LOG(LL_INFO, ("Failed to read data from sensor"));
        return;
    }

    LOG(LL_INFO, ("T: %.2f degC, P: %.2f hPa, H %.2f %%rH G: %f ohms", bme680_data.temp / 100.0f, bme680_data.press / 100.0f, bme680_data.humid / 1000.0f, bme680_data.gas));

    struct mgos_sgp30_data sgp30_data;

    rslt = mgos_sgp30_read(&sgp30_data);
    if (BME680_OK != rslt) {
        LOG(LL_INFO, ("Failed to read data from sgp30"));
        return;
    }

    LOG(LL_INFO, ("TVOC: %dppb Co2: %dppm", sgp30_data.tvoc, sgp30_data.co2));
}

enum mgos_app_init_result mgos_app_init(void)
{
    if ((s_bme680 = mgos_bme680_i2c_create(0x77)) == NULL) return MGOS_APP_INIT_ERROR;
    if (!mgos_sgp30_setup()) return MGOS_APP_INIT_ERROR;

    mgos_set_timer(2000 /* ms */, true /* repeat */, iot_timer_cb, NULL);

    // mgos_aws_shadow_set_state_handler(aws_shadow_state_handler, NULL);

    mgos_event_add_handler(MGOS_SHADOW_BASE, shadow_event_cb, NULL);

    return MGOS_APP_INIT_SUCCESS;
}
