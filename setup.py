#!/usr/bin/python
#   vim:fileencoding=utf-8
# (c) 2012-2020 Michał Górny <mgorny@gentoo.org>
# Released under the terms of the 2-clause BSD license.

from distutils.core import setup, Extension

import os
import shlex
import subprocess
import sys


PKG_CONFIG = os.environ.get('PKG_CONFIG', 'pkg-config')


def pkgconfig_get(*args):
    subp = subprocess.Popen(shlex.split(PKG_CONFIG) + list(args),
                            stdout=subprocess.PIPE)
    output = subp.communicate()[0]
    if subp.returncode != 0:
        sys.exit('\npkg-config failed to find %s, please install it first.'
                 % args[-1])
    return output.decode('utf8').strip().split()


cflags, libs = [pkgconfig_get(x, 'libh2o >= 0.2.1')
                for x in ('--cflags', '--libs')]


setup(
    name='pyh2o',
    version='1',
    author='Michał Górny',
    author_email='mgorny@gentoo.org',
    url='https://github.com/mgorny/pyh2o/',
    ext_modules=[
        Extension('h2o',
                  extra_compile_args=cflags,
                  extra_link_args=libs,
                  sources=['src/h2o.c',
                           'src/module.c',
                           'src/module2.c',
                           'src/module3.c',
                           ]),
    ],
    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Science/Research',
        'License :: OSI Approved :: BSD License',
        'Operating System :: OS Independent',
        'Programming Language :: C',
        'Topic :: Scientific/Engineering :: Physics',
    ],
    )
