CFLAGS=-g
COMMON_HEADERS=A5_provided_functions.h
COMMON_SOURCES=A5_provided_functions.c
ALL_COMMON=${COMMON_HEADERS} ${COMMON_SOURCES}

all: img_filter

q1_img_filter: q1_img_filter.c ${ALL_COMMON}
	gcc ${CFLAGS} q1_img_filter.c ${COMMON_SOURCES} -o q1_img_filter

q2_filter_with_history: q2_filter_with_history.c ${ALL_COMMON}
	gcc ${CFLAGS} q2_filter_with_history.c ${COMMON_SOURCES} -o q2_filter_with_history	

q1_test: q1_img_filter
	./q1_img_filter utah.bmp q1_utah_edges.bmp 3 1 1 1 1 -8 1 1 1 1
	./q1_img_filter utah.bmp q1_utah_mean.bmp 5 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 0.04 

q2_test: q2_filter_with_history
	./q2_filter_with_history load utah.bmp
	./q2_filter_with_history filter 3 -2 -1 0 -1 1 1 0 1 2
	cp output.bmp q2_utah_emboss.bmp
	./q2_filter_with_history undo
	cp output.bmp q2_utah_undo_once.bmp
	./q2_filter_with_history filter 1 1
	cp output.bmp q2_utah_identity.bmp
	./q2_filter_with_history undo
	cp output.bmp q2_utah_undo_twice.bmp
	./q2_filter_with_history filter 9 0 1 1 2 2 2 1 1 0 1 2 4 5 5 5 4 2 1 1 4 5 3 0 3 5 4 1 2 5 3 -12 -24 -12 3 5 2 2 5 0 -24 -40 -24 0 5 2 2 5 3 -12 -24 -12 3 5 2 1 4 5 3 0 3 5 4 1 1 2 4 5 5 5 4 2 1 0 1 1 2 2 2 1 1 0
	cp output.bmp q2_utah_LoG.bmp
	./q2_filter_with_history undo
	./q2_filter_with_history redo
	cp output.bmp q2_utah_LoG_undone_and_redone.bmp
