DATA_DIR=data

# 入力文字列
INPUT_STR=./data/sample_input.txt
HAIKU_PRF=./data/haiku_prefix.txt
CONTEXT=./data/context.txt

########################
#### 辞書用のデータ ####
########################
BCCWJ_LIST=$(DATA_DIR)/BCCWJ_frequencylist_suw_ver1_0.tsv

#########################################
#### N-Gram データ加工用のプログラム ####
#########################################
NGRAM_SORT                  =ngramSort
NGRAM_MERGE                 =ngramMerge
NGRAM_EXTRACT               =ngramExtract.sh
NGRAM_DOWNLOAD              =ngramDownload.sh
NGRAM_DISCARD_UNKNOWN_WORDS =ngramDiscardUnknownWords
NGRAM_REMOVE_INVALID_WEIGHTS=ngramRemoveInvalidWeights

##############################
#### シンボルファイルなど ####
##############################
SYMB_WORD=./word_symbols.txt
SYMB_CHAR=./character_symbols.txt
SYMB_BIN=./binary_symbols.txt
PRON_WORD=./word_prons.txt

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

$(BCCWJ_LIST):
	wget -N -P $(DATA_DIR) \
		https://pj.ninjal.ac.jp/corpus_center/bccwj/data-files/frequency-list/BCCWJ_frequencylist_suw_ver1_0.zip
	unzip -d $(DATA_DIR) $(@:%.tsv=%.zip)


$(SYMB_CHAR) $(SYMB_WORD) $(SYMB_BIN) $(PRON_WORD): makeSymbolFiles $(BCCWJ_LIST)
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


$(NGRAM_SORT)                  : $(NGRAM_SORT).cpp                   $(INC_NGRAM)
	g++ -o $@ $^
$(NGRAM_MERGE)                 : $(NGRAM_MERGE).cpp                  $(INC_NGRAM)
	g++ -o $@ $^
$(NGRAM_DISCARD_UNKNOWN_WORDS) : $(NGRAM_DISCARD_UNKNOWN_WORDS).cpp  $(INC_NGRAM)
	g++ -o $@ $^
$(NGRAM_REMOVE_INVALID_WEIGHTS): $(NGRAM_REMOVE_INVALID_WEIGHTS).cpp $(INC_NGRAM)
	g++ -o $@ $^


#######################
#### N-Gram の生成 ####
#######################

NGRAM_TEMP_DIR=./ngram_temp_dir

GM1_XZ=data/nwc2010-ngrams/word/over999/1gms/1gm-0000.xz
GM2_XZ=data/nwc2010-ngrams/word/over999/2gms/2gm-0000.xz
GM3_XZ=data/nwc2010-ngrams/word/over999/3gms/3gm-0000.xz
GM4_XZ=data/nwc2010-ngrams/word/over999/4gms/4gm-0000.xz
GM5_XZ=data/nwc2010-ngrams/word/over999/5gms/5gm-0000.xz
GM6_XZ=data/nwc2010-ngrams/word/over999/6gms/6gm-0000.xz
GM7_XZ=data/nwc2010-ngrams/word/over999/7gms/7gm-0000.xz

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

$(GM1_XZ):
	wget -N -P data -xnH https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over999/1gms/1gm-0000.xz
	touch ./$@
$(GM2_XZ):
	wget -N -P data -xnH https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over999/2gms/2gm-0000.xz
	touch ./$@
$(GM3_XZ):                                                                                    
	wget -N -P data -xnH https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over999/3gms/3gm-0000.xz
	touch ./$@
$(GM4_XZ):                                                                                    
	wget -N -P data -xnH https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over999/4gms/4gm-0000.xz
	touch ./$@
$(GM5_XZ):                                                                                    
	wget -N -P data -xnH https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over999/5gms/5gm-0000.xz
	touch ./$@
$(GM6_XZ):                                                                                    
	wget -N -P data -xnH https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over999/6gms/6gm-0000.xz
	touch ./$@
$(GM7_XZ):                                                                                    
	wget -N -P data -xnH https://s3-ap-northeast-1.amazonaws.com/nwc2010-ngrams/word/over999/7gms/7gm-0000.xz
	touch ./$@



$(GM1): $(GM1_XZ)
	mkdir -p $(NGRAM_TEMP_DIR)   # make temporary directory
	./$(NGRAM_EXTRACT) -n 1 > $@ # extract 1-gram data
$(GM2): $(GM2_XZ)
	mkdir -p $(NGRAM_TEMP_DIR)
	./$(NGRAM_EXTRACT) -n 2 > $@
$(GM3): $(GM3_XZ)
	mkdir -p $(NGRAM_TEMP_DIR)
	./$(NGRAM_EXTRACT) -n 3 > $@
$(GM4): $(GM4_XZ)
	mkdir -p $(NGRAM_TEMP_DIR)
	./$(NGRAM_EXTRACT) -n 4 > $@
$(GM5): $(GM5_XZ)
	mkdir -p $(NGRAM_TEMP_DIR)
	./$(NGRAM_EXTRACT) -n 5 > $@
$(GM6): $(GM6_XZ)
	mkdir -p $(NGRAM_TEMP_DIR)
	./$(NGRAM_EXTRACT) -n 6 > $@
$(GM7): $(GM7_XZ)
	mkdir -p $(NGRAM_TEMP_DIR)
	./$(NGRAM_EXTRACT) -n 7 > $@

$(GM1_SRT): $(GM1) $(NGRAM_DISCARD_UNKNOWN_WORDS) $(NGRAM_SORT) $(SYMB_WORD)
	./$(NGRAM_DISCARD_UNKNOWN_WORDS) -i $< | ./$(NGRAM_SORT) > $@
$(GM2_SRT): $(GM2) $(NGRAM_DISCARD_UNKNOWN_WORDS) $(NGRAM_SORT) $(SYMB_WORD)
	./$(NGRAM_DISCARD_UNKNOWN_WORDS) -i $< | ./$(NGRAM_SORT) > $@
$(GM3_SRT): $(GM3) $(NGRAM_DISCARD_UNKNOWN_WORDS) $(NGRAM_SORT) $(SYMB_WORD)
	./$(NGRAM_DISCARD_UNKNOWN_WORDS) -i $< | ./$(NGRAM_SORT) > $@
$(GM4_SRT): $(GM4) $(NGRAM_DISCARD_UNKNOWN_WORDS) $(NGRAM_SORT) $(SYMB_WORD)
	./$(NGRAM_DISCARD_UNKNOWN_WORDS) -i $< | ./$(NGRAM_SORT) > $@
$(GM5_SRT): $(GM5) $(NGRAM_DISCARD_UNKNOWN_WORDS) $(NGRAM_SORT) $(SYMB_WORD)
	./$(NGRAM_DISCARD_UNKNOWN_WORDS) -i $< | ./$(NGRAM_SORT) > $@
$(GM6_SRT): $(GM6) $(NGRAM_DISCARD_UNKNOWN_WORDS) $(NGRAM_SORT) $(SYMB_WORD)
	./$(NGRAM_DISCARD_UNKNOWN_WORDS) -i $< | ./$(NGRAM_SORT) > $@
$(GM7_SRT): $(GM7) $(NGRAM_DISCARD_UNKNOWN_WORDS) $(NGRAM_SORT) $(SYMB_WORD)
	./$(NGRAM_DISCARD_UNKNOWN_WORDS) -i $< | ./$(NGRAM_SORT) > $@

$(NGM): $(NGRAM_MERGE) \
	$(GM1_SRT) \
	$(GM2_SRT) \
	$(GM3_SRT) \
	$(GM4_SRT)

	./$< \
		$(GM1_SRT) \
		$(GM2_SRT) \
		$(GM3_SRT) \
		$(GM4_SRT) > $@

$(FST_NGRAM:%.fst=%_raw.fst): $(NGM) $(SYMB_WORD)
	ngramread \
		--epsilon_symbol="<eps>" \
		--start_symbol="<S>"     \
		--end_symbol="</S>"      \
		--symbols=$(SYMB_WORD) $< > $@

$(FST_NGRAM): $(FST_NGRAM:%.fst=%_raw.fst) $(NGRAM_REMOVE_INVALID_WEIGHTS)
	fstprint $< | ./$(NGRAM_REMOVE_INVALID_WEIGHTS) | fstcompile --arc_type="standard" --isymbols=$(SYMB_WORD) --osymbols=$(SYMB_WORD) --keep_isymbols --keep_osymbols > $@

.PHONY: clean
clean: 
	rm -f makeInputFst makeSymbolFiles makeDictFst makeHaikuJudge makeHaikuPrefix
	rm -f makeContextAbsorbFst makeContextGenerateFst
	rm -f ngramDiscardUnknownWords ngramSort ngramMerge ngramRemoveInvalidWeights
	rm -f $(GM1_SRT) $(GM2_SRT) $(GM3_SRT) $(GM4_SRT) $(GM5_SRT) $(GM6_SRT) $(GM7_SRT)
	rm -f *.dot *.fst *.png *.txt temp*
