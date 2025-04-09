#!/bin/bash

# Test 2: Menguji eksekusi perintah 'ls'
echo "Menjalankan tes kedua: Eksekusi perintah ls"
echo "Memasukkan perintah: ls"
echo "Harus menampilkan daftar file dan direktori"
echo "----------------------------"

# Menjalankan shell untuk tes
./bin/shell <<EOF
ls
exit
EOF
