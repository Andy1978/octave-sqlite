.PHONY:clean

TARGETS = foobar.db

all: $(TARGETS)

foobar.db: create_database.sql fill_database.sql
	sqlite3 $@ < create_database.sql
	sqlite3 $@ < fill_database.sql

clean:
	find . -name "octave-workspace" -exec rm {} \;
	rm -f $(TARGETS)
