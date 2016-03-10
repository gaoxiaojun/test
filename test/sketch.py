#!/usr/bin/env python3

import pickle

man = []
other = []
try:
    data = open('sketch.txt')

    for each_line in data:
        try:
            (role, line_spoken) = each_line.split(':', 1)
            line_spoken = line_spoken.strip()
            if role == 'Man':
                man.append(line_spoken)
            elif role == 'Other Man':
                other.append(line_spoken)
            #print(role, end='')
            #print(' said: ', end='')
            #print(line_spoken, end='')
        except ValueError:
            pass

    #print('Man said:')
    #for each_line in man:
    #    print(each_line)

    #print('Other man said:')
    #for each_line in other:
    #    print(each_line)
    
    data.close()
except IOError:
    print('The data file is missing.')

#try:
#    man_file = open('man_data.txt', 'w')
#    other_file = open('other_data.txt', 'w')
    #for each_line in man:
    #    print(each_line, file=man_file)
    #for each_line in other:
    #    print(each_line, file=other_file)
#    print(man, file=man_file)
#    print(other, file=other_file)
#except IOError:
#    print('File operation error')
#finally:
#    man_file.close()
#    other_file.close()

try:
    with open('man_data.txt', 'wb') as man_file:
        #print(man, file=man_file)
        pickle.dump(man, man_file)
    with open('other_data.txt', 'wb') as other_file:
        #print(other, file=other_file)
        pickle.dump(other, other_file)
except IOError as err:
    print(str(err))
except pickle.PickleError as err:
    print('PickleError: ' + str(err))

try:
    with open('man_data.txt', 'rb') as man_file:
        man = pickle.load(man_file)
    print(man)
except IOError as err:
    print(str(err))
except pickle.PickleError as err:
    print('PickleError: ' + str(err))
