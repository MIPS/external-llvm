LOCAL_PATH := $(call my-dir)

mips_codegen_TBLGEN_TABLES :=	\
	MipsGenRegisterInfo.inc \
	MipsGenMCCodeEmitter.inc \
	MipsGenInstrInfo.inc \
	MipsGenAsmWriter.inc \
	MipsGenDAGISel.inc \
	MipsGenCallingConv.inc \
	MipsGenCodeEmitter.inc \
	MipsGenSubtargetInfo.inc

mips_codegen_SRC_FILES :=	\
	MipsAsmPrinter.cpp \
	MipsDelaySlotFiller.cpp \
	MipsEmitGPRestore.cpp \
	MipsExpandPseudo.cpp  \
	MipsFrameLowering.cpp \
	MipsInstrInfo.cpp \
	MipsISelLowering.cpp \
	MipsISelDAGToDAG.cpp \
	MipsCodeEmitter.cpp \
	MipsSubtarget.cpp \
	MipsTargetMachine.cpp \
	MipsSelectionDAGInfo.cpp \
	MipsRegisterInfo.cpp \
	MipsTargetObjectFile.cpp \
	MipsJITInfo.cpp \
	MipsMCInstLower.cpp

# For the host
# =====================================================
ifeq ($(TARGET_ARCH),mips)
include $(CLEAR_VARS)
include $(CLEAR_TBLGEN_VARS)

TBLGEN_TABLES := $(mips_codegen_TBLGEN_TABLES)

LOCAL_SRC_FILES := $(mips_codegen_SRC_FILES)

LOCAL_MODULE:= libLLVMMipsCodeGen

LOCAL_MODULE_TAGS := optional

include $(LLVM_HOST_BUILD_MK)
include $(LLVM_TBLGEN_RULES_MK)
include $(LLVM_GEN_INTRINSICS_MK)
include $(BUILD_HOST_STATIC_LIBRARY)

# For the device only
# =====================================================
include $(CLEAR_VARS)
include $(CLEAR_TBLGEN_VARS)

TBLGEN_TABLES := $(mips_codegen_TBLGEN_TABLES)

LOCAL_SRC_FILES := $(mips_codegen_SRC_FILES)

LOCAL_MODULE:= libLLVMMipsCodeGen

LOCAL_MODULE_TAGS := optional

include $(LLVM_DEVICE_BUILD_MK)
include $(LLVM_TBLGEN_RULES_MK)
include $(LLVM_GEN_INTRINSICS_MK)
include $(BUILD_STATIC_LIBRARY)
endif

