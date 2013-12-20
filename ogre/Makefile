all: ogre

OGRE_VERSION = ogre_src_v1-7-3
TARBALL = build/$(OGRE_VERSION).tar.bz2
TARBALL_URL = http://static.strawlab.org/downloads/$(OGRE_VERSION).tar.bz2
SOURCE_DIR = build/$(OGRE_VERSION)
UNPACK_CMD = tar xf
MD5SUM_FILE = $(OGRE_VERSION).tar.bz2.md5sum
TARBALL_PATCH = hidden_window.patch hidden_window_glx_fix.patch multi_scene_manager.patch
#INITIAL_DIR = build/ogre
include $(shell rospack find mk)/download_unpack_build.mk

ROOT = $(shell rospack find ogre)
OGREROOT = $(shell rospack find ogre)/ogre

CFLAGS = -g -O3 \
         $(shell rosboost-cfg --cflags)

LFLAGS = $(shell rosboost-cfg --lflags thread) \
         -lpthread

# Ogre tools currently don't build on OSX, so disable building them there.
BUILD_TOOLS = $(shell if test `uname` = Darwin ; then echo off; else echo on; fi)

CMAKE = cmake
CMAKE_ARGS = -D CMAKE_INSTALL_PREFIX=$(OGREROOT)\
                                                 -D CMAKE_OSX_ARCHITECTURES="x86_64"\
                                                 -D OGRE_BUILD_TOOLS="$(BUILD_TOOLS)"\
						 -D OGRE_BUILD_SAMPLES="off"\
						 -D OGRE_BUILD_TESTS="off"\
						 -D CMAKE_BUILD_TYPE="RelWithDebInfo" \
						 -D CMAKE_SHARED_LINKER_FLAGS="-Wl,-rpath,$(ROOT)/ogre/lib"

CMAKE_CMD = $(CMAKE) $(CMAKE_ARGS)

$(SOURCE_DIR)/Makefile: $(SOURCE_DIR)/unpacked changelist.txt
	make clean
	if test `uname` = Darwin ; then \
	  touch $(SOURCE_DIR)/Makefile; \
	else \
	  cd $(SOURCE_DIR) && \
	  export LDFLAGS="$(LFLAGS)" && \
	  export CFLAGS="$(CFLAGS)" && \
	  export CXXFLAGS="$(CFLAGS)" && \
	  mkdir -p $(ROOT)/ogre && \
	  mkdir -p $(ROOT)/ogre/lib && \
	  mkdir -p $(ROOT)/ogre/include && \
	  mkdir -p $(ROOT)/ogre/share && \
	  mkdir -p $(ROOT)/ogre/bin; \
	fi

.PHONY: ogre


 # -target RenderSystem_GL.dylib -target Plugin_CgProgramManager.dylib \
	   -target Plugin_ParticleFX.dylib -target Plugin_OctreeZone.dylib -target libOgrePaging.dylib \
	   -target libOgreProperty.dylib -target libOgreTerrain.dylib -target libOgreRTShaderSystem.dylib -target Ogre
installed_osx: $(SOURCE_DIR)/unpacked
        # Replace @executable_path/../... with @rpath/
	find $(SOURCE_DIR) -name CMakeLists.txt -exec sed -i bak s:executable_path/../Frameworks:rpath/: {} \;
	find $(SOURCE_DIR) -name CMakeLists.txt -exec sed -i bak s:executable_path/../Components:rpath/: {} \;
	find $(SOURCE_DIR) -name CMakeLists.txt -exec sed -i bak s:executable_path/../Plugins:rpath/: {} \;
	mkdir -p $(SOURCE_DIR)/build
	cd $(SOURCE_DIR)/build && \
	  cmake $(CMAKE_ARGS) -G Xcode -DCMAKE_INSTALL_PREFIX=$(ROOT)/ogre .. && \
	  xcodebuild -configuration RelWithDebInfo -target install
	rm -rf ogre/lib/Ogre.framework
	mv ogre/lib/RelWithDebInfo/Ogre.framework ogre/lib
	touch installed_osx

ogre: $(SOURCE_DIR)/unpacked
	./ensure-nvidia-cg-toolkit-is-installed.sh
	@if test `uname` = Darwin ; then \
	  make installed_osx; \
	  if test ! -f ogre/lib/Plugin_CgProgramManager.dylib; then \
	    echo "BUILD FAILED: Plugin_CgProgramManager.dylib not created.   Is nvidia-cg-toolkit installed correctly?"; \
	    exit 1; \
	  fi \
	else \
	  make ogre/lib/pkgconfig/OGRE.pc; \
	  if test ! -f ogre/lib/OGRE/Plugin_CgProgramManager.so; then \
            echo "BUILD FAILED: Plugin_CgProgramManager.so not created.  Is nvidia-cg-toolkit installed correctly?"; \
            exit 1; \
          fi \
	fi

ogre/lib/pkgconfig/OGRE.pc: $(SOURCE_DIR)/CMakeLists.txt $(SOURCE_DIR)
        # building OGRE
	mkdir -p $(ROOT)/$(SOURCE_DIR)/build;
	cd $(ROOT)/$(SOURCE_DIR)/build && $(CMAKE_CMD) ..;
	cd $(ROOT)/$(SOURCE_DIR)/build && make $(ROS_PARALLEL_JOBS); 
	cd $(ROOT)/$(SOURCE_DIR)/build && make install;

	@echo "patch OGRE.pc to pass -Wl,-rpath,-L{libdir}"
	-mv ogre/lib/pkgconfig/OGRE.pc ogre/lib/pkgconfig/OGRE.bak
#	sed 's%exec_prefix=$${prefix}%exec_prefix=$${prefix}\n' ogre/lib/pkgconfig/OGRE.bak > ogre/lib/pkgconfig/OGRE.tmp
	sed 's%Libs: -L$${libdir} -lOgreMain%Libs: -Wl,-rpath,$${libdir},-rpath,$${libdir}/OGRE -L$${libdir} -lOgreMain%g' ogre/lib/pkgconfig/OGRE.bak > ogre/lib/pkgconfig/OGRE.pc
	@echo "done"

.PHONY: clean wipe

clean_osx:
	-cd $(ROOT)/$(SOURCE_DIR)/build && xcodebuild -configuration RelWithDebInfo clean
	-rm -rf ogre
	-rm -rf installed_osx

clean:
	@if test `uname` = Darwin ; then \
	  make clean_osx; \
	else \
	  rm -rf ogre; \
	  rm -rf installed_osx; \
	  cd $(ROOT)/$(SOURCE_DIR)/build && make clean; \
	fi

wipe: 
	-rm -rf build
	-rm -rf ogre

