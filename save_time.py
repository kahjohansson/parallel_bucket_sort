import fileinput, re


metrics = list()
for i, line in enumerate(fileinput.input()):
  line = line.replace(',','.')
  if i == 0:
    metrics.append(line[0])
  if i == 2:
    metrics.insert(0,line.split(' ')[0][2:])
  
  a = re.findall(r'[+-]?\d+\.\d+',line)
  for el in a:
    metrics.append(el)

new_line = ';'.join(metrics)+';'
f = open('results.csv', mode='a')
f.write(new_line)
f.write('\n')
