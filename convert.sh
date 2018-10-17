echo $1 $2
build/b8_texat_convertWaveform $1
wait
mv output.root $2