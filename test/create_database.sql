/*
  04.02.2016 Andreas Weber
  create dummy tables

  https://www.sqlite.org/docs.html
*/

DROP TABLE IF EXISTS foo1;
DROP TABLE IF EXISTS foo2;

PRAGMA foreign_keys = ON;

---------------------------------------------------------------------------

CREATE TABLE foo1 ( id INTEGER PRIMARY KEY, -- a comment
                    mybool BOOLEAN,
                    myint INTEGER,
                    mytext TEXT,
                    myreal REAL
                    );

---------------------------------------------------------------------------

CREATE TABLE foo2 ( id INTEGER PRIMARY KEY,
                    foo1_id INTEGER,
                    anotherint INTEGER,
                    ts TEXT,  -- store date/time
                    baz REAL,
                    FOREIGN KEY(foo1_id) REFERENCES foo1(id)
                    );
