add_llvm_library(LLVMDebugInfo
  DIContext.cpp
  DWARFAbbreviationDeclaration.cpp
  DWARFCompileUnit.cpp
  DWARFContext.cpp
  DWARFDebugAbbrev.cpp
  DWARFDebugArangeSet.cpp
  DWARFDebugAranges.cpp
  DWARFDebugInfoEntry.cpp
  DWARFDebugLine.cpp
  DWARFFormValue.cpp
  )

add_llvm_library_dependencies(LLVMDebugInfo
  LLVMSupport
  )
