DATA_DIR=data

# 入力文字列
INPUT_STR=./data/sample_input.txt

# シンボルファイルなど
SYMB_WORD=./word_symbols.txt
SYMB_CHAR=./character_symbols.txt
SYMB_BIN=./binary_symbols.txt
PRON_WORD=./word_prons.txt

FST_INPUT=./input.fst
FST_INPUT_DICT=./input_dict.fst
FST_INPUT_DICT_NGRAM=./input_dict_ngram.fst
FST_DICT=./dict.fst
FST_DICT_NGRAM_HAIKU=./dict_ngram_haiku.fst
FST_NGRAM=./ngram.fst
FST_NGRAM_HAIKU=./ngram_haiku.fst
FST_HAIKU_JUDGE=./haiku_judge.fst
FST_HAIKU_PREFIX=./haiku_prefix.fst

IMG_INPUT=$(FST_INPUT%.fst=.png)#./input.png
IMG_INPUT_DICT=./input_dict.png
IMG_DICT=./dict.png
IMG_NGRAM_HAIKU=./ngram_haiku.png

TARGET= \
				$(IMG_INPUT) \
				$(IMG_INPUT_DICT) \
				$(IMG_NGRAM_HAIKU) \
				$(FST_NGRAM) \
				$(FST_HAIKU_JUDGE) \
				$(FST_NGRAM_HAIKU) \
				$(FST_INPUT_DICT_NGRAM)

all: $(TARGET)


$(IMG_INPUT): $(FST_INPUT) $(SYMB_CHAR)
	./fst2png.sh -i $(SYMB_CHAR) -o $(SYMB_CHAR) $<
$(IMG_INPUT_DICT): $(FST_INPUT_DICT) $(SYMB_CHAR) $(SYMB_WORD)
	./fst2png.sh -i $(SYMB_CHAR) -o $(SYMB_WORD) $<
$(IMG_NGRAM_HAIKU): $(FST_NGRAM_HAIKU) $(SYMB_WORD) $(SYMB_BIN)
	./fst2png.sh -i $(SYMB_WORD) -o $(SYMB_BIN) $<


$(SYMB_CHAR) $(SYMB_WORD) $(SYMB_BIN) $(PRON_WORD): makeSymbolFiles
	./$< -c $(SYMB_CHAR) -w $(SYMB_WORD) -b $(SYMB_BIN)
$(FST_INPUT): makeInputFst $(INPUT_STR) $(SYMB_CHAR) 
	./$< -o $@ -i $(INPUT_STR) -c $(SYMB_CHAR)
$(FST_DICT) : makeDictFst  $(SYMB_CHAR) $(SYMB_WORD)
	./$< -o $@ -c $(SYMB_CHAR) -w $(SYMB_WORD)
$(FST_HAIKU_JUDGE): makeHaikuJudge $(SYMB_WORD) $(SYMB_BIN) $(PRON_WORD)
	./$< -o $@ -w $(SYMB_WORD) -p $(PRON_WORD)
$(FST_HAIKU_PREFIX): makeHaikuPrefix $(SYMB_WORD)
	./$< -o $@ -w $(SYMB_WORD)



$(FST_INPUT_DICT): $(FST_INPUT) $(FST_DICT)
	fstarcsort --sort_type=olabel $(FST_INPUT) $(FST_INPUT).srt
	fstarcsort --sort_type=ilabel $(FST_DICT)  $(FST_DICT).srt
	fstcompose $(FST_INPUT).srt $(FST_DICT).srt $@.raw
	fstrmepsilon $@.raw $@

$(FST_INPUT_DICT_NGRAM): $(FST_INPUT_DICT) $(FST_NGRAM)
	fstarcsort --sort_type=olabel $(FST_INPUT_DICT) $(FST_INPUT_DICT).srt
	fstarcsort --sort_type=ilabel $(FST_NGRAM) $(FST_NGRAM).srt
	fstcompose $(FST_INPUT_DICT) $(FST_NGRAM) $@.raw
	fstrmepsilon $@.raw $@

$(FST_NGRAM_HAIKU): $(FST_NGRAM) $(FST_HAIKU_JUDGE) $(FST_HAIKU_PREFIX)
	fstarcsort --sort_type=olabel $(FST_NGRAM) $(FST_NGRAM).srt
	fstarcsort --sort_type=ilabel $(FST_HAIKU_PREFIX) $(FST_HAIKU_PREFIX).srt
	fstcompose $(FST_NGRAM).srt $(FST_HAIKU_PREFIX).srt $@.raw
	fstarcsort --sort_type=olabel $@.raw $@.raw.srt
	fstarcsort --sort_type=ilabel $(FST_HAIKU_JUDGE) $(FST_HAIKU_JUDGE).srt
	fstcompose $@.raw.srt $(FST_HAIKU_JUDGE).srt $@.raw2
	fstshortestpath $@.raw2 $@.raw3
	fstrmepsilon $@.raw3 $@



# 必要なファイルをコンパイルする

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


NGRAM_TEMP_DIR=./ngram_temp_dir
$(NGRAM_TEMP_DIR):
	mkdir ${NGRAM_TEMP_DIR}

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

GM1=$(NGRAM_TEMP_DIR)/1gm.txt
GM2=$(NGRAM_TEMP_DIR)/2gm.txt
GM3=$(NGRAM_TEMP_DIR)/3gm.txt
GM4=$(NGRAM_TEMP_DIR)/4gm.txt
GM5=$(NGRAM_TEMP_DIR)/5gm.txt
GM6=$(NGRAM_TEMP_DIR)/6gm.txt
GM7=$(NGRAM_TEMP_DIR)/7gm.txt
NGM=$(NGRAM_TEMP_DIR)/ngm.txt

GM1_SRT=$(NGRAM_TEMP_DIR)/1gm-sorted.txt
GM2_SRT=$(NGRAM_TEMP_DIR)/2gm-sorted.txt
GM3_SRT=$(NGRAM_TEMP_DIR)/3gm-sorted.txt
GM4_SRT=$(NGRAM_TEMP_DIR)/4gm-sorted.txt
GM5_SRT=$(NGRAM_TEMP_DIR)/5gm-sorted.txt
GM6_SRT=$(NGRAM_TEMP_DIR)/6gm-sorted.txt
GM7_SRT=$(NGRAM_TEMP_DIR)/7gm-sorted.txt

$(GM1): $(NGRAM_TEMP_DIR)
	./ngramExtract.sh 1 > $@
$(GM2): $(NGRAM_TEMP_DIR)
	./ngramExtract.sh 2 > $@
$(GM3): $(NGRAM_TEMP_DIR)
	./ngramExtract.sh 3 > $@
$(GM4): $(NGRAM_TEMP_DIR)
	./ngramExtract.sh 4 > $@
$(GM5): $(NGRAM_TEMP_DIR)
	./ngramExtract.sh 5 > $@
$(GM6): $(NGRAM_TEMP_DIR)
	./ngramExtract.sh 6 > $@
$(GM7): $(NGRAM_TEMP_DIR)
	./ngramExtract.sh 7 > $@

$(GM1_SRT): $(GM1) ngramDiscardUnknownWords ngramSort $(NGRAM_TEMP_DIR) $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM2_SRT): $(GM2) ngramDiscardUnknownWords ngramSort $(NGRAM_TEMP_DIR) $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM3_SRT): $(GM3) ngramDiscardUnknownWords ngramSort $(NGRAM_TEMP_DIR) $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM4_SRT): $(GM4) ngramDiscardUnknownWords ngramSort $(NGRAM_TEMP_DIR) $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM5_SRT): $(GM5) ngramDiscardUnknownWords ngramSort $(NGRAM_TEMP_DIR) $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM6_SRT): $(GM6) ngramDiscardUnknownWords ngramSort $(NGRAM_TEMP_DIR) $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@
$(GM7_SRT): $(GM7) ngramDiscardUnknownWords ngramSort $(NGRAM_TEMP_DIR) $(SYMB_WORD)
	./ngramDiscardUnknownWords -i $< | ./ngramSort > $@

$(NGM): ngramMerge \
	$(GM1_SRT) \
	$(GM2_SRT) \
	$(GM3_SRT) \
	$(GM4_SRT) \
	$(GM5_SRT) \
	$(GM6_SRT) \
	$(GM7_SRT)

	./$< \
		$(GM1_SRT) \
		$(GM2_SRT) \
		$(GM3_SRT) \
		$(GM4_SRT) \
		$(GM5_SRT) \
		$(GM6_SRT) \
		$(GM7_SRT) > $@

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
	ngramread --epsilon_symbol="<eps>" --start_symbol="<S>" --end_symbol="</S>" --symbols=$(SYMB_WORD) $< > $@.raw
	fstprint $@.raw | ./ngramRemoveInvalidWeights | fstcompile --arc_type="standard" --isymbols=$(SYMB_WORD) --osymbols=$(SYMB_WORD) --keep_isymbols --keep_osymbols > $@

.PHONY: clean
clean: 
	rm -f makeInputFst makeSymbolFiles makeDictFst makeHaikuJudge makeHaikuPrefix
	rm -f ngramDiscardUnknownWords ngramSort ngramMerge ngramRemoveInvalidWeights
	rm -f $(GM1_SRT) $(GM2_SRT) $(GM3_SRT) $(GM4_SRT) $(GM5_SRT) $(GM6_SRT) $(GM7_SRT)
	rm -f *.dot *.fst *.png *symbols.txt *.raw *.raw2 *.raw3 *.srt
