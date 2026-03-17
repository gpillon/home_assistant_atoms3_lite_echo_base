import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.const import CONF_ID, CONF_SAMPLE_RATE

AUTO_LOAD = ["audio"]
DEPENDENCIES = ["esp32"]

CONF_M5ECHO_BASE_ID = "m5echo_base_id"
CONF_VOLUME_MIN = "volume_min"
CONF_VOLUME_MAX = "volume_max"

m5echo_base_ns = cg.esphome_ns.namespace("m5echo_base")
M5EchoBaseHub = m5echo_base_ns.class_("M5EchoBaseHub", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(M5EchoBaseHub),
        cv.Optional(CONF_SAMPLE_RATE, default=16000): cv.one_of(16000, int=True),
        cv.Optional(CONF_VOLUME_MIN, default=45): cv.int_range(min=0, max=100),
        cv.Optional(CONF_VOLUME_MAX, default=85): cv.int_range(min=0, max=100),
    }
).extend(cv.COMPONENT_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    cg.add(var.set_sample_rate(config[CONF_SAMPLE_RATE]))
    cg.add(var.set_volume_range(config[CONF_VOLUME_MIN], config[CONF_VOLUME_MAX]))

    cg.add_library("Wire", None)
    cg.add_library("ESP_I2S", None)