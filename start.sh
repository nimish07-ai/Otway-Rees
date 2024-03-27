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
    "Csocket/Csocket.cpp"
    "displayFunctions/displayFunctions.cpp"
    "logger/logger.cpp"
    "message/diffiehelman/diffiehelman.cpp"
    "message/encryption_algorithm/encryption_algorithm.cpp"
    "message/mes_100/message_100.cpp"
    "message/mes_200/message_200.cpp"
    "message/mes_300/message_300.cpp"
    "message/message_helper/message_helper.cpp"
)

# Compile the C++ program with the specified source files
echo "Compiling with the following command:"
echo "g++ -o $2 $1 ${source_files[@]} -lcryptopp"

# Start compilation in the background
(g++ -o "$2" "$1" "${source_files[@]}" -lcryptopp) &

# Get the process ID of the compilation process
pid=$!

# Start loading animation
loading_animation $pid

# Once the compilation is complete, execute the compiled program
# ./"$2"


# g++ client.cpp Csocket/Csocket.cpp displayFunctions/displayFunctions.cpp logger/logger.cpp message/diffiehelman/diffiehelman.cpp message/encryption_algorithm/encryption_algorithm.cpp message/message_helper/message_helper.cpp message/mes_100/message_100.cpp  -lcryptopp
# g++ client.cpp Csocket/Csocket.cpp displayFunctions/displayFunctions.cpp logger/logger.cpp message/diffiehelman/diffiehelman.cpp message/diffiehelman/diffiehelman.cpp message/encryption_algorith/encryption_algorith.cpp message/mes_100/message_100.cpp message/main_message.cpp