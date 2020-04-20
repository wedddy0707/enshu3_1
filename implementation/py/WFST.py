from itertools import product as cartesian_product
import copy

from semiring import TropicalSemiring as ts


class WeightedFiniteStateTransducer():
    ### 重み付き有限状態トランスデューサー

    __epsilon = "" # 空文字
    __redundent_epsilon = -1 # composition のときに必要

    def __init__(self,
            in_alphabet,
            out_alphabet,
            states,
            init_states,
            final_states,
            transitions,
            init_weights,
            final_weights,
            semiring=None):
        self.in_alphabet    = set(in_alphabet)
        self.out_alphabet   = set(out_alphabet)
        self.states         = set(states)
        self.init_states    = set(init_states)
        self.final_states   = set(final_states)
        self.transitions    = list(transitions) # タプルのリスト.
        self.init_weights   = dict(init_weights)
        self.final_weights  = dict(final_weights)

        if semiring == None:
            self.semiring = self.__inferSemiringClass()
        else:
            self.semiring = semiring

    def makeNewState(self,prefix='q',initial=False,final=False):
        i = 0
        while True:
            q = str(prefix)+str(i)
            if q in self.states:
                i += 1
            else:
                self.states.add(q)
                if initial:
                    self.init_states.add(q)
                    self.init_weights[q] = self.semiring.one()
                if final:
                    self.final_states.add(q)
                    self.final_weights[q] = self.semiring.one()
                return q

    @classmethod
    def eps(cls):
        return cls.__epsilon
        
    # Private Method
    def __inferSemiringClass(self):
        if len(self.transitions) > 0:
            return self.transitions[0][3].__class__
        elif len(self.init_weights) > 0:
            return list(self.init_weights.values())[0].__class__
        elif len(self.final_weights) > 0:
            return list(self.final_weights.values())[0].__class__
        else:
            return None # たぶんありえない

    def __arcsFrom(self,q,i=None):
        arcs = []
        for t in self.transitions:
            ## t is (q0,i,o,w,q1)
            if q == t[0]:
                if i == None or i == t[1]:
                    arcs.append(t)
        return arcs
    
    ################################################
    # Methods for composition
    #
    @classmethod
    def __newEpsilon(cls):
        cls.__redundent_epsilon -= 1
        return cls.__redundent_epsilon

    def __filterTransducer(self,alphabet,eps1,eps2):
        q0,q1,q2 = range(3)
        in_alphabet   = alphabet | {eps1,eps2}
        out_alphabet  = alphabet | {eps1,eps2}
        states        = {q0,q1,q2}
        init_states   = {q0}
        final_states  = {q0,q1,q2}
        init_weights  = {q0:self.semiring.one()}
        final_weights = {q0:self.semiring.one(),
                         q1:self.semiring.one(),
                         q2:self.semiring.one()}
        transitions  = [
            (q0,eps1,eps1,self.semiring.one(),q1),
            (q0,eps2,eps1,self.semiring.one(),q0),
            (q1,eps1,eps1,self.semiring.one(),q1),
            (q2,eps2,eps2,self.semiring.one(),q2)
        ]
        for x in alphabet:
            if not(x == eps1 or x == eps2):
                transitions.append((q0,x,x,self.semiring.one(),q0))
                transitions.append((q1,x,x,self.semiring.one(),q0))
                transitions.append((q2,x,x,self.semiring.one(),q0))

        return self.__class__(
                in_alphabet,
                out_alphabet,
                states,
                init_states,
                final_states,
                transitions,
                init_weights,
                final_weights,
                self.semiring
               )
                
    def __composeIgnoringEpsilon(self,other): # イプシロン遷移を考慮しない
        prod_init_states  = set(cartesian_product(self.init_states, other.init_states ))
        prod_final_states = set(cartesian_product(self.final_states,other.final_states))
        states       = copy.deepcopy(prod_init_states)
        init_states  = set()
        final_states = set()
        transitions  = []
        init_weights = {}
        final_weights= {}
        queue        = copy.deepcopy(list(prod_init_states))

        while not queue == []:
            q     = queue.pop(0)
            q1,q2 = q
            if q in prod_init_states:
                init_states.add(q)
                init_weights[q] = self.init_weights[q1]*other.init_weights[q2]
            if q in prod_final_states:
                final_states.add(q)
                final_weights[q] = self.final_weights[q1]*other.final_weights[q2]

            for arc1 in self.__arcsFrom(q1):
                for arc2 in self.__arcsFrom(q2):
                    (prev_q1,i1,o1,w1,next_q1) = arc1
                    (prev_q2,i2,o2,w2,next_q2) = arc2
                    if not o1 == i2:
                        continue
                    prev_q = (prev_q1,prev_q2)
                    next_q = (next_q1,next_q2)
                    transitions.append((prev_q,i1,o2,w1*w2,next_q))
                    if next_q not in states:
                        states.add  (next_q)
                        queue.append(next_q)

        return WeightedFiniteStateTransducer(
                self.in_alphabet  ,
                other.out_alphabet,
                states,
                init_states,
                final_states,
                transitions,
                init_weights,
                final_weights)

    def compose(self,ohter):
        ########################################
        # イプシロン遷移に対応するための下準備 #
        ########################################
        # 新しいイプシロンを２つ用意
        eps1 = self.__newEpsilon()
        eps2 = self.__newEpsilon()
        # 各トランスデューサーをコピー
        t1 = copy.deepcopy(self)
        t2 = copy.deepcopy(other)
        # t1のアウトプット、t2のインプットにε1,ε2を追加
        t1.out_alphabet.add(eps1)
        t1.out_alphabet.add(eps2)
        t2.in_alphabet.add(eps1)
        t2.in_alphabet.add(eps2)
        # ちょっと遷移規則を作り変える
        for i in range(len(self.transitions)):
            (q0,i,o,w,q1) = self.transitions[i]
            if o == self.__class__.__epsilon:
                t1.transitions.pop(i)
                t1.transitions.append((q0,i,eps2,w,q1))
        for i in range(len(other.transitions)):
            (q0,i,o,w,q1) = other.transitions[i]
            if i == self.__class__.__epsilon:
                t2.transitions.pop(i)
                t2.transitions.append((q0,eps1,o,w,q1))
        # ちょっと遷移規則を増やす
        for q in self.states:
            t1.transitions.append((q,self.__class__.__epsilon,eps1,semiring.one(),q))
        for q in self.states:
            t2.transitions.append((q,eps2,self.__class__.__epsilon,semiring.one(),q))
        # Filter Transducer を用意する
        f = self.__filterTransducer(alphabet,eps1,eps2)
        
        ########
        # 合成 #
        ########
        return t1.__composeIgnoringEpsilon(f).__composeIgnoringEpsilon(t2)

    def closure(self):
        t = copy.deepcopy(self)
        t.init_states =  set()
        t.transitions = list()
        new_q = t.makeNewState(prefix='*_',initial=True,final=True)

        for q in self.states:
            for arc in self.__arcsFrom(q):
                (prev_q,i,o,w,next_q) = arc
                if prev_q in self.init_states:
                    t.transitions.append((
                        prev_q,
                        i,
                        o,
                        self.init_weights[prev_q]*w,
                        next_q
                    ))
                else:
                    t.transitions.append(arc)
        for init_q in self.init_states:
            t.transitions.append((
                new_q,
                self.__class__.__epsilon,
                self.__class__.__epsilon,
                self.semiring.one(),
                init_q
            ))
            for final_q in self.final_states:
                t.transitions.append((
                    final_q,
                    self.__class__.__epsilon,
                    self.__class__.__epsilon,
                    self.final_weights[final_q],
                    init_q
                ))

        return t

    def run(self,string): # assume deterministic
        state  = list(self.init_states)[0] # 初期状態が複数あるかもしれないが、とりあえず０番目を
        weight = self.semiring.one()
        output = ""
        for x in string:
            if x not in self.in_alphabet:
                print("ERROR: Illegal Character \"%s\"" % x)
                return output,weight,False
            else:
                # 遷移先が複数あるかもしれないが、とりあえず０番目
                o,w,state = self.__arcsFrom(state,x)[0][2:5]
                output = output + o
                weight = weight * w
        return output, weight, state in self.final_states,state

    def paths(self,q0,x,q1,limit=None):
        if limit < 1:
            return list()
        elif limit == None:
            new_limit = None
        else:
            new_limit = limit-1

        p = list()
        for o,w,q in map(lambda x:x[2:5],self.__arcsFrom(q0,self.__class__.__epsilon)):
            p += list(map(lambda x:x.insert(0,(q0,self.__class__.__epsilon,o,w,q)),self.paths(q,x,q1)))

        if not x == "":
            for o,w,q in map(lambda x:x[2:5],self.__arcsFrom(q0,x[0])):
                p += list(map(lambda x:x.insert(0,(q0,x[0],o,w,q)),self.paths(q,x[1:],q1)))

