import sys
import random
import time

if len(sys.argv) == 6:
    fn, sx, sy, sz, oc = str(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4]), int(sys.argv[5])

    print("| File: %s\n| Size: %dx%dx%d\n| Object Count: %d" % (fn, sx, sy, sz, oc))

    startTime = time.time()

    objects = []

    objectLibFile = open("ObjectLib.txt")
    objectLib = [line.replace("\n", "") for line in objectLibFile.readlines()]

    random.shuffle(objectLib)

    coordsX = random.sample(range(int(-sx/2), int(sx/2)), oc)
    coordsY = random.sample(range(int(-sy/2), int(sy/2)), oc)
    coordsZ = random.sample(range(int(-sz/2), int(sz/2)), oc)

    for i in range(oc):
        objects.append("%d,%d,%d,%s,%d,%s,%s,%s" % (coordsX[i], coordsY[i], coordsZ[i], objectLib[i % len(objectLib)], random.randrange(0, oc), objectLib[i % len(objectLib)], objectLib[i % len(objectLib)], objectLib[i % len(objectLib)]))

    generatedMapFile = open(fn, "w")
    generatedMapFile.write("\n".join(objects))

    endTime = time.time()

    print("| Finished generating map!")

    print("| Time elapsed: %d milliseconds" % (int(endTime * 1000 - startTime * 1000)))

else:
    print("Please enter five arguments (filename, x_size, y_size, z_size, and object_count")