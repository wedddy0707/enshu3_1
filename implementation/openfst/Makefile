DIC_ISYMB=./data/character_symbols.txt
DIC_OSYMB=./data/word_symbols.txt

INP_ISYMB=./data/character_symbols.txt
INP_OSYMB=./data/character_symbols.txt

all: result.png #dic.png inp.png

main: main.cpp inc/*.c* inc/*.h /usr/local/lib/libfst.so
	g++ -o $@ $^

inp_binary.fst dic_binary.fst: main
	./main

dic_binary.dot: dic_binary.fst $(DIC_ISYMB) $(DIC_OSYMB) 
	fstdraw --isymbols=$(DIC_ISYMB) --osymbols=$(DIC_OSYMB) $< $@

inp_binary.dot: inp_binary.fst $(INP_ISYMB) $(INP_OSYMB)
	fstdraw --isymbols=$(INP_ISYMB) --osymbols=$(INP_OSYMB) $< $@

#dic.png: dic_binary.dot
#	dot -Gdpi=300 -Tpng $< -o $@
inp.png: inp_binary.dot
	dot -Tpng $< -o $@

inp_sorted.fst: inp_binary.fst
	fstarcsort --sort_type=olabel $< $@
dic_sorted.fst: dic_binary.fst
	fstarcsort --sort_type=ilabel $< $@

comp.fst: inp_sorted.fst dic_sorted.fst
	fstcompose inp_sorted.fst dic_sorted.fst $@
# 合成順を間違えたくないので、念のためベタ書き

rmeps.fst: comp.fst
	fstrmepsilon $< $@

result.dot: rmeps.fst $(INP_ISYMB) $(DIC_OSYMB)
	fstdraw --isymbols=$(INP_ISYMB) --osymbols=$(DIC_OSYMB) $< $@

result.png: result.dot
	dot -Gdpi=500 -Tpng $< -o $@

.PHONY:
clean: main *.dot *.fst *.png
	rm -f $^
