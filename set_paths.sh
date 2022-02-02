#!/bin/bash

install_dir="$( pwd )/DiffNN-Code"
export LD_LIBRARY_PATH="$install_dir/lib:$LD_LIBRARY_PATH"
export LIBRARY_PATH="$install_dir/lib:$LIBRARY_PATH"
export C_INCLUDE_PATH="$install_dir/include:$C_INCLUDE_PATH"

echo "Paths set. To make the changes permanent add the following lines to your preferred .*rc file:"
echo "export LD_LIBRARY_PATH=\"$install_dir/lib:\$LD_LIBRARY_PATH\""
echo "export LIBRARY_PATH=\"$install_dir/lib:\$LIBRARY_PATH\""
echo "export C_INCLUDE_PATH=\"$install_dir/include:$C_INCLUDE_PATH\""


