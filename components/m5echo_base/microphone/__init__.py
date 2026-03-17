import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import microphone, audio
from esphome.const import CONF_ID

from .. import m5echo_base_ns, M5EchoBaseHub, CONF_M5ECHO_BASE_ID

AUTO_LOAD = ["audio"]
DEPENDENCIES = ["esp32", "m5echo_base"]

M5EchoBaseMicrophone = m5echo_base_ns.class_(
    "M5EchoBaseMicrophone", cg.Component, microphone.Microphone
)

def _set_stream_limits(config):
    audio.set_stream_limits(
        min_bits_per_sample=16,
        max_bits_per_sample=16,
        min_channels=2,
        max_channels=2,
        min_sample_rate=16000,
        max_sample_rate=16000,
    )(config)
    return config

CONFIG_SCHEMA = cv.All(
    microphone.MICROPHONE_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(M5EchoBaseMicrophone),
            cv.Required(CONF_M5ECHO_BASE_ID): cv.use_id(M5EchoBaseHub),
        }
    ).extend(cv.COMPONENT_SCHEMA),
    _set_stream_limits,
)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await microphone.register_microphone(var, config)

    hub = await cg.get_variable(config[CONF_M5ECHO_BASE_ID])
    cg.add(var.set_hub(hub))

    cg.add_library("Wire", None)
    cg.add_library("ESP_I2S", None)