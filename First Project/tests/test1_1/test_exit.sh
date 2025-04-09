#!/bin/bash

# Test 1: Menguji perintah 'exit'
echo "Menjalankan tes pertama: Prompt dan perintah exit"
echo "Memasukkan perintah: exit"
echo "Harus keluar setelah mengetik 'exit'"
echo "----------------------------"

# Menjalankan shell untuk tes
./bin/shell <<EOF
exit
EOF
