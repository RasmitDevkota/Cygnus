import sys
import random

if len(sys.argv) == 5:
    fn, sx, sy, sz, oc = str(sys.argv[0]), int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4])
    
    print('| Generating Map...\n| File: %s\n| Size: %dx%dx%d\n|Object Count: %d' (fn, sx, sy, sz, oc))

    objects = []

    for i in range(oc):
        x = random.randrange(0, sx)
        y = random.randrange(0, sy)
        z = random.randrange(0, sz)

        for o in objects:
            if o[0]
else:
    print("Please enter five arguments (filename, x_size, y_size, z_size, and object_count")