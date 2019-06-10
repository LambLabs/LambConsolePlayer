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

#include "xBMP.h"

namespace AVlib {

//=============================================================================================================================================================================

int32 xBMP::xBitmapFileHeader::Absorb(xByteBuffer* Input)
{
  m_Type      = Input->extractU16_LE();
  m_FileSize  = Input->extractU32_LE();
  m_Reserved1 = Input->extractU16_LE();
  m_Reserved2 = Input->extractU16_LE();
  m_Offset    = Input->extractU32_LE();
  return c_HeaderLength;
}
int32 xBMP::xBitmapFileHeader::Emit(xByteBuffer* Output)
{
  Output->appendU16_LE(m_Type     );
  Output->appendU32_LE(m_FileSize );
  Output->appendU16_LE(m_Reserved1);
  Output->appendU16_LE(m_Reserved2);
  Output->appendU32_LE(m_Offset   );
  return c_HeaderLength;
}

//=============================================================================================================================================================================

int32 xBMP::xBitmapInfoHeader::Absorb(xByteBuffer* Input)
{
  uint32 HeaderSize = Input->peekU32_LE();
  if(HeaderSize != c_HeaderLength) { return NOT_VALID; }

  m_HeaderSize      = Input->extractU32_LE();
  m_Width           = Input->extractU32_LE();
  m_Height          = Input->extractU32_LE();
  m_Planes          = Input->extractU16_LE();
  m_BitsPerPixel    = Input->extractU16_LE();
  m_Compression     = Input->extractU32_LE();
  m_ImageSize       = Input->extractU32_LE();
  m_PelsPerMeterX   = Input->extractU32_LE();
  m_PelsPerMeterY   = Input->extractU32_LE();
  m_ColorsUsed      = Input->extractU32_LE();
  m_ColorsImportant = Input->extractU32_LE();
  return c_HeaderLength;
}
int32 xBMP::xBitmapInfoHeader::Emit(xByteBuffer* Output)
{
  Output->appendU32_LE(m_HeaderSize     );
  Output->appendU32_LE(m_Width          );
  Output->appendU32_LE(m_Height         );
  Output->appendU16_LE(m_Planes         );
  Output->appendU16_LE(m_BitsPerPixel   );
  Output->appendU32_LE(m_Compression    );
  Output->appendU32_LE(m_ImageSize      );
  Output->appendU32_LE(m_PelsPerMeterX  );
  Output->appendU32_LE(m_PelsPerMeterY  );
  Output->appendU32_LE(m_ColorsUsed     );
  Output->appendU32_LE(m_ColorsImportant);
  return c_HeaderLength;
}

//=====================================================================================================================================================================================

} //end of namespace AVLib
