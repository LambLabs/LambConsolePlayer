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

#include "xSys.h"

namespace AVlib {

//=============================================================================================================================================================================
// Compiler
//=============================================================================================================================================================================
void x_sys_print_compiler()
{
  printf("Compiler: %s  version: %d\n", X_COMPILER_NAME, X_COMPILER_VER);
}

//=============================================================================================================================================================================
// CPU
//=============================================================================================================================================================================
#if defined(X_SYSTEM_WINDOWS)
void xCpuInfo::xCPUID(uint32* RegistersTable, uint32 FunctionParameter)
{
  __cpuid((int*)RegistersTable, FunctionParameter);
}
#elif defined(X_SYSTEM_LINUX)
void xCpuInfo::xCPUID(uint32* RegistersTable, uint32 FunctionParameter)
{ 
  __get_cpuid_count (FunctionParameter, 0, RegistersTable + c_RegEAX, RegistersTable + c_RegEBX, RegistersTable + c_RegECX, RegistersTable + c_RegEDX);
}
#endif

void xCpuInfo::checkCPIUD()
{
  // http://www.sandpile.org/x86/cpuid.htm
  uint32 CPUInfo[4]; //[0] =EAX, [1]=EBX, [2]=ECX, [3]=EDX  
  memset(this, 0, sizeof(xCpuInfo));
  m_CPUID = true;

  //StandardLevel = 0
  xCPUID(CPUInfo, 0);
  uint32 HighestFunctionSupported = CPUInfo[0];
  memcpy(m_VendorID, &CPUInfo[1], 3*sizeof(int32));

  //StandardLevel = 1
  if(HighestFunctionSupported>=1)
  {
    xCPUID(CPUInfo, 1);
    //EDX
    m_FPU               = (CPUInfo[c_RegEDX] & (1<< 0)) != 0;
    //vme              
    //de               
    m_PSE               = (CPUInfo[c_RegEDX] & (1<< 3)) != 0;
    m_TSC               = (CPUInfo[c_RegEDX] & (1<< 4)) != 0;
    //msr              
    m_PAE               = (CPUInfo[c_RegEDX] & (1<< 6)) != 0;
    //mce              
    m_CMPXCHG8          = (CPUInfo[c_RegEDX] & (1<< 8)) != 0;
    //apic             
    //NN               
    m_SEP               = (CPUInfo[c_RegEDX] & (1<<11)) != 0;
    //mtrr             
    //pge              
    //mca              
    m_CMOV              = (CPUInfo[c_RegEDX] & (1<<15)) != 0;
    //pat              
    m_PSE36             = (CPUInfo[c_RegEDX] & (1<<17)) != 0;
    //psn              
    m_CLFLUSH           = (CPUInfo[c_RegEDX] & (1<<19)) != 0;
    //NN               
    //ds               
    //acpi             
    m_MMX               = (CPUInfo[c_RegEDX] & (1<<23)) != 0;
    m_FXRS              = (CPUInfo[c_RegEDX] & (1<<24)) != 0;
    m_SSE1              = (CPUInfo[c_RegEDX] & (1<<25)) != 0;
    m_SSE2              = (CPUInfo[c_RegEDX] & (1<<26)) != 0;
    //ss               
    m_HT                = (CPUInfo[c_RegEDX] & (1<<28)) != 0;
    //tm               
    //ia64             
    //pbe              
                       
    //ECX              
    m_SSE3              = (CPUInfo[c_RegECX] & (1<< 0)) != 0;
    m_CLMUL             = (CPUInfo[c_RegECX] & (1<< 1)) != 0;
    //dtes64           
    //monitor          
    //cr8_legacy       
    m_LZCNT             = (CPUInfo[c_RegECX] & (1<< 5)) != 0;
    m_SSSE3             = (CPUInfo[c_RegECX] & (1<< 9)) != 0;
    m_FMA3              = (CPUInfo[c_RegECX] & (1<<12)) != 0;
    m_CMPXCHG16B        = (CPUInfo[c_RegECX] & (1<<13)) != 0;
    m_SSE4_1            = (CPUInfo[c_RegECX] & (1<<19)) != 0;
    m_SSE4_2            = (CPUInfo[c_RegECX] & (1<<20)) != 0;
    m_MOVBE             = (CPUInfo[c_RegECX] & (1<<22)) != 0;
    m_POPCNT            = (CPUInfo[c_RegECX] & (1<<23)) != 0;
    m_AES               = (CPUInfo[c_RegECX] & (1<<25)) != 0;
    m_AVX1              = (CPUInfo[c_RegECX] & (1<<28)) != 0;
    m_FP16C             = (CPUInfo[c_RegECX] & (1<<29)) != 0;
    m_RDRAND            = (CPUInfo[c_RegECX] & (1<<30)) != 0;
  }

  //StandardLevel = 7
  if(HighestFunctionSupported>=7)
  {
    xCPUID(CPUInfo, 7);    
    //EBX
    //fsgsbase
    m_SGX               = (CPUInfo[c_RegEBX] & (1<< 2)) != 0;
    m_BMI1              = (CPUInfo[c_RegEBX] & (1<< 3)) != 0;
    m_HLE               = (CPUInfo[c_RegEBX] & (1<< 4)) != 0;
    m_AVX2              = (CPUInfo[c_RegEBX] & (1<< 5)) != 0;
    //NN                
    //smep             
    m_BMI2              = (CPUInfo[c_RegEBX] & (1<< 8)) != 0;
    //erms             
    m_INVPCID           = (CPUInfo[c_RegEBX] & (1<<10)) != 0;
    m_RTM               = (CPUInfo[c_RegEBX] & (1<<11)) != 0;
    //pqm              
    //NN                
    m_MPX               = (CPUInfo[c_RegEBX] & (1<<14)) != 0;
    //pqe              
    m_AVX512F           = (CPUInfo[c_RegEBX] & (1<<16)) != 0;
    m_AVX512DQ          = (CPUInfo[c_RegEBX] & (1<<17)) != 0;
    m_RDSEED            = (CPUInfo[c_RegEBX] & (1<<18)) != 0;
    m_ADX               = (CPUInfo[c_RegEBX] & (1<<19)) != 0;
    //smap             
    m_AVX512IFMA        = (CPUInfo[c_RegEBX] & (1<<21)) != 0;
    //pcommit
    //clflushopt
    m_CLWB              = (CPUInfo[c_RegEBX] & (1<<24)) != 0;
    //intel_pt         
    m_AVX512PFI         = (CPUInfo[c_RegEBX] & (1<<26)) != 0;
    m_AVX512ERI         = (CPUInfo[c_RegEBX] & (1<<27)) != 0;
    m_AVX512CDI         = (CPUInfo[c_RegEBX] & (1<<28)) != 0;
    m_SHA               = (CPUInfo[c_RegEBX] & (1<<29)) != 0;
    m_AVX512BW          = (CPUInfo[c_RegEBX] & (1<<30)) != 0;
    m_AVX512VL          = (CPUInfo[c_RegEBX] & (1<<31)) != 0;    
                       
    //ECX              
    m_PREFETCHW         = (CPUInfo[c_RegECX] & (1<< 0)) != 0;
    m_AVX512VMBI        = (CPUInfo[c_RegECX] & (1<< 1)) != 0;
    m_UMIP              = (CPUInfo[c_RegECX] & (1<< 2)) != 0;
    //pku              
    //ospke            
    //NN               
    m_AVX512_VBMI2      = (CPUInfo[c_RegECX] & (1<< 6)) != 0;
    //NN               
    m_AVX512_GFNI       = (CPUInfo[c_RegECX] & (1<< 8)) != 0;
    m_AVX512_VAES       = (CPUInfo[c_RegECX] & (1<< 9)) != 0;
    m_AVX512_VPCLMULQDQ = (CPUInfo[c_RegECX] & (1<<10)) != 0;
    m_AVX512_VNNI       = (CPUInfo[c_RegECX] & (1<<11)) != 0;
    m_AVX512_BITALG     = (CPUInfo[c_RegECX] & (1<<12)) != 0;
    //NN
    m_AVX512_VPOPCNTDQ  = (CPUInfo[c_RegECX] & (1<<14)) != 0;
    //NN
    //NN
    //mawau
    //mawau
    //mawau
    //mawau
    //mawau
    m_RDPID             = (CPUInfo[c_RegECX] & (1<<22)) != 0;
    //NN
    //NN
    //NN
    //NN
    //NN
    //NN
    //NN
    //sgx_ic
    //NN

    //EDX
    //NN
    //NN
    m_AVX512_4VNNIW     = (CPUInfo[c_RegEDX] & (1<< 2)) != 0;
    m_AVX512_4FMAPS     = (CPUInfo[c_RegEDX] & (1<< 3)) != 0;
  }

  //derrived
  m_LZCNT       = m_BMI1;
  m_ABM         = m_LZCNT & m_POPCNT;
  m_TSX         = m_RTM & m_HLE;

  
  //ExtendedStandardLevel = 0x80000000
  xCPUID(CPUInfo, 0x80000000);
  unsigned int HighestExtendedFunctionSupported = CPUInfo[0];

  //ExtendedStandardLevel = 0x80000001
  if(HighestExtendedFunctionSupported>=0x80000001)
  {
    xCPUID(CPUInfo, 0x80000001);
    m_SSE4_A      = (CPUInfo[c_RegECX] & (1<< 6)) != 0;
    m_XOP         = (CPUInfo[c_RegECX] & (1<<11)) != 0;
    m_FMA4        = (CPUInfo[c_RegECX] & (1<<16)) != 0;
    m_TBM         = (CPUInfo[c_RegECX] & (1<<21)) != 0;
    m_3DNow       = (CPUInfo[c_RegEDX] & (1<<31)) != 0;
    m_3DNowExt    = (CPUInfo[c_RegEDX] & (1<<30)) != 0;
  }  
}

void xCpuInfo::printCpuInfo()
{
  if(!m_CPUID) { checkCPIUD(); }

  //available
  printf("# Detected CPU feature list:\n");
  printf("+ Available extensions:\n");
  if(m_FPU              ) printf("FPU ");
  if(m_CMPXCHG8         ) printf("CMPXCHG8 ");
  if(m_MMX              ) printf("MMX ");
  if(m_CMOV             ) printf("CMOV ");
  if(m_PSE              ) printf("PSE ");
  if(m_TSC              ) printf("TSC ");
  if(m_PAE              ) printf("PAE ");
  if(m_SEP              ) printf("SEP ");
  if(m_PSE36            ) printf("PSE36 ");
  if(m_SSE1             ) printf("SSE1 ");
  if(m_FXRS             ) printf("FXRS ");
  if(m_SSE2             ) printf("SSE2 ");
  if(m_CLFLUSH          ) printf("CLFLUSH ");
  if(m_HT               ) printf("HT ");
  if(m_SSE3             ) printf("SSE3 ");
  if(m_CMPXCHG16B       ) printf("CMPXCHG16B ");
  if(m_SSSE3            ) printf("SSSE3 ");
  if(m_SSE4_1           ) printf("SSE4.1 ");
  if(m_SSE4_2           ) printf("SSE4.2 ");
  if(m_POPCNT           ) printf("POPCNT ");
  if(m_AES              ) printf("AES ");
  if(m_CLMUL            ) printf("CLMUL "); 
  if(m_AVX1             ) printf("AVX1 ");
  if(m_FP16C            ) printf("FP16C ");  
  if(m_RDRAND           ) printf("RDRAND ");
  if(m_AVX2             ) printf("AVX2 ");
  if(m_LZCNT            ) printf("LZCNT ");
  if(m_MOVBE            ) printf("MOVBE "); 
  if(m_ABM              ) printf("ABM "); 
  if(m_BMI1             ) printf("BMI1 ");
  if(m_BMI2             ) printf("BMI2 ");
  if(m_FMA3             ) printf("FMA3 ");
  if(m_RTM              ) printf("RTM ");
  if(m_HLE              ) printf("HLE ");
  if(m_TSX              ) printf("TSX ");
  if(m_INVPCID          ) printf("INVPCID "); 
  if(m_ADX              ) printf("ADX "); 
  if(m_RDSEED           ) printf("RDSEED "); 
  if(m_PREFETCHW        ) printf("PREFETCHW ");  
  if(m_MPX              ) printf("MPX ");
  if(m_SGX              ) printf("SGX ");
  if(m_SHA              ) printf("SHA ");
  if(m_AVX512F          ) printf("AVX512F ");  
  if(m_AVX512VL         ) printf("AVX512VL ");
  if(m_AVX512BW         ) printf("AVX512BW ");
  if(m_AVX512DQ         ) printf("AVX512DQ ");
  if(m_AVX512CDI        ) printf("AVX512CD ");
  if(m_AVX512ERI        ) printf("AVX512ER ");
  if(m_AVX512PFI        ) printf("AVX512PF ");  
  if(m_AVX512VMBI       ) printf("AVX512VMBI ");  
  if(m_AVX512IFMA       ) printf("AVX512IFMA ");  
  if(m_AVX512_4VNNIW    ) printf("AVX512_4VNNIW ");  
  if(m_AVX512_4FMAPS    ) printf("AVX512_4FMAPS ");  
  if(m_CLWB             ) printf("CLWB ");  
  if(m_RDPID            ) printf("RDPID ");  
  if(m_AVX512_VNNI      ) printf("AVX512_VNNI ");  
  if(m_AVX512_VBMI2     ) printf("AVX512_VBMI2 ");  
  if(m_AVX512_BITALG    ) printf("AVX512_BITALG ");  
  if(m_AVX512_VPOPCNTDQ ) printf("AVX512_VPOPCNTDQ ");  
  if(m_AVX512_VPCLMULQDQ) printf("AVX512_VPCLMULQDQ ");  
  if(m_AVX512_VAES      ) printf("AVX512_VAES ");  
  if(m_AVX512_GFNI      ) printf("AVX512_GFNI ");  
  if(m_3DNow            ) printf("3DNow ");
  if(m_3DNowExt         ) printf("3DNowExt ");
  if(m_SSE4_A           ) printf("SSE4A ");
  if(m_XOP              ) printf("XOP ");
  if(m_FMA4             ) printf("FMA4 ");
  if(m_TBM              ) printf("TBM ");
  printf("\n");

  //not available
  printf("- Not available extensions:\n");
  if(!m_FPU              ) printf("FPU ");
  if(!m_CMPXCHG8         ) printf("CMPXCHG8 ");
  if(!m_MMX              ) printf("MMX ");
  if(!m_CMOV             ) printf("CMOV ");
  if(!m_PSE              ) printf("PSE ");
  if(!m_TSC              ) printf("TSC ");
  if(!m_PAE              ) printf("PAE ");
  if(!m_SEP              ) printf("SEP ");
  if(!m_PSE36            ) printf("PSE36 ");
  if(!m_SSE1             ) printf("SSE1 ");
  if(!m_FXRS             ) printf("FXRS ");
  if(!m_SSE2             ) printf("SSE2 ");
  if(!m_CLFLUSH          ) printf("CLFLUSH ");
  if(!m_HT               ) printf("HT ");
  if(!m_SSE3             ) printf("SSE3 ");
  if(!m_CMPXCHG16B       ) printf("CMPXCHG16B ");
  if(!m_SSSE3            ) printf("SSSE3 ");
  if(!m_SSE4_1           ) printf("SSE4.1 ");
  if(!m_SSE4_2           ) printf("SSE4.2 ");
  if(!m_POPCNT           ) printf("POPCNT ");
  if(!m_AES              ) printf("AES ");
  if(!m_CLMUL            ) printf("CLMUL "); 
  if(!m_AVX1             ) printf("AVX1 ");
  if(!m_FP16C            ) printf("FP16C ");  
  if(!m_RDRAND           ) printf("RDRAND ");
  if(!m_AVX2             ) printf("AVX2 ");
  if(!m_LZCNT            ) printf("LZCNT ");
  if(!m_MOVBE            ) printf("MOVBE "); 
  if(!m_ABM              ) printf("ABM "); 
  if(!m_BMI1             ) printf("BMI1 ");
  if(!m_BMI2             ) printf("BMI2 ");
  if(!m_FMA3             ) printf("FMA3 ");
  if(!m_RTM              ) printf("RTM ");
  if(!m_HLE              ) printf("HLE ");
  if(!m_TSX              ) printf("TSX ");
  if(!m_INVPCID          ) printf("INVPCID "); 
  if(!m_ADX              ) printf("ADX "); 
  if(!m_RDSEED           ) printf("RDSEED "); 
  if(!m_PREFETCHW        ) printf("PREFETCHW ");  
  if(!m_MPX              ) printf("MPX ");
  if(!m_SGX              ) printf("SGX ");
  if(!m_SHA              ) printf("SHA ");
  if(!m_AVX512F          ) printf("AVX512F ");  
  if(!m_AVX512VL         ) printf("AVX512VL ");
  if(!m_AVX512BW         ) printf("AVX512BW ");
  if(!m_AVX512DQ         ) printf("AVX512DQ ");
  if(!m_AVX512CDI        ) printf("AVX512CD ");
  if(!m_AVX512ERI        ) printf("AVX512ER ");
  if(!m_AVX512PFI        ) printf("AVX512PF ");  
  if(!m_AVX512VMBI       ) printf("AVX512VMBI ");  
  if(!m_AVX512IFMA       ) printf("AVX512IFMA ");  
  if(!m_AVX512_4VNNIW    ) printf("AVX512_4VNNIW ");  
  if(!m_AVX512_4FMAPS    ) printf("AVX512_4FMAPS ");  
  if(!m_CLWB             ) printf("CLWB ");  
  if(!m_RDPID            ) printf("RDPID ");  
  if(!m_AVX512_VNNI      ) printf("AVX512_VNNI ");  
  if(!m_AVX512_VBMI2     ) printf("AVX512_VBMI2 ");  
  if(!m_AVX512_BITALG    ) printf("AVX512_BITALG ");  
  if(!m_AVX512_VPOPCNTDQ ) printf("AVX512_VPOPCNTDQ ");  
  if(!m_AVX512_VPCLMULQDQ) printf("AVX512_VPCLMULQDQ ");  
  if(!m_AVX512_VAES      ) printf("AVX512_VAES ");  
  if(!m_AVX512_GFNI      ) printf("AVX512_GFNI ");  
  if(!m_3DNow            ) printf("3DNow ");
  if(!m_3DNowExt         ) printf("3DNowExt ");
  if(!m_SSE4_A           ) printf("SSE4A ");
  if(!m_XOP              ) printf("XOP ");
  if(!m_FMA4             ) printf("FMA4 ");
  if(!m_TBM              ) printf("TBM ");
  printf("\n");
}

int xCpuInfo::checkCompatibility()
{
  if(!m_CPUID) { checkCPIUD(); }

  int32 compatible = 0;
#if X_SSE1
  if(!m_SSE1) { compatible -= 1; printf("x_sys_cpu: your CPU is incompatible with SSE instruction set\n" ); }
#endif
#if X_SSE2
  if(!m_SSE2) { compatible -= 1; printf("x_sys_cpu: your CPU is incompatible with SSE2 instruction set\n" ); }
#endif
#if X_SSE3
  if(!m_SSE3) { compatible -= 1; printf("x_sys_cpu: your CPU is incompatible with SSE3 instruction set\n" ); }
#endif
#if X_SSSE3
  if(!m_SSSE3) { compatible -= 1; printf("x_sys_cpu: your CPU is incompatible with SSSE3 instruction set\n" ); }
#endif
#if X_SSE4_1
  if(!m_SSE4_1) { compatible -= 1; printf("x_sys_cpu: your CPU is incompatible with SSE4.1 instruction set\n" ); }
#endif
#if X_SSE4_2
  if(!m_SSE4_2) { compatible -= 1; printf("x_sys_cpu: your CPU is incompatible with SSE4.2 instruction set\n" ); }
#endif
#if X_AVX1
  if(!m_AVX1) { compatible -= 1; printf("x_sys_cpu: your CPU is incompatible with AVX instruction set\n" ); }
#endif
#if X_AVX2
  if(!m_AVX2) { compatible -= 1; printf("x_sys_cpu: your CPU is incompatible with AVX2 instruction set\n" ); }
#endif

  return compatible;
}

//=============================================================================================================================================================================

} //end of namespace AVLib
