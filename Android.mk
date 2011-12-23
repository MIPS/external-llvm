LOCAL_PATH := $(call my-dir)
LLVM_ROOT_PATH := $(LOCAL_PATH)
LLVM_ENABLE_ASSERTION := false

include $(CLEAR_VARS)

subdirs := $(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, \
		lib/Support	\
		lib/TableGen	\
		utils/TableGen	\
		tools/llvm-as	\
		tools/llvm-link \
		lib/VMCore	\
		lib/Bitcode/Reader	\
		lib/Bitcode/Writer	\
		lib/Analysis	\
		lib/Analysis/IPA	\
		lib/AsmParser \
		lib/Transforms/IPO	\
		lib/Transforms/Utils	\
		lib/Transforms/Scalar	\
		lib/Transforms/InstCombine	\
		lib/Transforms/Instrumentation	\
		lib/CodeGen	\
		lib/CodeGen/SelectionDAG	\
		lib/CodeGen/AsmPrinter	\
		lib/Target	\
		lib/ExecutionEngine/JIT	\
		lib/MC	\
		lib/MC/MCParser	\
		lib/Target/ARM \
		lib/Target/ARM/AsmParser \
		lib/Target/ARM/InstPrinter \
		lib/Target/ARM/TargetInfo \
		lib/Target/ARM/MCTargetDesc \
		lib/Target/X86  \
        lib/Target/X86/AsmParser    \
        lib/Target/X86/InstPrinter  \
        lib/Target/X86/MCTargetDesc \
        lib/Target/X86/TargetInfo   \
        lib/Target/X86/Utils \
		lib/Target/Mips \
		lib/Target/Mips/InstPrinter \
		lib/Target/Mips/TargetInfo \
		lib/Target/Mips/MCTargetDesc \
        lib/Linker      \
	))

ifneq ($(TARGET_ARCH),mips)
subdirs += $(addprefix $(LOCAL_PATH)/,$(addsuffix /Android.mk, \
        lib/Target/X86/Disassembler \
		lib/Target/ARM/Disassembler \
    ))
endif

include $(LOCAL_PATH)/llvm.mk

include $(subdirs)

