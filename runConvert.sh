for RUN in 111 113 114 116 117 118 120 121 122 127 129 136 138 139 140 141 142 143 144
do
    echo ./convert.sh $RUN run$RUN.root
    ./convert.sh $RUN run$RUN.root
    wait
done
