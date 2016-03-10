#!/usr/bin/env python

leapYearMonth = (31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31)
nomalYearMonth = (31, 28, 31, 31, 31, 30, 31, 31, 30, 31, 30, 31)

def isLeap(y):
    return y % 4 == 0 and y % 100 != 0 or y % 400 == 0

def dayInYear(year, month, day):
    if isLeap(year):
	yearMonth = leapYearMonth
    else:
        yearMonth = nomalYearMonth

    days = 0
    i = 0
    while i < month - 1:
        days += yearMonth[i]
	i += 1
    days += day
    
    return days

def daysBetweenYears(year1, month1, day1, year2, month2, day2):
    days1 = dayInYear(year1, month1, day1)
    days2 = dayInYear(year2, month2, day2)

    if year1 == year2 and month1 == month2:
        return day2 - days1
    elif year1 == year2:
        return days2 - days1
    else:
        if isLeap(year1):
	    d1 = 366 - days1
	else:
	    d1 = 365 - days1
	d2 = days2
	d3 = 0
	year = year1 + 1
	while year < year2:
	    if isLeap(year):
		d3 += 366
	    else:
	        d3 += 365
	    year += 1
	return d1 + d2 + d3

date1_input = raw_input('Enter the first date(yyyy/mm/dd): ')
date2_input = raw_input('Enter the second date(yyyy/mm/dd): ')

date1 = date1_input.split('/')
date2 = date2_input.split('/')

year1 = int(date1[0])
month1 = int(date1[1])
day1 = int(date1[2])
year2 = int(date2[0])
month2 = int(date2[1])
day2 = int(date2[2])

print 'The distance between %s and %s is:' % (date1_input, date2_input)
print daysBetweenYears(year1, month1, day1, year2, month2, day2)
