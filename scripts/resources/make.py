import os, shutil, re

import all_parse_shader
import imagecompress
import struct

def Run( fileParser ):
    print( 'Resources package manager[ '+( fileParser.platform )+' ]' )
    print( '=========================' )
    rootDir = os.path.dirname( __file__ )
    dataDir = rootDir + '/' + fileParser.GetDataSrcName()
    releaseDirBackup = rootDir + '/~release-'+( fileParser.platform )+'_backup'
    fileParser.SetRoot( rootDir )
    releaseDir = fileParser.GetRelease() + '/' + fileParser.GetDataDestName()

    if os.path.isdir( releaseDir ):
        if os.path.isdir( releaseDirBackup ):
            shutil.rmtree( releaseDirBackup )
        shutil.move( releaseDir, releaseDirBackup )
    os.mkdir( releaseDir )

    itemsList = ScanDir( dataDir, fileParser.GetDataSrcName(), fileParser.GetDataDestName(), [ '~release_backup', 'release' ] )
    NextDir( itemsList, '', '', fileParser )
#def Run

def ScanDir( rootDir, setSrcName = 'root', setDestName = 'root', excludeNames = [] ):
    res = { 'name': setSrcName, 'nameDest': setDestName, 'dirs': [], 'files': [] }

    for item in os.listdir( rootDir ):
        if item not in excludeNames:
            if os.path.isdir( rootDir + '/' + item ):
                res['dirs'].append( ScanDir( rootDir + '/' + item, item, item ) )
            else:
                res['files'].append( item )
    
    return res
#def ScanDir

def NextDir( dir, currentDirName = '', currentDestDirName = '', fileParser = {} ):
    pathSrc = ( currentDirName + '/' if len( currentDirName ) else '' ) + dir['name']
    pathDest = ( currentDestDirName + '/' if len( currentDestDirName ) else '' ) + dir['nameDest']
    for fileName in dir['files']:
        NextFile( pathSrc + '/' + fileName, pathDest + '/' + fileName, fileParser )

    if len( dir['dirs'] ):
        for subDir in dir['dirs']:
            NextDir( subDir, pathSrc, pathDest, fileParser )
#def NextDir

def NextFile( pathSrc, pathDest, fileParser ):
    fileParser.Parse( pathSrc, pathDest )
#def NextFile

#
#
#
class FileParser:
    def __init__( self ):
        self.platform = 'unknown'
        self.parseByExt = {
        }
    #def __init__

    def SetRoot( self, dir ):
        self.rootDir = dir + '/'

    def SetRelease( self, dir ):
        self.releaseDir = dir + '/'

    def GetRelease( self ):
        return self.releaseDir

    def GetDataSrcName( self ):
        return self.dataSrcName

    def GetDataDestName( self ):
        return self.dataDestName

    def Parse( self, pathSrc, pathDest ):
        fileInfo = os.path.splitext( pathSrc )
        ext = fileInfo[ 1 ]
        if ext in self.parseByExt:
            self.parseByExt[ ext ]( pathSrc, pathDest, self )
        else:
            self.RawCopy( pathSrc, pathDest )

    def MakePath( self, path ):
        dir = os.path.dirname( path )
        if not os.path.isdir( dir ):
            os.mkdir( dir )

    def RawCopy( self, pathSrc, pathDest ):
        self.MakePath( self.releaseDir + pathDest )
        shutil.copyfile( self.rootDir + pathSrc, self.releaseDir + pathDest )

    def TGA2DXT( self, pathSrc, pathDest, _ ):
        rgba = imagecompress.tga2rgba( self.rootDir + pathSrc )
        dxt = imagecompress.rgba2dxt1( rgba )
        self.WriteDXTCompressedFile( dxt, self.releaseDir + pathDest, 1 )

    def WriteDXTCompressedFile( self, dxtData, fileDest, dxtFormat ):
        self.MakePath( fileDest )
        fileDest = open( fileDest, 'wb' )
        fileDest.write( struct.pack( '4s', str.encode( 'DXT%d' % dxtFormat ) ) )
        fileDest.write( dxtData['data'] )
        fileDest.close()

    def ParseShader( self, pathSrc, pathDest ):
        self.MakePath( self.releaseDir + pathDest )
        fileSrc = open( self.rootDir + pathSrc, 'r' )
        fileContent = fileSrc.read()
        fileContent = re.sub( r'^\/\/\['+( self.platform )+'\] (.*?)$', r'\1', fileContent, 0, re.I | re.S | re.M )
        fileContent = re.sub( r'^\/\/\[(android|win32|linux)\] .*?$', '', fileContent, 0, re.I | re.S | re.M )
        fileSrc.close()
        fileDest = open( self.releaseDir + pathDest, 'w' )
        fileDest.write( fileContent )
        fileDest.close()
        print( '[shader] file: ' + pathDest )

#class FileParser

#
#
#
class FileParserWin32( FileParser ):
    def __init__( self, setReleaseDir, setDataSrcName, setDataDestName ):
        self.platform = 'win32'
        self.releaseDir = setReleaseDir
        self.dataSrcName = setDataSrcName
        self.dataDestName = setDataDestName
        self.parseByExt = {
            '.fs': all_parse_shader.Do,
            '.vs': all_parse_shader.Do,
            '.tga': self.TGA2DXT,
        }
    #def __init__
#class FileParserWin32

#
#
#
class FileParserLinux( FileParser ):
    def __init__( self, setReleaseDir, setDataSrcName, setDataDestName ):
        self.platform = 'linux'
        self.releaseDir = setReleaseDir
        self.dataSrcName = setDataSrcName
        self.dataDestName = setDataDestName
        self.parseByExt = {
            '.fs': all_parse_shader.Do,
            '.vs': all_parse_shader.Do,
        }
    #def __init__
#class FileParserLinux

#
#
#
class FileParserAndroid( FileParser ):
    def __init__( self, setReleaseDir, setDataSrcName, setDataDestName ):
        self.platform = 'android'
        self.releaseDir = setReleaseDir
        self.dataSrcName = setDataSrcName
        self.dataDestName = setDataDestName
        self.parseByExt = {
            '.fs': all_parse_shader.Do,
            '.vs': all_parse_shader.Do,
            #'.tga': self.ParseImage,
            #'.bmp': self.ParseImage,
        }
    #def __init__

    def ParseImage( self, pathSrc, pathDest ):
        print( '[android/pack-image] file: ' + pathDest )
        pass
    #def ParseImage
#class FileParserAndroid

#Begin
Run( FileParserWin32( 'C:/temp/git/Biominator/vc/Biominator/', 'data', 'data' ) )
#Run( FileParserAndroid( 'j:/android/projects/Biominator/', 'data', 'Assets' ) )
#Run( FileParserLinux( '/home/komato3/workspace/Biominator/linux/', 'data', 'data' ) )

input( '\nDone' )
