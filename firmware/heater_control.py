from enum import Enum
from pid import PID
from wideband_config import HEATER_CONTROL_PERIOD, HEATER_TARGET_ESR, HEATER_PREHEAT_TIME,HEATER_BATTERY_STAB_TIME

class HeaterStatus(Enum):
    Preheat = 1,
    WarmupRamp = 2 ,
    ClosedLoop = 3,
    Stopped = 4


# local const
preheatTimeCounter = HEATER_PREHEAT_TIME / HEATER_CONTROL_PERIOD
batteryStabTimeCounter = HEATER_BATTERY_STAB_TIME / HEATER_CONTROL_PERIOD

# local vars
heater_pid = PID(0.3, 0.3, 0.01, 3.0, HEATER_CONTROL_PERIOD)
heater_status = HeaterStatus.Preheat
time_counter = preheatTimeCounter
battery_stab_time = batteryStabTimeCounter
ramp_voltage = 0

def get_voltage_for_state(state, heaterEsr):
    if(state == HeaterStatus.Preheat):
        return 1.5
    
    if(state == HeaterStatus.WarmupRamp):
        if(ramp_voltage < 10):
            #0.3 volt per second, divided by battery voltage and update rate
            heater_frequency = 1000.0 / HEATER_CONTROL_PERIOD
            ramp_voltage += (0.3 / heater_frequency)
    
    if(state == HeaterStatus.ClosedLoop):
        # "nominal" heater voltage is 7.5v, so apply correction around that point (instead of relying on integrator so much)
        # Negated because lower resistance -> hotter
         return 7.5 - heater_pid.GetOutput(HEATER_TARGET_ESR, heaterEsr)
    
    if(state == HeaterStatus.Stopped):
        # algo c rompio:
        return 0

    return None

""" def get_heater_status():
    return heater_status """

def is_running_closed_loop() -> bool:
    return heater_status == HeaterStatus.ClosedLoop


def get_heater_duty():
    return -1

def describe_heater_status():
    if(heater_status == HeaterStatus.Preheat):
        return "Preheat"
    elif(heater_status == HeaterStatus.WarmupRamp):
        return "WarmupRamp"
    elif(heater_status == HeaterStatus.ClosedLoop):
        return "ClosedLoop"
    elif(heater_status == HeaterStatus.Stopped):
        return "Stopped"
    return "Unknow"
