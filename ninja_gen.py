#!/usr/bin/env python

import re

from subprocess import check_output
from glob import glob
from os.path import dirname, join
from ninja_syntax import Writer

repo_root = dirname(__file__)
src_dir = join(repo_root, 'src')

def sh(cmd):
    return check_output(cmd).decode()

def strip_suffix(filename):
    return re.sub(r'\.[^.]+$', '', filename)

def mglob(*patterns):
    pathnames = []
    for p in patterns:
        pathnames.extend(glob(p))
    return pathnames

app = 'nerdtracker'

deps = [
    'glesv2',
    'glfw3',
    'rtaudio',
    'rtmidi',
]

cpp_compiler = 'g++'
cpp_std = 'c++17'

warn_flags = [
    '-Wall', '-Wextra',
    '-Werror', '-Wfatal-errors',
    '-Wno-unused-function',
    '-Wno-unused-variable',
    '-Wno-unused-parameter',
]

include_flags = [
    '-I{}'.format(repo_root),
    '-I{}'.format(src_dir),
]

cppflags = warn_flags + include_flags + [
    '-std={}'.format(cpp_std),
    '-g',
    sh(['pkgconf', '--cflags'] + deps)
]

c_compiler = 'gcc'
c_std = 'c11'

cflags = warn_flags + include_flags + [
    '-std={}'.format(c_std),
]

linker = 'g++'

ldflags = [
    '-ldl',
    sh(['pkgconf', '--libs'] + deps),
]

cpp_sources = mglob(
    'src/*.cc',
    'src/w/*.cc',
    'src/vfx/*.cc',
    'src/ops/*.cc',
)

c_sources = mglob(
    'src/*.c',
    'ext/*.c',
)

binary_data = {
    'DroidSansMonoTTF': 'fonts/DroidSansMono.ttf'
}

with open('build.ninja', 'w') as f:
    w = Writer(f)
    w.variable('builddir', 'build')
    w.variable('cppflags', cppflags)
    w.variable('cflags', cflags)
    w.variable('ldflags', ldflags)
    w.rule('cpp', 
        command=[cpp_compiler, 
                 '$cppflags', 
                 '-MMD', '-MF', '$out.d', 
                 '-c', 
                 '-o', '$out', 
                 '$in'],
        depfile='$out.d')
    w.rule('cc', 
        command=[c_compiler, 
                 '$cflags', 
                 '-MMD', '-MF', '$out.d', 
                 '-c', 
                 '-o', '$out', 
                 '$in'],
        depfile='$out.d')
    w.rule('bin2c',
        command=['python',
                 'bin2c.py',
                 '$varname',
                 '$in',
                 '$out'])
    objs = []
    for src in cpp_sources:
        stem = strip_suffix(src)
        obj = '$builddir/' + stem + '.o'
        objs.append(obj)
        w.build(obj, rule='cpp', inputs=src)
    for src in c_sources:
        stem = strip_suffix(src)
        obj = '$builddir/' + stem + '.o'
        objs.append(obj)
        w.build(obj, rule='cc', inputs=src)
    for varname, filename in binary_data.items():
        c_file = '$builddir/generated/'+filename+'.c'
        w.build(c_file,
                rule='bin2c', 
                inputs=filename,
                variables={'varname': varname})
        o_file = '$builddir/'+filename+'.o'
        w.build(o_file,
                rule='cc',
                inputs=c_file)
        objs.append(o_file)
    w.rule('ld',
        command=[linker,
                 '-o', '$out', 
                 '$in', 
                 '$ldflags'])
    w.build(app, 'ld', inputs=objs)
