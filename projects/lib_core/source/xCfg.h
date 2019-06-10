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

#include "xLicense.h"
#include "xCommon.h"
#include "xString.h"
#include <map>

namespace AVlib {

//=====================================================================================================================================================================================

class xCmdParamSimple
{
public:
  static int32        argi(int32 argc, char *argv[], char* header, int32 deft);
  static float        argf(int32 argc, char *argv[], char* header, float deft);
  static char*        args(int32 argc, char *argv[], char* header, char* deft);
  static int32        argt(int32 argc, char *argv[], char* header);
  static int32        argl(int32 argc, char *argv[], char* header, char* label);
};

#define _ARGI(h,d)    AVlib::xCmdParamSimple::argi(argc, argv, (h), (d)) 
#define _ARGF(h,d)    AVlib::xCmdParamSimple::argf(argc, argv, (h), (d)) 
#define _ARGS(h,d)    AVlib::xCmdParamSimple::args(argc, argv, (h), (d)) 
#define _ARGT(h)      AVlib::xCmdParamSimple::argt(argc, argv, (h))
#define _ARGL(h,d)    AVlib::xCmdParamSimple::argl(argc, argv, (h), (d))

//=====================================================================================================================================================================================

class xCfgCmdParam
{
protected:
  std::string         m_CmdName;
  std::string         m_SectionName;
  std::string         m_ParamName;
  bool                m_IsFlag;
  std::string         m_FlagValue;
  std::string         m_Comment;

public:
  xCfgCmdParam () {}; 
  xCfgCmdParam (std::string CmdName, std::string SectionName, std::string ParamName                       ) { m_CmdName = CmdName; m_SectionName = SectionName; m_ParamName = ParamName; m_IsFlag = false; m_FlagValue = ""       ; m_Comment = ""; }
  xCfgCmdParam (std::string CmdName, std::string SectionName, std::string ParamName, std::string FlagValue) { m_CmdName = CmdName; m_SectionName = SectionName; m_ParamName = ParamName; m_IsFlag = true;  m_FlagValue = FlagValue; m_Comment = ""; }
  
public:
  void                 setCmdName     (std::string CmdName    ) { m_CmdName = CmdName;        }
  std::string          getCmdName     (                       ) { return m_CmdName;           } 
  void                 setSectionName (std::string SectionName) { m_SectionName = SectionName;}
  std::string          getSectionName (                       ) { return m_SectionName;       } 
  void                 setParamName   (std::string ParamName  ) { m_ParamName = ParamName;    }
  std::string          getParamName   (                       ) { return m_ParamName;         } 
  void                 setIsFlag      (bool        IsFlag     ) { m_IsFlag = IsFlag;          }
  bool                 getIsFlag      (                       ) { return m_IsFlag;            } 
  void                 setFlagValue   (std::string FlagValue  ) { m_FlagValue = FlagValue;    }
  std::string          getFlagValue   (                       ) { return m_FlagValue;         }
};

//=====================================================================================================================================================================================

class xCfgParam
{
protected:
  std::string               m_Name;
  std::vector<std::string>  m_Args;
  std::string               m_Comment;

public:
  void                      setName(const std::string& Name) { m_Name = Name; }
  const std::string&        getName(                       ) { return m_Name; }

  void                      addArg(const std::string& Arg) { m_Args.push_back(Arg); }
  const std::string&        getArg(uint32 ArgIdx, const std::string& Default) { return m_Args.size()>ArgIdx ? m_Args[0] : Default; }

  const std::string&        get1stArg(const std::string& Default) { return getArg(0, Default); }
  double                    get1stArg(double             Default) { return xString::xStringToXXX<double>(get1stArg(std::string("")), Default); }
  float                     get1stArg(float              Default) { return xString::xStringToXXX<float> (get1stArg(std::string("")), Default); }
  int32                     get1stArg(int32              Default) { return xString::xStringToXXX<int32> (get1stArg(std::string("")), Default); }
  int64                     get1stArg(int64              Default) { return xString::xStringToXXX<int64> (get1stArg(std::string("")), Default); }
  bool                      get1stArg(bool               Default) { return xString::xStringToXXX<bool > (get1stArg(std::string("")), Default); }
                                                       
  std::vector<std::string>& getArgs(              ) { return m_Args; }
  std::vector<float      >  getArgs(float  Default) { return xString::xVecOfStringToVecOfXXX(m_Args, Default); }
  std::vector<double     >  getArgs(double Default) { return xString::xVecOfStringToVecOfXXX(m_Args, Default); }
  std::vector<int32      >  getArgs(int32  Default) { return xString::xVecOfStringToVecOfXXX(m_Args, Default); }
  std::vector<int64      >  getArgs(int64  Default) { return xString::xVecOfStringToVecOfXXX(m_Args, Default); }
  std::vector<bool       >  getArgs(bool   Default) { return xString::xVecOfStringToVecOfXXX(m_Args, Default); }

  void                      setComment(const std::string& Comment) { m_Comment = Comment; }
  const std::string&        getComment(                          ) { return m_Comment; }
};

//=====================================================================================================================================================================================

class xCfgSection
{
protected:
  std::string                         m_Name;
  std::map<std::string, xCfgParam>    m_Params;
  std::map<std::string, xCfgSection>  m_SubSections;
  std::string                         m_Comment;

public:
  void                                setName(const std::string& Name) { m_Name = Name; }
  const std::string&                  getName(                       ) { return m_Name; }

  void                                addParam    (xCfgParam Param) { m_Params[Param.getName()] = Param; }
  std::map<std::string, xCfgParam>&   getParams   () { return m_Params; }
  bool                                findParam   (const std::string& ParamName) { return (m_Params.find(ParamName) != m_Params.end()); }
  xCfgParam&                          getParam    (const std::string& ParamName) { return m_Params[ParamName]; }

  std::string                         getParamArg (const std::string& ParamName, const std::string& Default) { return findParam(ParamName) ? getParam(ParamName).get1stArg(Default) : Default; } //operates on current section
  double                              getParamArg (const std::string& ParamName, double             Default) { return findParam(ParamName) ? getParam(ParamName).get1stArg(Default) : Default; } //operates on current section
  float                               getParamArg (const std::string& ParamName, float              Default) { return findParam(ParamName) ? getParam(ParamName).get1stArg(Default) : Default; } //operates on current section
  int32                               getParamArg (const std::string& ParamName, int32              Default) { return findParam(ParamName) ? getParam(ParamName).get1stArg(Default) : Default; } //operates on current section
  int64                               getParamArg (const std::string& ParamName, int64              Default) { return findParam(ParamName) ? getParam(ParamName).get1stArg(Default) : Default; } //operates on current section
  bool                                getParamArg (const std::string& ParamName, bool               Default) { return findParam(ParamName) ? getParam(ParamName).get1stArg(Default) : Default; } //operates on current section

  std::vector<std::string>            getParamArgs(const std::string& ParamName)                  { return findParam(ParamName) ? getParam(ParamName).getArgs(       )   : std::vector<std::string>(); }
  std::vector<int32 >                 getParamArgs(const std::string& ParamName, int32   Default) { return findParam(ParamName) ? getParam(ParamName).getArgs(Default)   : std::vector<int32      >(); }
  std::vector<int64 >                 getParamArgs(const std::string& ParamName, int64   Default) { return findParam(ParamName) ? getParam(ParamName).getArgs(Default)   : std::vector<int64      >(); }
  std::vector<float >                 getParamArgs(const std::string& ParamName, float   Default) { return findParam(ParamName) ? getParam(ParamName).getArgs(Default)   : std::vector<float      >(); }
  std::vector<double>                 getParamArgs(const std::string& ParamName, double  Default) { return findParam(ParamName) ? getParam(ParamName).getArgs(Default)   : std::vector<double     >(); }
  
  void                                addSection (xCfgSection Section) { m_SubSections[Section.getName()] = Section; }
  std::map<std::string, xCfgSection>& getSections(                   ) { return m_SubSections; }
  bool                                findSection(const std::string& SectionName) { return (m_SubSections.find(SectionName) != m_SubSections.end()); }
  xCfgSection&                        getSection (const std::string& SectionName) { return m_SubSections[SectionName]; }

  void                                setComment(const std::string& Comment) { m_Comment = Comment; }
  const std::string&                  getComment(                          ) { return m_Comment; }

  void                                clear     () { m_Name.clear(); m_Params.clear(); m_SubSections.clear(); m_Comment.clear(); }
};

//=====================================================================================================================================================================================

class xCfgParser
{
protected:
  xCfgSection                         m_RootSection;
  std::stringstream                   m_Cfg;

  std::map<std::string, xCfgCmdParam> m_CmdParams;

protected:
  const  static char c_CmdToken       = '-';
  const  static char c_SectionNameBeg = '[';
  const  static char c_SectionNameEnd = ']';
  const  static char c_SectionBeg     = '{';
  const  static char c_SectionEnd     = '}';
  const  static char c_NamePrefix     = '$';
  const  static char c_QuoteMarkA     = '"';
  const  static char c_QuoteMarkB     = '\'';
  const  static char c_AssignmentA    = '=';
  const  static char c_AssignmentB    = ':';
  const  static char c_Separator      = ',';
  const  static char c_Comment        = '#';

  static inline bool xIsAlpha         (char c) { return ((c>='a' && c<='z') || (c>='A' && c<='Z')); }  
  static inline bool xIsNumeric       (char c) { return ( c>='0' && c<='9'); }
  static inline bool xIsAlphaNum      (char c) { return (xIsAlpha(c) || xIsNumeric(c)); }
  static inline bool xIsBlank         (char c) { return (c==' '  || c=='\a' || c=='\b' || c=='\t' || c=='\f'); }
  static inline bool xIsEndl          (char c) { return (c=='\n' || c=='\r' || c=='\v'); }

  static inline bool xIsNamePrefix    (char c) { return (c==c_NamePrefix); }

  static inline bool xIsSectionNameBeg(char c) { return (c==c_SectionNameBeg); }
  static inline bool xIsSectionNameEnd(char c) { return (c==c_SectionNameEnd); }
  static inline bool xIsSectionName   (char c) { return (xIsSectionNameBeg(c)  || xIsSectionNameEnd(c)); }

  static inline bool xIsSectionBeg    (char c) { return (c==c_SectionBeg); }
  static inline bool xIsSectionEnd    (char c) { return (c==c_SectionEnd); }
  static inline bool xIsSection       (char c) { return (xIsSectionBeg(c)  || xIsSectionEnd(c)); }

  static inline bool xIsQuoteMark     (char c) { return (c==c_QuoteMarkA   || c==c_QuoteMarkB ); }
  static inline bool xIsAssignment    (char c) { return (c==c_AssignmentA  || c==c_AssignmentB); }
  static inline bool xIsSeparator     (char c) { return (c==c_Separator); }

  static inline bool xIsComment       (char c) { return (c==c_Comment); }

  static inline bool xIsName          (char c) { return (!xIsBlank(c) && !xIsEndl(c) && !xIsAssignment(c) && !xIsSectionName(c) && !xIsSection(c) && !xIsSeparator(c) && !xIsComment(c)); }

public:
  static void   printCommandlineArgs(int argc, char* argv[]);

public:
  void          addCommandlineParam (xCfgCmdParam CmdParam) { m_CmdParams.insert(std::pair<std::string, xCfgCmdParam>(CmdParam.getCmdName(), CmdParam)); }
  bool          loadFromCommandline (int argc, char* argv[], std::string CfgFileToken);
  bool          loadFromFile        (const std::string&  FileName);
  void          load                (std::string&        Buffer  );
  void          storeToFile         (const std::string&  FileName);
  void          store               (std::string&        Buffer  );
  void          clear               (                            ) { m_RootSection.clear(); m_CmdParams.clear(); }

protected:
  int32         xLoadSection        (xCfgSection& ParrentSection);
  int32         xLoadParam          (xCfgSection& ParrentSection, bool OneArgOnly);
  void          xStoreSection       (xCfgSection& Section);
  void          xStoreParam         (xCfgParam&   Param);

  int32         xSkipBlank          ();
  int32         xSkipBlankAndEndl   ();
  int32         xSkipAnyTillEndl    ();
  int32         xSkipAnyAndEndl     ();
  int32         xReadSectionName    (std::string& SectionName);
  int32         xReadParamName      (std::string& ParamName);
  int32         xReadParamArg       (std::string& ParamArg);
  int32         xReadComment        (std::string& Comment);

public:
  xCfgSection&  getRootSection() { return m_RootSection; }
  bool          findParam  (const std::string& ParamName) { return m_RootSection.findParam(ParamName); }; //operates on root section
  xCfgParam     getParam   (const std::string& ParamName) { return m_RootSection.getParam(ParamName); } //operates on root section
  std::string   getParamArg(const std::string& ParamName, const std::string& Default) { return m_RootSection.getParamArg(ParamName, Default); } //operates on root section
  double        getParamArg(const std::string& ParamName, double             Default) { return m_RootSection.getParamArg(ParamName, Default); } //operates on root section
  int32         getParamArg(const std::string& ParamName, int32              Default) { return m_RootSection.getParamArg(ParamName, Default); } //operates on root section
  bool          getParamArg(const std::string& ParamName, bool               Default) { return m_RootSection.getParamArg(ParamName, Default); } //operates on root section
};

//=====================================================================================================================================================================================

class xFilenameParser
{
public:
  static int32V2 FilenameToResolution  (const std::string& Filename);
  static eCrF    FilenameToChromaFormat(const std::string& Filename);
  static int32   FilenameToBitDepth    (const std::string& Filename);
};

//=====================================================================================================================================================================================

} //end of namespace AVLib