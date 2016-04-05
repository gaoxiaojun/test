title: MySQL Cookbook 第一章 使用mysql客户端
date: 2016-04-05 15:56:30
tags: [MySQL]

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




