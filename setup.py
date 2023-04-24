from setuptools import setup, Extension
from Cython.Build import cythonize
from sys import platform


def readme():
    with open("README.md") as f:
        return f.read()


extra_compile_args = []
extra_link_args = []

if platform == "win32":
    extra_compile_args = ["/std:c++17", "/O2"]
elif platform == "linux":
    extra_compile_args = ["-std=c++17", "-O3"]
    extra_link_args = ["-Wl,-O3"]
elif platform == "darwin":  # macOS
    extra_compile_args = ["-std=c++17", "-O3"]
    extra_link_args = ["-Wl,-dead_strip"]

setup(
    name="nazo_rand",
    ext_modules=cythonize(
        Extension(
            name="",
            sources=["ip2asn/ip2asn.pyx", "ip2asn/ipasn.hpp"],
            language="c++",
            extra_compile_args=extra_compile_args,
            extra_link_args=extra_link_args,
        ),
        compiler_directives={
            "language_level": 3,
            "boundscheck": False,
            "wraparound": False,
            "binding": True,
            "cdivision": True,
        },
    ),
    author="bymoye",
    author_email="s3moye@gmail.com",
    version="0.0.1",
    description="A ip2asn tools for python.",
    long_description=readme(),
    long_description_content_type="text/markdown",
    license="MIT",
    package_data={
        "": [
            "ip2asn/ip2asn.pyi",
            "ip2asn/ip2asn.pyx",
        ]
    },
    include_package_data=True,
    classifiers=[
        "Development Status :: 5 - Production/Stable",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Cython",
        "Programming Language :: C++",
        "Operating System :: POSIX :: Linux",
        "Operating System :: MacOS :: MacOS X",
        "Topic :: Software Development :: Libraries :: Python Modules",
    ],
    python_requires=">=3.8",
    packages=["ip2asn"],
)
