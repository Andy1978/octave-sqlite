autoload ("exec_sql", which ("sqlite3.oct"));

a = sqlite3 ("test/foobar.db")

#exec_sql (a, "INSERT INTO foo1 (mybool, myint, mytext, myreal)\
# VALUES (?1, ?2, ?3, ?4);", {[1 0], [333 444], {"hello", "world"}, [10.123, 11.234]});

#exec_sql (a, "INSERT INTO foo1 (mybool, myint, mytext, myreal)\
# VALUES (?1, ?2, ?3, ?4);", {1, 134, {"foo"}, 3*pi});

exec_sql (a, "SELECT * from foo1;");
