/*
  04.02.2016 Andreas Weber
  fill dummy tables

  https://www.sqlite.org/docs.html
*/

INSERT INTO foo1 ( mybool,
                   myint,
                   mytext,
                   myreal)
       VALUES (1, 1234, "hello world", 3.1415),
              (0, 6666, "foobar", 2.718);


INSERT INTO foo2 ( foo1_id,
                   anotherint,
                   ts,
                   baz)
       VALUES (1, 444, "2015-11-17 11:01:02", 2.43),
              (2, 888, datetime (), -5.67);
