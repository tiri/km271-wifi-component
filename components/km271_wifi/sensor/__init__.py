import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID

from .. import KM271Component, CONF_PARAM_ID, CONF_KM271_ID, CONF_SENSOR_ENCODING, km271_ns

AUTOLOAD = ["km271_wifi"]

KM271Sensor = km271_ns.class_("KM271Sensor", sensor.Sensor, cg.Component)

SensorEncodingOption = km271_ns.enum("SensorEncodingOption")
ENCODING_OPTIONS = {
    "none": SensorEncodingOption.NONE,
    "halfdegree": SensorEncodingOption.HALFDEGREE,
    "negative": SensorEncodingOption.NEGATIVE,
}

CONFIG_SCHEMA = sensor.sensor_schema().extend(
    {
        cv.GenerateID(): cv.declare_id(KM271Sensor),
        cv.GenerateID(CONF_KM271_ID): cv.use_id(KM271Component),
        cv.Required(CONF_PARAM_ID): cv.hex_int,
        cv.Optional(CONF_SENSOR_ENCODING): cv.enum(ENCODING_OPTIONS)
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_PARAM_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    km271 = await cg.get_variable(config[CONF_KM271_ID])
    cg.add(km271.register_listener(var))
