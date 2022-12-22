CC?=gcc
CFLAGS?=-Wall -Wextra -pedantic -std=gnu99 -g
LDFLAGS?=-lm
PROGRAMS=random_ids id_query_naive id_query_indexed id_query_binsort coord_query_naive nonotfound trim_output qtimes
TESTS=test_id_naive test_id_indexed test_id_binsort test_qtimes test_cleanup

.PHONY: all test clean ../src.zip

all: $(PROGRAMS)

nonoutfound: nonotfound.c
	gcc -o nonotfound nonotfound.c $(CFLAGS)

trim_output: trim_output.c
	gcc -o trim_output trim_output.c $(CFLAGS)

qtimes: qtimes.c
	gcc -o qtimes qtimes.c $(CFLAGS)

random_ids: random_ids.o record.o
	gcc -o $@ $^ $(LDFLAGS)

id_query_%: id_query_%.o record.o id_query.o
	gcc -o $@ $^ $(LDFLAGS)

coord_query_%: coord_query_%.o record.o coord_query.o
	gcc -o $@ $^ $(LDFLAGS)

id_query.o: id_query.c
	$(CC) -c $< $(CFLAGS)

coord_query.o: coord_query.c
	$(CC) -c $< $(CFLAGS)

record.o: record.c
	$(CC) -c $< $(CFLAGS)

sort.o: sort.c
	$(CC) -c $< $(CFLAGS)

test: $(TESTS)
	@set e; for test in $(TESTS); do echo ./$$test; ./$$test; done

test_id_naive:
	./random_ids 19999records.tsv | head -n 1000 > 1000ids
	./id_query_naive 19999records.tsv < 1000ids > 1000queries
	./nonotfound 1000queries

test_id_indexed:
	./id_query_indexed 19999records.tsv < 1000ids > 1000indexedqueries
	./trim_output 1000indexedqueries > 1kiqt
	./trim_output 1000queries > 1kqt
	diff 1kqt 1kiqt

test_id_binsort:
	./id_query_binsort 19999records.tsv < 1000ids > 1000binsortqueries
	./trim_output 1000binsortqueries > 1kbsqt
	./trim_output 1000queries > 1kqt
	diff 1kqt 1kbsqt

test_qtimes:
	./qtimes 1000queries 1000indexedqueries 1000binsortqueries

test_cleanup:
	rm 1000queries
	rm 1kqt
	rm 1000indexedqueries
	rm 1kiqt
	rm 1000binsortqueries
	rm 1kbsqt

clean:
	rm -rf core *.o $(PROGRAMS)

planet-latest-geonames.tsv:
	wget https://github.com/OSMNames/OSMNames/releases/download/v2.0.4/planet-latest_geonames.tsv.gz
	gunzip planet-latest_geonames.tsv.gz

../src.zip:
	make clean
	cd .. && zip src.zip -r src

.SECONDARY:
