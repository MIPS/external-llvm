LOCAL_PATH := $(call my-dir)

mips_mc_desc_TBLGEN_TABLES := \
	MipsGenSubtargetInfo.inc \
	MipsGenRegisterInfo.inc	\
	MipsGenInstrInfo.inc \
	MipsGenMCCodeEmitter.inc

mips_mc_desc_SRC_FILES :=   \
	MipsMCAsmInfo.cpp \
	MipsMCTargetDesc.cpp \
	MipsAsmBackend.cpp \
	MipsMCCodeEmitter.cpp

# For the host
# =====================================================
include $(CLEAR_VARS)
include $(CLEAR_TBLGEN_VARS)

TBLGEN_TABLES := $(mips_mc_desc_TBLGEN_TABLES)

LOCAL_SRC_FILES := $(mips_mc_desc_SRC_FILES)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_MODULE:= libLLVMMipsDesc

LOCAL_MODULE_TAGS := optional

include $(LLVM_HOST_BUILD_MK)
include $(LLVM_TBLGEN_RULES_MK)
include $(LLVM_GEN_INTRINSICS_MK)
include $(BUILD_HOST_STATIC_LIBRARY)


# For the device only
# =====================================================
ifeq ($(TARGET_ARCH),mips)
include $(CLEAR_VARS)
include $(CLEAR_TBLGEN_VARS)

TBLGEN_TABLES := $(mips_mc_desc_TBLGEN_TABLES)

LOCAL_SRC_FILES := $(mips_mc_desc_SRC_FILES)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_MODULE:= libLLVMMipsDesc

LOCAL_MODULE_TAGS := optional

include $(LLVM_DEVICE_BUILD_MK)
include $(LLVM_TBLGEN_RULES_MK)
include $(LLVM_GEN_INTRINSICS_MK)
include $(BUILD_STATIC_LIBRARY)
endif
