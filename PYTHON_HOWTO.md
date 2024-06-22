# HOWTO ~~~~~:>

### shebang

```python
#!/usr/bin/env python3
```

### args

```python
import os
os.argv
```

### list rec all paths

```python
import sys
import os

def lsrec(startdir):
    pathsarr = []

    for dirpath, dirnames, files in os.walk(startdir):
        for f in dirnames + files:
            path = os.path.join(dirpath, f)
            pathsarr.append(path)
    return pathsarr

if __name__ == '__main__':
    startdir = ''
    if len(sys.argv) < 2:
        startdir = '.'
    else:
        startdir = sys.argv[1]

    pathsarr = lsrec(startdir)

    print(pathsarr)
```

### run and wait

```python
import subprocess

subprocess.run(['ls', '-l', directory])

# get type of a file
def filerun(fname):
    return subprocess.run(['file', '-b', fname], capture_output=True).stdout.decode().strip()
```


### join paths

```python
import os
os.path.join(dirpath, f)
```

### realpath and links

```python
os.path.realpath(path)
```