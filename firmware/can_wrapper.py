from enum import Enum


class HeaterAllow(Enum):
    Unknown = 0,
    Allowed = 1,
    NotAllowed = 2,


heater_can_status = HeaterAllow.Unknown


def get_heater_allowed():
    return heater_can_status


def get_efi_voltage():
    return 12
