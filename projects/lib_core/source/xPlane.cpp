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

#define AVlib_xPlane_IMPLEMENTATION
#include "xPlane.h"

namespace AVlib {

//=============================================================================================================================================================================
// instantiation for base types
//=============================================================================================================================================================================

template class xPlane<uint8 >;
template class xPlane< int8 >;
template class xPlane<uint16>;
template class xPlane< int16>;
template class xPlane<uint32>;
template class xPlane< int32>;
template class xPlane<uint64>;
template class xPlane< int64>;
template class xPlane<float >;
template class xPlane<double>;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

template class xPlaneTank<uint8 >;
template class xPlaneTank< int8 >;
template class xPlaneTank<uint16>;
template class xPlaneTank< int16>;
template class xPlaneTank<uint32>;
template class xPlaneTank< int32>;
template class xPlaneTank<uint64>;
template class xPlaneTank< int64>;
template class xPlaneTank<float >;
template class xPlaneTank<double>;

//=============================================================================================================================================================================

} //end of namespace AVLib