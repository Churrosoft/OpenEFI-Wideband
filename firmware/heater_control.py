from enum import Enum
from pid import PID
from can_wrapper import HeaterAllow, get_heater_allowed
from wideband_config import (HEATER_CONTROL_PERIOD, HEATER_TARGET_ESR,
                             HEATER_PREHEAT_TIME, HEATER_BATTERY_STAB_TIME,
                             HEATER_BATTETY_OFF_VOLTAGE,
                             HEATER_BATTERY_ON_VOLTAGE,
                             HEATER_CLOSED_LOOP_THRESHOLD_ESR,
                             HEATER_WARMUP_TIMEOUT,
                             HEATER_OVERHEAT_ESR, HEATER_UNDERHEAT_ESR)


class HeaterStatus(Enum):
    Preheat = 1,
    WarmupRamp = 2,
    ClosedLoop = 3,
    Stopped = 4


class HS():
    # local const
    preheatTimeCounter = HEATER_PREHEAT_TIME / HEATER_CONTROL_PERIOD
    batteryStabTimeCounter = HEATER_BATTERY_STAB_TIME / HEATER_CONTROL_PERIOD

    # local vars
    heater_pid = PID(0.3, 0.3, 0.01, 3.0, HEATER_CONTROL_PERIOD)
    heater_status = HeaterStatus.Preheat
    time_counter = preheatTimeCounter
    battery_stab_time = batteryStabTimeCounter
    ramp_voltage = 0


def get_next_state(heaterAllowState, batteryVoltage, sensorEsr):
    heaterAllowed = heaterAllowState == HeaterAllow.Allowed

    if (heaterAllowState == HeaterAllow.Unknown):
        # measured voltage too low to auto-start heating
        if (batteryVoltage < HEATER_BATTETY_OFF_VOLTAGE):
            HS.battery_stab_time = HS.batteryStabTimeCounter

        if ((batteryVoltage > HEATER_BATTERY_ON_VOLTAGE)
                and (HS.battery_stab_time > 0)):
            HS.battery_stab_time -= 1

        heaterAllowed = HS.battery_stab_time == 0

    if (not heaterAllowed):
        HS.time_counter = HS.preheatTimeCounter
        return HeaterStatus.Preheat

    if (HS.heater_status == HeaterStatus.Preheat):
        HS.time_counter -= 1

        # If preheat timeout, or sensor is already hot (engine running?)
        if (HS.time_counter <= 0
                or (sensorEsr < HEATER_CLOSED_LOOP_THRESHOLD_ESR)):

            HS.ramp_voltage = 4
            HS.time_counter = HEATER_WARMUP_TIMEOUT / HEATER_CONTROL_PERIOD
            return HeaterStatus.WarmupRamp

    if(HS.heater_status == HeaterStatus.WarmupRamp):
        if(sensorEsr < HEATER_CLOSED_LOOP_THRESHOLD_ESR):
            return HeaterStatus.ClosedLoop
        elif(HS.time_counter == 0):
            # tendrriiiia que haltear el micro aca y mostrar algo en el 7seg
            return HeaterStatus.Stopped

    if(HS.heater_status == HeaterStatus.ClosedLoop):
        if(sensorEsr < HEATER_OVERHEAT_ESR):
            # otro error, se tosto la wideba'
            return HeaterStatus.Stopped
        elif(sensorEsr > HEATER_UNDERHEAT_ESR):
            # congelada como culo de pingünio
            return HeaterStatus.Stopped

    return HS.heater_status


def get_voltage_for_state(state, heaterEsr):
    if (state == HeaterStatus.Preheat):
        return 1.5

    if (state == HeaterStatus.WarmupRamp):
        if (HS.ramp_voltage < 10):
            # 0.3 volt per second, divided by battery voltage and update rate
            HS.heater_frequency = 1000.0 / HEATER_CONTROL_PERIOD
            HS.ramp_voltage += (0.3 / HS.heater_frequency)

    if (state == HeaterStatus.ClosedLoop):
        # "nominal" heater voltage is 7.5v, so apply correction around that
        # point (instead of relying on integrator so much)
        # Negated because lower resistance -> hotter
        return 7.5 - HS.heater_pid.GetOutput(HEATER_TARGET_ESR, heaterEsr)

    if (state == HeaterStatus.Stopped):
        # algo c rompio:
        return 0

    return None


""" def get_heater_status():
    return heater_status """


def is_running_closed_loop() -> bool:
    return HS.heater_status == HeaterStatus.ClosedLoop


def get_heater_duty():
    return -1


def describe_heater_status():
    if (HS.heater_status == HeaterStatus.Preheat):
        return "Preheat"
    elif (HS.heater_status == HeaterStatus.WarmupRamp):
        return "WarmupRamp"
    elif (HS.heater_status == HeaterStatus.ClosedLoop):
        return "ClosedLoop"
    elif (HS.heater_status == HeaterStatus.Stopped):
        return "Stopped"
    return "Unknow"
