SWIG="/dev/swig-4.3.1/bin/swig"
$SWIG -I../../../include -v -O -c++ -csharp -namespace FreeSWITCH.Native -dllimport mod_core -DSWIG_CSHARP_NO_STRING_HELPER freeswitch.i
rm -f swig.csx

for f in *.cs; do
    cat "$f" >> swig.csx
done

mkdir -p core
mv swig.csx core/swig.cs

rm -f *.cs

dos2unix core/swig.cs
dos2unix freeswitch_wrap.cxx
