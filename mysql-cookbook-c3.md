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

# 3.6. Writing Comparisons Involving NULL in Programs

Problem

You’re writing a program that looks for rows containing a specific value, but it fails when
the value is NULL.

Solution

Choose the proper comparison operator according to whether the comparison value is
or is not NULL.

Discussion

A comparison of score = NULL is never true, so that statement returns no rows. To take
into account the possibility that $score could be undef, construct the statement using
the appropriate comparison operator like this:

    $operator = defined ($score) ? "=" : "IS";
    $sth = $dbh->prepare ("SELECT * FROM expt WHERE score $operator ?");
    $sth->execute ($score);

For inequality tests, set $operator like this instead:

    $operator = defined ($score) ? "<>" : "IS NOT";

# 3.7. Using Views to Simplify Table Access

Problem

You want to refer to values calculated from expressions without writing the expressions
each time you retrieve them.

Solution

Use a view defined such that its columns perform the desired calculations.

Discussion

To make the statement results easier to access, use a view, which is a virtual table
that contains no data. Instead, it’s defined as the SELECT statement that retrieves the data
of interest. The following view, mail_view, is equivalent to the SELECT statement just
shown:

    mysql> CREATE VIEW mail_view AS
        -> SELECT
        -> DATE_FORMAT(t,'%M %e, %Y') AS date_sent,
        -> CONCAT(srcuser,'@',srchost) AS sender,
        -> CONCAT(dstuser,'@',dsthost) AS recipient,
        -> size FROM mail;

To access the view contents, refer to it like any other table. You can select some or all of
its columns, add a WHERE clause to restrict which rows to retrieve, use ORDER BY to sort
the rows, and so forth. For example:

    mysql> SELECT date_sent, sender, size FROM mail_view
        -> WHERE size > 100000 ORDER BY size;
    +--------------+---------------+---------+
    | date_sent    | sender        | size    |
    +--------------+---------------+---------+
    | May 12, 2014 | tricia@mars   |  194925 | 
    | May 15, 2014 | gene@mars     |  998532 | 
    | May 14, 2014 | tricia@saturn | 2394482 | 
    +--------------+---------------+---------+

Stored programs provide another way to encapsulate calculations

# 3.8. Selecting Data from Multiple Tables

Problem

The answer to a question requires data from more than one table.

Solution

Use a join or a subquery.

Discussion

The queries shown so far select data from a single table, but sometimes you must retrieve
information from multiple tables. Two types of statements that accomplish this are joins
and subqueries. A join matches rows in one table with rows in another and enables you
to retrieve output rows that contain columns from either or both tables. A subquery is
one query nested within another, to perform a comparison between values selected by
the inner query against values selected by the outer query

    mysql> SELECT id, name, service, contact_name
        -> FROM profile INNER JOIN profile_contact ON id = profile_id;
    +----+---------+----------+--------------+
    | id | name    | service  | contact_name |
    +----+---------+----------+--------------+
    |  1 | Sybil   | Twitter  | user1-twtrid | 
    |  1 | Sybil   | Facebook | user1-fbid   | 
    |  2 | Nancy   | Twitter  | user2-fbrid  | 
    |  2 | Nancy   | Facebook | user2-msnid  | 
    |  2 | Nancy   | LinkedIn | user2-lnkdid | 
    |  4 | Lothair | LinkedIn | user4-lnkdid | 
    +----+---------+----------+--------------+
    mysql> SELECT * FROM profile_contact
        -> WHERE profile_id = (SELECT id FROM profile WHERE name = 'Nancy');
    +------------+----------+--------------+
    | profile_id | service  | contact_name |
    +------------+----------+--------------+
    |          2 | Twitter  | user2-fbrid  | 
    |          2 | Facebook | user2-msnid  | 
    |          2 | LinkedIn | user2-lnkdid | 
    +------------+----------+--------------+

# 3.9. Selecting Rows from the Beginning, End, or Middle of Query Results

Problem

You want only certain rows from a result set, such as the first one, the last five, or rows
21 through 40.

Solution

Use a LIMIT clause, perhaps in conjunction with an ORDER BY clause.

Discussion

MySQL supports a LIMIT clause that tells the server to return only part of a result set.
LIMIT is a MySQL-specific extension to SQL that is extremely valuable when your result
set contains more rows than you want to see at a time. It enables you to retrieve an
arbitrary section of a result set. Typical LIMIT uses include the following kinds of prob‐
lems:

• Answering questions about first or last, largest or smallest, newest or oldest, least
or most expensive, and so forth.
• Splitting a result set into sections so that you can process it one piece at a time. This
technique is common in web applications for displaying a large search result across
several pages. Showing the result in sections enables display of smaller, easier-to-
understand pages.

    mysql> SELECT * FROM profile LIMIT 1;
    +----+-------+------------+-------+----------------------+------+
    | id | name  | birth      | color | foods                | cats |
    +----+-------+------------+-------+----------------------+------+
    |  1 | Sybil | 1970-04-13 | black | lutefisk,fadge,pizza |    0 |
    +----+-------+------------+-------+----------------------+------+
    mysql> SELECT * FROM profile LIMIT 3;
    +----+-------+------------+-------+-----------------------+------+
    | id | name  | birth      | color | foods                 | cats |
    +----+-------+------------+-------+-----------------------+------+
    |  1 | Sybil | 1970-04-13 | black | lutefisk,fadge,pizza  |    0 |
    |  2 | Nancy | 1969-09-30 | white | burrito,curry,eggroll |    3 |
    |  3 | Ralph | 1973-11-02 | red   | eggroll,pizza         |    4 |
    +----+-------+------------+-------+-----------------------+------+

LIMIT n means “return at most n rows.” If you specify LIMIT 10 , and the result set has
only four rows, the server returns four rows.

    mysql> SELECT * FROM profile ORDER BY birth LIMIT 1;
    +----+--------+------------+-------+----------------+------+
    | id | name   | birth      | color | foods          | cats |
    +----+--------+------------+-------+----------------+------+
    |  7 | Joanna | 1952-08-20 | green | lutefisk,fadge |    0 |
    +----+--------+------------+-------+----------------+------+
    mysql> SELECT * FROM profile ORDER BY birth DESC LIMIT 1;
    +----+-------+------------+-------+---------------+------+
    | id | name  | birth      | color | foods         | cats |
    +----+-------+------------+-------+---------------+------+
    |  3 | Ralph | 1973-11-02 | red   | eggroll,pizza |    4 |
    +----+-------+------------+-------+---------------+------+
    mysql> SELECT name, DATE_FORMAT(birth,'%m-%d') AS birthday
        -> FROM profile ORDER BY birthday LIMIT 1;
    +-------+----------+
    | name  | birthday |
    +-------+----------+
    | Henry | 02-14    |
    +-------+----------+
    mysql> SELECT * FROM profile ORDER BY birth LIMIT 2,1;
    +----+---------+------------+-------+---------------+------+
    | id | name    | birth      | color | foods         | cats |
    +----+---------+------------+-------+---------------+------+
    |  4 | Lothair | 1963-07-04 | blue  | burrito,curry |    5 |
    +----+---------+------------+-------+---------------+------+
    mysql> SELECT * FROM profile ORDER BY birth DESC LIMIT 2,1;
    +----+-------+------------+-------+-----------------------+------+
    | id | name  | birth      | color | foods                 | cats |
    +----+-------+------------+-------+-----------------------+------+
    |  2 | Nancy | 1969-09-30 | white | burrito,curry,eggroll |    3 |
    +----+-------+------------+-------+-----------------------+------+

    SELECT * FROM profile ORDER BY name LIMIT 0, 3;
    SELECT * FROM profile ORDER BY name LIMIT 3, 3;
    SELECT * FROM profile ORDER BY name LIMIT 6, 3;

    SELECT SQL_CALC_FOUND_ROWS * FROM profile ORDER BY name LIMIT 4;
    SELECT FOUND_ROWS();

The keyword SQL_CALC_FOUND_ROWS in the first statement tells MySQL to calculate the
size of the entire result set even though the statement requests that only part of it be
returned. The row count is available by calling FOUND_ROWS() . If that function returns
a value greater than three, there are other rows yet to be retrieved.

LIMIT is useful in combination with RAND() to make random selections from a set of
items. See Recipe 15.8.
You can use LIMIT to restrict the effect of a DELETE or UPDATE statement to a subset of
the rows that would otherwise be deleted or updated, respectively. For more information
about using LIMIT for duplicate row removal, see Recipe 16.4.

# 3.10. What to Do When LIMIT Requires the “Wrong” Sort Order

Problem

LIMIT usually works best in conjunction with an ORDER BY clause that sorts rows. But
sometimes that sort order differs from what you want for the final result.

Solution

Use LIMIT in a subquery to retrieve the desired rows, then use the outer query to sort
them.

Discussion

If you want the last four rows of a result set, you can obtain them easily by sorting the
set in reverse order and using LIMIT 4 .What if you want the output rows to appear in ascending order instead?
Use the SELECT as a subquery of an outer statement that re-sorts the rows in the desired
final order:

    mysql> SELECT * FROM
        -> (SELECT name, birth FROM profile ORDER BY birth DESC LIMIT 4) AS t
        -> ORDER BY birth;
    +-------+------------+
    | name  | birth      |
    +-------+------------+
    | Aaron | 1968-09-17 |
    | Nancy | 1969-09-30 |
    | Sybil | 1970-04-13 |
    | Ralph | 1973-11-02 |
    +-------+------------+

AS t is used here because any table referred to in the FROM clause must have a name, even
a “derived” table produced from a subquery.

# 3.11. Calculating LIMIT Values from Expressions

Problem

You want to use expressions to specify the arguments for LIMIT.

Solution

Sadly, you cannot. LIMIT arguments must be literal integers—unless you issue the state‐
ment in a context that permits the statement string to be constructed dynamically. In
that case, you can evaluate the expressions yourself and insert the resulting values into
the statement string.

Discussion

Arguments to LIMIT must be literal integers, not expressions. Statements such as the
following are illegal:

    SELECT * FROM profile LIMIT 5+5;
    SELECT * FROM profile LIMIT @skip_count, @show_count;

The same “no expressions permitted” principle applies if you use an expression to cal‐
culate a LIMIT value in a program that constructs a statement string. You must evaluate
the expression first, and then place the resulting value in the statement. For example, if
you produce a statement string in Perl or PHP as follows, an error will result when you
attempt to execute the statement:

    $str = "SELECT * FROM profile LIMIT $x + $y";

To avoid the problem, evaluate the expression first:

    $z = $x + $y;
    $str = "SELECT * FROM profile LIMIT $z";

Or do this (don’t omit the parentheses or the expression won’t evaluate properly):

    $str = "SELECT * FROM profile LIMIT " . ($x + $y);
