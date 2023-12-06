#  -- Nernst voltage & ESR sense --

#  Nernst AC injection resistor value (2.2k)
ESR_SUPPLY_R = 22000

# Heater low pass filter
ESR_SENSE_ALPHA = 0.002

# -- Pump current sense --
PUMP_CURRENT_SENSE_GAIN = 10

# LSU sense resistor - 61.9 ohms
LSU_SENSE_R = 61.9

"""  Pump low pass filter alpha
 sampling at 2.5khz, alpha of 0.01 gives about 50hz bandwidth """
PUMP_FILTER_ALPHA = 0.02

# -- Pump controller --
NERNST_TARGET = 0.45

# -- Heater controller config --
HEATER_CONTROL_PERIOD = 50
HEATER_PREHEAT_TIME = 5000
HEATER_WARMUP_TIMEOUT = 60000
HEATER_BATTERY_STAB_TIME = 3000

# minimal battery voltage to start heating without CAN command
HEATER_BATTERY_ON_VOLTAGE = 9.5

# mininal battery voltage to continue heating
HEATER_BATTETY_OFF_VOLTAGE = 8.5

HEATER_CLOSED_LOOP_THRESHOLD_ESR = 500
HEATER_TARGET_ESR = 300
HEATER_OVERHEAT_ESR = 150
HEATER_UNDERHEAT_ESR = 700