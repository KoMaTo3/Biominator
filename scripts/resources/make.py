import os, shutil, re, struct, time, datetime
import all_parse_shader, all_parse_picture

#config
platform = 'win32'

def Run( fileParser ):
    print( 'Resources package manager[ '+( fileParser.platform )+' ]' )
    print( datetime.date.today().strftime( '%d.%m.%Y' ) + time.strftime( ' - %H:%M:%S' ) )
    print( '=========================\n' )
    rootDir = os.path.dirname( __file__ )
    dataDir = rootDir + '/' + fileParser.GetDataSrcName()
    releaseDirBackup = rootDir + '/~release-'+( fileParser.platform )+'_backup'
    fileParser.SetRoot( rootDir )
    releaseDir = fileParser.GetRelease() + '/' + fileParser.GetDataDestName()

    if os.path.isdir( releaseDir ):
        if os.path.isdir( releaseDirBackup ):
            try:
                shutil.rmtree( releaseDirBackup )
            except:
                print( '[Warning] Can\'t remove directory '+( releaseDirBackup ) )
        try:
            shutil.move( releaseDir, releaseDirBackup )
        except:
            print( '[Warning] Can\'t move directory "%s" to "%s"' % ( releaseDir, releaseDirBackup ) )
            try:
                shutil.copy( releaseDir, releaseDirBackup )
            except:
                print( '[Warning] can\'t copy directory "%s" to "%s"' % ( releaseDir, releaseDirBackup ) )
    try:
        os.mkdir( releaseDir )
    except:
        print( '[Warning] Can\'t create directory "%s"' % ( releaseDir ) )

    itemsList = ScanDir( dataDir, fileParser.GetDataSrcName(), fileParser.GetDataDestName(), [ '~release_backup', 'release', '.rules-win32', '.rules-android', '.rules-linux' ] )
    ParseDir( itemsList, '', '', fileParser )
    fileParser.WriteFilesList()
#def Run

def GetCurrentTime():
    return int( round( time.time() * 1000 ) )

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

def ParseDir( dir, currentDirName = '', currentDestDirName = '', fileParser = {} ):
    pathSrc = ( currentDirName + '/' if len( currentDirName ) else '' ) + dir['name']
    pathDest = ( currentDestDirName + '/' if len( currentDestDirName ) else '' ) + dir['nameDest']
    for fileName in dir['files']:
        resultFileName = ParseFile( pathSrc + '/' + fileName, pathDest + '/' + fileName, fileParser )
        fileParser.filesList[ pathSrc + '/' + fileName ] = resultFileName

    if len( dir['dirs'] ):
        for subDir in dir['dirs']:
            ParseDir( subDir, pathSrc, pathDest, fileParser )
#def ParseDir

def ParseFile( pathSrc, pathDest, fileParser ):
    return fileParser.Parse( pathSrc, pathDest )
#def ParseFile

#
#
#
class FileParser:
    def __init__( self ):
        self.platform = 'unknown'
        self.filesList = {}
        self.parseByExt = {
        }

    def SetConfig( self, config = {} ):
        defaultConfig = dict(
            release_dir = None,
            data_src_name = 'data',
            data_dest_name = 'data',
            rules = None,
        )
        defaultConfig.update( config )
        config = defaultConfig
        self.releaseDir = config['release_dir']
        self.dataSrcName = config['data_src_name']
        self.dataDestName = config['data_dest_name']
        self.rulesFile = config['rules']
        self.rootDir = os.path.dirname( __file__ )
        self.rules = {}
        if self.rulesFile is not None:
            self.ParseRules()

    def ParseRules( self ):
        f = open( self.rootDir + '/' + self.dataSrcName + '/' + self.rulesFile, 'r' )
        for line in f:
            tmp = re.search( '^(.+):(.+)$', line.strip() )
            if tmp:
                fileName = self.dataSrcName + '/' + tmp.group( 1 ).strip()
                params = {}
                parmsTmp = tmp.group( 2 ).strip().split( ' ' )
                for item in parmsTmp:
                    tmp = item.strip().split( '=' )
                    params[ tmp[ 0 ] ] = tmp[ 1 ]
                self.rules[ fileName ] = params
        f.close()

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
            pathDest = self.parseByExt[ ext ]( pathSrc, pathDest, self, self.rules[ pathDest ] if pathDest in self.rules else None )
        else:
            self.RawCopy( pathSrc, pathDest )
        return pathDest

    def MakePath( self, path ):
        dir = os.path.dirname( path )
        if not os.path.isdir( dir ):
            os.mkdir( dir )

    def RawCopy( self, pathSrc, pathDest ):
        self.MakePath( self.releaseDir + pathDest )
        shutil.copyfile( self.rootDir + pathSrc, self.releaseDir + pathDest )

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

    def WriteFilesList( self ):
        fileName = self.releaseDir + self.dataDestName + '/' + '.list'
        f = open( fileName, 'w+' )
        f.write( '//Generated: %s\n' % ( datetime.date.today().strftime( '%d.%m.%Y' ) + time.strftime( ' - %H:%M:%S' ) ) )
        for file in self.filesList:
            f.write( file + ':' + self.filesList[ file ] + '\n' )
            print( file + '\t=> ' + self.filesList[ file ] )
        f.close()

#class FileParser

#
#
#
class FileParserWin32( FileParser ):
    def __init__( self, config = {} ):
        self.SetConfig( config )
        self.platform = 'win32'
        self.filesList = {}
        self.parseByExt = {
            '.fs': all_parse_shader.Do,
            '.vs': all_parse_shader.Do,
            '.tga': all_parse_picture.Do,
            '.jpg': all_parse_picture.Do,
            '.png': all_parse_picture.Do,
        }
    #def __init__
#class FileParserWin32

#
#
#
class FileParserLinux( FileParser ):
    def __init__( self, config = {} ):
        self.SetConfig( config )
        self.platform = 'linux'
        self.filesList = {}
        self.parseByExt = {
            '.fs': all_parse_shader.Do,
            '.vs': all_parse_shader.Do,
            '.tga': all_parse_picture.Do,
            '.jpg': all_parse_picture.Do,
            '.png': all_parse_picture.Do,
        }
    #def __init__
#class FileParserLinux

#
#
#
class FileParserAndroid( FileParser ):
    def __init__( self, config = {} ):
        self.SetConfig( config )
        self.platform = 'android'
        self.filesList = {}
        self.parseByExt = {
            '.fs': all_parse_shader.Do,
            '.vs': all_parse_shader.Do,
            '.tga': all_parse_picture.Do,
            '.jpg': all_parse_picture.Do,
            '.png': all_parse_picture.Do,
        }
    #def __init__

    def ParseImage( self, pathSrc, pathDest ):
        print( '[android/pack-image] file: ' + pathDest )
        pass
    #def ParseImage
#class FileParserAndroid

#Begin

def CreateWin32Parser():
    return FileParserWin32( dict( release_dir = 'C:/temp/git/Biominator/vc/Biominator/', data_src_name = 'data', data_dest_name = 'data', rules = '.rules-win32' ) )

def CreateAndroidParser():
    return FileParserAndroid( dict( release_dir = 'j:/android/projects/Biominator/', data_src_name = 'data', data_dest_name = 'assets', rules = '.rules-android' ) )

def CreateLinuxParser():
    return FileParserLinux( dict( release_dir = '/home/komato3/workspace/Biominator/linux/', data_src_name = 'data', data_dest_name = 'data', rules = '.rules-linux' ) )

moduleTimerStart = GetCurrentTime()
Run( dict(
        win32 = CreateWin32Parser,
        android = CreateAndroidParser,
        linux = CreateLinuxParser,
    ).get( platform )() )
moduleTime = GetCurrentTime() - moduleTimerStart
print( '\n=========================' )
print( datetime.date.today().strftime( '%d.%m.%Y' ) + time.strftime( ' - %H:%M:%S' ) )
print( 'Elapsed time: %.3fs\nDone' % ( moduleTime / 1000 ) )
input()
