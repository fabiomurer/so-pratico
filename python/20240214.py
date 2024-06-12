"""
Esercizio 3: Python o bash: 10 punti
Scrivere un programma python o uno script bash che crei un catalogo dei file presenti nella directory
passata come parametro (o la directory corrente se manca il parametro).
Il catalogo deve essere ordinato in categorie in base alla stringa ritornata dal comando 'file'.
Es:
$ catls
ASCII text:
testo1
favourites.txt
directory:
mydir
lib
Unicode text, UTF-8 text:
unitesto
"""

import os
import sys
import subprocess

def filerun(fname):
    return subprocess.run(['file', '-b', fname], capture_output=True).stdout.decode().strip()

def mkcatalog(startdir):
    catalog = {}

    # Walking a directory tree and printing the names of the directories and files
    for dirpath, dirnames, files in os.walk(startdir):
        for f in dirnames + files:
            ftype = filerun(os.path.join(dirpath, f))
            if not ftype in catalog:
                catalog[ftype] = []
            catalog[ftype].append(f)


    return catalog


if __name__ == '__main__':
    startdir = ''
    if len(sys.argv) < 2:
        startdir = '.'
    else:
        startdir = sys.argv[1]
    
    catalog = mkcatalog(startdir)

    for types, files in catalog.items():
        print(types)
        for f in files:
            print(f)