#!/bin/bash

set -e -x

PY_MAJOR=${PYTHON_VERSION%%.*}
PY_MINOR=${PYTHON_VERSION#*.}

# yum update
# yum install -y epel-release
# yum install -y boost-devel

yum install -y wget

wget https://archives.boost.io/release/1.87.0/source/boost_1_87_0.tar.gz
tar -xzvf boost_1_87_0.tar.gz
cd boost_1_87_0
./bootstrap.sh --with-libraries=system,thread
sudo ./b2 install

ML_PYTHON_VERSION="cp${PY_MAJOR}${PY_MINOR}-cp${PY_MAJOR}${PY_MINOR}"
if [ "${PY_MAJOR}" -lt "4" -a "${PY_MINOR}" -lt "8" ]; then
    ML_PYTHON_VERSION+="m"
fi

# Compile wheels
PYTHON="/opt/python/${ML_PYTHON_VERSION}/bin/python"
PIP="/opt/python/${ML_PYTHON_VERSION}/bin/pip"
"${PIP}" install --upgrade setuptools pip wheel cython
cd "${GITHUB_WORKSPACE}"

rm -rf dist/
"${PYTHON}" setup.py bdist_wheel

# Bundle external shared libraries into the wheels.
for whl in "${GITHUB_WORKSPACE}"/dist/*.whl; do
    auditwheel repair $whl -w "${GITHUB_WORKSPACE}"/dist/
    rm "${GITHUB_WORKSPACE}"/dist/*-linux_*.whl
done
