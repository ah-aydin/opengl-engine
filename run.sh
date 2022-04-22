# Variables to keep track of the arguments
debug=0
build=0
run=0
clean=0

# Read arguments
for arg in "$@"
do
    if [ "$arg" = "b" ]
    then
        build=1
    elif [ "$arg" = "d" ] 
    then
        debug=1
    elif [ "$arg" = "r" ]
    then
        run=1
    elif [ "$arg" = "c" ]
    then
        clean=1
    fi
done

# Set the build and bin directories
if [ $debug -gt 0 ] # Debug
then
    build_dir=./build/debug
    bin_dir=./bin/debug
    make_mode=debug
else # Release
    build_dir=./build/release
    bin_dir=./bin/release
    make_mode=release
fi

# Clean
if [ $clean -gt 0 ]
then
    make clean BUILD_DIR=$build_dir
fi

# Build the program
if [ $build -gt 0 ]
then
    make $make_mode BUILD_DIR=$build_dir
    mkdir -p $build_dir
    cp $build_dir/opengl_engine $bin_dir
fi

# Run the program
if [ $run -gt 0 ]
then
    cd $bin_dir
    ./opengl_engine
    cd ../../
fi

