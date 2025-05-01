RACK_DIR ?= ../..
METAMODULE_SDK_DIR ?= metamodule-plugin-sdk

SOURCES =
SOURCES += src/vcv/comm_module.cc
SOURCES += src/plugin-vcv.cc

# Nothing is in this dir, but see README for its use
SOURCES += $(wildcard src/modules/vcv/*.cc)
SOURCES += $(wildcard src/modules/vcv/*.cpp)

INCLUDES = -Isrc \
		   -Isrc/modules \
		   -I$(METAMODULE_SDK_DIR)/metamodule-core-interface \
		   -I$(METAMODULE_SDK_DIR)/cpputil

FLAGS += $(INCLUDES) -std=c++20

DISTRIBUTABLES += $(wildcard LICENSE*) res

MMBUILD_DIR = build

res/%.svg: src/modules/svg/%_info.svg
	scripts/svgextract/svgextract.py createVcvSvg $< $@

MODULES := $(notdir $(wildcard src/modules/svg/*_info.svg))
VCV_SVGS := $(addprefix res/,$(MODULES:_info.svg=.svg))

update-svg: $(VCV_SVGS)

.PHONY: mm config clean-mm

include $(RACK_DIR)/plugin.mk

$(MMBUILD_DIR)/CMakeCache.txt:
	cmake --fresh -B $(MMBUILD_DIR) -G Ninja -DMETAMODULE_SDK_DIR=$(METAMODULE_SDK_DIR)

config:
	cmake --fresh -B $(MMBUILD_DIR) -G Ninja -DMETAMODULE_SDK_DIR=$(METAMODULE_SDK_DIR) -DINSTALL_DIR=$(INSTALL_DIR)


mm: $(MMBUILD_DIR)/CMakeCache.txt
	cmake --build $(MMBUILD_DIR)
	
clean-mm:
	rm -rf metamodule-plugins

clean: clean-mm
