
CPU_NUMS=8

if [ $# -gt 0 ]
then
	CPU_NUMS=$1
fi

rm -rf build
mkdir -p build
pushd build
cmake .. && make -j$CPU_NUMS
popd