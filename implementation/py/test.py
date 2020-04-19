from semiring import TropicalSemiring as ts
from WFST import WeightedFiniteStateTransducer as wfst

a, b, c = range(3)
states = {a, b, c}
alphabets = {"0", "1"}
transitions = {
    (a,"0","0",ts(10),a),
    (a,"1","1",ts(20),b),
    (b,"0","0",ts(20),c),
    (b,"1","1",ts(30),a),
    (c,"0","0",ts(40),b),
    (c,"1","1",ts(50),c)
}
init_states  = [a]
final_states = [a]
init_weight  = {a:ts(3)}
final_weight = {a:ts(4)}

transducer = wfst (
    alphabets,
    alphabets,
    states,
    init_states,
    final_states,
    transitions,
    init_weight,
    final_weight,
    ts
)

tests = (("00000", True),       # 0
         ("0011", True),        # 3
         ("01110", False),      # 14
         ("1001", True),        # 9
         ("1110101110", True))  # 942

print("is_divisible_by_3".center(70, "-"))
for string, expected in tests:
    output,weight,result = transducer.run(string)
    print("%s: \"%s\"" % (str(result).ljust(5), string))
    if result != expected:
        print("ERROR")
