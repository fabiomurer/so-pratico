"""
Esercizio 3: Python o bash: 10 punti
Scrivere un programma python o uno script bash chiamato tcmp che confronti gli alberi del file
system di due directory.. A seconda del parametro deve elencare i pathname di file e di directory che
• sono comuni ad entrambi i sottoalberi, se manca il parametro
• esistono solo nel primo sottoalbero, se il parametro è -1
• esistono solo nel secondo sottoalbero se il parametro è -2
esempi:
$ . /tcmp dir1 dir2
stampa l'elenco dei path che esistono sia in dir1 sia in dir2
$ . /tmcp -1 dir1 dir2
stampa l'elenco dei path che esistono in dir1 ma non in dir2
"""
import os, sys

def lsrec(startdir):
    pathsarr = []

    for dirpath, dirnames, files in os.walk(startdir):
        for f in dirnames + files:
            path = os.path.join(dirpath, f)
            pathsarr.append(path)
    return pathsarr

if __name__ == '__main__':
    dir1 = ''
    dir2 = ''
    action = 0
    if len(sys.argv) <= 3:
        dir1 = sys.argv[1]
        dir2 = sys.argv[2]
    elif len(sys.argv) <= 4:
        action = sys.argv[1]
        dir1 = sys.argv[2]
        dir2 = sys.argv[3]
    else:
        print("sas")
        sys.exit(-1)
    
    lsdir1 = lsrec(dir1)
    lsdir2 = lsrec(dir2)

    if action == 0:
        for p in [x for x in lsdir1 if x in lsdir2]:
            print(p)
    elif action == -1:
        for p in [x for x in lsdir1 if not x in lsdir2]:
            print(p)
    elif action == -2:
        for p in [x for x in lsdir2 if not x in lsdir1]:
            print(p)