#!/bin/python

from waflib import *
import os, sys

top = '.'
out = 'build'

g_cflags = ["-Wall", "-Wextra", "-std=c++17"]

def btype_cflags(ctx):
	
	return {
		"DEBUG"   : g_cflags + ["-Og", "-ggdb3", "-march=core2", "-mtune=native"],
		"NATIVE"  : g_cflags + ["-Ofast", "-march=native", "-mtune=native"],
		"RELEASE" : g_cflags + ["-O3", "-march=core2", "-mtune=generic"],
	}.get(ctx.env.BUILD_TYPE, g_cflags)

# ================================
# OPTIONS
# ================================

def options(opt):
	
	opt.add_option('--build-type', dest='build_type', type="string", default='RELEASE', action='store', help="DEBUG, NATIVE, RELEASE")
	
# ================================
# CONFIGURE
# ================================

def configure(ctx):
	
	ctx.load("g++")
	
	btup = ctx.options.build_type.upper()
	if btup in ["DEBUG", "NATIVE", "RELEASE"]:
		Logs.pprint("PINK", "Setting up environment for known build type: " + btup)
		ctx.env.BUILD_TYPE = btup
		ctx.env.CXXFLAGS = btype_cflags(ctx)
		Logs.pprint("PINK", "CXXFLAGS: " + ' '.join(ctx.env.CXXFLAGS))
		if btup == "DEBUG":
			ctx.define("DEBUG", 1)
		else:
			ctx.define("NDEBUG", 1)
	else:
		Logs.error("UNKNOWN BUILD TYPE: " + btup)
		
# ================================
# BUILD/INSTALL
# ================================
		
def build(bld):
	
	bld_install_files = bld.path.ant_glob('src/meadow/*.hh')
	bld.install_files('${PREFIX}/include/meadow', bld_install_files)
	
	vnum = '0.0.1'
	
	lib = bld (
		features = "cxx cxxshlib",
		target = 'meadow',
		source = bld.path.ant_glob('src/**/*.cc'),
		vnum = vnum
	)

	tests = bld (
		features = "cxx cxxprogram",
		target = 'meadow_tests',
		source = bld.path.ant_glob('test/**/*.cc'),
		use = ['meadow'],
		vnum = vnum
	)
