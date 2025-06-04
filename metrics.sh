#!/bin/bash

FILES=(
  "test_files/c_40t_100.txt"
  "test_files/c_100t_100000.txt"
  "test_files/c_100t_10000000.txt"
)

THREADS=(1 4 8)
VERSIONS=("main" "main_v2" "main_v3")

echo "Versión,Archivo,Hilos,Promedio (s)"

for version in "${VERSIONS[@]}"; do
  exe="src/code"
  src_file="src/${version}.c"

  echo "Compilando $src_file..."
  gcc "$src_file" -o "$exe" -lpthread || {
    echo "Error compilando $src_file"
    continue
  }

  for file in "${FILES[@]}"; do
    for threads in "${THREADS[@]}"; do
      sum=0
      for i in {1..3}; do
        output=$($exe "$file" "$threads")
        tiempo=$(echo "$output" | head -n 1) # toma solo la primera línea por seguridad
        if [[ -z "$tiempo" ]]; then
          tiempo=0
        fi
        sum=$(echo "$sum + $tiempo" | bc -l)
      done
      avg=$(echo "scale=6; $sum / 3" | bc -l)
      echo "${version},${file},${threads},${avg}"
    done
  done
done
