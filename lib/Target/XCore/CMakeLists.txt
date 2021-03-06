set(LLVM_TARGET_DEFINITIONS XCore.td)

tablegen(LLVM XCoreGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM XCoreGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM XCoreGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM XCoreGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM XCoreGenCallingConv.inc -gen-callingconv)
tablegen(LLVM XCoreGenSubtargetInfo.inc -gen-subtarget)
add_public_tablegen_target(XCoreCommonTableGen)

add_llvm_target(XCoreCodeGen
  XCoreAsmPrinter.cpp
  XCoreFrameLowering.cpp
  XCoreInstrInfo.cpp
  XCoreISelDAGToDAG.cpp
  XCoreISelLowering.cpp
  XCoreRegisterInfo.cpp
  XCoreSubtarget.cpp
  XCoreTargetMachine.cpp
  XCoreTargetObjectFile.cpp
  XCoreSelectionDAGInfo.cpp
  )

add_llvm_library_dependencies(LLVMXCoreCodeGen
  LLVMAsmPrinter
  LLVMCodeGen
  LLVMCore
  LLVMMC
  LLVMSelectionDAG
  LLVMSupport
  LLVMTarget
  LLVMXCoreDesc
  LLVMXCoreInfo
  )

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)
