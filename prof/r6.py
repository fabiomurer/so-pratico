#!/usr/bin/env python3

def compute_parity(tab, nrow, ncol):
  for i in range(nrow):
    p = 0
    for j in range(ncol):
      p ^= tab[i][j]
    tab[i][ncol] = p

def compute_dparity(tab, nrow, ncol):
  p = [0] * ncol
  for i in range(nrow):
    for j in range(ncol):
      p[(i+j) % ncol] ^= tab[i][j]
# print(p)
  s = p[ncol - 1]
  for i in range(nrow):
    tab[i][ncol + 1] = p[i] ^ s

def print_table(tab, nrow, ncol):
  for i in range(nrow):
    for j in range(ncol):
      print(tab[i][j], end=" ")
    print()
  print()

def fail(tab, nrow, f):
  for i in range(nrow):
    tab[i][f] = 'x'

def restorep(tab, nrow, ncol, f):
  for i in range(nrow):
    p = tab[i][ncol]
    for j in range(ncol):
      if j != f:
        p ^= tab[i][j]
    tab[i][f] = p

def restored(tab, nrow, ncol, f):
  p = [tab[i][ncol + 1] for i in range(nrow)] + [0]
  for i in range(nrow):
    for j in range(ncol):
      if j != f:
        p[(i+j) % ncol] ^= tab[i][j]
  s = p[(f - 1) % ncol]
# print(p, s)
  for i in range(nrow):
    tab[i][f] = s ^ p[(f + i) % ncol]

def restore2(tab, nrow, ncol, f1, f2):
  s = 0
  for i in range(nrow):
    s ^= tab[i][ncol] ^ tab[i][ncol + 1]
  p = [s ^ tab[i][ncol + 1] for i in range(nrow)] + [s]
# print(s, p)
  nindex = (nrow + f2) % ncol
  for _ in range(nrow):
    v = p[nindex]
#   print(nindex, (nindex + 1) % ncol)
    for i in range(nrow):
      j = (nindex - i) % ncol
      if j != f1:
        v ^= tab[i][j]
      else:
        row = i
#   print(row)
    tab[row][f1] = v
    nindex = (row + f2) % ncol
    v = tab[row][ncol]
    for j in range(ncol):
      if j != f2:
        v ^= tab[row][j]
      tab[row][f2] = v

def main():
  tab=[]
  while True:
    line = input("")
    if len(line) <= 1:
      break
    tab.append([0 if x == "0" else 1 for x in line.split()] + [0, 0])
  nrow = len(tab)
  ncol = len(tab[0]) - 2
# print_table(tab, nrow, ncol)
  compute_parity(tab, nrow, ncol)
# print_table(tab, nrow, ncol + 1)
  compute_dparity(tab, nrow, ncol)
  print_table(tab, nrow, ncol + 2)

  savetab = tab

  while True:
    line = input("failure ")
    s = line.split()
    if len(s) == 2:
      f1, f2 = int(s[0]), int(s[1])
    elif len(s) == 1:
      f1 = f2 = int(s[0])
    else:
      return

    fail(tab, nrow, f1)
    fail(tab, nrow, f2)
    print_table(tab, nrow, ncol + 2)
    if f1 == f2:
      print("1 faulty", f1)
      if f1 == ncol:
        compute_parity(tab, nrow, ncol)
      elif (f1 == ncol + 1):
        compute_dparity(tab, nrow, ncol)
      else:
        restorep(tab, nrow, ncol, f1)
    else:
      if f1 > f2:
        f1, f2 = f2, f1
      print("2 faulty", f1, f2)
      if f1 == ncol:
        compute_parity(tab, nrow, ncol)
        compute_dparity(tab, nrow, ncol)
      elif f2 == ncol + 1:
        restorep(tab, nrow, ncol, f1)
        compute_dparity(tab, nrow, ncol)
      elif f2 == ncol:
        restored(tab, nrow, ncol, f1)
        compute_parity(tab, nrow, ncol)
        pass
      else:
        restore2(tab, nrow, ncol, f1, f2)
    print_table(tab, nrow, ncol + 2)
    if tab == savetab:
      print('OK')
    else:
      print('ERR!!!!')

if __name__ == "__main__":
  main()
