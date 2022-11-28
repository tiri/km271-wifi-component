import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID

from .. import KM271Component, CONF_PARAM_ID, CONF_KM271_ID, km271_ns

AUTOLOAD = ["km271_wifi"]

KM271BinarySensor = km271_ns.class_("KM271BinarySensor", binary_sensor.BinarySensor)

CONFIG_SCHEMA = binary_sensor.binary_sensor_schema().extend(
    {
        cv.GenerateID(): cv.declare_id(KM271BinarySensor),
        cv.GenerateID(CONF_KM271_ID): cv.use_id(KM271Component),
        cv.Required(CONF_PARAM_ID): cv.hex_int
    }
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID], config[CONF_PARAM_ID])
    await binary_sensor.register_binary_sensor(var, config)
    km271 = await cg.get_variable(config[CONF_KM271_ID])
    cg.add(km271.register_listener(var))
