import re

import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import core
from esphome.components import uart, sensor
from esphome.const import CONF_ID

CODEOWNERS = ["@tiri"]

DEPENDENCIES = ["uart"]

km271_ns = cg.esphome_ns.namespace("KM271")
KM271 = km271_ns.class_("KM271Component", cg.Component, uart.UARTDevice)
MULTI_CONF = True

CONF_KM271_ID = "km271_id"
CONF_SERVER_ID = "server_id"

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(KM271),
        }
    )
    .extend(uart.UART_DEVICE_SCHEMA)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await uart.register_uart_device(var, config)
