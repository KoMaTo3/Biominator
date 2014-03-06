# .tga
# .png
# .bmp

import imagecompress, os.path, struct

def Do( pathSrc, pathDest, fileParser, rules ):
    fileParser.MakePath( fileParser.releaseDir + pathDest )
    if type( rules ) is not dict:
        rules = {}

    if 'compress' in rules:
        print( '=> '+pathSrc )
        compressList = [ 'dxt1', 'dxt3', 'dxt5' ]
        if rules['compress'] not in compressList:
            print( '[Error] unknown compression method "%s"' % rules['compress'] )
            return
        fileName, fileExtension = os.path.splitext( pathSrc )
        if fileExtension == '.tga':
            image = imagecompress.tga2rgba( fileParser.rootDir + pathSrc )
        elif fileExtension == '.jpg':
            image = imagecompress.jpg2rgba( fileParser.rootDir + pathSrc )
        elif fileExtension == '.png':
            image = imagecompress.png2rgba( fileParser.rootDir + pathSrc )
        image = dict(
                dxt1 = imagecompress.rgba2dxt1,
                dxt3 = imagecompress.rgba2dxt3,
                dxt5 = imagecompress.rgba2dxt5,
            ).get( rules['compress'] )( image )
        fileContent = rules['compress'].encode() + image['data']
        pathDest = pathDest.replace( fileExtension, '.' + rules['compress'] )
    else:
        fileSrc = open( fileParser.rootDir + pathSrc, 'rb' )
        fileContent = fileSrc.read()
        fileSrc.close()

    fileDest = open( fileParser.releaseDir + pathDest, 'wb' )
    fileDest.write( fileContent )
    fileDest.close()

    return pathDest
