import numpy as np

class TropicalSemiring(object):
    def __init__(self,x=0):
        if x < 0:
            x = 0
            print("Warning: Tropical Semiring does not contain negative numbers. Regarded as 0.")
        self.value  = x

    def __str__(self):
        return str(self.value)
    def __add__(self,other):
        return TropicalSemiring(min(self.value,other.value))
    def __mul__(self,other):
        return TropicalSemiring(self.value + other.value)
    @classmethod
    def one(cls):
        return cls(0)
    @classmethod
    def zero(cls):
        return cls(np.inf)
