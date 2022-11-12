import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID

CODEOWNERS = ["@tiri"]

DEPENDENCIES = ["uart"]

km271_ns = cg.esphome_ns.namespace("KM271")
KM271Component = km271_ns.class_("KM271Component", cg.Component, uart.UARTDevice)

CONF_PARAM_ID = "param_id"
CONF_KM271_ID = "km271_id"
CONF_SENSOR_ENCODING = "encoding"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(KM271Component),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
