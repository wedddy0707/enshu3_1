DATA_DIR=data

# 入力文字列
INPUT_STR=./data/sample_input.txt
HAIKU_PRF=./data/haiku_prefix.txt
CONTEXT=./data/context.txt

# シンボルファイルなど
SYMB_WORD=./word_symbols.txt
SYMB_CHAR=./character_symbols.txt
SYMB_BIN=./binary_symbols.txt
PRON_WORD=./word_prons.txt

FST_INPUT=./input.fst
FST_INPUT_DICT=./input_dict.fst FST_INPUT_DICT_NGRAM=./input_dict_ngram.fst FST_DICT=./dict.fst
FST_CONTEXT=./context.fst
FST_CONTEXT_ABSORB=./context_absorb.fst
FST_CONTEXT_GENERATE=./context_generate.fst
FST_HAIKU=./haiku.fst
FST_HAIKU_PREFIX=./haiku_prefix.fst
FST_HAIKU_JUDGE=./haiku_judge.fst
FST_NGRAM=./ngram.fst
FST_NGRAM_HAIKU=./ngram_haiku.fst
FST_NGRAM_WITH_CONTEXT=./ngram_with_context.fst
FST_NGRAM_FOR_HAIKU_WITH_CONTEXT=./result.fst

IMG_NGRAM_FOR_HAIKU_WITH_CONTEXT=$(FST_NGRAM_FOR_HAIKU_WITH_CONTEXT:%.fst=%.png)


TARGET= $(IMG_NGRAM_FOR_HAIKU_WITH_CONTEXT)

all: $(TARGET)

########################
#### 結果画像の生成 ####
########################

$(IMG_NGRAM_FOR_HAIKU_WITH_CONTEXT): $(FST_NGRAM_FOR_HAIKU_WITH_CONTEXT) $(SYMB_WORD)
	./fst2png.sh -i $(SYMB_WORD) -o $(SYMB_WORD) $<
	xdg-open $@ &



################################
#### シンボルファイルの生成 ####
################################

$(SYMB_CHAR) $(SYMB_WORD) $(SYMB_BIN) $(PRON_WORD): makeSymbolFiles
	./$< -c $(SYMB_CHAR) -w $(SYMB_WORD) -b $(SYMB_BIN)




##############################
###### FST の生成と合成 ######
##############################

#### input fst の生成

$(FST_INPUT): makeInputFst $(INPUT_STR) $(SYMB_CHAR) 
	./$< -o $@ -i $(INPUT_STR) -c $(SYMB_CHAR)


#### dictionary fst の生成

$(FST_DICT) : makeDictFst  $(SYMB_CHAR) $(SYMB_WORD)
	./$< -o $@ -c $(SYMB_CHAR) -w $(SYMB_WORD)


#### context absorb fst の生成

$(FST_CONTEXT_ABSORB): makeContextAbsorbFst $(SYMB_WORD) $(CONTEXT)
	./$< -o $@ -w $(SYMB_WORD) -i $(CONTEXT)


#### context generate fst の生成 ####

$(FST_CONTEXT_GENERATE): makeContextGenerateFst $(SYMB_WORD) $(CONTEXT)
	./$< -o $@ -w $(SYMB_WORD) -i $(CONTEXT)


#### haiku judge fst の生成

$(FST_HAIKU_JUDGE): makeHaikuJudge $(SYMB_WORD) $(SYMB_BIN) $(PRON_WORD)
	./$< -o $@ -w $(SYMB_WORD) -p $(PRON_WORD)


#### hauku prefix fst の生成

$(FST_HAIKU_PREFIX): makeHaikuPrefix $(SYMB_WORD) $(HAIKU_PRF)
	./$< -o $@ -w $(SYMB_WORD) -i $(HAIKU_PRF)


#### context_generate `compose` ngram `compose` context_absorb

$(FST_NGRAM_WITH_CONTEXT): $(FST_NGRAM) $(FST_CONTEXT_ABSORB) $(FST_CONTEXT_GENERATE)
	fstarcsort --sort_type=olabel $(FST_CONTEXT_GENERATE) temp
	fstcompose temp $(FST_NGRAM) temp2
	fstarcsort --sort_type=ilabel $(FST_CONTEXT_ABSORB) temp3
	fstcompose temp2 temp3 $@

####
#### 
####
$(FST_NGRAM_FOR_HAIKU_WITH_CONTEXT): $(FST_NGRAM_WITH_CONTEXT) $(FST_HAIKU_PREFIX) $(FST_HAIKU_JUDGE)
	fstarcsort --sort_type=ilabel $(FST_HAIKU_PREFIX) temp
	fstcompose $(FST_NGRAM_WITH_CONTEXT) temp temp2
	fstprune   --weight=30 temp2 temp3
	fstarcsort --sort_type=ilabel $(FST_HAIKU_JUDGE) temp10
	fstarcsort --sort_type=olabel temp3 temp4
	fstcompose temp4 temp10 temp5 # かなり重い
	fstprune   --weight=30 temp5 temp6 # 枝狩り
	fstdisambiguate temp6 temp7 # かなり重い
	fstrmepsilon temp7 temp8 # ここで RmEpsilon をしないと ShortestPath が何故か失敗する.
	fstshortestpath --nshortest=20 temp8 temp9 # 画像にしやすいように Top 20 に絞る.
	fstrmepsilon temp9 $@ # 見やすさのため、もう一度 RmEpsilon.

$(FST_HAIKU_PREFIX:%.fst=%_ilabel_sort.fst): $(FST_HAIKU_PREFIX)
	fstarcsort --sort_type=ilabel $< $@

$(FST_HAIKU_JUDGE:%.fst=%_ilabel_sort.fst): $(FST_HAIKU_JUDGE)
	fstarcsort --sort_type=ilabel $< $@

########################################
#### 必要なファイルをコンパイルする ####
########################################

LIBFST=/usr/local/lib/libfst.so

INC_MAKE_SYMBOL_FILES = \
												./inc/utils.cpp  \
												./inc/utils.h    \
												./inc/symbol.cpp \
												./inc/symbol.h

INC_MAKE_FST_INPUT = \
										 ./inc/utils.cpp  \
										 ./inc/utils.h    \
										 ./inc/symbol.cpp \
										 ./inc/symbol.h   \
										 ./inc/inputFst.cpp \
										 ./inc/inputFst.h \
										 $(LIBFST)

INC_MAKE_FST_DICT = \
										./inc/utils.cpp  \
										./inc/utils.h    \
										./inc/symbol.cpp \
										./inc/symbol.h   \
										./inc/dictFst.cpp\
										./inc/dictFst.h  \
										$(LIBFST)

INC_MAKE_HAIKU_JUDGE = \
											 ./inc/utils.cpp  \
											 ./inc/utils.h    \
											 ./inc/symbol.cpp \
											 ./inc/symbol.h   \
											 ./inc/haikuFst.cpp \
											 ./inc/haikuFst.h \
											 $(LIBFST)

INC_MAKE_HAIKU_PREFIX = \
											 ./inc/utils.cpp  \
											 ./inc/utils.h    \
											 ./inc/symbol.cpp \
											 ./inc/symbol.h   \
											 ./inc/haikuFst.cpp \
											 ./inc/haikuFst.h \
											 $(LIBFST)

INC_MAKE_FST_CONTEXT = \
											 ./inc/utils.cpp  \
											 ./inc/utils.h    \
											 ./inc/symbol.cpp \
											 ./inc/symbol.h   \
											 ./inc/contextFst.cpp \
											 ./inc/contextFst.h \
											 $(LIBFST)

INC_NGRAM = \
						./inc/utils.cpp \
					 	./inc/utils.h   \
					 	./inc/symbol.cpp \
						./inc/symbol.h

makeSymbolFiles: makeSymbolFiles.cpp $(INC_MAKE_SYMBOL_FILES)
	g++ -o $@ $^
makeInputFst: makeInputFst.cpp $(INC_MAKE_FST_INPUT)
	g++ -o $@ $^
makeDictFst: makeDictFst.cpp $(INC_MAKE_FST_DICT)
	g++ -o $@ $^
makeHaikuJudge: makeHaikuJudge.cpp $(INC_MAKE_HAIKU_JUDGE)
	g++ -o $@ $^
makeHaikuPrefix: makeHaikuPrefix.cpp $(INC_MAKE_HAIKU_PREFIX)
	g++ -o $@ $^
makeContextAbsorbFst: makeContextAbsorbFst.cpp $(INC_MAKE_FST_CONTEXT)
	g++ -o $@ $^
makeContextGenerateFst: makeContextGenerateFst.cpp $(INC_MAKE_FST_CONTEXT)
	g++ -o $@ $^


#
# Makefile上でのfor文の挙動がわからないので、展開して書く
#
ngramDiscardUnknownWords: ngramDiscardUnknownWords.cpp $(INC_NGRAM)
	g++ -o $@ $^
ngramSort:                ngramSort.cpp $(INC_NGRAM)
	g++ -o $@ $^
ngramMerge:               ngramMerge.cpp $(INC_NGRAM)
	g++ -o $@ $^
ngramRemoveInvalidWeights: ngramRemoveInvalidWeights.cpp $(INC_NGRAM)
	g++ -o $@ $^


#######################
#### N-Gram の生成 ####
#######################

NGRAM_TEMP_DIR=./ngram_temp_dir
GM1=$(NGRAM_TEMP_DIR)/1gm.txt
GM2=$(NGRAM_TEMP_DIR)/2gm.txt
GM3=$(NGRAM_TEMP_DIR)/3gm.txt
GM4=$(NGRAM_TEMP_DIR)/4gm.txt
GM5=$(NGRAM_TEMP_DIR)/5gm.txt
GM6=$(NGRAM_TEMP_DIR)/6gm.txt
GM7=$(NGRAM_TEMP_DIR)/7gm.txt
NGM=$(NGRAM_TEMP_DIR)/ngm.txt

GM1_SRT=$(GM1:%.txt=%_srt.txt)
GM2_SRT=$(GM2:%.txt=%_srt.txt)
GM3_SRT=$(GM3:%.txt=%_srt.txt)
GM4_SRT=$(GM4:%.txt=%_srt.txt)
GM5_SRT=$(GM5:%.txt=%_srt.txt)
GM6_SRT=$(GM6:%.txt=%_srt.txt)
GM7_SRT=$(GM7:%.txt=%_srt.txt)

$(GM1):
	mkdir -p $(NGRAM_TEMP_DIR)
	./ngramExtract.sh -n 1 > $@
$(GM2):
	mkdir -p $(NGRAM_TEMP_DIR)
	./ngramExtract.sh -n 2 > $@
$(GM3):
	mkdir -p $(NGRAM_TEMP_DIR)
	./ngramExtract.sh -n 3 > $@
$(GM4):
	mkdir -p $(NGRAM_TEMP_DIR)
	./ngramExtract.sh -n 4 > $@
$(GM5):
	mkdir -p $(NGRAM_TEMP_DIR)
	./ngramExtract.sh -n 5 > $@
$(GM6):
	mkdir -p $(NGRAM_TEMP_DIR)
	./ngramExtract.sh -n 6 > $@
$(GM7):
	mkdir -p $(NGRAM_TEMP_DIR)
	./ngramExtract.sh -n 7 > $@

$(GM1_SRT): $(GM1) ngramDiscardUnknownWords ngramSort $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM2_SRT): $(GM2) ngramDiscardUnknownWords ngramSort $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM3_SRT): $(GM3) ngramDiscardUnknownWords ngramSort $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM4_SRT): $(GM4) ngramDiscardUnknownWords ngramSort $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM5_SRT): $(GM5) ngramDiscardUnknownWords ngramSort $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM6_SRT): $(GM6) ngramDiscardUnknownWords ngramSort $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM7_SRT): $(GM7) ngramDiscardUnknownWords ngramSort $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@

$(NGM): ngramMerge \
	$(GM1_SRT) \
	$(GM2_SRT) \
	$(GM3_SRT) \
	$(GM4_SRT)

	./$< \
		$(GM1_SRT) \
		$(GM2_SRT) \
		$(GM3_SRT) \
		$(GM4_SRT) > $@

#
# ngramread コマンドにはいくつか罠がある.
#
# - データが予めソートされていなければならない.
#   -- 上で頑張ってソートしているのはそのため.
# - 空文字のデフォルトは <epsilon>. なぜ <eps> ではないのか...
# - BOS のデフォルトは <s>.  拾ってきたデータでは <S>.
# - EOS のデフォルトは </s>. 拾ってきたデータでは </S>.
#
$(FST_NGRAM): $(NGM) $(SYMB_WORD) ngramRemoveInvalidWeights
	ngramread --epsilon_symbol="<eps>" --start_symbol="<S>" --end_symbol="</S>" --symbols=$(SYMB_WORD) $< > $(@:%.fst=%_raw.fst)
	fstprint $(@:%.fst=%_raw.fst) | ./ngramRemoveInvalidWeights | fstcompile --arc_type="standard" --isymbols=$(SYMB_WORD) --osymbols=$(SYMB_WORD) --keep_isymbols --keep_osymbols > $@

.PHONY: clean
clean: 
	rm -f makeInputFst makeSymbolFiles makeDictFst makeHaikuJudge makeHaikuPrefix
	rm -f makeContextAbsorbFst makeContextGenerateFst
	rm -f ngramDiscardUnknownWords ngramSort ngramMerge ngramRemoveInvalidWeights
	rm -f $(GM1_SRT) $(GM2_SRT) $(GM3_SRT) $(GM4_SRT) $(GM5_SRT) $(GM6_SRT) $(GM7_SRT)
	rm -f *.dot *.fst *.png *.txt temp*
