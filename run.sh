debug=0
build=0
run=0
clean=0
# Read arguments
for arg in "$@" 
do
    if [ "$arg" = "d" ]
    then
        debug=1
    elif [ "$arg" = "b" ]
    then
        build=1
    elif [ "$arg" = "r" ]
    then
        run=1
    elif [ "$arg" = "c" ]
    then
        clean=1
    fi
done

# Clean
if [ $clean -gt 0 ]
then
    make clean
fi

# Build the program in debug or release mode and copy the binary file to the proper folder
if [ $build -gt 0 ]
then
    if [ $debug -gt 0 ]
    then
        make debug
        mkdir -p ./bin/debug
        cp ./build/opengl_engine ./bin/debug
    else
        make
        mkdir -p ./bin/release
        cp ./build/opengl_engine ./bin/release
    fi
fi

if [ $run -gt 0 ]
then
    if [ $debug -gt 0 ]
    then
        cd ./bin/debug
        ./opengl_engine
        cd ../../
    else
        cd ./bin/release
        ./opengl_engine
        cd ../../
    fi
fi