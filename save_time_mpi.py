import fileinput, re


metrics = list()
for i, line in enumerate(fileinput.input()):
    print(line)
    if i == 0:
      continue
    a = re.findall(r'[+-]?\d+\.\d+',line)
    if i == 2:
      metrics.insert(0,line.split(' ')[3])
      metrics.insert(1,line.split(' ')[2])
    for el in a:
      metrics.append(el)

new_line = ';'.join(metrics)+';'
# print(new_line)
f = open('results.csv', mode='a')
f.write(new_line)
f.write('\n')
