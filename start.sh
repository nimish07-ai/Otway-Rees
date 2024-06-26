#!/bin/bash

# Check if the correct number of arguments is provided
if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <source_file.cpp> <output_filename>"
    exit 1
fi

# Function to display loading animation
function loading_animation() {
    local pid=$1
    local delay=0.75
    local spin='-\|/'
    while ps a | awk '{print $1}' | grep -q "$pid"; do
        local temp=${spin#?}
        printf " [%c]  " "$spin"
        local spin=$temp${spin%"$temp"}
        sleep $delay
        printf "\b\b\b\b\b\b"
    done
    printf "    \b\b\b\b"
}

# Array to store the list of source files
source_files=(
    "global/global.cpp"
    "sharedmaps/sharedmaps.cpp"
    "sharedmaps/shared_maps_methods.cpp"
    "Csocket/Csocket.cpp"
    "displayFunctions/displayFunctions.cpp"
    "logger/logger.cpp"
    "message/diffiehelman/diffiehelman.cpp"
    "message/encryption_algorithm/encryption_algorithm.cpp"
    "message/main_message.cpp"
    "message/mes_100/message_100.cpp"
    "message/mes_200/message_200.cpp"
    "message/mes_300/message_300.cpp"
    "message/mes_400/message_400.cpp"
    "message/message_helper/message_helper.cpp"
)

# Compile the C++ program with the specified source files
echo "Compiling with the following command:"
echo "g++ -o $2 $1 ${source_files[*]} -lcryptopp"

# Start compilation in the background
(g++ -o "$2" "$1" "${source_files[@]}" -lcryptopp) &

# Get the process ID of the compilation process
pid=$!

# Start loading animation
loading_animation $pid

# Wait for compilation to finish
wait $pid

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. You can run the compiled code using ./$2"
    ./"$2"
else
    echo "Compilation failed. Please check the error messages."
fi
