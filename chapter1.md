title: MySQL Cookbook 第一章 使用mysql客户端
date: 2016-04-05 15:56:30
tags: [MySQL]
---

# 1.0 介绍

This chapter describes mysql’s capabilities so that you can use it more effectively:

* Setting up a MySQL account for using the cookbook database
* Specifying connection parameters and using option files
* Executing SQL statements interactively and in batch mode
* Controlling mysql output format
* Using user-defined variables to save information

To try for yourself the examples shown in this book, you need a MySQL user account
and a database. The first two recipes in this chapter describe how to use mysql to set
those up, based on these assumptions:

* The MySQL server is running locally on your own system
* Your MySQL username and password are cbuser and cbpass
* Your database is named cookbook

# 1.1 Setting Up a MySQL User Account

**Problem**

You need an account for connecting to your MySQL server.

**Solution**

Use **CREATE USER** and **GRANT** statements to set up the account. Then use the account
name and password to make connections to the server.

**Discussion**

    % mysql -h localhost -u root -p
    Enter password: ******
    mysql> CREATE USER 'cbuser'@'localhost' IDENTIFIED BY 'cbpass';
    mysql> GRANT ALL ON cookbook.* TO 'cbuser'@'localhost';
    Query OK, 0 rows affected (0.09 sec)
    mysql> quit
    Bye

If you plan to connect to the server from another
host, substitute that host in the CREATE USER and GRANT statements. For example, if you’ll connect to the server from a host named myhost.example.com, the statements look like
this:

    mysql> CREATE USER 'cbuser'@'myhost.example.com' IDENTIFIED BY 'cbpass';
    mysql> GRANT ALL ON cookbook.* TO 'cbuser'@'myhost.example.com';

I’m assuming that you can already connect as the MySQL root user because CREATE USER and
GRANT can be used only by a user such as root that has the administrative privileges
needed to set up other user accounts.

After creating the cbuser account, verify that you can use it to connect to the MySQL
server. From the host that was named in the CREATE USER statement, run the following
command to do this (the host named after -h should be the host where the MySQL
server is running):

    % mysql -h localhost -u cbuser -p
    Enter password: cbpass

# 1.2 Creating a Database and a Sample Table

**Problem**

You want to create a database and set up tables within it.

**Solution**

Use a CREATE DATABASE statement to create the database, a CREATE TABLE statement for
each table, and INSERT statements to add rows to the tables.

**Discussion**

Connect to the MySQL server as shown at the end of Recipe 1.1, then create the database
like this:

    mysql> CREATE DATABASE cookbook;

Now that you have a database, you can create tables in it. First, select cookbook as the
default database:

    mysql> USE cookbook;

Then create a simple table:

    mysql> CREATE TABLE limbs (thing VARCHAR(20), legs INT, arms INT);

And populate it with a few rows:

    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('human',2,2);
    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('insect',6,0);
    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('squid',0,10);
    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('fish',0,0);
    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('centipede',100,0);
    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('table',4,0);
    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('armchair',4,2);
    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('phonograph',0,1);
    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('tripod',3,0);
    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('Peg Leg Pete',1,2);
    mysql> INSERT INTO limbs (thing,legs,arms) VALUES('space alien',NULL,NULL);

Verify that the rows were added to the limbs table by executing a SELECT statement:

    mysql> SELECT * FROM limbs;
    +--------------+------+------+
    | thing        | legs | arms |
    +--------------+------+------+
    | human        |    2 |    2 | 
    | insect       |    6 |    0 | 
    | squid        |    0 |   10 | 
    | fish         |    0 |    0 | 
    | centipede    |  100 |    0 | 
    | table        |    4 |    0 | 
    | armchair     |    4 |    2 | 
    | phonograph   |    0 |    1 | 
    | tripod       |    3 |    0 | 
    | Peg Leg Pete |    1 |    2 | 
    | space alien  | NULL | NULL | 
    +--------------+------+------+

# 1.3 What to Do if mysql Cannot Be Found

**Problem**

When you invoke mysql from the command line, your command interpreter can’t find
it.

**Solution**

Add the directory where mysql is installed to your PATH setting. Then you can run mysql
from any directory easily

**Discussion**

One way to tell your command interpreter where to find mysql is to type its full pathname
each time you run it. The command might look like this under Unix:

    % /usr/local/mysql/bin/mysql

Or like this under Windows:

    C:\> "C:\Program Files\MySQL\MySQL Server 5.6\bin\mysql"

A better solution is to modify your PATH search-path environment variable, which
specifies directories where the command interpreter looks for commands. Add to the
PATH value the directory where mysql is installed. Then you can invoke mysql from any
location by entering only its name, which eliminates pathname typing. 

A significant additional benefit of being able to easily run mysql from anywhere is that
you need not put your datafiles in the directory where mysql is located. You’re free to
organize your files in a way that makes sense to you, not a way imposed by some artificial
necessity.

# 1.4. Specifying mysql Command Options

Problem
When you invoke the mysql program without command options, it exits immediately
with an “access denied” message.
Solution
You must specify connection parameters. Do this on the command line, in an option
file, or using a mix of the two.

If you invoke mysql with no command options, the result may be an “access denied”
error. To avoid that, connect to the MySQL server as shown in Recipe 1.1, using mysql
like this:

    % mysql -h localhost -u cbuser -p
    Enter password: cbpass

Each option is the single-dash “short” form: -h and -u to specify the hostname and
username, and -p to be prompted for the password. There are also corresponding
double-dash “long” forms: --host, --user, and --password. Use them like this:

    % mysql --host=localhost --user=cbuser --password
    Enter password: cbpass

To see all options that mysql supports, use this command:

    % mysql --help

The way you specify command options for mysql also applies to other MySQL programs
such as mysqldump and mysqladmin. For example, to generate a dump file named
cookbook.sql that contains a backup of the tables in the cookbook database, execute
mysqldump like this:

    % mysqldump -h localhost -u cbuser -p cookbook > cookbook.sql
    Enter password: cbpass

Some operations require an administrative MySQL account. The mysqladmin program
can perform operations that are available only to the MySQL root account. For example,
to stop the server, invoke mysqladmin as follows:

    % mysqladmin -h localhost -u root -p shutdown
    Enter password: ← enter MySQL root account password here

If the value that you use for an option is the same as its default value, you can omit the
option. However, there is no default password. If you like, you can specify the password
directly on the command line by using -ppassword (with no space between the option and the password) or --password=password. I don’t recommend this because the pass‐
word is visible to onlookers and, on multiple-user systems, may be discoverable to other
users who run tools such as ps that report process information.

Because the default host is localhost, the same value we’ve been specifying explicitly,
you can omit the -h (or --host) option from the command line:

    % mysql -u cbuser -p

But suppose that you’d really rather not specify any options. How can you get mysql to
“just know” what values to use? That’s easy because MySQL programs support option
files:

* If you put an option in an option file, you need not specify it on the command line
each time you invoke a given program.
* You can mix command-line and option-file options. This enables you to store the
most commonly used option values in a file but override them as desired on the
command line.

>   **The Meaning of localhost in MySQL**

>Under Unix, MySQL programs behave
differently: by convention, they treat the hostname localhost specially and attempt to
connect to the local server using a Unix domain socket file. To force a TCP/IP connection
to the local server, use the IP address 127.0.0.1 (or ::1 if your system is configured to
support IPv6) rather than the hostname localhost. 

>The default port number is 3306 for TCP/IP connections. The pathname for the Unix
domain socket varies, although it’s often /tmp/mysql.sock. To name the socket file path‐
name explicitly, use -S file_name or --socket=file_name.

## Specifying connection parameters using option files

To avoid entering options on the command line each time you invoke mysql, put them
in an option file for mysql to read automatically. Option files are plain-text files:

* Under Unix, your personal option file is named .my.cnf in your home directory.
There are also site-wide option files that administrators can use to specify parameters that apply globally to all users. You can use the my.cnf file in the /etc or /etc/
mysql directory, or in the etc directory under the MySQL installation directory.

* Under Windows, files you can use include the my.ini or my.cnf file in your MySQL
installation directory (for example, C:\Program Files\MySQL\MySQL Server 5.6),
your Windows directory (likely C:\WINDOWS), or the C:\ directory.

The following example illustrates the format used in MySQL option files:

    # general client program connection options
    [client]
    host = localhost
    user = cbuser
    password = cbpass
    # options specific to the mysql program
    [mysql]
    skip-auto-rehash
    pager="/usr/bin/less -E" # specify pager for interactive mode

With connection parameters listed in the [client] group as just shown, you can con‐
nect as cbuser by invoking mysql with no options on the command line:

    % mysql

To find out which options the mysql program will read from option files, use this com‐
mand:

    % mysql --print-defaults

You can also use the my_print_defaults utility, which takes as arguments the names of
the option-file groups that it should read. For example, mysqldump looks in both the
[client] and [mysqldump] groups for options. To check which option-file settings are
in those groups, use this command:

    % my_print_defaults client mysqldump

## Mixing command-line and option-file parameters

It’s possible to mix command-line options and options in option files. Perhaps you want
to list your username and server host in an option file, but would rather not store your
password there. That’s okay; MySQL programs first read your option file to see what
connection parameters are listed there, then check the command line for additional
parameters. This means you can specify some options one way, and some the other way.
For example, you can list your username and hostname in an option file, but use a
password option on the command line:

    % mysql -p
    Enter password: ← enter your password here

Command-line parameters take precedence over parameters found in your option file,
so to override an option file parameter, just specify it on the command line. For example,
you can list your regular MySQL username and password in the option-file for generalpurpose use. Then, if you must connect on occasion as the MySQL root user, specify
the user and password options on the command line to override the option-file values:

    % mysql -u root -p
    Enter password: ← enter MySQL root account password here

To explicitly specify “no password” when there is a nonempty password in the option
file, use --skip-password on the command line:

    % mysql --skip-password

## Protecting option files from other users

On a multiple-user operating system such as Unix, protect the option file located in
your home directory to prevent other users from reading it and finding out how to
connect to MySQL using your account. Use chmod to make the file private by setting
its mode to enable access only by yourself. Either of the following commands do this:

    % chmod 600 .my.cnf
    % chmod go-rwx .my.cnf

# 1.5. Executing SQL Statements Interactively

**Problem**

You’ve started mysql. Now you want to send SQL statements to the MySQL server to be
executed.

**Solution**

Just type them in, letting mysql know where each one ends. Or specify “one-liners”
directly on the command line.

**Discussion**

The semicolon is the most common terminator,
but you can also use \g (“go”) as a synonym for the semicolon.

    mysql> SELECT NOW();
    +---------------------+
    | NOW()               |
    +---------------------+
    | 2016-04-05 20:20:31 |
    +---------------------+
    1 row in set (0.00 sec)

    mysql> SELECT
        -> NOW()\g
    +---------------------+
    | NOW()               |
    +---------------------+
    | 2016-04-05 20:20:40 |
    +---------------------+
    1 row in set (0.00 sec)

The ; and \g statement terminators are not part of the statement itself. They’re con‐
ventions used by the mysql program, which recognizes these terminators and strips
them from the input before sending the statement to the MySQL server.

Some statements generate output lines that are so long they take up more than one line
on your terminal, which can make query results difficult to read. To avoid this problem,
generate “vertical” output by terminating the statement with \G rather than with ; or
\g . The output shows column values on separate lines:

    mysql> SHOW FULL COLUMNS FROM limbs LIKE 'thing'\G
    *************************** 1. row ***************************
         Field: thing
          Type: varchar(20)
     Collation: latin1_swedish_ci
          Null: YES
           Key: 
       Default: NULL
         Extra: 
    Privileges: select,insert,update,references
       Comment: 

To produce vertical output for all statements executed within a session, invoke mysql
with the -E (or --vertical ) option. To produce vertical output only for those results
that exceed your terminal width, use --auto-vertical-output .

To execute a statement directly from the command line, specify it using the -e (or --
execute ) option. This is useful for “one-liners.” For example, to count the rows in the
limbs table, use this command:

    [xiaoyu@ubuntu:~]$ mysql -u cbuser -p -e "SELECT COUNT(*) FROM limbs" cookbook
    Enter password: 
    +----------+
    | COUNT(*) |
    +----------+
    |       11 |
    +----------+

To execute multiple statements, separate them with semicolons:

    [xiaoyu@ubuntu:~]$ mysql -u cbuser -p -e "SELECT COUNT(*) FROM limbs; SELECT NOW()" cookbook
    Enter password: 
    +----------+
    | COUNT(*) |
    +----------+
    |       11 |
    +----------+
    +---------------------+
    | NOW()               |
    +---------------------+
    | 2016-04-05 20:27:14 |
    +---------------------+

# 1.6. Executing SQL Statements Read from a File or Program

**Problem**

You want mysql to read statements stored in a file so that you need not enter them
manually. Or you want mysql to read the output from another program.

**Solution**

To read a file, redirect mysql’s input, or use the source command. To read from a pro‐
gram, use a pipe.

**Discussion**

By default, the mysql program reads input interactively from the terminal, but you can
feed it statements using other input sources such as a file or program.

To create an SQL script for mysql to execute in batch mode, put your statements in a
text file. Then invoke mysql and redirect its input to read from that file:

    % mysql cookbook < file_name

Statements read from an input file substitute for what you’d normally enter interactively
by hand, so they must be terminated with ; , \g , or \G , just as if you were entering them
manually. Interactive and batch modes do differ in default output format. For interactive
mode, the default is tabular (boxed) format. For batch mode, the default is tab-delimited
format. To override the default, use the appropriate command option

To execute the statements in this SQL script file, change location into the tables directory
of the recipes distribution and run this command:

    % mysql cookbook < limbs.sql

The command just shown illustrates how to specify an input file for mysql on the com‐
mand line. Alternatively, to read a file of SQL statements from within a mysql session,
use a source filename command (or \. filename , which is synonymous):

    mysql> source limbs.sql;
    mysql> \. limbs.sql;

SQL scripts can themselves include source or \. commands to include other scripts.
This gives you additional flexibility, but take care to avoid source loops.

A file to be read by mysql need not be written by hand; it could be program generated.
For example, the mysqldump utility generates database backups by writing a set of SQL
statements that re-create the database. To reload mysqldump output, feed it to mysql.
For example, you can copy a database over the network to another MySQL server like
this:

    % mysqldump cookbook > dump.sql
    % mysql -h other-host.example.com cookbook < dump.sql

mysql can also read a pipe, so it can take output from other programs as its input. Any
command that produces output consisting of properly terminated SQL statements can
be used as an input source for mysql. The dump-and-reload example can be rewritten to connect the two programs directly with a pipe, avoiding the need for an intermediary
file:

    % mysqldump cookbook | mysql -h other-host.example.com cookbook

Program-generated SQL also can be useful for populating a table with test data without
writing the INSERT statements by hand. Create a program that generates the statements,
then send its output to mysql using a pipe:

    % generate-test-data | mysql cookbook

# 1.7. Controlling mysql Output Destination and Format

Problem

You want mysql output to go somewhere other than your screen. And you don’t neces‐
sarily want the default output format.

Solution

Redirect the output to a file, or use a pipe to send the output to a program. You can also
control other aspects of mysql output to produce tabular, tab-delimited, HTML, or XML
output; suppress column headers; or make mysql more or less verbose.

Discussion

Unless you send mysql output elsewhere, it goes to your screen. To save output from
mysql in a file, use your shell’s redirection capability:
% mysql cookbook > outputfile

If you run mysql interactively with the output redirected, you can’t see what you type,
so in this case you usually also read the input from a file (or another program):
% mysql cookbook < inputfile > outputfile
To send the output to another program (for example, to mail query results to someone),
use a pipe:
% mysql cookbook < inputfile | mail paul

## Producing tabular or tab-delimited output

mysql chooses its default output format by whether it runs interactively or noninterac‐
tively. For interactive use, mysql writes output to the terminal using tabular (boxed)
format:

    mysql> SELECT * FROM limbs WHERE legs = 0;
    +------------+------+------+
    | thing      | legs | arms |
    +------------+------+------+
    | squid      |    0 |   10 |
    | fish       |    0 |    0 |
    | phonograph |    0 |    1 |
    +------------+------+------+
    3 rows in set (0.03 sec)

For noninteractive use (when the input or output is redirected), mysql writes tab-
delimited output:

    [xiaoyu@ubuntu:~]$ echo "SELECT * FROM limbs WHERE legs = 0" | mysql -u cbuser -p cookbook
    Enter password: 
    thing   legs    arms
    squid   0   10
    fish    0   0
    phonograph  0   1

Because mysql runs noninteractively in that context, it produces tab-delimited output,
which the mail recipient may find more difficult to read than tabular output. Use the -
t (or --table ) option to produce more readable tabular output:

    [xiaoyu@ubuntu:~]$ echo "SELECT * FROM limbs WHERE legs = 0" | mysql -u cbuser -p cookbook -t
    Enter password: 
    +------------+------+------+
    | thing      | legs | arms |
    +------------+------+------+
    | squid      |    0 |   10 |
    | fish       |    0 |    0 |
    | phonograph |    0 |    1 |
    +------------+------+------+

The inverse operation is to produce batch (tab-delimited) output in interactive mode.
To do this, use -B or --batch .

## Producing HTML or XML output

mysql generates an HTML table from each query result set if you use the -H (or --
html ) option. This enables you to easily produce output for inclusion in a web page that
shows a query result. Here’s an example (with line breaks added to make the output
easier to read):

    [xiaoyu@ubuntu:~]$ mysql -u cbuser -p -H -e "SELECT * FROM limbs WHERE legs = 0"  cookbook
    Enter password: 
    <TABLE BORDER=1><TR><TH>thing</TH><TH>legs</TH><TH>arms</TH></TR><TR><TD>squid</TD><TD>0</TD><TD>10</TD></TR><TR><TD>fish</TD><TD>0</TD><TD>0</TD></TR><TR><TD>phonograph</TD><TD>0</TD><TD>1</TD></TR></TABLE>

You can save the output in a file, then view it with a web browser. For example, on Mac
OS X, do this:

    % mysql -H -e "SELECT * FROM limbs WHERE legs=0" cookbook > limbs.html
    % open -a safari limbs.html

To generate an XML document instead of HTML, use the -X (or --xml ) option:

    [xiaoyu@ubuntu:~]$ mysql -u cbuser -p -X -e "SELECT * FROM limbs WHERE legs = 0"  cookbook > limbs.xml

Use the transform like this:

    % mysql -X -e "SELECT * FROM limbs WHERE legs=0" cookbook \
    | xsltproc mysql-xml.xsl -
    Query: SELECT * FROM limbs WHERE legs=0
    Result set:
    squid, 0, 10
    fish, 0, 0
    phonograph, 0, 1

The -H , --html -X , and --xml options produce output only for statements that generate
a result set, not for statements such as INSERT or UPDATE .

## Suppressing column headings in query output

To create output that contains only
data values, suppress the header row with the --skip-column-names option:

    % mysql --skip-column-names -e "SELECT arms FROM limbs" cookbook | summarize

Specifying the “silent” option ( -s or --silent ) twice achieves the same effect:

    % mysql -ss -e "SELECT arms FROM limbs" cookbook | summarize

## Specifying the output column delimiter

In noninteractive mode, mysql separates output columns by tabs and there is no option
for specifying the output delimiter. To produce output that uses a different delimiter,
postprocess mysql output. Suppose that you want to create an output file for use by a
program that expects values to be separated by colon characters ( : ) rather than tabs.
Under Unix, you can convert tabs to arbitrary delimiters by using a utility such as tr or
sed. Any of the following commands change tabs to colons ( TAB indicates where you
type a tab character):

    % mysql cookbook < inputfile | sed -e "s/TAB/:/g" > outputfile
    % mysql cookbook < inputfile | tr "TAB" ":" > outputfile
    % mysql cookbook < inputfile | tr "\011" ":" > outputfile

sed is more powerful than tr because it understands regular expressions and permits
multiple substitutions. This is useful for producing output in something like comma-
separated values (CSV) format, which requires three substitutions:

1. Escape any quote characters that appear in the data by doubling them, so that when
you use the resulting CSV file, they won’t be interpreted as column delimiters.
2. Change the tabs to commas.
3. Surround column values with quotes.

sed permits all three substitutions to be performed in a single command line:

    % mysql cookbook < inputfile \
    | sed -e 's/"/""/g' -e 's/TAB/","/g' -e 's/^/"/' -e 's/$/"/' > outputfile

## Controlling mysql’s verbosity level

To tell mysql to be more verbose, use -v or --
verbose , specifying the option multiple times for increasing verbosity. Try the following
commands to see how the output differs:

    [xiaoyu@ubuntu:~]$ mysql -u cbuser -p -e "SELECT NOW()"
    [xiaoyu@ubuntu:~]$ mysql -u cbuser -p -e "SELECT NOW()" -v
    [xiaoyu@ubuntu:~]$ mysql -u cbuser -p -e "SELECT NOW()" -vv
    [xiaoyu@ubuntu:~]$ mysql -u cbuser -p -e "SELECT NOW()" -vvv

The counterparts of -v and --verbose are -s and --silent , which also can be used
multiple times for increased effect.

# 1.8. Using User-Defined Variables in SQL Statements 

Problem

You want to use a value in one statement that is produced by an earlier statement.

Solution

Save the value in a user-defined variable to store it for later use.

Discussion

To save a value returned by a SELECT statement, assign it to a user-defined variable. This
enables you to refer to it in other statements later in the same session (but not across
sessions). User variables are a MySQL-specific extension to standard SQL. They will not
work with other database engines.

To assign a value to a user variable within a SELECT statement, use @var_name := val
ue syntax. The variable can be used in subsequent statements wherever an expression
is permitted, such as in a WHERE clause or in an INSERT statement.

    mysql> SELECT @max_limbs := MAX(arms + legs) FROM limbs;
    +--------------------------------+
    | @max_limbs := MAX(arms + legs) |
    +--------------------------------+
    |                            100 |
    +--------------------------------+
    1 row in set (0.02 sec)

    mysql> SELECT * FROM limbs WHERE arms + legs = @max_limbs;
    +-----------+------+------+
    | thing     | legs | arms |
    +-----------+------+------+
    | centipede |  100 |    0 |
    +-----------+------+------+
    1 row in set (0.03 sec)

Another use for a variable is to save the result from LAST_INSERT_ID() after creating a
new row in a table that has an AUTO_INCREMENT column:

    mysql> SELECT @last_id := LAST_INSERT_ID();

LAST_INSERT_ID() returns the most recent AUTO_INCREMENT value. By saving it in a
variable, you can refer to the value several times in subsequent statements, even if you
issue other statements that create their own AUTO_INCREMENT values and thus change
the value returned by LAST_INSERT_ID() .

User variables hold single values. If a statement returns multiple rows, the value from
the last row is assigned:

    mysql> SELECT @name := thing FROM limbs WHERE legs = 0;
    +----------------+
    | @name := thing |
    +----------------+
    | squid          |
    | fish           |
    | phonograph     |
    +----------------+
    3 rows in set (0.00 sec)

    mysql> SELECT @name;
    +------------+
    | @name      |
    +------------+
    | phonograph |
    +------------+
    1 row in set (0.00 sec)

If the statement returns no rows, no assignment takes place, and the variable retains its
previous value. If the variable has not been used previously, its value is NULL :

    mysql> SELECT @name2 := thing FROM limbs WHERE legs < 0;
    Empty set (0.00 sec)

    mysql> SELECT @name2;
    +--------+
    | @name2 |
    +--------+
    | NULL   |
    +--------+
    1 row in set (0.00 sec)

To set a variable explicitly to a particular value, use a SET statement. SET syntax can use
either := or = as the assignment operator:

    mysql> SET @sum = 4 + 7;
    Query OK, 0 rows affected (0.00 sec)

    mysql> SELECT @sum;
    +------+
    | @sum |
    +------+
    |   11 |
    +------+
    1 row in set (0.00 sec)

You can assign a SELECT result to a variable, provided that you write it as a scalar sub‐
query (a query within parentheses that returns a single value):

    mysql> SET @max_limbs = (SELECT MAX(arms+legs) FROM limbs);

User variable names are not case sensitive:

    mysql> SET @x = 1, @X = 2; SELECT @x, @X;
    Query OK, 0 rows affected (0.00 sec)

    +------+------+
    | @x   | @X   |
    +------+------+
    |    2 |    2 |
    +------+------+
    1 row in set (0.00 sec)

User variables can appear only where expressions are permitted, not where constants
or literal identifiers must be provided. It’s tempting to attempt to use variables for such
things as table names, but it doesn’t work. For example, if you try to generate a temporary
table name using a variable as follows, it fails:

    mysql> SET @tbl_name = CONCAT('tmp_tbl_', CONNECTION_ID());
    mysql> CREATE TABLE @tbl_name (int_col INT);
    ERROR 1064: You have an error in your SQL syntax near '@tbl_name
    (int_col INT)'

SET is also used to assign values to stored program parameters and local variables, and
to system variables.



