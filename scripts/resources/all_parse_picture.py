import imagecompress, os.path, struct, re

supportedFormats = imagecompress.supportedFormats()

def Do( pathSrc, pathDest, fileParser, rules ):
    fileParser.MakePath( fileParser.releaseDir + pathDest )
    if type( rules ) is not dict:
        rules = {}

    fileName, fileExtension = os.path.splitext( pathSrc )
    if fileExtension[1:] not in supportedFormats:
        fileSrc = open( fileParser.rootDir + pathSrc, 'rb' )
        fileContent = fileSrc.read()
        fileSrc.close()
        CopyContentToFile( fileParser.releaseDir + pathDest, fileContent )
        return pathDest

    if 'compress' in rules:
        compressList = [ 'dxt1', 'dxt3', 'dxt5', 'etc1' ]
        cmp = re.findall( r'^([a-z0-9]+)(\/([0-9a-z]*?))?$', rules['compress'], re.I | re.S | re.M )
        compressionMethod = cmp[ 0 ][ 0 ]
        if compressionMethod not in compressList:
            print( '[Error] unknown compression method "%s"' % compressionMethod )
            return
        if compressionMethod == 'etc1':
            quality = cmp[ 0 ][ 2 ]
            image = imagecompress.picture2etc1( fileParser.rootDir + pathSrc, int( quality ) )
        else:
            image = imagecompress.picture2dxt( fileParser.rootDir + pathSrc, compressionMethod )
        fileContent = compressionMethod.encode() + struct.pack( 'I', image['width'] ) + struct.pack( 'I', image['height'] ) + image['data']
        pathDest = pathDest.replace( fileExtension, '.' + compressionMethod )
    else:
        fileSrc = open( fileParser.rootDir + pathSrc, 'rb' )
        fileContent = fileSrc.read()
        fileSrc.close()

    CopyContentToFile( fileParser.releaseDir + pathDest, fileContent )

    return pathDest

def CopyContentToFile( destPath, content ):
    fileDest = open( destPath, 'wb' )
    fileDest.write( content )
    fileDest.close()
