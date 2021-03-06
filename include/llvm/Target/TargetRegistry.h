//===-- Target/TargetRegistry.h - Target Registration -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file exposes the TargetRegistry interface, which tools can use to access
// the appropriate target specific classes (TargetMachine, AsmPrinter, etc.)
// which have been registered.
//
// Target specific class implementations should register themselves using the
// appropriate TargetRegistry interfaces.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_TARGET_TARGETREGISTRY_H
#define LLVM_TARGET_TARGETREGISTRY_H

#include "llvm/MC/MCCodeGenInfo.h"
#include "llvm/ADT/Triple.h"
#include <string>
#include <cassert>

namespace llvm {
  class AsmPrinter;
  class Module;
  class MCAssembler;
  class MCAsmInfo;
  class MCAsmParser;
  class MCCodeEmitter;
  class MCContext;
  class MCDisassembler;
  class MCInstPrinter;
  class MCInstrInfo;
  class MCRegisterInfo;
  class MCStreamer;
  class MCSubtargetInfo;
  class MCCodeGenInfo;
  class TargetAsmBackend;
  class TargetAsmLexer;
  class TargetAsmParser;
  class TargetMachine;
  class raw_ostream;
  class formatted_raw_ostream;

  MCStreamer *createAsmStreamer(MCContext &Ctx, formatted_raw_ostream &OS,
                                bool isVerboseAsm,
                                bool useLoc, bool useCFI,
                                MCInstPrinter *InstPrint,
                                MCCodeEmitter *CE,
                                TargetAsmBackend *TAB,
                                bool ShowInst);

  /// Target - Wrapper for Target specific information.
  ///
  /// For registration purposes, this is a POD type so that targets can be
  /// registered without the use of static constructors.
  ///
  /// Targets should implement a single global instance of this class (which
  /// will be zero initialized), and pass that instance to the TargetRegistry as
  /// part of their initialization.
  class Target {
  public:
    friend struct TargetRegistry;

    typedef unsigned (*TripleMatchQualityFnTy)(const std::string &TT);

    typedef MCAsmInfo *(*MCAsmInfoCtorFnTy)(const Target &T,
                                            StringRef TT);
    typedef MCCodeGenInfo *(*MCCodeGenInfoCtorFnTy)(StringRef TT, Reloc::Model M);
    typedef MCInstrInfo *(*MCInstrInfoCtorFnTy)(void);
    typedef MCRegisterInfo *(*MCRegInfoCtorFnTy)(StringRef TT);
    typedef MCSubtargetInfo *(*MCSubtargetInfoCtorFnTy)(StringRef TT,
                                                        StringRef CPU,
                                                        StringRef Features);
    typedef TargetMachine *(*TargetMachineCtorTy)(const Target &T,
                                                  StringRef TT,
                                                  StringRef CPU,
                                                  StringRef Features,
                                                  Reloc::Model RM);
    typedef AsmPrinter *(*AsmPrinterCtorTy)(TargetMachine &TM,
                                            MCStreamer &Streamer);
    typedef TargetAsmBackend *(*AsmBackendCtorTy)(const Target &T,
                                                  const std::string &TT);
    typedef TargetAsmLexer *(*AsmLexerCtorTy)(const Target &T,
                                              const MCAsmInfo &MAI);
    typedef TargetAsmParser *(*AsmParserCtorTy)(MCSubtargetInfo &STI,
                                                MCAsmParser &P);
    typedef MCDisassembler *(*MCDisassemblerCtorTy)(const Target &T);
    typedef MCInstPrinter *(*MCInstPrinterCtorTy)(const Target &T,
                                                  unsigned SyntaxVariant,
                                                  const MCAsmInfo &MAI);
    typedef MCCodeEmitter *(*CodeEmitterCtorTy)(const MCInstrInfo &II,
                                                const MCSubtargetInfo &STI,
                                                MCContext &Ctx);
    typedef MCStreamer *(*ObjectStreamerCtorTy)(const Target &T,
                                                const std::string &TT,
                                                MCContext &Ctx,
                                                TargetAsmBackend &TAB,
                                                raw_ostream &_OS,
                                                MCCodeEmitter *_Emitter,
                                                bool RelaxAll,
                                                bool NoExecStack);
    typedef MCStreamer *(*AsmStreamerCtorTy)(MCContext &Ctx,
                                             formatted_raw_ostream &OS,
                                             bool isVerboseAsm,
                                             bool useLoc,
                                             bool useCFI,
                                             MCInstPrinter *InstPrint,
                                             MCCodeEmitter *CE,
                                             TargetAsmBackend *TAB,
                                             bool ShowInst);

  private:
    /// Next - The next registered target in the linked list, maintained by the
    /// TargetRegistry.
    Target *Next;

    /// TripleMatchQualityFn - The target function for rating the match quality
    /// of a triple.
    TripleMatchQualityFnTy TripleMatchQualityFn;

    /// Name - The target name.
    const char *Name;

    /// ShortDesc - A short description of the target.
    const char *ShortDesc;

    /// HasJIT - Whether this target supports the JIT.
    bool HasJIT;

    /// MCAsmInfoCtorFn - Constructor function for this target's MCAsmInfo, if
    /// registered.
    MCAsmInfoCtorFnTy MCAsmInfoCtorFn;

    /// MCCodeGenInfoCtorFn - Constructor function for this target's MCCodeGenInfo,
    /// if registered.
    MCCodeGenInfoCtorFnTy MCCodeGenInfoCtorFn;

    /// MCInstrInfoCtorFn - Constructor function for this target's MCInstrInfo,
    /// if registered.
    MCInstrInfoCtorFnTy MCInstrInfoCtorFn;

    /// MCRegInfoCtorFn - Constructor function for this target's MCRegisterInfo,
    /// if registered.
    MCRegInfoCtorFnTy MCRegInfoCtorFn;

    /// MCSubtargetInfoCtorFn - Constructor function for this target's
    /// MCSubtargetInfo, if registered.
    MCSubtargetInfoCtorFnTy MCSubtargetInfoCtorFn;

    /// TargetMachineCtorFn - Construction function for this target's
    /// TargetMachine, if registered.
    TargetMachineCtorTy TargetMachineCtorFn;

    /// AsmBackendCtorFn - Construction function for this target's
    /// TargetAsmBackend, if registered.
    AsmBackendCtorTy AsmBackendCtorFn;

    /// AsmLexerCtorFn - Construction function for this target's TargetAsmLexer,
    /// if registered.
    AsmLexerCtorTy AsmLexerCtorFn;

    /// AsmParserCtorFn - Construction function for this target's
    /// TargetAsmParser, if registered.
    AsmParserCtorTy AsmParserCtorFn;

    /// AsmPrinterCtorFn - Construction function for this target's AsmPrinter,
    /// if registered.
    AsmPrinterCtorTy AsmPrinterCtorFn;

    /// MCDisassemblerCtorFn - Construction function for this target's
    /// MCDisassembler, if registered.
    MCDisassemblerCtorTy MCDisassemblerCtorFn;

    /// MCInstPrinterCtorFn - Construction function for this target's
    /// MCInstPrinter, if registered.
    MCInstPrinterCtorTy MCInstPrinterCtorFn;

    /// CodeEmitterCtorFn - Construction function for this target's CodeEmitter,
    /// if registered.
    CodeEmitterCtorTy CodeEmitterCtorFn;

    /// ObjectStreamerCtorFn - Construction function for this target's
    /// ObjectStreamer, if registered.
    ObjectStreamerCtorTy ObjectStreamerCtorFn;

    /// AsmStreamerCtorFn - Construction function for this target's
    /// AsmStreamer, if registered (default = llvm::createAsmStreamer).
    AsmStreamerCtorTy AsmStreamerCtorFn;

  public:
    Target() : AsmStreamerCtorFn(llvm::createAsmStreamer) {}

    /// @name Target Information
    /// @{

    // getNext - Return the next registered target.
    const Target *getNext() const { return Next; }

    /// getName - Get the target name.
    const char *getName() const { return Name; }

    /// getShortDescription - Get a short description of the target.
    const char *getShortDescription() const { return ShortDesc; }

    /// @}
    /// @name Feature Predicates
    /// @{

    /// hasJIT - Check if this targets supports the just-in-time compilation.
    bool hasJIT() const { return HasJIT; }

    /// hasTargetMachine - Check if this target supports code generation.
    bool hasTargetMachine() const { return TargetMachineCtorFn != 0; }

    /// hasAsmBackend - Check if this target supports .o generation.
    bool hasAsmBackend() const { return AsmBackendCtorFn != 0; }

    /// hasAsmLexer - Check if this target supports .s lexing.
    bool hasAsmLexer() const { return AsmLexerCtorFn != 0; }

    /// hasAsmParser - Check if this target supports .s parsing.
    bool hasAsmParser() const { return AsmParserCtorFn != 0; }

    /// hasAsmPrinter - Check if this target supports .s printing.
    bool hasAsmPrinter() const { return AsmPrinterCtorFn != 0; }

    /// hasMCDisassembler - Check if this target has a disassembler.
    bool hasMCDisassembler() const { return MCDisassemblerCtorFn != 0; }

    /// hasMCInstPrinter - Check if this target has an instruction printer.
    bool hasMCInstPrinter() const { return MCInstPrinterCtorFn != 0; }

    /// hasCodeEmitter - Check if this target supports instruction encoding.
    bool hasCodeEmitter() const { return CodeEmitterCtorFn != 0; }

    /// hasObjectStreamer - Check if this target supports streaming to files.
    bool hasObjectStreamer() const { return ObjectStreamerCtorFn != 0; }

    /// hasAsmStreamer - Check if this target supports streaming to files.
    bool hasAsmStreamer() const { return AsmStreamerCtorFn != 0; }

    /// @}
    /// @name Feature Constructors
    /// @{

    /// createMCAsmInfo - Create a MCAsmInfo implementation for the specified
    /// target triple.
    ///
    /// \arg Triple - This argument is used to determine the target machine
    /// feature set; it should always be provided. Generally this should be
    /// either the target triple from the module, or the target triple of the
    /// host if that does not exist.
    MCAsmInfo *createMCAsmInfo(StringRef Triple) const {
      if (!MCAsmInfoCtorFn)
        return 0;
      return MCAsmInfoCtorFn(*this, Triple);
    }

    /// createMCCodeGenInfo - Create a MCCodeGenInfo implementation.
    ///
    MCCodeGenInfo *createMCCodeGenInfo(StringRef Triple, Reloc::Model M) const {
      if (!MCCodeGenInfoCtorFn)
        return 0;
      return MCCodeGenInfoCtorFn(Triple, M);
    }

    /// createMCInstrInfo - Create a MCInstrInfo implementation.
    ///
    MCInstrInfo *createMCInstrInfo() const {
      if (!MCInstrInfoCtorFn)
        return 0;
      return MCInstrInfoCtorFn();
    }

    /// createMCRegInfo - Create a MCRegisterInfo implementation.
    ///
    MCRegisterInfo *createMCRegInfo(StringRef Triple) const {
      if (!MCRegInfoCtorFn)
        return 0;
      return MCRegInfoCtorFn(Triple);
    }

    /// createMCSubtargetInfo - Create a MCSubtargetInfo implementation.
    ///
    /// \arg Triple - This argument is used to determine the target machine
    /// feature set; it should always be provided. Generally this should be
    /// either the target triple from the module, or the target triple of the
    /// host if that does not exist.
    /// \arg CPU - This specifies the name of the target CPU.
    /// \arg Features - This specifies the string representation of the
    /// additional target features.
    MCSubtargetInfo *createMCSubtargetInfo(StringRef Triple, StringRef CPU,
                                           StringRef Features) const {
      if (!MCSubtargetInfoCtorFn)
        return 0;
      return MCSubtargetInfoCtorFn(Triple, CPU, Features);
    }

    /// createTargetMachine - Create a target specific machine implementation
    /// for the specified \arg Triple.
    ///
    /// \arg Triple - This argument is used to determine the target machine
    /// feature set; it should always be provided. Generally this should be
    /// either the target triple from the module, or the target triple of the
    /// host if that does not exist.
    TargetMachine *createTargetMachine(StringRef Triple, StringRef CPU,
                                       StringRef Features,
                                       Reloc::Model RM = Reloc::Default) const {
      if (!TargetMachineCtorFn)
        return 0;
      return TargetMachineCtorFn(*this, Triple, CPU, Features, RM);
    }

    /// createAsmBackend - Create a target specific assembly parser.
    ///
    /// \arg Triple - The target triple string.
    /// \arg Backend - The target independent assembler object.
    TargetAsmBackend *createAsmBackend(const std::string &Triple) const {
      if (!AsmBackendCtorFn)
        return 0;
      return AsmBackendCtorFn(*this, Triple);
    }

    /// createAsmLexer - Create a target specific assembly lexer.
    ///
    TargetAsmLexer *createAsmLexer(const MCAsmInfo &MAI) const {
      if (!AsmLexerCtorFn)
        return 0;
      return AsmLexerCtorFn(*this, MAI);
    }

    /// createAsmParser - Create a target specific assembly parser.
    ///
    /// \arg Parser - The target independent parser implementation to use for
    /// parsing and lexing.
    TargetAsmParser *createAsmParser(MCSubtargetInfo &STI,
                                     MCAsmParser &Parser) const {
      if (!AsmParserCtorFn)
        return 0;
      return AsmParserCtorFn(STI, Parser);
    }

    /// createAsmPrinter - Create a target specific assembly printer pass.  This
    /// takes ownership of the MCStreamer object.
    AsmPrinter *createAsmPrinter(TargetMachine &TM, MCStreamer &Streamer) const{
      if (!AsmPrinterCtorFn)
        return 0;
      return AsmPrinterCtorFn(TM, Streamer);
    }

    MCDisassembler *createMCDisassembler() const {
      if (!MCDisassemblerCtorFn)
        return 0;
      return MCDisassemblerCtorFn(*this);
    }

    MCInstPrinter *createMCInstPrinter(unsigned SyntaxVariant,
                                       const MCAsmInfo &MAI) const {
      if (!MCInstPrinterCtorFn)
        return 0;
      return MCInstPrinterCtorFn(*this, SyntaxVariant, MAI);
    }


    /// createCodeEmitter - Create a target specific code emitter.
    MCCodeEmitter *createCodeEmitter(const MCInstrInfo &II,
                                     const MCSubtargetInfo &STI,
                                     MCContext &Ctx) const {
      if (!CodeEmitterCtorFn)
        return 0;
      return CodeEmitterCtorFn(II, STI, Ctx);
    }

    /// createObjectStreamer - Create a target specific MCStreamer.
    ///
    /// \arg TT - The target triple.
    /// \arg Ctx - The target context.
    /// \arg TAB - The target assembler backend object. Takes ownership.
    /// \arg _OS - The stream object.
    /// \arg _Emitter - The target independent assembler object.Takes ownership.
    /// \arg RelaxAll - Relax all fixups?
    /// \arg NoExecStack - Mark file as not needing a executable stack.
    MCStreamer *createObjectStreamer(const std::string &TT, MCContext &Ctx,
                                     TargetAsmBackend &TAB,
                                     raw_ostream &_OS,
                                     MCCodeEmitter *_Emitter,
                                     bool RelaxAll,
                                     bool NoExecStack) const {
      if (!ObjectStreamerCtorFn)
        return 0;
      return ObjectStreamerCtorFn(*this, TT, Ctx, TAB, _OS, _Emitter, RelaxAll,
                                  NoExecStack);
    }

    /// createAsmStreamer - Create a target specific MCStreamer.
    MCStreamer *createAsmStreamer(MCContext &Ctx,
                                  formatted_raw_ostream &OS,
                                  bool isVerboseAsm,
                                  bool useLoc,
                                  bool useCFI,
                                  MCInstPrinter *InstPrint,
                                  MCCodeEmitter *CE,
                                  TargetAsmBackend *TAB,
                                  bool ShowInst) const {
      // AsmStreamerCtorFn is default to llvm::createAsmStreamer
      return AsmStreamerCtorFn(Ctx, OS, isVerboseAsm, useLoc, useCFI,
                               InstPrint, CE, TAB, ShowInst);
    }

    /// @}
  };

  /// TargetRegistry - Generic interface to target specific features.
  struct TargetRegistry {
    class iterator {
      const Target *Current;
      explicit iterator(Target *T) : Current(T) {}
      friend struct TargetRegistry;
    public:
      iterator(const iterator &I) : Current(I.Current) {}
      iterator() : Current(0) {}

      bool operator==(const iterator &x) const {
        return Current == x.Current;
      }
      bool operator!=(const iterator &x) const {
        return !operator==(x);
      }

      // Iterator traversal: forward iteration only
      iterator &operator++() {          // Preincrement
        assert(Current && "Cannot increment end iterator!");
        Current = Current->getNext();
        return *this;
      }
      iterator operator++(int) {        // Postincrement
        iterator tmp = *this;
        ++*this;
        return tmp;
      }

      const Target &operator*() const {
        assert(Current && "Cannot dereference end iterator!");
        return *Current;
      }

      const Target *operator->() const {
        return &operator*();
      }
    };

    /// @name Registry Access
    /// @{

    static iterator begin();

    static iterator end() { return iterator(); }

    /// lookupTarget - Lookup a target based on a target triple.
    ///
    /// \param Triple - The triple to use for finding a target.
    /// \param Error - On failure, an error string describing why no target was
    /// found.
    static const Target *lookupTarget(const std::string &Triple,
                                      std::string &Error);

    /// getClosestTargetForJIT - Pick the best target that is compatible with
    /// the current host.  If no close target can be found, this returns null
    /// and sets the Error string to a reason.
    ///
    /// Maintained for compatibility through 2.6.
    static const Target *getClosestTargetForJIT(std::string &Error);

    /// @}
    /// @name Target Registration
    /// @{

    /// RegisterTarget - Register the given target. Attempts to register a
    /// target which has already been registered will be ignored.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Name - The target name. This should be a static string.
    /// @param ShortDesc - A short target description. This should be a static
    /// string.
    /// @param TQualityFn - The triple match quality computation function for
    /// this target.
    /// @param HasJIT - Whether the target supports JIT code
    /// generation.
    static void RegisterTarget(Target &T,
                               const char *Name,
                               const char *ShortDesc,
                               Target::TripleMatchQualityFnTy TQualityFn,
                               bool HasJIT = false);

    /// RegisterMCAsmInfo - Register a MCAsmInfo implementation for the
    /// given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct a MCAsmInfo for the target.
    static void RegisterMCAsmInfo(Target &T, Target::MCAsmInfoCtorFnTy Fn) {
      // Ignore duplicate registration.
      if (!T.MCAsmInfoCtorFn)
        T.MCAsmInfoCtorFn = Fn;
    }

    /// RegisterMCCodeGenInfo - Register a MCCodeGenInfo implementation for the
    /// given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct a MCCodeGenInfo for the target.
    static void RegisterMCCodeGenInfo(Target &T,
                                     Target::MCCodeGenInfoCtorFnTy Fn) {
      // Ignore duplicate registration.
      if (!T.MCCodeGenInfoCtorFn)
        T.MCCodeGenInfoCtorFn = Fn;
    }

    /// RegisterMCInstrInfo - Register a MCInstrInfo implementation for the
    /// given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct a MCInstrInfo for the target.
    static void RegisterMCInstrInfo(Target &T, Target::MCInstrInfoCtorFnTy Fn) {
      // Ignore duplicate registration.
      if (!T.MCInstrInfoCtorFn)
        T.MCInstrInfoCtorFn = Fn;
    }

    /// RegisterMCRegInfo - Register a MCRegisterInfo implementation for the
    /// given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct a MCRegisterInfo for the target.
    static void RegisterMCRegInfo(Target &T, Target::MCRegInfoCtorFnTy Fn) {
      // Ignore duplicate registration.
      if (!T.MCRegInfoCtorFn)
        T.MCRegInfoCtorFn = Fn;
    }

    /// RegisterMCSubtargetInfo - Register a MCSubtargetInfo implementation for
    /// the given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct a MCSubtargetInfo for the target.
    static void RegisterMCSubtargetInfo(Target &T,
                                        Target::MCSubtargetInfoCtorFnTy Fn) {
      // Ignore duplicate registration.
      if (!T.MCSubtargetInfoCtorFn)
        T.MCSubtargetInfoCtorFn = Fn;
    }

    /// RegisterTargetMachine - Register a TargetMachine implementation for the
    /// given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct a TargetMachine for the target.
    static void RegisterTargetMachine(Target &T,
                                      Target::TargetMachineCtorTy Fn) {
      // Ignore duplicate registration.
      if (!T.TargetMachineCtorFn)
        T.TargetMachineCtorFn = Fn;
    }

    /// RegisterAsmBackend - Register a TargetAsmBackend implementation for the
    /// given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct an AsmBackend for the target.
    static void RegisterAsmBackend(Target &T, Target::AsmBackendCtorTy Fn) {
      if (!T.AsmBackendCtorFn)
        T.AsmBackendCtorFn = Fn;
    }

    /// RegisterAsmLexer - Register a TargetAsmLexer implementation for the
    /// given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct an AsmLexer for the target.
    static void RegisterAsmLexer(Target &T, Target::AsmLexerCtorTy Fn) {
      if (!T.AsmLexerCtorFn)
        T.AsmLexerCtorFn = Fn;
    }

    /// RegisterAsmParser - Register a TargetAsmParser implementation for the
    /// given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct an AsmParser for the target.
    static void RegisterAsmParser(Target &T, Target::AsmParserCtorTy Fn) {
      if (!T.AsmParserCtorFn)
        T.AsmParserCtorFn = Fn;
    }

    /// RegisterAsmPrinter - Register an AsmPrinter implementation for the given
    /// target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct an AsmPrinter for the target.
    static void RegisterAsmPrinter(Target &T, Target::AsmPrinterCtorTy Fn) {
      // Ignore duplicate registration.
      if (!T.AsmPrinterCtorFn)
        T.AsmPrinterCtorFn = Fn;
    }

    /// RegisterMCDisassembler - Register a MCDisassembler implementation for
    /// the given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct an MCDisassembler for the target.
    static void RegisterMCDisassembler(Target &T,
                                       Target::MCDisassemblerCtorTy Fn) {
      if (!T.MCDisassemblerCtorFn)
        T.MCDisassemblerCtorFn = Fn;
    }

    /// RegisterMCInstPrinter - Register a MCInstPrinter implementation for the
    /// given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct an MCInstPrinter for the target.
    static void RegisterMCInstPrinter(Target &T,
                                      Target::MCInstPrinterCtorTy Fn) {
      if (!T.MCInstPrinterCtorFn)
        T.MCInstPrinterCtorFn = Fn;
    }

    /// RegisterCodeEmitter - Register a MCCodeEmitter implementation for the
    /// given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct an MCCodeEmitter for the target.
    static void RegisterCodeEmitter(Target &T, Target::CodeEmitterCtorTy Fn) {
      if (!T.CodeEmitterCtorFn)
        T.CodeEmitterCtorFn = Fn;
    }

    /// RegisterObjectStreamer - Register a object code MCStreamer implementation
    /// for the given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct an MCStreamer for the target.
    static void RegisterObjectStreamer(Target &T, Target::ObjectStreamerCtorTy Fn) {
      if (!T.ObjectStreamerCtorFn)
        T.ObjectStreamerCtorFn = Fn;
    }

    /// RegisterAsmStreamer - Register an assembly MCStreamer implementation
    /// for the given target.
    ///
    /// Clients are responsible for ensuring that registration doesn't occur
    /// while another thread is attempting to access the registry. Typically
    /// this is done by initializing all targets at program startup.
    ///
    /// @param T - The target being registered.
    /// @param Fn - A function to construct an MCStreamer for the target.
    static void RegisterAsmStreamer(Target &T, Target::AsmStreamerCtorTy Fn) {
      if (T.AsmStreamerCtorFn == createAsmStreamer)
        T.AsmStreamerCtorFn = Fn;
    }

    /// @}
  };


  //===--------------------------------------------------------------------===//

  /// RegisterTarget - Helper template for registering a target, for use in the
  /// target's initialization function. Usage:
  ///
  ///
  /// Target TheFooTarget; // The global target instance.
  ///
  /// extern "C" void LLVMInitializeFooTargetInfo() {
  ///   RegisterTarget<Triple::foo> X(TheFooTarget, "foo", "Foo description");
  /// }
  template<Triple::ArchType TargetArchType = Triple::InvalidArch,
           bool HasJIT = false>
  struct RegisterTarget {
    RegisterTarget(Target &T, const char *Name, const char *Desc) {
      TargetRegistry::RegisterTarget(T, Name, Desc,
                                     &getTripleMatchQuality,
                                     HasJIT);
    }

    static unsigned getTripleMatchQuality(const std::string &TT) {
      if (Triple(TT).getArch() == TargetArchType)
        return 20;
      return 0;
    }
  };

  /// RegisterMCAsmInfo - Helper template for registering a target assembly info
  /// implementation.  This invokes the static "Create" method on the class to
  /// actually do the construction.  Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterMCAsmInfo<FooMCAsmInfo> X(TheFooTarget);
  /// }
  template<class MCAsmInfoImpl>
  struct RegisterMCAsmInfo {
    RegisterMCAsmInfo(Target &T) {
      TargetRegistry::RegisterMCAsmInfo(T, &Allocator);
    }
  private:
    static MCAsmInfo *Allocator(const Target &T, StringRef TT) {
      return new MCAsmInfoImpl(T, TT);
    }

  };

  /// RegisterMCAsmInfoFn - Helper template for registering a target assembly info
  /// implementation.  This invokes the specified function to do the
  /// construction.  Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterMCAsmInfoFn X(TheFooTarget, TheFunction);
  /// }
  struct RegisterMCAsmInfoFn {
    RegisterMCAsmInfoFn(Target &T, Target::MCAsmInfoCtorFnTy Fn) {
      TargetRegistry::RegisterMCAsmInfo(T, Fn);
    }
  };

  /// RegisterMCCodeGenInfo - Helper template for registering a target codegen info
  /// implementation.  This invokes the static "Create" method on the class
  /// to actually do the construction.  Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterMCCodeGenInfo<FooMCCodeGenInfo> X(TheFooTarget);
  /// }
  template<class MCCodeGenInfoImpl>
  struct RegisterMCCodeGenInfo {
    RegisterMCCodeGenInfo(Target &T) {
      TargetRegistry::RegisterMCCodeGenInfo(T, &Allocator);
    }
  private:
    static MCCodeGenInfo *Allocator(StringRef TT, Reloc::Model M) {
      return new MCCodeGenInfoImpl();
    }
  };

  /// RegisterMCCodeGenInfoFn - Helper template for registering a target codegen
  /// info implementation.  This invokes the specified function to do the
  /// construction.  Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterMCCodeGenInfoFn X(TheFooTarget, TheFunction);
  /// }
  struct RegisterMCCodeGenInfoFn {
    RegisterMCCodeGenInfoFn(Target &T, Target::MCCodeGenInfoCtorFnTy Fn) {
      TargetRegistry::RegisterMCCodeGenInfo(T, Fn);
    }
  };

  /// RegisterMCInstrInfo - Helper template for registering a target instruction
  /// info implementation.  This invokes the static "Create" method on the class
  /// to actually do the construction.  Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterMCInstrInfo<FooMCInstrInfo> X(TheFooTarget);
  /// }
  template<class MCInstrInfoImpl>
  struct RegisterMCInstrInfo {
    RegisterMCInstrInfo(Target &T) {
      TargetRegistry::RegisterMCInstrInfo(T, &Allocator);
    }
  private:
    static MCInstrInfo *Allocator() {
      return new MCInstrInfoImpl();
    }
  };

  /// RegisterMCInstrInfoFn - Helper template for registering a target
  /// instruction info implementation.  This invokes the specified function to
  /// do the construction.  Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterMCInstrInfoFn X(TheFooTarget, TheFunction);
  /// }
  struct RegisterMCInstrInfoFn {
    RegisterMCInstrInfoFn(Target &T, Target::MCInstrInfoCtorFnTy Fn) {
      TargetRegistry::RegisterMCInstrInfo(T, Fn);
    }
  };

  /// RegisterMCRegInfo - Helper template for registering a target register info
  /// implementation.  This invokes the static "Create" method on the class to
  /// actually do the construction.  Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterMCRegInfo<FooMCRegInfo> X(TheFooTarget);
  /// }
  template<class MCRegisterInfoImpl>
  struct RegisterMCRegInfo {
    RegisterMCRegInfo(Target &T) {
      TargetRegistry::RegisterMCRegInfo(T, &Allocator);
    }
  private:
    static MCRegisterInfo *Allocator(StringRef TT) {
      return new MCRegisterInfoImpl();
    }
  };

  /// RegisterMCRegInfoFn - Helper template for registering a target register
  /// info implementation.  This invokes the specified function to do the
  /// construction.  Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterMCRegInfoFn X(TheFooTarget, TheFunction);
  /// }
  struct RegisterMCRegInfoFn {
    RegisterMCRegInfoFn(Target &T, Target::MCRegInfoCtorFnTy Fn) {
      TargetRegistry::RegisterMCRegInfo(T, Fn);
    }
  };

  /// RegisterMCSubtargetInfo - Helper template for registering a target
  /// subtarget info implementation.  This invokes the static "Create" method
  /// on the class to actually do the construction.  Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterMCSubtargetInfo<FooMCSubtargetInfo> X(TheFooTarget);
  /// }
  template<class MCSubtargetInfoImpl>
  struct RegisterMCSubtargetInfo {
    RegisterMCSubtargetInfo(Target &T) {
      TargetRegistry::RegisterMCSubtargetInfo(T, &Allocator);
    }
  private:
    static MCSubtargetInfo *Allocator(StringRef TT, StringRef CPU,
                                      StringRef FS) {
      return new MCSubtargetInfoImpl();
    }
  };

  /// RegisterMCSubtargetInfoFn - Helper template for registering a target
  /// subtarget info implementation.  This invokes the specified function to
  /// do the construction.  Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterMCSubtargetInfoFn X(TheFooTarget, TheFunction);
  /// }
  struct RegisterMCSubtargetInfoFn {
    RegisterMCSubtargetInfoFn(Target &T, Target::MCSubtargetInfoCtorFnTy Fn) {
      TargetRegistry::RegisterMCSubtargetInfo(T, Fn);
    }
  };

  /// RegisterTargetMachine - Helper template for registering a target machine
  /// implementation, for use in the target machine initialization
  /// function. Usage:
  ///
  /// extern "C" void LLVMInitializeFooTarget() {
  ///   extern Target TheFooTarget;
  ///   RegisterTargetMachine<FooTargetMachine> X(TheFooTarget);
  /// }
  template<class TargetMachineImpl>
  struct RegisterTargetMachine {
    RegisterTargetMachine(Target &T) {
      TargetRegistry::RegisterTargetMachine(T, &Allocator);
    }

  private:
    static TargetMachine *Allocator(const Target &T, StringRef TT,
                                    StringRef CPU, StringRef FS,
                                    Reloc::Model RM) {
      return new TargetMachineImpl(T, TT, CPU, FS, RM);
    }
  };

  /// RegisterAsmBackend - Helper template for registering a target specific
  /// assembler backend. Usage:
  ///
  /// extern "C" void LLVMInitializeFooAsmBackend() {
  ///   extern Target TheFooTarget;
  ///   RegisterAsmBackend<FooAsmLexer> X(TheFooTarget);
  /// }
  template<class AsmBackendImpl>
  struct RegisterAsmBackend {
    RegisterAsmBackend(Target &T) {
      TargetRegistry::RegisterAsmBackend(T, &Allocator);
    }

  private:
    static TargetAsmBackend *Allocator(const Target &T,
                                       const std::string &Triple) {
      return new AsmBackendImpl(T, Triple);
    }
  };

  /// RegisterAsmLexer - Helper template for registering a target specific
  /// assembly lexer, for use in the target machine initialization
  /// function. Usage:
  ///
  /// extern "C" void LLVMInitializeFooAsmLexer() {
  ///   extern Target TheFooTarget;
  ///   RegisterAsmLexer<FooAsmLexer> X(TheFooTarget);
  /// }
  template<class AsmLexerImpl>
  struct RegisterAsmLexer {
    RegisterAsmLexer(Target &T) {
      TargetRegistry::RegisterAsmLexer(T, &Allocator);
    }

  private:
    static TargetAsmLexer *Allocator(const Target &T, const MCAsmInfo &MAI) {
      return new AsmLexerImpl(T, MAI);
    }
  };

  /// RegisterAsmParser - Helper template for registering a target specific
  /// assembly parser, for use in the target machine initialization
  /// function. Usage:
  ///
  /// extern "C" void LLVMInitializeFooAsmParser() {
  ///   extern Target TheFooTarget;
  ///   RegisterAsmParser<FooAsmParser> X(TheFooTarget);
  /// }
  template<class AsmParserImpl>
  struct RegisterAsmParser {
    RegisterAsmParser(Target &T) {
      TargetRegistry::RegisterAsmParser(T, &Allocator);
    }

  private:
    static TargetAsmParser *Allocator(MCSubtargetInfo &STI, MCAsmParser &P) {
      return new AsmParserImpl(STI, P);
    }
  };

  /// RegisterAsmPrinter - Helper template for registering a target specific
  /// assembly printer, for use in the target machine initialization
  /// function. Usage:
  ///
  /// extern "C" void LLVMInitializeFooAsmPrinter() {
  ///   extern Target TheFooTarget;
  ///   RegisterAsmPrinter<FooAsmPrinter> X(TheFooTarget);
  /// }
  template<class AsmPrinterImpl>
  struct RegisterAsmPrinter {
    RegisterAsmPrinter(Target &T) {
      TargetRegistry::RegisterAsmPrinter(T, &Allocator);
    }

  private:
    static AsmPrinter *Allocator(TargetMachine &TM, MCStreamer &Streamer) {
      return new AsmPrinterImpl(TM, Streamer);
    }
  };

  /// RegisterCodeEmitter - Helper template for registering a target specific
  /// machine code emitter, for use in the target initialization
  /// function. Usage:
  ///
  /// extern "C" void LLVMInitializeFooCodeEmitter() {
  ///   extern Target TheFooTarget;
  ///   RegisterCodeEmitter<FooCodeEmitter> X(TheFooTarget);
  /// }
  template<class CodeEmitterImpl>
  struct RegisterCodeEmitter {
    RegisterCodeEmitter(Target &T) {
      TargetRegistry::RegisterCodeEmitter(T, &Allocator);
    }

  private:
    static MCCodeEmitter *Allocator(const MCInstrInfo &II,
                                    const MCSubtargetInfo &STI,
                                    MCContext &Ctx) {
      return new CodeEmitterImpl();
    }
  };

}

#endif
