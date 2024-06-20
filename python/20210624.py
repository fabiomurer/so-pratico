import sys
import os
import subprocess

"""
Esercizio 3: Python o bash: (10 punti)
Scrivere uno script o un programma python che corregga l'indentazione di tutti i file .c e .h presenti
nel sottoalbero della directory passata come parametro (la working directory se non vi sono
parametri).
Hint: il comando:
ex -n '+norm!gg=G' +wq prog.c
corrregge l'indentazione del programma sorgente C prog.c.
"""

def lsrec(startdir):
    pathsarr = []

    for dirpath, dirnames, files in os.walk(startdir):
        for f in files:
            if f.endswith('.c') or f.endswith('.h'):
                path = os.path.join(dirpath, f)
                pathsarr.append(path)
    return pathsarr

def exrun(filename):
    subprocess.run(['ex', '-n', '+norm!gg=G', '+wq', filename])

if __name__ == '__main__':
    startdir = ''
    if len(sys.argv) < 2:
        startdir = '.'
    else:
        startdir = sys.argv[1]

    pathsarr = lsrec(startdir)

    print(pathsarr)

    for f in pathsarr:
        exrun(f)