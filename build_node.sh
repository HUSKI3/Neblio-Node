#python ./build_scripts/CompileOpenSSL-Linux.py
#python ./build_scripts/CompileCurl-Linux.py
#python ./build_scripts/CompileQREncode-Linux.py
#python ./build_scripts/CompileBoost-Linux.py
export PKG_CONFIG_PATH=$PWD/curl_build/lib/pkgconfig/
export OPENSSL_INCLUDE_PATH=$PWD/openssl_build/include/
export OPENSSL_LIB_PATH=$PWD/openssl_build/lib/
export BOOST_INCLUDE_PATH=$PWD/boost_build/include/
export BOOST_LIB_PATH=$PWD/boost_build/lib/
export QRENCODE_INCLUDE_PATH=$PWD/qrencode_build/include/
export QRENCODE_LIB_PATH=$PWD/qrencode_build/lib/
cd wallet && make "STATIC=1" -B -w -f makefile.unix -j4 && strip ./nebliod