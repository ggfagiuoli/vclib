from dumper import *

def qdump__vcl__BitSet(d, value):
    size = value["mBits"].type.size() * 8

    mBits = value["mBits"].integer()
    d.putValue("BitSet of size " + str(size))

    retList = []
    for i in range(size):
        bit = mBits & (1 << i)
        retList.append(('[%d]' % i, bit != 0))
    d.putExpandable()
    d.putNumChild(size)
    if d.isExpanded():
        with Children(d, size, maxNumChild=10000):
            for i in d.childRange():
                bit = mBits & (1 << i)
                if bit != 0:
                    bit = 1
                with SubItem(d, i):
                    d.putValue(bit)
                    d.putType('bool')
