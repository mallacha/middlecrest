#
# Build SQLite
#

#
# Compile-time options
#
#DSQLITE_THREADSAFE=0 : Omits mutex and thread-safety logic. This compile-time option makes the most difference in optimizing the performance.
#
#DSQLITE_DEFAULT_MEMSTATUS=0 : Disables memory tracking. Helps malloc routines run much faster and helps make the entire library faster.
#
#DSQLITE_OMIT_DEPRECATED : Omits deprecated code.
#
#DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1 : Faster transaction commits in WAL mode while ensuring database safety.
#
#DSQLITE_LIKE_DOESNT_MATCH_BLOBS : Simplifies LIKE optimization and allows them to run faster.
#
#DSQLITE_MAX_EXPR_DEPTH=0 : Simplifies code and helps parse tree run faster.
#
#DSQLITE_OMIT_DECLTYPE : Allows prepared statements to consume less memory by omittig a seldom-needed ability.
#
#DSQLITE_OMIT_PROGRESS_CALLBACK : Helps SQL statements run slightly faster
#
#DSQLITE_OMIT_SHARED_CACHE : Omits shared cache which eliminates conditionals in performance-critical sections of code and gives a noticible performance.
#
#Above compile-time options decrease code size 3% and estimated to use 5% fewer CPU cycles
#
#DSQLITE_TEMP_STORE=2 : Temporary files use memory by default but can be overridden to use disk.
#
#DSQLITE_OMIT_AUTOINIT : Removes backwards-compatibility feature. Must initialize database each time.
#
#DSQLITE_OMIT_COMPILEOPTION_DIAGS : Omits ability to determine compile-time options.


all: shell.c sqlite3.c
	gcc -DSQLITE_THREADSAFE=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1 -DSQLITE_LIKE_DOESNT_MATCH_BLOBS -DSQLITE_MAX_EXPR_DEPTH=0 -DSQLITE_OMIT_DECLTYPE -DSQLITE_OMIT_PROGRESS_CALLBACK -DSQLITE_OMIT_SHARED_CACHE -DSQLITE_TEMP_STORE=2 -DSQLITE_OMIT_AUTOINIT -DSQLITE_OMIT_COMPILEOPTION_DIAGS sqlite3.c shell.c -o sqlite.o
