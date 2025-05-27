RACK_DIR ?= ../..
METAMODULE_SDK_DIR ?= metamodule-plugin-sdk

BRAND_SLUG := 4ms-XOXDrums

SOURCES =
SOURCES += src/vcv/comm_module.cc
SOURCES += src/plugin-vcv.cc

INCLUDES = -Isrc \
		   -Isrc/modules \
		   -I$(METAMODULE_SDK_DIR)/metamodule-core-interface \
		   -I$(METAMODULE_SDK_DIR)/cpputil

FLAGS += $(INCLUDES) -std=c++20

DISTRIBUTABLES += $(wildcard LICENSE*) res

MMBUILD_DIR = build

MM_INSTALL_DIR := $(PWD)/metamodule-plugins

.PHONY: mm config clean-mm 

clean: clean-mm 

# 
# Build Rack plugin (`make all`)
#

MACHINE := $(shell $(CC) -dumpmachine)
ifneq (,$(findstring -darwin,$(MACHINE)))
	EXTRA_FLAGS := -mmacosx-version-min=10.13
endif

include $(RACK_DIR)/plugin.mk

# 
# Build MetaModule Plugin
#

$(MMBUILD_DIR)/CMakeCache.txt: config

config:
	cmake --fresh -B $(MMBUILD_DIR) -G Ninja -DMETAMODULE_SDK_DIR=$(METAMODULE_SDK_DIR) -DMM_INSTALL_DIR=$(MM_INSTALL_DIR)

mm: $(MMBUILD_DIR)/CMakeCache.txt
	cmake --build $(MMBUILD_DIR)
	
clean-mm:
	rm -rf $(MM_INSTALL_DIR)
	rm -rf $(MMBUILD_DIR)


# 
# Generate SVGs, PNGs, info headers
#

INFO_SVGS := $(notdir $(wildcard module-svg/*_info.svg))
MODULES := $(INFO_SVGS:_info.svg=)
VCV_SVGS := $(addsuffix .svg,$(addprefix res/,$(MODULES)))
INFO_HEADERS := $(addsuffix _info.hh,$(addprefix src/modules/info/,$(MODULES)))
MODULE_PNGS := $(addsuffix .png,$(addprefix assets/,$(MODULES)))

# Call this if an info.svg file is updated
update-images: vcv-svgs info-headers asset-pngs

vcv-svgs: $(VCV_SVGS)

info-headers: $(INFO_HEADERS)

asset-pngs: $(MODULE_PNGS)

res/%.svg: module-svg/%_info.svg
	scripts/vcv-artwork.py --input $< --output $@

src/modules/info/%.hh: module-svg/%.svg
	scripts/createinfo.py --input $< --outdir src/modules/info/ --brand $(BRAND_SLUG)

assets/%.png: res/%.svg
	metamodule-plugin-sdk/scripts/SvgToPng.py --input $< --output assets/ --height 240 --white

