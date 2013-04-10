import math

f = open("data.csv.txt", 'r')

filenumber = "0"
filename = ""
logfile = "log.txt"

for line in f:
    if line.find("CSV") != -1:
        temp_filenumber = line[0:line.find(".")]

        if math.fabs(int(filenumber) - int(temp_filenumber)) >= 20 or int(temp_filenumber)<20:
            filenumber = temp_filenumber
            filename = str(filenumber) + ".csv"
            temp = open(filename, "a")
            temp.close()
        else:
            temp = open(logfile, "a")
            temp.write('Reuse file: ' + filename + '\n')
            temp.close()

    elif line.find(",") != -1:
        temp = open(filename, "a")
        temp.write(line)
        temp.close()
    
f.close()
