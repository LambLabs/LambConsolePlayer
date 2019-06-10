#pragma once
//============================================================\\
//                         AVLib++                            \\
//============================================================\\
//           .----.                      .----.               \\
//          /      '.                  .'      \              \\
//       .------.    '.              .'    .-----.            \\
//      /        '-.   \            /   .-'        \          \\
//     |            '.  \          /  .'            |         \\
//      \             \  |        |  /             /          \\
//       '.            \ |   __   | /            .'           \\
//         '.           \|_-"__"-_|/           .'             \\
//           '.        ./ .\/ .\/ .\.        .'               \\
//             '-.    / \__/\__/\__/ \    .-'                 \\
//                '--|  / .\/ .\/ .\  |--'                    \\
//                   |  \__/\__/\__/  |                       \\
//                    \ / .\/ .\/ .\ /                        \\
//                   .-`\__/\__/\__/'-.                       \\
//                  /     `-....-'     \                      \\
//                 _\                  /_                     \\
//                __  __         _ Lider VIII                 \\
//               |  \/  |_  _ __| |_  __ _ ®                  \\
//               | |\/| | || / _| ' \/ _` |                   \\
//               |_|  |_|\_,_\__|_||_\__,_|                   \\
//                                                            \\
// "System rejrestacji i przetwarzania obrazu przestrzennego" \\
//   Project funded by The National Centre for Research and   \\
//           Development in the LIDER Programme               \\
//            (LIDER/34/0177/L-8/16/NCBR/2017)                \\
//============================================================\\

#include "xCommon.h"
#include <cstdio>

#ifdef _MSC_VER
#include <intrin.h>
#endif // _MSC_VER
#ifdef __GNUC__
#include <cpuid.h>
#endif // __GNUC__

namespace AVlib {

void x_sys_print_compiler();

//=============================================================================================================================================================================
// CPU
//=============================================================================================================================================================================
class xCpuInfo
{
protected:
  static const uint32 c_RegEAX = 0;
  static const uint32 c_RegEBX = 1;
  static const uint32 c_RegECX = 2;
  static const uint32 c_RegEDX = 3;

protected:
  bool  m_CPUID;
  char  m_VendorID[13];

  //Intel
  //x87
  bool m_FPU               : 1; //Floating Point Unit (integrated since 80486)
  //486                    
  bool m_CMPXCHG8          : 1;
  //P55C                   
  bool m_MMX               : 1;
  bool m_CMOV              : 1; //Conditional move
  bool m_PSE               : 1; //Page Size Extension (PSE)
  bool m_TSC               : 1; //Time Stamp Counter (TSC) 
  //P6                     
  bool m_PAE               : 1; //Physical Address Extension (PAE)
  //Klamath                
  bool m_SEP               : 1; //SYSENTER and SYSEXIT instructions
  //Deschutes              
  bool m_PSE36             : 1; //36-bit Page Size Extension
  //Katmai                 
  bool m_SSE1              : 1;
  bool m_FXRS              : 1; //FXSAVE, FXRESTOR instructions
  //Willamette             
  bool m_SSE2              : 1;
  bool m_CLFLUSH           : 1;
  //Northwood              
  bool m_HT                : 1; //Hyperthreading
  //Prescott               
  bool m_SSE3              : 1;
  bool m_CMPXCHG16B        : 1;
  //Merom                  
  bool m_SSSE3             : 1; //SupplementalSSE3
  //Penryn                 
  bool m_SSE4_1            : 1;
  //Nehalem                
  bool m_SSE4_2            : 1;
  bool m_POPCNT            : 1;
  //Westmere               
  bool m_AES               : 1; //EAS encryption accelerator 
  bool m_CLMUL             : 1; //Carry-less Multiplication
  //Sandy Bridge           
  bool m_AVX1              : 1;
  //Ivy Bridge             
  bool m_FP16C             : 1; //Half float convertion
  bool m_RDRAND            : 1; //SP 800-90A - Cryptographically secure pseudorandom number generator
  //Haswell                
  bool m_AVX2              : 1;
  bool m_LZCNT             : 1; //Leading zero count instruction
  bool m_MOVBE             : 1; //Load and Store of Big Endian forms
  bool m_ABM               : 1; //Advanced Bit Manipulation (LZCNT + POPCNT)
  bool m_BMI1              : 1; //Bit Manipulation Instructions 1
  bool m_BMI2              : 1; //Bit Manipulation Instructions 2
  bool m_FMA3              : 1; //Fused multiple-add
  bool m_RTM               : 1; //Restricted Transactional Memory
  bool m_HLE               : 1; //Hardware Lock Elision
  bool m_TSX               : 1; //TSX=RTM+HLE Transactional Synchronization Extensions
  bool m_INVPCID           : 1; //Invalidate processor context ID 
  //Broadwell
  bool m_ADX               : 1; //Multi-Precision Add-Carry Instruction Extensions (ADOX, ADCX, MULX)
  bool m_RDSEED            : 1; //SP 800-90B & C - Non-deterministic random bit generator
  bool m_PREFETCHW         : 1; 
  //Skylake
  bool m_MPX               : 1; //Memory Protection Extensions  
  bool m_SGX               : 1; //Software Guard Extensions
  bool m_SHA               : 1; //Intel SHA Extensions
  //Skylake-X                
  bool m_AVX512F           : 1; //Foundation  
  bool m_AVX512VL          : 1; //Vector Length Extensions
  bool m_AVX512BW          : 1; //Byte and Word Instructions
  bool m_AVX512DQ          : 1; //Doubleword and Quadword Instructions
  bool m_AVX512CDI         : 1; //Conflict Detection Instructions
  //Knights Landing
  bool m_AVX512ERI         : 1; //Exponential and Reciprocal Instructions
  bool m_AVX512PFI         : 1; //Prefetch Instructions    
  //Cannonlake
  bool m_UMIP              : 1; //User-Mode Instruction Prevention
  bool m_AVX512VMBI        : 1; //Vector Byte Manipulation Instructions
  bool m_AVX512IFMA        : 1; //Integer Fused Multiply Add (52-bit Integer Multiply-Add)
  //Knights Mill            
  bool m_AVX512_4VNNIW     : 1; //4-register Neural Network Instructions / Vector Neural Network Instructions Word variable precision
  bool m_AVX512_4FMAPS     : 1; //4-register Multiply Accumulation Single precision
  //Ice Lake
  bool m_CLWB              : 1; //Cache Line Write Back
  bool m_RDPID             : 1; //Read Processor ID
  bool m_AVX512_VNNI       : 1; //Vector Neural Network Instructions
  bool m_AVX512_VBMI2      : 1; //Vector Byte Manipulation Instructions 2
  bool m_AVX512_BITALG     : 1; //Support for VPOPCNT[B,W] and VPSHUF-BITQMB
  bool m_AVX512_VPOPCNTDQ  : 1; //Vector POPCNT
  bool m_AVX512_VPCLMULQDQ : 1; //Carry-Less Multiplication Quadword
  bool m_AVX512_VAES       : 1; //Vector AES
  bool m_AVX512_GFNI       : 1; //Galois Field New Instructions

  //AMD
  //Chompers
  bool m_3DNow             : 1;
  //Thunderbird          
  bool m_3DNowExt          : 1;
  //Barcelona            
  bool m_SSE4_A            : 1;
  //Bulldozer            
  bool m_XOP               : 1;
  bool m_FMA4              : 1;
  //Piledriver           
  bool m_TBM               : 1;

protected:
  static void xCPUID(uint32* RegistersTable, uint32 FunctionParameter);

public:
  xCpuInfo() { memset(this, 0, sizeof(xCpuInfo)); }
  void  checkCPIUD();
  void  printCpuInfo();
  int32 checkCompatibility();

public:
  bool  hasSSE () { if(!m_CPUID) { checkCPIUD(); } return (m_SSE1 && m_SSE2 && m_SSE3 && m_SSSE3 && m_SSE4_1 && m_SSE4_2); }
  bool  hasAVX () { return (hasSSE() && m_AVX1); }
  bool  hasAVX2() { return (hasAVX() && m_AVX2); }
  bool  hasFMA () { return (hasAVX() && m_FMA3); }
};

//=============================================================================================================================================================================

} //end of namespace AVLib

//template<size_t S> class Sizer { }; Sizer<sizeof(xCpuInfo)> xCpuInfoSize;



 
