.PHONY: build_battito_lib copy_lib build_max_external copy_max_external

all: build_battito_lib copy_lib build_max_external copy_max_external

build_battito_lib:
	cd ~/projects/battito; cargo build --release

copy_lib:
	cp ~/projects/battito/target/release/libbattito_max.a .
	cp ~/projects/battito/battito-max/src/battito_max.h .

build_max_external:
	xcodebuild -scheme max-external build

copy_max_external:
	rm -rf ~/Documents/Max\ 8/Library/battito.mxo
	cp -r ../../../externals/battito.mxo ~/Documents/Max\ 8/Library/
