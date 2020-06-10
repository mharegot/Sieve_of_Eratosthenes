import sys


class Record:
    threads = None
    size = None
    runtime = None

    def __init__(self, chunk):
        chunklines = chunk.split("\n")
        for line in chunklines:
            if "threads =" in line:
                numstart = line.index("=") + 2
                self.threads = int(line[numstart:])
            if "size =" in line:
                numstart = line.index("=") + 2
                self.size = int(line[numstart:])
            if "runtime =" in line:
                numstart = line.index("=") + 2
                numend = line.index("seconds") - 1
                self.runtime = float(line[numstart:numend])
        if self.threads is None or self.size is None \
                or self.runtime is None:
            raise ValueError()

    def readableString(self):
        return "[threads = " + str(self.threads) + \
                ", size = " + str(self.size ) + \
                 ", runtime = " + str(self.runtime) + "]"

    def __str__(self):
        return str(self.threads) + \
                "," + str(self.size ) + \
                 "," + str(self.runtime) + "\n"

if __name__ == "__main__":
    # bySize = {}
    # size = 1
    # while size <= 2048:
    #     bySize[size] = {};
    #     threadn = 1
    #     while threadn <= 32:
    #         bySize[size][threadn] = []
    #         threadn = threadn*2
    #     size = size * 2
    if len(sys.argv) < 3:
        print("Usage: python3 testparser.py [infile] [outfile]")
        exit()

    infileName = sys.argv[1]
    outfileName = sys.argv[2]

    recordlist = []

    with open(infileName, "r") as infile:
        filestring = infile.read()

    chunks = filestring.split("=========================")
    for chunk in chunks:
        if "Script started on" in chunk or "Script done on" in chunk:
            continue
        thisRecord = Record(chunk)
        recordlist.append(thisRecord)

    # for oneRecord in recordlist:
    #     print(oneRecord)

    with open(outfileName, "w") as outfile:
        outfile.write("Threads,Size,Runtime\n")
        for oneRecord in recordlist:
            outfile.write(str(oneRecord))

