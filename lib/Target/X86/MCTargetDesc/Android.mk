LOCAL_PATH := $(call my-dir)

x86_mc_desc_TBLGEN_TABLES :=	\
	X86GenRegisterInfo.inc	\
	X86GenInstrInfo.inc	\
	X86GenSubtargetInfo.inc

x86_mc_desc_SRC_FILES :=   \
	X86MCAsmInfo.cpp \
	X86MCTargetDesc.cpp \
	X86AsmBackend.cpp \
	X86MachObjectWriter.cpp \
	X86MCCodeEmitter.cpp

# For the host
# =====================================================
include $(CLEAR_VARS)
include $(CLEAR_TBLGEN_VARS)

TBLGEN_TABLES := $(x86_mc_desc_TBLGEN_TABLES)

LOCAL_SRC_FILES := $(x86_mc_desc_SRC_FILES)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_MODULE:= libLLVMX86Desc

LOCAL_MODULE_TAGS := optional

include $(LLVM_HOST_BUILD_MK)
include $(LLVM_TBLGEN_RULES_MK)
include $(LLVM_GEN_INTRINSICS_MK)
include $(BUILD_HOST_STATIC_LIBRARY)

# For the device only
# =====================================================
ifeq ($(TARGET_ARCH),x86)
include $(CLEAR_VARS)
include $(CLEAR_TBLGEN_VARS)

TBLGEN_TABLES := $(x86_mc_desc_TBLGEN_TABLES)

LOCAL_SRC_FILES := $(x86_mc_desc_SRC_FILES)

LOCAL_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_MODULE:= libLLVMX86Desc

LOCAL_MODULE_TAGS := optional

include $(LLVM_DEVICE_BUILD_MK)
include $(LLVM_TBLGEN_RULES_MK)
include $(LLVM_GEN_INTRINSICS_MK)
include $(BUILD_STATIC_LIBRARY)
endif
