import itertools # -- product()
import copy      # -- deepcopy()

from semiring import TropicalSemiring as ts


class WeightedFiniteStateTransducer():
    ### 重み付き有限状態トランスデューサー

    epsilon = -1 # 空文字は-1としておく

    def __init__(self,
            in_alphabets,
            out_alphabets,
            states,
            init_states,
            final_states,
            transitions,
            init_weights,
            final_weights,
            semiring):
        self.in_alphabets   = in_alphabets
        self.out_alphabets  = out_alphabets
        self.states         = states
        self.init_states    = init_states
        self.final_states   = final_states
        self.transitions    = transitions    # タプルのリスト.連想リストにしない.
        self.init_weights   = init_weights   # 連想リストでいいかな
        self.final_weights  = final_weights  #
        self.semiring       = semiring       # 重みを表す値が属す半環
    
    # Private Method
    def __arcsFrom(self,q,i=None):
        arcs = []
        for t in self.transitions:
            ## t is (q0,i,o,w,q1)
            if q == t[0]:
                if i == None or i == t[1]:
                    arcs.append(t)
        return arcs
    
    def run(self,string): # assume deterministic
        state  = self.init_states[0] # 初期状態が複数あるかもしれないが、とりあえず０番目を
        weight = self.semiring.one()
        output = ""
        for x in string:
            if x not in self.in_alphabets:
                print("ERROR: Illegal Character \"%s\"" % x)
                return output,weight,False
            else:
                # 遷移先が複数あるかもしれないが、とりあえず０番目
                o,w,state = self.__arcsFrom(state,x)[0][2:5]
                output = output + o
                weight = weight * w

        return output, weight, state in self.final_states

    def composite(self,other):
        prod_init_states  = list(itertools.product(self.init_states, other.init_states ))
        prod_final_states = list(itertools.product(self.final_states,other.final_states))
        states       = copy.deepcopy(prod_init_states)
        init_states  = []
        final_states = []
        transitions  = []
        queue        = copy.deepcopy(prod_init_states)

        while not queue == []:
            q     = queue.pop(0)
            q1,q2 = q
            if   q in prod_init_states:
                init_states.append(q)
                init_weights[q] = self.init_weights[q1]*other.init_weights[q2]
            elif q in prod_final_states:
                final_states.append(q)
                final_weights[q] = self.final_weights[q1]*other.final_weights[q2]

            for arc1,arc2 in itertools.product(self.__arcsFrom(q1),self.__arcsFrom(q2)):
                (prev_q1,i1,o1,w1,next_q1) = arc1
                (prev_q2,i2,o2,w2,next_q2) = arc2
                if not o1 == i2:
                    continue
                prev_q = (prev_q1,prev_q2)
                next_q = (next_q1,next_q2)
                if next_q in states:
                    states.append(next_q)
                    queue.append (next_q)
                transitions.append((prev_q,i1,o2,w1+w2,next_q))

        return WeightedFiniteStateTransducer(
                self.in_alphabets  ,
                other.out_alphabets,
                states,
                init_states,
                final_states,
                transitions,
                init_weights,
                final_weights)

