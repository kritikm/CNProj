import os
import io

# directory = os.getcwd()
# print os.listdir(directory)

try:
    os.mkdir("Formatted")
    for i in range(1, 11):
        filename = str(i) + ".txt"
        # print filename
        file_old = open(filename, 'r')
        file_new = open("Formatted/" + filename, 'w')
        for each_line in file_old:
            # print each_line
            if len(each_line) == 1:
                continue
            print each_line + str(len(each_line))
            if '.' in each_line:
                each_line = each_line[3:]
            if ':' in each_line:
                each_line = each_line[1].lower()

            file_new.write(each_line)
        file_old.close()
        file_new.close()

except IOError as err:
    print 'File handling error:'
    print str(err)
