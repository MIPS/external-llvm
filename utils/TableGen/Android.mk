LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES :=	\
	AsmMatcherEmitter.cpp   \
	AsmWriterEmitter.cpp    \
	AsmWriterInst.cpp   \
	CallingConvEmitter.cpp  \
	CodeEmitterGen.cpp  \
	CodeGenDAGPatterns.cpp  \
	CodeGenInstruction.cpp  \
	CodeGenTarget.cpp   \
	CodeGenRegisters.cpp \
	DAGISelEmitter.cpp  \
	DAGISelMatcher.cpp  \
	DAGISelMatcherEmitter.cpp   \
	DAGISelMatcherGen.cpp   \
	DAGISelMatcherOpt.cpp   \
	DisassemblerEmitter.cpp \
	EDEmitter.cpp   \
	FastISelEmitter.cpp \
	FixedLenDecoderEmitter.cpp \
	InstrEnumEmitter.cpp    \
	InstrInfoEmitter.cpp    \
	IntrinsicEmitter.cpp    \
	PseudoLoweringEmitter.cpp \
	RegisterInfoEmitter.cpp \
	SetTheory.cpp \
	StringMatcher.cpp \
	SubtargetEmitter.cpp    \
	TGValueTypes.cpp    \
	TableGen.cpp    \
	X86DisassemblerTables.cpp   \
	X86RecognizableInstr.cpp

REQUIRES_EH := 1
REQUIRES_RTTI := 1

LOCAL_STATIC_LIBRARIES := libLLVMTblGen \
						  libLLVMSupport

LOCAL_MODULE := tblgen
LOCAL_LDLIBS += -lm
ifeq ($(HOST_OS),windows)
  LOCAL_LDLIBS += -limagehlp -lpsapi
else
  LOCAL_LDLIBS += -lpthread -ldl
endif

LOCAL_MODULE_TAGS := optional

include $(LLVM_HOST_BUILD_MK)
include $(BUILD_HOST_EXECUTABLE)
