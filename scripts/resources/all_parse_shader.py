# .fs
# .vs
import re

def Do( pathSrc, pathDest, fileParser, rules ):
    fileParser.MakePath( fileParser.releaseDir + pathDest )
    fileSrc = open( fileParser.rootDir + pathSrc, 'r' )
    fileContent = fileSrc.read()
    fileContent = re.sub( r'^\/\/\['+( fileParser.platform )+'\] (.*?)$', r'\1', fileContent, 0, re.I | re.S | re.M )
    fileContent = re.sub( r'^\/\/\[(android|win32|linux)\] .*?$', '', fileContent, 0, re.I | re.S | re.M )
    fileSrc.close()
    fileDest = open( fileParser.releaseDir + pathDest, 'w' )
    fileDest.write( fileContent )
    fileDest.close()

    return pathDest
