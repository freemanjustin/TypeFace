import sys, os, posix, commands, math, string
import glob

directories = glob.glob("helvetica_96/*")
print directories

for i in range (0,len(directories)):
    files =  glob.glob("./%s/*.png" % directories[i])
    #print files
    count = 0
    for j in range (0,len(files)):
        cmd = "cp %s %s/%04d.png" % (files[j],directories[i],count)
        print cmd
        commands.getstatusoutput(cmd)
        count = count + 1
