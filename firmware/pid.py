
class  PID:

    integrator = 0
    last_error = 0

    # period in mS
    def __init__(self, kP, kI, kD, clamp, period) -> None:
        self.kP = kP
        self.kI = kI
        self.kD = kD
        self.clamp = clamp
        self.period = period / 1000.0
        
    def get_output(self,setPoint, observation) -> float: 
        error = setPoint - observation
        self.integrator += error * self.period * self.kI  
        
        # Diff
        errorDiff = (error - self.last_error) / self.period
        self.last_error = error

        if(self.integrator > self.clamp):
            self.integrator = self.clamp
        elif (self.integrator < -self.clamp):
            self.integrator = -self.clamp

        return self.kP * error + self.integrator + self.kD * errorDiff
