from esphome import pins
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, text_sensor
from esphome.const import (
    CONF_ADDRESS, CONF_ID,
    CONF_INTERRUPT_PIN,
    CONF_RESET_PIN
)

cst816s_touchscreen_ns = cg.esphome_ns.namespace('cst816s_touchscreen')
CST816STouchScreen = cst816s_touchscreen_ns.class_('CST816STouchScreen', text_sensor.TextSensor, cg.Component)

DEPENDENCIES = ["i2c"]

CONFIG_SCHEMA = text_sensor.TEXT_SENSOR_SCHEMA.extend(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(CST816STouchScreen),
            cv.Optional(CONF_ADDRESS): cv.hex_uint64_t,
            cv.Required(CONF_INTERRUPT_PIN): cv.All(
                pins.internal_gpio_input_pin_schema
            ),
            cv.Optional(CONF_RESET_PIN): pins.gpio_output_pin_schema,
        }
    )
    .extend(i2c.i2c_device_schema(0x15))
    .extend(cv.COMPONENT_SCHEMA)
)

async def to_code(config):
    cg.add_library(
        name="CST816S",
        repository="https://github.com/GadgetFactory/CST816S.git",
        version="1.1.1",
    )

    if CONF_ADDRESS in config:
        cg.add_define('CST816S_ADDRESS', config[CONF_ADDRESS])

    var = cg.new_Pvariable(config[CONF_ID])

    await text_sensor.register_text_sensor(var, config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    interrupt_pin = await cg.gpio_pin_expression(config[CONF_INTERRUPT_PIN])
    cg.add(var.set_interrupt_pin(interrupt_pin))

    if CONF_RESET_PIN in config:
        reset_pin = await cg.gpio_pin_expression(config[CONF_RESET_PIN])
        cg.add(var.set_reset_pin(reset_pin))
    else:
        reset_pin_def = cg.RawExpression(f"RST_PIN")
        cg.add(var.set_reset_pin(reset_pin_def))
