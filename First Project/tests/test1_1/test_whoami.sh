#!/bin/bash

# Test 3: Menguji prompt dengan nama pengguna dan nama mesin
echo "Menjalankan tes ketiga: Memeriksa apakah prompt menampilkan nama pengguna dan nama mesin"
echo "Memasukkan perintah: whoami"
echo "Harus menampilkan nama pengguna yang aktif"
echo "----------------------------"

# Menjalankan shell untuk tes
./bin/shell <<EOF
whoami
exit
EOF
