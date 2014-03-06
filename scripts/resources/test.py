import imagecompress

print('Start')

image = imagecompress.tga2rgba( 'C:/temp/git/Biominator/vc/Biominator/data/textures/glow.tga' )
print( 'Image: %ix%i data[%i]' % ( image['width'], image['height'], image['length'] ) )

dxt1 = imagecompress.rgba2dxt5( image )
print( 'Compressed image: %ix%i data[%i]' % ( dxt1['width'], dxt1['height'], dxt1['length'] ) )
print( dxt1 )

imagecompress.free()

input('End')
