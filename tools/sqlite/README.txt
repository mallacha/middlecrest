-------------------------------
For Linux/Mac/Unix-like users:
-------------------------------

type 'make'

Brute simple Makefile that includes all the necessary compile-time options preferred by this project



-------------------------------
For Windows users (or Linux/Mac/Unix-like users more comfortable with IDEs than Makefile):
-------------------------------

Uses Code::Blocks IDE to build, like the rest of the project.

Click on 'sqlite.cbp'

Code::Blocks will open and already have the appropriate files laid out. Hit the compile button.

object file named 'sqlite.o' will be dumped into an obj/ directory. Move sqlite.o into this directory (tools/sqlite) so it can be used by the rest of the project.



-------------------------------
Advanced
-------------------------------

Can download newer SQLite source from https://sqlite.org/download.html. Unpack the files into tools/sqlite and make the necessary adjustments to build a custom version.
