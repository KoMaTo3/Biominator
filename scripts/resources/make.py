import os, shutil, re

def Run( fileParser ):
  print( "Resources package manager[ "+( fileParser.platform )+" ]" )
  print( "=========================" )
  rootDir = os.path.dirname( __file__ )
  dataDir = rootDir + "/data"
  releaseDir = rootDir + "/release-"+fileParser.platform
  releaseDirBackup = rootDir + "/~release-"+( fileParser.platform )+"_backup"
  fileParser.SetRoot( rootDir )
  fileParser.SetRelease( releaseDir )

  if os.path.isdir( releaseDir ):
    if os.path.isdir( releaseDirBackup ):
      shutil.rmtree( releaseDirBackup )
    #if
    shutil.move( releaseDir, releaseDirBackup )
  #if
  os.mkdir( releaseDir )

  itemsList = ScanDir( dataDir, "data", [ "~release_backup", "release" ] )
  NextDir( itemsList, "", fileParser )
#def Run

def ScanDir( rootDir, setName = "root", excludeNames = [] ):
  res = { "name": setName, "dirs": [], "files": [] }

  for item in os.listdir( rootDir ):
    if item not in excludeNames:
      if os.path.isdir( rootDir + "/" + item ):
        res["dirs"].append( ScanDir( rootDir + "/" + item, item ) )
      else:
        res["files"].append( item )
      #if
    #if
  #for
  
  return res
#def ScanDir

def NextDir( dir, currentDirName = "", fileParser = {} ):
  if not len( dir["files"] ):
    return
  #if
  path = ( currentDirName + "/" if len( currentDirName ) else "" ) + dir["name"]
  for fileName in dir["files"]:
    NextFile( path + "/" + fileName, fileParser )
  #for
  if len( dir["dirs"] ):
    for subDir in dir["dirs"]:
      NextDir( subDir, path, fileParser )
#def NextDir

def NextFile( path, fileParser ):
  fileParser.Parse( path )
#def NextFile

#
#
#
class FileParser:
  def __init__( self ):
    self.platform = "unknown"
    self.parseByExt = {
      '.fs': self.ParseShaderFragment,
    }
  #def __init__

  def SetRoot( self, dir ):
    self.rootDir = dir + "/"
  #SetRoot

  def SetRelease( self, dir ):
    self.releaseDir = dir + "/"
  #SetRoot

  def Parse( self, path ):
    fileInfo = os.path.splitext( path )
    ext = fileInfo[ 1 ]
    if ext in self.parseByExt:
      self.parseByExt[ ext ]( path )
    else:
      self.RawCopy( path )
  #Parse

  def MakePath( self, path ):
    dir = os.path.dirname( path )
    if not os.path.isdir( dir ):
      os.mkdir( dir )
    #if
  #MakePath

  def RawCopy( self, path ):
    self.MakePath( self.releaseDir + path )
    shutil.copyfile( self.rootDir + path, self.releaseDir + path )
    #print( "copy from ["+( self.rootDir + path )+"] to ["+( self.releaseDir + path )+"]" )
    #print( "[raw-copy] file: " + path )
    pass
  #def RawCopy

  def ParseShader( self, path ):
    self.MakePath( self.releaseDir + path )
    fileSrc = open( self.rootDir + path, "r" )
    fileContent = fileSrc.read()
    fileContent = re.sub( r"^\/\/\["+( self.platform )+"\] (.*?)$", r'\1', fileContent, 0, re.I | re.S | re.M )
    fileContent = re.sub( r"^\/\/\[(android|win32|linux)\] .*?$", '', fileContent, 0, re.I | re.S | re.M )
    fileSrc.close()
    fileDest = open( self.releaseDir + path, "w" )
    fileDest.write( fileContent )
    fileDest.close()
    print( "[android/fragment-shader] file: " + path )
  #def ParseShader
#class FileParser

#
#
#
class FileParserWin32( FileParser ):
  def __init__( self ):
    self.platform = "win32"
    self.parseByExt = {
      ".fs": self.ParseShader,
      ".vs": self.ParseShader,
      #".tga": self.ParseImage,
      #".bmp": self.ParseImage,
    }
  #def __init__
#class FileParserWin32

#
#
#
class FileParserAndroid( FileParser ):
  def __init__( self ):
    self.platform = "android"
    self.parseByExt = {
      ".fs": self.ParseShader,
      ".vs": self.ParseShader,
      #".tga": self.ParseImage,
      #".bmp": self.ParseImage,
    }
  #def __init__

  def ParseImage( self, path ):
    print( "[android/pack-image] file: " + path )
    pass
  #def ParseImage
#class FileParserAndroid

#Begin
Run( FileParserWin32() )
Run( FileParserAndroid() )

input( "\nDone" )
