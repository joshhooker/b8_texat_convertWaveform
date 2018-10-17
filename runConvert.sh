for RUN in 172 175 178 180 181 182 183 184 185 186 187 188 189 190 191 192 193 195 196 197 198 199 200 201 202 203 204 205 206 207 208 210 211 212 213 214 215 216 217 218 219 220 221 223 224
do
    echo ./convert.sh $RUN run$RUN.root
    ./convert.sh $RUN run$RUN.root
    # echo ./convert.sh run$RUN.root /hd3/research/data/run0817a/rootM2R-WaveformModule/run$RUN.root
    # ./convert.sh run$RUN.root /hd3/research/data/run0817a/rootM2R-WaveformModule/run$RUN.root
    wait
done