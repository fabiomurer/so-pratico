"""
Esercizio 3: Python o bash: 10 punti
Scrivere un programma python o uno script bash che data una directory passata come parametro
produca una lista dei link simbolici presenti nel sottoalbero che fanno riferimento allo stesso file.
Esempio, in questo caso:
$ ls -lR /tmp/test
/tmp/test:
total 4
drwxr-xr-x 2 renzo renzo 4096 Sep 10 15:45 d
-rw-r--r-- 1 renzo renzo 0 Sep 10 15:41 file1
-rw-r--r-- 1 renzo renzo 0 Sep 10 15:41 file2
lrwxrwxrwx 1 renzo renzo 5 Sep 10 15:42 sl1 -> file1
lrwxrwxrwx 1 renzo renzo 5 Sep 10 15:42 sl1bis -> file1
lrwxrwxrwx 1 renzo renzo 5 Sep 10 15:49 sl2 -> file2
/tmp/test/d:
total 0
lrwxrwxrwx 1 renzo renzo 15 Sep 10 15:45 gsld -> /tmp/test/file1
lrwxrwxrwx 1 renzo renzo 8 Sep 10 15:43 sld -> ../file1
il programma lanciato con parametro /tmp/test deve trovare che sl1, sl1bis, d/sld e d/gllsd indicano lo
stesso file. (similmente dovrebbe rilevare altri insiemi di link simbolici che indicano lo stesso file)
"""

import os, sys, pathlib

def lsrec(startdir):
    pathsarr = []

    for dirpath, dirnames, files in os.walk(startdir):
        for f in dirnames + files:
            path = os.path.join(dirpath, f)
            pathsarr.append(path)
    return pathsarr

def getsymlinks(listdirs):
    out = []
    for d in listdirs:
        if pathlib.Path(d).is_symlink():
            out.append(d)
    return out

def sortlinks(links):
    db = {}
    for l in links:
        to = str(pathlib.Path(l).readlink())
        if to not in db:
            db[to] = [l]
        else:
            db[to].append(l)
    return db

if __name__ == '__main__':
    d = sys.argv[1]
    listdirs = lsrec(d)
    links = getsymlinks(listdirs)
    db = sortlinks(links)

    for f, l in db.items():
        print(f'{f}:')
        for x in l:
            print(x)


