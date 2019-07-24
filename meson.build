project('Blobwars', 'cpp',
	version: '2.00',
	license: 'GPL2+',
	default_options: [
		'cpp_std=c++17',
	]
)

fmt = dependency('fmt')
sdl2 = dependency('SDL2')
sdl2_image = dependency('SDL2_image')
sdl2_mixer = dependency('SDL2_mixer')
sdl2_net = dependency('SDL2_net')
sdl2_ttf = dependency('SDL2_ttf')
yaml_cpp = dependency('yaml-cpp')
zlib = dependency('zlib')

prefixdir = get_option('prefix')
bindir = join_paths(prefixdir, 'games')
datadir = join_paths(prefixdir, get_option('datadir'), 'games', 'blobwars')
desktopdir = join_paths(prefixdir, get_option('datadir'), 'applications')
docdir = join_paths(prefixdir, get_option('datadir'), 'doc', 'blobwars')
icondir = join_paths(prefixdir, get_option('datadir'), 'icons', 'hicolor')
localedir = join_paths(prefixdir, get_option('localedir'))

add_global_arguments('-DVERSION=' + meson.project_version(), language: 'cpp')
add_global_arguments('-DUSEPAK=0', language: 'cpp')

add_global_arguments('-DLOCALEDIR="' + localedir + '"', language: 'cpp')
add_global_arguments('-DPAKNAME="blobwars.pak"', language: 'cpp')
add_global_arguments('-DPAKLOCATION="' + datadir + '"', language: 'cpp')
add_global_arguments('-DGAMEPLAYMANUAL="' + join_paths(docdir, 'manual.html') + '"', language: 'cpp')

add_global_arguments('-DMEDAL_SERVER_HOST="www.stephenjsweeney.com"', language: 'cpp')
add_global_arguments('-DMEDAL_SERVER_PORT=80', language: 'cpp')

if get_option('buildtype').startswith('debug')
	add_global_arguments('-DRELEASE=0', language: 'cpp')
else
	add_global_arguments('-DRELEASE=1', language: 'cpp')
endif

subdir('src')

install_subdir('data', install_dir: datadir)
install_subdir('gfx', install_dir: datadir)
install_subdir('music', install_dir: datadir)
install_subdir('sound', install_dir: datadir)

install_subdir('doc', install_dir: docdir, strip_directory: true)

install_data(
	[
		'icons/blobwars16x16.png',
		'icons/blobwars32x32.png',
		'icons/blobwars48x48.png',
		'icons/blobwars64x64.png',
	],
	rename: [
		'16x16/apps/blobwars.png',
		'32x32/apps/blobwars.png',
		'48x48/apps/blobwars.png',
		'64x64/apps/blobwars.png',
	],
	install_dir: icondir,
)

install_data(
	[
		'icons/blobwars.desktop',
		'icons/blobwars.ico',
	],
	install_dir: desktopdir,
)
