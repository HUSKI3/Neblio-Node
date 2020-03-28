#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import multiprocessing as mp
import neblio_ci_libs as nci

nci.setup_travis_or_gh_actions_env_vars()

working_dir = os.getcwd()
build_dir = "wallet"
deploy_dir = os.path.join(os.environ['BUILD_DIR'],'deploy', '')

packages_to_install = \
[
"ccache",
"qt5-default",
"qt5-qmake",
"qtbase5-dev-tools",
"qttools5-dev-tools",
"qttools5-dev",
"build-essential",
"libssl-dev",
"libdb++-dev",
"libminiupnpc-dev",
"libqrencode-dev",
"libcurl4-openssl-dev",
"libldap2-dev",
"libidn11-dev",
"librtmp-dev",
"lib32z1",
"libx32z1",
"libx32z1-dev",
"zlib1g",
"zlib1g-dev",
"lib32z1-dev",
"libsodium-dev",
"libboost-all-dev",
"libdbus-glib-1-dev",
"gdb"
]

nci.install_packages_debian(packages_to_install)

nci.mkdir_p(deploy_dir)
os.chdir(build_dir)

nci.call_with_err_code('ccache -s')

# nci.call_with_err_code('python ../build_scripts/CompileOpenSSL-Linux.py')
# nci.call_with_err_code('python ../build_scripts/CompileCurl-Linux.py')
# nci.call_with_err_code('python ../build_scripts/CompileBoost-Linux.py')

# os.environ['PKG_CONFIG_PATH'] = os.path.join(working_dir, build_dir, 'curl_build/lib/pkgconfig/')
# os.environ['OPENSSL_INCLUDE_PATH'] = os.path.join(working_dir, build_dir, 'openssl_build/include/')
# os.environ['OPENSSL_LIB_PATH'] = os.path.join(working_dir, build_dir, 'openssl_build/lib/')

nci.call_with_err_code('ccache -s')

# prepend ccache to the path, necessary since prior steps prepend things to the path
os.environ['PATH'] = '/usr/lib/ccache:' + os.environ['PATH']


nci.call_with_err_code('cmake -DNEBLIO_CMAKE=1 -DCMAKE_BUILD_TYPE=Debug ..')
nci.call_with_err_code("make -j" + str(mp.cpu_count()))

nci.call_with_err_code('ccache -s')

# run tests
os.chdir('./wallet/test')
nci.call_with_err_code('gdb ./neblio-tests')

file_name = '$(date +%Y-%m-%d)---' + os.environ['BRANCH'] + '-' + os.environ['COMMIT'][:7] + '---nebliod---ubuntu16.04.tar.gz'

nci.call_with_err_code('tar -zcvf "' + file_name + '" ./nebliod')
nci.call_with_err_code('mv ' + file_name + ' ' + deploy_dir)
nci.call_with_err_code('echo "Binary package at ' + deploy_dir + file_name + '"')
# set the SOURCE_DIR & SOURCE_PATH env vars, these point to the binary that will be uploaded
nci.call_with_err_code('echo "::set-env name=SOURCE_DIR::'  + deploy_dir + '"')
nci.call_with_err_code('echo "::set-env name=SOURCE_PATH::' + deploy_dir + file_name + '"')

nci.call_with_err_code('ccache -s')

print("")
print("")
print("Building finished successfully.")
print("")
