import struct

idx = 0
with open('info.data', 'rb') as infofile:
    data = infofile.read()
    while True:
        (tz_length, ) = struct.unpack_from('i', data, idx)
        idx += 4
        tzbytes = b''
        for i in range(tz_length):
            (c, ) = struct.unpack_from('c', data, idx)
            idx += 1
            tzbytes += c
        tz = tzbytes.decode('ascii')
        print(tz)
        (poly_length, ) = struct.unpack_from('i', data, idx)
        idx += 4
        for _ in range(poly_length):
            (lat, lon) = struct.unpack_from('dd', data, idx)
            idx += 16
            print(lat, lon)
