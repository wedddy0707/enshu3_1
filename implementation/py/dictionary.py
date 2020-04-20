from WFST import WeightedFiniteStateTransducer as wfst
from semiring import TropicalSemiring as ts

class DictionaryWFST(wfst):
    ### 重み付き有限状態トランスデューサーで辞書を記述

    def __init__(self,words):
        self.in_alphabet   = set(''.join(words))
        self.out_alphabet  = set(words)

        init_q, final_q = 0,1
        self.states        = {init_q,final_q}
        self.init_states   = {init_q}
        self.final_states  = {final_q}
        self.transitions   = list()
        self.init_weights  = {init_q:ts.one()}
        self.final_weights = {final_q:ts.one()}
        self.semiring      = ts

        for each_word in words:
            prev_q = init_q
            for char in each_word:
                next_q = super().makeNewState(prefix=char)
                self.transitions.append((
                    prev_q,
                    char,
                    self.__class__.eps(),
                    self.semiring.one(),
                    next_q
                ))
                prev_q = next_q
            self.transitions.append((
                prev_q,
                self.__class__.eps(),
                each_word,
                self.semiring.one(), # ほんとは推定コストなど指定したい
                final_q
            ))

