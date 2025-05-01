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

.PHONY: mm config clean-mm 

clean: clean-mm 

# 
# Build Rack plugin
#

include $(RACK_DIR)/plugin.mk

# 
# Build MetaModule Plugin
#

$(MMBUILD_DIR)/CMakeCache.txt:
	cmake --fresh -B $(MMBUILD_DIR) -G Ninja -DMETAMODULE_SDK_DIR=$(METAMODULE_SDK_DIR)

config:
	cmake --fresh -B $(MMBUILD_DIR) -G Ninja -DMETAMODULE_SDK_DIR=$(METAMODULE_SDK_DIR) -DINSTALL_DIR=$(INSTALL_DIR)

mm: $(MMBUILD_DIR)/CMakeCache.txt
	cmake --build $(MMBUILD_DIR)
	
clean-mm:
	rm -rf metamodule-plugins
	rm -rf $(MMBUILD_DIR)


# 
# Generate SVGs, PNGs, info headers
#

INFO_SVGS := $(notdir $(wildcard src/modules/svg/*_info.svg))
MODULES := $(INFO_SVGS:_info.svg=)
VCV_SVGS := $(addsuffix .svg,$(addprefix res/,$(MODULES)))
INFO_HEADERS := $(addsuffix _info.hh,$(addprefix src/modules/info/,$(MODULES)))
MODULE_PNGS := $(addsuffix .png,$(addprefix assets/,$(MODULES)))

vcv-svgs: $(VCV_SVGS)

info-headers: $(INFO_HEADERS)

asset-pngs: $(MODULE_PNGS)

res/%.svg: src/modules/svg/%_info.svg
	scripts/svgextract/svgextract.py createVcvSvg $< $@


src/modules/info/%.hh: src/modules/svg/%.svg
	scripts/svgextract/svgextract.py createInfo $< src/modules/info/ 4msDrums

assets/%.png: res/%.svg
	scripts/svgextract/svgextract.py convertFaceplateToPng $< assets/
