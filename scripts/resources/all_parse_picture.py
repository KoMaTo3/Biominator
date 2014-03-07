import imagecompress, os.path, struct

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
        compressList = [ 'dxt1', 'dxt3', 'dxt5' ]
        if rules['compress'] not in compressList:
            print( '[Error] unknown compression method "%s"' % rules['compress'] )
            return
        image = imagecompress.picture2dxt( fileParser.rootDir + pathSrc, rules['compress'] )
        fileContent = rules['compress'].encode() + image['data']
        pathDest = pathDest.replace( fileExtension, '.' + rules['compress'] )
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
