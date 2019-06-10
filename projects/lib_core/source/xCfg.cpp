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

#include "xCfg.h"
#include <locale>
#include <codecvt>

namespace AVlib {

using std::string;
using std::vector;
using std::map;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::istringstream;
using std::ostringstream;

//=====================================================================================================================================================================================
// Simple commandline
//================================================================================================
int32 xCmdParamSimple::argi(int32 argc, char *argv[], char* header, int32 deft ) { for(int32 i=1; i < argc; i++) { if(!strcmp(argv[i], header)) { if(i < (argc - 1)) { return(       atoi(argv[i + 1])); } } }  return(deft); }
float xCmdParamSimple::argf(int32 argc, char *argv[], char* header, float deft ) { for(int32 i=1; i < argc; i++) { if(!strcmp(argv[i], header)) { if(i < (argc - 1)) { return((float)atof(argv[i + 1])); } } }  return(deft); }
char* xCmdParamSimple::args(int32 argc, char *argv[], char* header, char* deft ) { for(int32 i=1; i < argc; i++) { if(!strcmp(argv[i], header)) { if(i < (argc - 1)) { return(argv[i + 1]); } } } return(deft); }
int32 xCmdParamSimple::argt(int32 argc, char *argv[], char* header             ) { for(int32 i=1; i < argc; i++) { if(!strcmp(argv[i], header)) { return(1); } }  return(0); }
int32 xCmdParamSimple::argl(int32 argc, char *argv[], char* header, char* label) { for(int32 i=1; i < argc; i++) { if(!strcmp(argv[i], header)) { if(i < (argc - 1)) { if(!strcmp(argv[i + 1], label)) { return(1); } } } } return(0); }

//=====================================================================================================================================================================================

void xCfgParser::printCommandlineArgs(int argc, char* argv[])
{
  printf("argc  = %d\n", argc);
  for(int32 i=0; i<argc; i++) { printf("argv[%d] = %s\n", i, argv[i]); }
  printf("\n");
}
bool xCfgParser::loadFromCommandline(int argc, char* argv[], string CfgFileToken)
{
  if(argc == 1) { printf("cfg: no commandline arguments!\n"); return false; }

  if(!CfgFileToken.empty())
  {
    for(int i=1; i < argc; i++)
    {
      string OneArg = argv[i];
      string CfgFileName;
      if(OneArg.find(CfgFileToken) != string::npos)
      {
        m_Cfg.str("");
        m_Cfg.clear();
        m_Cfg << OneArg;
        m_Cfg.ignore(1024, '=');
        xReadParamArg(CfgFileName);
        bool LoadedCorectly = loadFromFile(CfgFileName);
        if(!LoadedCorectly) { return false; }
      }
    }
  }

  for(int i=1; i<argc; i++)
  {
    string OneArg = argv[i];
    if(xIsAlpha(OneArg[0]) && OneArg.find(CfgFileToken) == string::npos)
    {
      m_Cfg.str("");   
      m_Cfg.clear();
      m_Cfg << OneArg;
      xLoadParam(m_RootSection, true);
    }
    if(!m_CmdParams.empty() && (OneArg[0] == c_CmdToken && OneArg.find(CfgFileToken) == string::npos))
    {
      string Short = argv[i];
      if(m_CmdParams.find(Short) != m_CmdParams.end()) //is in short list
      {
        xCfgCmdParam& CmdParam = m_CmdParams[Short];
        xCfgParam CfgParam;
        CfgParam.setName(CmdParam.getParamName());
        if(CmdParam.getIsFlag()) { CfgParam.addArg(CmdParam.getFlagValue()); } //flag only
        else if(++i < argc) { CfgParam.addArg(argv[i]); }
        CfgParam.setComment(c_Comment + " generated based on short commandline param");
        if(CmdParam.getSectionName().empty())
        {
          m_RootSection.addParam(CfgParam);
        }
        else
        {
          if(m_RootSection.findSection(CmdParam.getSectionName())) //section alredy exist
          {
            m_RootSection.getSection(CmdParam.getSectionName()).addParam(CfgParam);
          }
          else
          {
            xCfgSection Section;
            Section.setName(CmdParam.getSectionName());
            Section.setComment(c_Comment + " generated based on short commandline param");
            Section.addParam(CfgParam);
            m_RootSection.addSection(Section);
          }
        }
      }
      else
      {
        if(i + 1 < argc)
        {
          if(argv[i + 1][0] != c_CmdToken) { i++; }
        }
      }
    }
  }  
  return true;
}
bool xCfgParser::loadFromFile(const string& FileName)
{
  ifstream File (FileName.c_str(), ifstream::in);
  if(File.good() && File.is_open())
  {
    string Buffer ((std::istreambuf_iterator<char>(File)), std::istreambuf_iterator<char>());
    File.close();
    load(Buffer);  
    return true;
  }
  return false;
}
void xCfgParser::load(string& Buffer)
{
  m_Cfg.str("");
  m_Cfg.clear();
  m_Cfg << Buffer;
  int32 RetVal = 0;

  m_RootSection.setName("Root");
  //fprintf(stdout, "V xCfg: Loading cfg\n");
  while (!m_Cfg.eof())
  {
    char Token = m_Cfg.peek();
    if(xIsBlank(Token) || xIsEndl(Token) || Token==EOF) //whitespace
    {
      m_Cfg.ignore(1); //skip one char
    }
    else if(xIsComment(Token)) //comment
    {
      string Comment;
      xReadComment(Comment);
    }
    else if(xIsSectionNameBeg(Token))
    {
      RetVal = xLoadSection(m_RootSection);
    }
    else if(xIsNamePrefix(Token)) //parameter with $ prefix
    {
      m_Cfg.ignore(1);
      RetVal = xLoadParam(m_RootSection, false);
    }
    else if(xIsAlpha(Token)) //param  
    {
      RetVal = xLoadParam(m_RootSection, false);
    }
    else
    {
      printf("cfg: wrong start symbol!\n");
      break;
    } 
    if(RetVal==-1)
    {
      xSkipAnyAndEndl(); //skip to the end of line
    }
  }
}
int32 xCfgParser::xLoadSection(xCfgSection &ParrentSection)
{
  char        Token;  
  int32       RetVal;
  xCfgSection Section;

  //fprintf(stdout, "V xCfg: Loading section: ");
  
  //section name extraction
  string SectionName;
  RetVal = xReadSectionName(SectionName);
  if (RetVal) { printf("cfg: wrong section name"); return RetVal; }
  Section.setName(SectionName);
  //fprintf(stdout, "name=%s \n", SectionName.c_str());

  xSkipBlank();
  Token = m_Cfg.peek();
  if(xIsComment(Token)) //comment
  {
    string Comment;
    xReadComment(Comment);
    Section.setComment(Comment);
  }
  xSkipBlankAndEndl();  

  //load section body
  Token = m_Cfg.get();
  if(!xIsSectionBeg(Token)) return -1;
  xSkipBlankAndEndl();

  while (!m_Cfg.eof())
  {
    char Token = m_Cfg.peek();
    if(xIsBlank(Token) || xIsEndl(Token) || Token==EOF) //whitespace
    {
      m_Cfg.ignore(1); //skip one char
    }
    else if(xIsComment(Token)) //comment
    {
      string Comment;
      xReadComment(Comment);
    }
    else if(xIsSectionEnd(Token)) //end of the section
    {
      m_Cfg.ignore(1);
      ParrentSection.addSection(Section);
      //fprintf(stdout, "V xCfg: End of section\n");
      break;
    }
    else if(xIsSectionNameBeg(Token))
    {
      RetVal = xLoadSection(Section);
    }
    else if(xIsNamePrefix(Token)) //parameter with $ prefix
    {
      m_Cfg.ignore(1);
      RetVal = xLoadParam(Section, false);
    }
    else if(xIsAlpha(Token)) //param  
    {
      RetVal = xLoadParam(Section, false);
    }
    else
    {
      printf("cfg: wrong start symbol!\n");
      return -1;
    } 
    if(RetVal==-1)
    {
      xSkipAnyAndEndl(); //skip to the end of line
    }
  }
  return 0;
}
int32 xCfgParser::xLoadParam(xCfgSection &ParrentSection, bool OneArgOnly)
{
  char      Token;  
  int32     RetVal;
  string    Assignment;
  xCfgParam Param;

  //fprintf(stdout, "V xCfg: Loading parameter; ");

  //param name extraction
  string ParamName;
  RetVal = xReadParamName(ParamName);
  if (RetVal) { printf("cfg: wrong parameter name"); return RetVal; }
  Param.setName(ParamName);
  //fprintf(stdout, "name=%s ", ParamName.c_str());

  //separator extraction
  RetVal = xSkipBlank();
  Token  = m_Cfg.peek();
  if(xIsAssignment(Token))
  {
    Assignment = Token;
    m_Cfg.ignore();
    RetVal = xSkipBlank();
  }

  //arguments extraction
  Token = m_Cfg.get();
  if(xIsEndl(Token) || xIsAssignment(Token) || xIsComment(Token)) { printf("cfg: no argument found"); return -1; };
  
  do
  {
    m_Cfg.unget();
    string ParamArg;
    xReadParamArg(ParamArg);
    Param.addArg(ParamArg);
    //fprintf(stdout, "arg=%s ", ParamArg.c_str());
    xSkipBlank();
    Token = m_Cfg.get();
  }
  while (!xIsEndl(Token) && !xIsComment(Token) && !m_Cfg.eof() && !(OneArgOnly && Param.getArgs().size()>0));
  m_Cfg.unget();

  if(xIsComment(Token)) //comment
  {
    string Comment; 
    xReadComment(Comment);
    Param.setComment(Comment);
  }

  xSkipAnyAndEndl(); //skip to the end of line  

  ParrentSection.addParam(Param);

  //fprintf(stdout, "\n");
  return 0;  
}
int32 xCfgParser::xSkipBlank()
{
  char Token = m_Cfg.get();
  while(xIsBlank(Token))
  {
    Token = m_Cfg.get();
  }
  if(!m_Cfg.eof()) m_Cfg.unget();
  if(xIsEndl(Token) || m_Cfg.eof()) return -1;
  return 0;
}
int32 xCfgParser::xSkipBlankAndEndl()
{
  char Token = m_Cfg.get();
  while(xIsBlank(Token) || xIsEndl(Token))
  {
    Token = m_Cfg.get();
  }
  if(m_Cfg.eof()) return -1;
  m_Cfg.unget();
  return 0;
}
int32 xCfgParser::xSkipAnyTillEndl()
{
  char Token = m_Cfg.get();
  while(!xIsEndl(Token) && !m_Cfg.eof())
  {
    Token = m_Cfg.get();
  }
  if(m_Cfg.eof()) return -1;
  m_Cfg.unget();
  return 0;
}
int32 xCfgParser::xSkipAnyAndEndl()
{
  char Token = m_Cfg.get();
  while(!xIsEndl(Token) && !m_Cfg.eof() && !m_Cfg.fail())
  {
    Token = m_Cfg.get();
  }
  if(m_Cfg.eof() || m_Cfg.fail()) return -1;
  return 0;
}
int32 xCfgParser::xReadSectionName(string &SectionName)
{
  char Token = m_Cfg.get();
  if(!xIsSectionNameBeg(Token)) return -1;

  xSkipBlank();

  Token = m_Cfg.get();
  if(!xIsName(Token)) return -1;

  while(xIsName(Token))
  {
    SectionName += Token;
    Token = m_Cfg.get();
  }
  m_Cfg.unget();

  if(SectionName.length()==0) return -1;

  xSkipBlank();

  Token = m_Cfg.get();
  if(!xIsSectionNameEnd(Token)) return -1;

  return 0;
}
int32 xCfgParser::xReadParamName(string &ParamName)
{
  char Token = m_Cfg.get();
  if(!xIsName(Token)) return -1;  

  while(xIsName(Token))
  {
    ParamName += Token;
    Token = m_Cfg.get();
  }
  m_Cfg.unget();

  if(ParamName.length()==0) return -1;
  
  return 0;
}
int32 xCfgParser::xReadParamArg(string &ParamArg)
{
  char Token = m_Cfg.get();

  if(xIsQuoteMark(Token))
  {
    char QuoteMark = Token;
    Token = m_Cfg.get();
    while (!xIsEndl(Token) && Token!=QuoteMark && !m_Cfg.eof())
    {
      ParamArg += Token;
      Token = m_Cfg.get();
    }
    if(!m_Cfg.eof() && Token!=QuoteMark) { m_Cfg.unget(); }
    xSkipBlank();
    Token = m_Cfg.get();
    if(!xIsSeparator(Token)) m_Cfg.unget();
    if(!xIsEndl(Token)) return -1;
  }
  else
  {  
    while (xIsName(Token) && !m_Cfg.eof())
    {
      ParamArg += Token;
      Token = m_Cfg.get();
    }
    if(!m_Cfg.eof() && !xIsSeparator(Token)) m_Cfg.unget();
  } 

  if (ParamArg.length()==0) return -1;

  return 0;
}
int32 xCfgParser::xReadComment(string &Comment)
{ 
  char Token = m_Cfg.get();
  if(!xIsComment(Token)) return -1;

  xSkipBlank();
  Token = m_Cfg.get();
  while(!xIsEndl(Token) && !m_Cfg.eof())
  {
    Comment += Token;
    Token = m_Cfg.get();
  }

  if(!m_Cfg.eof()) m_Cfg.unget();
  return 0;
} 

//=====================================================================================================================================================================================

void xCfgParser::storeToFile(const string& FileName)
{
  string Buffer;
  store(Buffer);
  ofstream File (FileName.c_str(), ofstream::out);
  File << Buffer;  
  File.close();
}
void xCfgParser::store(string& Buffer)
{
  m_Cfg.str("");
  m_Cfg.clear();

  xStoreSection(m_RootSection);

  Buffer = m_Cfg.str();
}
void xCfgParser::xStoreSection(xCfgSection& Section)
{
  for(map<string, xCfgParam>::iterator ParamIter = Section.getParams().begin(); ParamIter != Section.getParams().end(); ParamIter++)
  {
    xCfgParam& Param = ParamIter->second;
    xStoreParam(Param);
  }

  for(map<string, xCfgSection>::iterator SectionIter = Section.getSections().begin(); SectionIter != Section.getSections().end(); SectionIter++)
  {
    xCfgSection& SubSection = SectionIter->second;
    m_Cfg << "[" << SubSection.getName() << "]";
    
    if(!SubSection.getComment().empty())
    {
      m_Cfg << "# " << SubSection.getComment();
    }

    m_Cfg << std::endl << "{" << std::endl;
    xStoreSection(SubSection);
    m_Cfg << "}" << std::endl;
  }
}
void xCfgParser::xStoreParam(xCfgParam& Param)
{
  m_Cfg << Param.getName() << " = ";

  vector<string>& Args = Param.getArgs();
  for(vector<string>::iterator ArgIter = Args.begin(); ArgIter != Args.end(); ArgIter++)
  {
    m_Cfg << (*ArgIter) << " ";
  }

  if(!Param.getComment().empty())
  {
    m_Cfg << "# " << Param.getComment();
  }
  m_Cfg << "\n";
}

//=====================================================================================================================================================================================

int32V2 xFilenameParser::FilenameToResolution(const std::string& Filename)
{
  int32V2 Result(NOT_VALID, NOT_VALID);
  if(Filename.empty() || Filename.length()<3) return Result;

  //seeking resolution pattern - looking for "x" in filename
  int32 Len = (int32)Filename.length();

  for(int32 Pos=Len-1; Pos>0; Pos--)
  {		
    if(Filename[Pos] == 'x' || Filename[Pos] == 'X')
    {
      int32 Beg = Pos;
      int32 End = Pos;

      //Looking for beginning and end of resolition pattern
      for(  ; Beg>=0;  Beg--) if(!isdigit(Filename[Beg-1])) break;
      for(  ; End<Len; End++) if(!isdigit(Filename[End+1])) break;

      //Test
      if(Beg == Pos || End == Pos) continue;

      //Parsing
      int32 SizeX = NOT_VALID;
      int32 SizeY = NOT_VALID;
      if(Filename[Pos] == 'x') { xString::scanString(Filename.substr(Beg), "%dx%d", &SizeX, &SizeY); }
      if(Filename[Pos] == 'X') { xString::scanString(Filename.substr(Beg), "%dX%d", &SizeX, &SizeY); }
      if(SizeX>0 && SizeY>0)
      {
        Result.set(SizeX, SizeY);
        return Result;
      }
    }		
  }
  return Result;
}
eCrF xFilenameParser::FilenameToChromaFormat(const std::string& Filename)
{
  if(Filename.empty() || Filename.length()<3) return CrF_UNKNOWN;

  //looking for _cfXXX string, where XXX is chroma format (444, 422, 420, 400) 
  int32 Len = (int32)Filename.length();

  //found "_cf" string
  for(int32 Pos = Len - 1; Pos >= 0; Pos--)
  {
    if(Filename[Pos] == '_' && Pos + 5 < Len)
    {
      if(Filename[Pos + 1] != 'c' && Filename[Pos + 1] != 'C') continue;
      if(Filename[Pos + 2] != 'f' && Filename[Pos + 2] != 'F') continue;
      if(!isdigit(Filename[Pos + 3]) || !isdigit(Filename[Pos + 4]) || !isdigit(Filename[Pos + 5])) continue;
      if(Pos + 6 < Len && isdigit(Filename[Pos + 6])) continue;

      int32 CFI = 100 * (int32)(Filename[Pos + 3] - '0') + 10 * (int32)(Filename[Pos + 4] - '0') + (int32)(Filename[Pos + 5] - '0');
      if(CFI == CrF_444 || CFI == CrF_422 || CFI == CrF_420 || CFI == CrF_400) { return (eCrF)CFI; }
      else { return CrF_INVALID; }
    }
  }
  return CrF_UNKNOWN;
}
int32 xFilenameParser::FilenameToBitDepth(const std::string& Filename)
{
  if(Filename.empty() || Filename.length()<3) return NOT_VALID;

  //looking for _XXbps or _Xbps string, where X,XX are bits per component sample  
  int32 Len = (int32)Filename.length();

  for(int32 Pos = Len - 1; Pos >= 0; Pos--)
  {
    if(Filename[Pos] == '_' && Pos + 5 < Len)
    {
      if(!isdigit(Filename[Pos + 1])) continue;
      if(isdigit(Filename[Pos + 2]))
      {
        if(Filename[Pos + 3] != 'b' && Filename[Pos - 1] != 'B') continue;
        if(Filename[Pos + 4] != 'p' && Filename[Pos - 2] != 'P') continue;
        if(Filename[Pos + 5] != 's' && Filename[Pos - 3] != 'S') continue;

        int32 BitDepth = 10 * (int32)(Filename[Pos + 1] - '0') + (int32)(Filename[Pos + 2] - '0');
        return BitDepth;
      }
      else
      {
        if(Filename[Pos + 2] != 'b' && Filename[Pos - 1] != 'B') continue;
        if(Filename[Pos + 3] != 'p' && Filename[Pos - 2] != 'P') continue;
        if(Filename[Pos + 4] != 's' && Filename[Pos - 3] != 'S') continue;
        int32 BitDepth = (int32)(Filename[Pos + 1] - '0');
        return BitDepth;
      }
    }
  }
  return NOT_VALID;
}

//=====================================================================================================================================================================================

} //end of namespace AVLib
