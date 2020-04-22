DIC_DIR=./data/dictionaryFst
DIC_ISYMB=$(DIC_DIR)/dictionary_isymb.txt
DIC_OSYMB=$(DIC_DIR)/dictionary_osymb.txt

INP_DIR=./data/inputFst
INP_ISYMB=$(INP_DIR)/input_isymb.txt
INP_OSYMB=$(INP_DIR)/input_osymb.txt

all: result.png

main: main.c inc/*.c* inc/*.h inc/openfst-1.7.7/src/lib/.libs/libfst.so
	g++ -o $@ -I inc/openfst-1.7.7 $^ -lpthread

inp_binary.fst dic_binary.fst: main
	./main

dic_binary.dot: dic_binary.fst $(DIC_ISYMB) $(DIC_OSYMB) 
	fstdraw --isymbols=$(DIC_ISYMB) --osymbols=$(DIC_OSYMB) $< $@

inp_binary.dot: inp_binary.fst $(INP_ISYMB) $(INP_OSYMB)
	fstdraw --isymbols=$(INP_ISYMB) --osymbols=$(INP_OSYMB) $< $@

dic.jpg: dic_binary.dot
	dot -Tjpeg $< -o $@
inp.jpg: inp_binary.dot
	dot -Tjpeg $< -o $@

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
	dot -Gdpi=1000 -Tpng $< -o $@

.PHONY:
clean: main *.dot *.fst result.png
	rm -f $^
