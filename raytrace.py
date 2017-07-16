import subprocess
import sys
import math
import png
import os
import time

file_name = sys.argv[1]
threads = int(sys.argv[2])
width = int(sys.argv[3])
height = int(sys.argv[4])

total_threads = threads * threads
w_size = int(math.ceil(float(width) / float(threads)))
h_size = int(math.ceil(float(height) / float(threads)))
idd = -1
args_arr = []
for i in range(threads):
  for j in range(threads):
    idd += 1
    result_name = './temp' + file_name[file_name.rfind('/'):file_name.rfind('.')] + str(idd) + '.png'
    x_start = i * w_size
    y_start = j * h_size
    args = ('./A4', \
      file_name, \
      str(x_start), \
      str(y_start), \
      str(w_size), \
      str(h_size), \
      str(idd), \
      str(width), \
      str(height), \
      result_name)
    args_arr.append(args)

start = time.time()

cur = 0
total = len(args_arr)
print total
MAX_THREAD = min(20, total_threads)
while(cur < total):
  processes = []
  for i in range(MAX_THREAD):
    if (cur + i >= total):
      break
    args = args_arr[cur + i]
    print "starting thread: " + args[6]
    print args
    popen = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    processes.append(popen)
  for p in processes:
    p.wait()
    output = p.stdout.read()
    error = p.stderr.read()
    print output
    print error
  cur += MAX_THREAD

end = time.time()

print 'time taken: ' + str(end - start)

img_arr = [[0 for _ in range(width*3)] for __ in range(height)]

for arg in args_arr:
  x_start = int(arg[2])
  y_start = int(arg[3])
  f_name = arg[9]
  r=png.Reader(f_name)
  w, h, data, meta = r.read()
  data_list = list(data)
  for x in range(x_start, min(x_start + w_size, width)):
    for y in range(y_start, min(y_start + h_size, height)):
      rgb = (0,0,0)
      if 'palette' in meta:
        rgb = meta['palette'][data_list[y][x]]
      else:
        if meta['greyscale']:
          rgb = (data_list[y][x], data_list[y][x], data_list[y][x])
        else:
          r = data_list[y][3*x]
          g = data_list[y][3*x+1]
          b = data_list[y][3*x+2]
          rgb = (r, g, b)
        
      img_arr[y][3*x] = rgb[0]
      img_arr[y][3*x+1] = rgb[1]
      img_arr[y][3*x+2] = rgb[2]


result = './results' + file_name[file_name.rfind('/'):file_name.rfind('.')] + '.png'
r = open(result, 'wb')
w = png.Writer(width, height)
w.write(r, img_arr)
r.close()

for arg in args_arr:
  f_name = arg[9]
  os.remove(f_name)
