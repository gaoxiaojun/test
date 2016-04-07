title: MySQL Cookbook 第3章 从表中查询数据
date: 2016-04-07 12:14:29
tags: [MySQL]
---

# 2.0. 介绍

## MySQL Client API Architecture

Each MySQL programming interface covered in this book uses a two-level architecture:

* The upper level provides database-independent methods that implement database
access in a portable way that’s the same whether you use MySQL, PostgreSQL, Ora‐
cle, or whatever.
* The lower level consists of a set of drivers, each of which implements the details for
a single database system.

# 3.1. Specifying Which Columns and Rows to Select

Problem

You want to display specific columns and rows from a table.

Solution

To indicate which columns to display, name them in the output column list. To indicate
which rows to display, use a WHERE clause that specifies conditions that rows must satisfy.

Discussion

The simplest way to display columns from a table is to use SELECT * FROM tbl_name. The
* specifier is a shortcut that means “all columns”:

    mysql> SELECT * FROM mail; 
    +---------------------+---------+---------+---------+---------+---------+
    | t                   | srcuser | srchost | dstuser | dsthost | size    |
    +---------------------+---------+---------+---------+---------+---------+
    | 2014-05-11 10:15:08 | barb    | saturn  | tricia  | mars    |   58274 | 
    | 2014-05-12 12:48:13 | tricia  | mars    | gene    | venus   |  194925 | 
    | 2014-05-12 15:02:49 | phil    | mars    | phil    | saturn  |    1048 | 
    | 2014-05-12 18:59:18 | barb    | saturn  | tricia  | venus   |     271 | 

Using * is easy, but you cannot select only certain columns or control column display
order. Naming columns explicitly enables you to select only the ones of interest, in any
order. This query omits the recipient columns and displays the sender before the date
and size:

    mysql> SELECT srcuser, srchost, t, size FROM mail;
    +---------+---------+---------------------+---------+
    | srcuser | srchost | t                   | size    |
    +---------+---------+---------------------+---------+
    | barb    | saturn  | 2014-05-11 10:15:08 |   58274 | 
    | tricia  | mars    | 2014-05-12 12:48:13 |  194925 | 
    | phil    | mars    | 2014-05-12 15:02:49 |    1048 | 
    | barb    | saturn  | 2014-05-12 18:59:18 |     271 | 

Unless you qualify or restrict a SELECT query in some way, it retrieves every row in your
table. To be more precise, provide a WHERE clause that specifies one or more conditions
that rows must satisfy.

Conditions can test for equality, inequality, or relative ordering. For some types of data,
such as strings, you can use pattern matches. The following statements select columns
from rows in the mail table containing srchost values that are exactly equal to the string
'venus' or that begin with the letter 's':

    mysql> SELECT t, srcuser, srchost FROM mail WHERE srchost = 'venus';
    +---------------------+---------+---------+
    | t                   | srcuser | srchost |
    +---------------------+---------+---------+
    | 2014-05-14 09:31:37 | gene    | venus   | 
    | 2014-05-14 14:42:21 | barb    | venus   | 
    | 2014-05-15 08:50:57 | phil    | venus   | 
    | 2014-05-16 09:00:28 | gene    | venus   | 
    | 2014-05-16 23:04:19 | phil    | venus   | 
    +---------------------+---------+---------+
    mysql> SELECT t, srcuser, srchost FROM mail WHERE srchost LIKE 's%';
    +---------------------+---------+---------+
    | t                   | srcuser | srchost |
    +---------------------+---------+---------+
    | 2014-05-11 10:15:08 | barb    | saturn  | 
    | 2014-05-12 18:59:18 | barb    | saturn  | 
    | 2014-05-14 17:03:01 | tricia  | saturn  | 
    | 2014-05-15 17:35:31 | gene    | saturn  | 
    | 2014-05-19 22:21:51 | gene    | saturn  | 
    +---------------------+---------+---------+

The LIKE operator in the previous query performs a pattern match, where % acts as a
wildcard that matches any string. Recipe 5.8 discusses pattern matching further.

A WHERE clause can test multiple conditions and different conditions can test different
columns. The following statement finds messages sent by barb to tricia:

    mysql> SELECT * FROM mail WHERE srcuser = 'barb' AND dstuser = 'tricia';
    +---------------------+---------+---------+---------+---------+-------+
    | t                   | srcuser | srchost | dstuser | dsthost | size  |
    +---------------------+---------+---------+---------+---------+-------+
    | 2014-05-11 10:15:08 | barb    | saturn  | tricia  | mars    | 58274 | 
    | 2014-05-12 18:59:18 | barb    | saturn  | tricia  | venus   |   271 | 
    +---------------------+---------+---------+---------+---------+-------+

Output columns can be calculated by evaluating expressions. This query combines the
srcuser and srchost columns using CONCAT() to produce composite values in email
address format:

    mysql> SELECT t, CONCAT(srcuser,'@',srchost), size FROM mail;
    +---------------------+-----------------------------+---------+
    | t                   | CONCAT(srcuser,'@',srchost) | size    |
    +---------------------+-----------------------------+---------+
    | 2014-05-11 10:15:08 | barb@saturn                 |   58274 | 
    | 2014-05-12 12:48:13 | tricia@mars                 |  194925 | 
    | 2014-05-12 15:02:49 | phil@mars                   |    1048 | 
    | 2014-05-12 18:59:18 | barb@saturn                 |     271 | 

# 3.2. Naming Query Result Columns

Problem

The column names in a query result are unsuitable, ugly, or difficult to work with.

Solution

Use aliases to choose your own column names.

Discussion

When you retrieve a result set, MySQL gives every output column a name. (That’s how
the mysql program gets the names you see displayed in the initial row of column headers
in result set output.) By default, MySQL assigns the column names specified in the CREATE TABLE or ALTER TABLE statement to output columns, but if these defaults are not
suitable, you can use column aliases to specify your own names.

If an output column comes directly from a table, MySQL uses the table column name
for the output column name. The following statement selects four table columns, the
names of which become the corresponding output column names:

    mysql> SELECT srcuser, srchost, t, size FROM mail;
    +---------+---------+---------------------+---------+
    | srcuser | srchost | t                   | size    |
    +---------+---------+---------------------+---------+
    | barb    | saturn  | 2014-05-11 10:15:08 |   58274 | 
    | tricia  | mars    | 2014-05-12 12:48:13 |  194925 | 
    | phil    | mars    | 2014-05-12 15:02:49 |    1048 | 
    | barb    | saturn  | 2014-05-12 18:59:18 |     271 |

If you generate a column by evaluating an expression, the expression itself is the column
name. This can produce long and unwieldy names in result sets, as illustrated by the
following statement that uses one expression to reformat the dates in the t column, and
another to combine srcuser and srchost into email address format:

    mysql> SELECT
        -> DATE_FORMAT(t, '%M %e, %Y'), CONCAT(srcuser, '@', srchost), size
        -> FROM mail;
    +-----------------------------+-------------------------------+---------+
    | DATE_FORMAT(t, '%M %e, %Y') | CONCAT(srcuser, '@', srchost) | size    |
    +-----------------------------+-------------------------------+---------+
    | May 11, 2014                | barb@saturn                   |   58274 | 
    | May 12, 2014                | tricia@mars                   |  194925 | 
    | May 12, 2014                | phil@mars                     |    1048 | 
    | May 12, 2014                | barb@saturn                   |     271 |

To choose your own output column name, use an AS name clause to specify a column
alias (the keyword AS is optional). The following statement retrieves the same result as
the previous one, but renames the first column to date_sent and the second to sender:

    mysql> SELECT
        -> DATE_FORMAT(t,'%M %e, %Y') AS date_sent,
        -> CONCAT(srcuser,'@',srchost) AS sender,
        -> size FROM mail;
    +--------------+---------------+---------+
    | date_sent    | sender        | size    |
    +--------------+---------------+---------+
    | May 11, 2014 | barb@saturn   |   58274 | 
    | May 12, 2014 | tricia@mars   |  194925 | 
    | May 12, 2014 | phil@mars     |    1048 | 
    | May 12, 2014 | barb@saturn   |     271 | 

The aliases make the column names more concise, easier to read, and more meaningful.
Aliases are subject to a few restrictions. For example, they must be quoted if they are
SQL keywords, entirely numeric, or contain spaces or other special characters (an alias
can consist of several words if you want to use a descriptive phrase). The following
statement retrieves the same data values as the preceding one but uses phrases to name
the output columns:

    mysql> SELECT
        -> DATE_FORMAT(t,'%M %e, %Y') AS 'Date of message',
        -> CONCAT(srcuser,'@',srchost) AS 'Message sender',
        -> size AS 'Number of bytes' FROM mail;

If MySQL complains about a single-word alias, the word probably is reserved. Quoting
the alias should make it legal:

    mysql> SELECT 1 AS INTEGER;
    You have an error in your SQL syntax near 'INTEGER'
    mysql> SELECT 1 AS 'INTEGER';
    +---------+
    | INTEGER |
    +---------+
    |       1 | 
    +---------+

You cannot refer to column aliases in a WHERE clause. Thus, the following statement is
illegal:

    mysql> SELECT t, srcuser, dstuser, size/1024 AS kilobytes
        -> FROM mail WHERE kilobytes > 500;

# 3.3. Sorting Query Results

Problem

Your query results aren’t sorted the way you want.

Solution

MySQL can’t read your mind. Use an ORDER BY clause to tell it how to sort result rows.

Discussion

When you select rows, the MySQL server is free to return them in any order unless you
instruct it otherwise by saying how to sort the result. There are lots of ways to use sorting
techniques, as Chapter 7 explores in detail. Briefly, to sort a result set, add an ORDER BY
clause that names the column or columns to use for sorting. This statement names
multiple columns in the ORDER BY clause to sort rows by host and by user within each
host:

    mysql> SELECT * FROM mail WHERE dstuser = 'tricia'
        -> ORDER BY srchost, srcuser;
    +---------------------+---------+---------+---------+---------+--------+
    | t                   | srcuser | srchost | dstuser | dsthost | size   |
    +---------------------+---------+---------+---------+---------+--------+
    | 2014-05-15 10:25:52 | gene    | mars    | tricia  | saturn  | 998532 | 
    | 2014-05-14 11:52:17 | phil    | mars    | tricia  | saturn  |   5781 | 
    | 2014-05-19 12:49:23 | phil    | mars    | tricia  | saturn  |    873 | 
    | 2014-05-11 10:15:08 | barb    | saturn  | tricia  | mars    |  58274 | 
    | 2014-05-12 18:59:18 | barb    | saturn  | tricia  | venus   |    271 | 
    +---------------------+---------+---------+---------+---------+--------+

To sort a column in reverse (descending) order, add the keyword DESC after its name in
the ORDER BY clause:

    mysql> SELECT * FROM mail WHERE size > 50000 ORDER BY size DESC;
    +---------------------+---------+---------+---------+---------+---------+
    | t                   | srcuser | srchost | dstuser | dsthost | size    |
    +---------------------+---------+---------+---------+---------+---------+
    | 2014-05-14 17:03:01 | tricia  | saturn  | phil    | venus   | 2394482 | 
    | 2014-05-15 10:25:52 | gene    | mars    | tricia  | saturn  |  998532 | 
    | 2014-05-12 12:48:13 | tricia  | mars    | gene    | venus   |  194925 | 
    | 2014-05-14 14:42:21 | barb    | venus   | barb    | venus   |   98151 | 
    | 2014-05-11 10:15:08 | barb    | saturn  | tricia  | mars    |   58274 | 
    +---------------------+---------+---------+---------+---------+---------+

# 3.4. Removing Duplicate Rows

Problem
Output from a query contains duplicate rows. You want to eliminate them.
Solution
Use DISTINCT.

Discussion

Some queries produce results containing duplicate rows. For example, to see who sent
mail, query the mail table like this:

    mysql> SELECT srcuser FROM mail;
    +---------+
    | srcuser |
    +---------+
    | barb    | 
    | tricia  | 
    | phil    | 
    | barb    | 
    | gene    | 
    | phil    | 
    | barb    | 
    | tricia  | 
    | gene    | 
    | phil    | 
    | gene    | 
    | gene    | 
    | gene    | 
    | phil    | 
    | phil    | 
    | gene    | 
    +---------+

That result is heavily redundant. To remove the duplicate rows and produce a set of
unique values, add DISTINCT to the query:

    mysql> SELECT DISTINCT srcuser FROM mail;
    +---------+
    | srcuser |
    +---------+
    | barb    | 
    | tricia  | 
    | phil    | 
    | gene    | 
    +---------+

To count the number of unique values in a column, use COUNT(DISTINCT):

    mysql> SELECT COUNT(DISTINCT srcuser) FROM mail;
    +-------------------------+
    | COUNT(DISTINCT srcuser) |
    +-------------------------+
    |                       4 | 
    +-------------------------+

DISTINCT works with multiple-column output, too. The following query shows which
dates are represented in the mail table:

    mysql> SELECT DISTINCT YEAR(t), MONTH(t), DAYOFMONTH(t) FROM mail;
    +---------+----------+---------------+
    | YEAR(t) | MONTH(t) | DAYOFMONTH(t) |
    +---------+----------+---------------+
    |    2014 |        5 |            11 | 
    |    2014 |        5 |            12 | 
    |    2014 |        5 |            14 | 
    |    2014 |        5 |            15 | 
    |    2014 |        5 |            16 | 
    |    2014 |        5 |            19 | 
    +---------+----------+---------------+

# 3.5. Working with NULL Values

Problem
You’re trying to to compare column values to NULL, but it isn’t working.
Solution
Use the proper comparison operators: IS NULL, IS NOT NULL, or <=>.

Discussion

Conditions that involve NULL are special because NULL means “unknown value.” Con‐
sequently, comparisons such as value = NULL or value <> NULL always produce a result
of NULL (not true or false) because it’s impossible to tell whether they are true or false.
Even NULL = NULL produces NULL because you can’t determine whether one unknown
value is the same as another.

    mysql> SELECT * FROM expt WHERE score = NULL;
    Empty set (0.01 sec)

    mysql> SELECT * FROM expt WHERE score <> NULL;
    Empty set (0.00 sec)

    mysql> SELECT * FROM expt WHERE score IS NULL;
    +---------+------+-------+
    | subject | test | score |
    +---------+------+-------+
    | Jane    | C    |  NULL | 
    | Jane    | D    |  NULL | 
    | Marvin  | D    |  NULL | 
    +---------+------+-------+
    mysql> SELECT * FROM expt WHERE score IS NOT NULL;
    +---------+------+-------+
    | subject | test | score |
    +---------+------+-------+
    | Jane    | A    |    47 | 
    | Jane    | B    |    50 | 
    | Marvin  | A    |    52 | 
    | Marvin  | B    |    45 | 
    | Marvin  | C    |    53 | 
    +---------+------+-------+

The MySQL-specific <=> comparison operator, unlike the = operator, is true even for
two NULL values:

    mysql> SELECT NULL = NULL, NULL <=> NULL;
    +-------------+---------------+
    | NULL = NULL | NULL <=> NULL |
    +-------------+---------------+
    |        NULL |             1 | 
    +-------------+---------------+

Sometimes it’s useful to map NULL values onto some other value that has more meaning
in the context of your application. For example, use IF() to map NULL onto the string
Unknown:

    mysql> SELECT subject, test, IF(score IS NULL,'Unknown',score) AS 'score'
        -> FROM expt;
    +---------+------+---------+
    | subject | test | score   |
    +---------+------+---------+
    | Jane    | A    | 47      | 
    | Jane    | B    | 50      | 
    | Jane    | C    | Unknown | 
    | Jane    | D    | Unknown | 
    | Marvin  | A    | 52      | 
    | Marvin  | B    | 45      | 
    | Marvin  | C    | 53      | 
    | Marvin  | D    | Unknown | 
    +---------+------+---------+

The preceding query can be written more concisely using IFNULL(), which tests its first
argument and returns it if it’s not NULL, or returns its second argument otherwise:

    SELECT subject, test, IFNULL(score,'Unknown') AS 'score'
    FROM expt;

In other words, these two tests are equivalent:

    IF(expr1 IS NOT NULL,expr1,expr2)
    IFNULL(expr1,expr2)

From a readability standpoint, IF() often is easier to understand than IFNULL(). From
a computational perspective, IFNULL() is more efficient because expr1 need not be
evaluated twice, as happens with IF().

