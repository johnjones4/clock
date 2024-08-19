import json
import struct

def get_geojson():
    with open('scripts/tz.json', 'r') as jsonfile:
        info = json.loads(jsonfile.read())
        return info['features']
    
def write_tz(infofile, tzinfo) -> list:
    tz = tzinfo['properties']['tzid']
    polys = [tzinfo['geometry']['coordinates']] if tzinfo['geometry']['type'] == 'Polygon' else tzinfo['geometry']['coordinates']
    for poly in polys:
        print(tz + ": " + str(len(poly[0])))
        bytes = bytearray()
        bytes += struct.pack('i', len(poly[0]))
        for coord in poly[0]:
            bytes += struct.pack('dd', coord[0], coord[1])
            print(coord[0], coord[1])
        infofile.write(bytes)

with open('info.data', 'wb') as infofile:
    geojson = get_geojson()
    for feature in geojson:
        write_tz(infofile, feature)
        
