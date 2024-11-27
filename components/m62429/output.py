import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import output
from esphome.const import (
    CONF_ID,
    CONF_CLK_PIN,
    CONF_DATA_PIN,
    CONF_INITIAL_VALUE,
    CONF_STEP_DELAY,
    CONF_VOLUME,
)

CODEOWNERS = ["@theocoutu"]

m62429_ns = cg.esphome_ns.namespace("m62429")

M62429Output = m62429_ns.class_("M62429Output", output.FloatOutput, cg.Component)

CONFIG_SCHEMA = cv.All(
    output.FLOAT_OUTPUT_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(M62429Output),
            cv.Required(CONF_CLK_PIN): pins.internal_gpio_output_pin_schema,
            cv.Required(CONF_DATA_PIN): pins.internal_gpio_output_pin_schema,
            cv.Optional(CONF_INITIAL_VALUE, default=0.5): cv.float_range(
                min=0.0, max=1.0
            ),
            cv.Optional(CONF_STEP_DELAY, default="2us"): cv.All(
                cv.positive_time_period_microseconds,
                cv.Range(
                    min=cv.TimePeriod(microseconds=1),
                    max=cv.TimePeriod(microseconds=2),
                ),
            ),
        }
    )
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await output.register_output(var, config)

    clk_pin = await cg.gpio_pin_expression(config[CONF_CLK_PIN])
    cg.add(var.set_clk_pin(clk_pin))
    data_pin = await cg.gpio_pin_expression(config[CONF_DATA_PIN])
    cg.add(var.set_data_pin(data_pin))

    cg.add(var.set_initial_value(config[CONF_INITIAL_VALUE]))
    cg.add(var.set_step_delay(config[CONF_STEP_DELAY]))
