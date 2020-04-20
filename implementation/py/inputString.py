from WFST import WeightedFiniteStateTransducer as wfst
from semiring import TropicalSemiring as ts

class InputStringWFST(wfst):
    ### 入力列を重み付き有限状態トランスデューサーで記述する

    def __init__(self,string):
        self.in_alphabet   = set(string)
        self.out_alphabet  = set(string)

        init_q = 'init'
        self.states        = set({init_q})
        self.init_states   = set({init_q})
        self.final_states  = set()
        self.transitions   = list()
        self.init_weights  = {init_q:ts.one()}
        self.final_weights = dict()
        self.semiring      = ts

        prev_q = init_q
        for char in string:
            next_q = super().makeNewState(prefix=char)
            self.transitions.append((
                prev_q,
                char,
                char,
                self.semiring.one(),
                next_q
            ))
            prev_q = next_q

        self.final_states.add(prev_q)
        self.final_weights[prev_q] = self.semiring.one()
