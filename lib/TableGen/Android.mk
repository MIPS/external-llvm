LOCAL_PATH:= $(call my-dir)

libtblgen_SRC_FILES :=	\
	Error.cpp \
	Main.cpp \
	Record.cpp \
	TableGenBackend.cpp \
	TGLexer.cpp \
	TGParser.cpp

# For the host
# =====================================================
include $(CLEAR_VARS)

LOCAL_SRC_FILES := $(libtblgen_SRC_FILES)

LOCAL_MODULE:= libLLVMTblGen

REQUIRES_EH := 1
REQUIRES_RTTI := 1

LOCAL_MODULE_TAGS := optional

include $(LLVM_HOST_BUILD_MK)
include $(BUILD_HOST_STATIC_LIBRARY)

