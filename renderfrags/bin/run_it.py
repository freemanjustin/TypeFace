import sys, os, posix, commands, math, string

# letters array corresponds to the input image file name
# and output directory
letters = "abcdefghijklmnopqrstuvwxyz"
chars = list(letters)

# output directory top level name
outdir = "helvetica_96"

for j in range(0,256,8):
    cmd = "mkdir -p ./%s/%03d" % (outdir,  j )
    #print cmd
    commands.getstatusoutput(cmd)
    for i in range(0,len(chars) ):
        cmd = "./renderfrags ../../renderfont/bin/%s_mono.png ./%s/%03d/%s.png %d" % (chars[i], outdir, j, chars[i], j)
        print cmd
        commands.getstatusoutput(cmd)

cmd = "mkdir -p ./%s/255" % (outdir)
commands.getstatusoutput(cmd)
for i in range(0,len(chars) ):
    cmd = "./renderfrags ../../renderfont/bin/%s_mono.png ./%s/255/%s.png 255" % (chars[i], outdir, chars[i])
    print cmd
    commands.getstatusoutput(cmd)

