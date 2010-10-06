//|
//|
//| Copyright (c) 2001-2005
//| Andrew Fedoniouk - andrew@terrainformatica.com
//|
//| aux stuff
//|
//|

#include "tool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef WINDOWS
#include <shlobj.h>
#include <urlmon.h>
#endif

#include <limits.h>

namespace tool 
{

void split_path(const char *path, string& drive, string& dir, string& name, string& ext)
{
#if defined(_MAX_DRIVE) && !defined(PLATFORM_WINCE)
  char cdrive[_MAX_DRIVE];
  char cdir[_MAX_DIR];
  char cname[_MAX_FNAME];
  char cext[_MAX_EXT];
  cdrive[0]=0;
  cdir[0]=0;
  cname[0]=0;
  cext[0]=0;
  _splitpath( path, cdrive, cdir, cname, cext );
  drive = cdrive;
  dir   = cdir;
  name  = cname;
  ext  = cext;
#else
  assert(false);
#endif
}

void split_path(const wchar *path, ustring& drive, ustring& dir, ustring& name, ustring& ext)
{
#if defined(_MAX_DRIVE) && !defined(PLATFORM_WINCE)
  wchar cdrive[_MAX_DRIVE];
  wchar cdir[_MAX_DIR];
  wchar cname[_MAX_FNAME];
  wchar cext[_MAX_EXT];
  cdrive[0]=0;
  cdir[0]=0;
  cname[0]=0;
  cext[0]=0;
  _wsplitpath( path, cdrive, cdir, cname, cext );
  drive = cdrive;
  dir   = cdir;
  name  = cname;
  ext  = cext;
#else
  wchars wc( path, wcslen(path) );
  int fnp = wc.last_index_of('/');
  if( fnp < 0 ) fnp = wc.last_index_of('\\');
  drive.clear();
  if( fnp >= 0 )
  {
    ++fnp;
    dir = ustring(wc.start, fnp );
    int extp = wc.last_index_of('.');
    if( extp < 0 )
      name = ustring(wc.start + fnp, wc.length - fnp);
    else
    {
      name = ustring(wc.start + fnp, extp - fnp);
      ext = ustring(wc.start + extp + 1, wc.length - extp - 1);
    }
  }


#endif
}

tstring get_home_dir(void* hinst)
{
#ifdef WIN32
  TCHAR buffer[2048]; buffer[0] = 0;
  GetModuleFileName(HINSTANCE(hinst),buffer, sizeof(buffer));

  tstring drive, dir, name, ext;
  split_path(buffer, drive, dir, name, ext);

  return drive + dir;
#else
#pragma TODO("Not supported yet!")
  return "";
#endif
}


tstring get_home_dir(const tchar* relpath, void* hinst)
{
#ifdef WIN32
  TCHAR buffer[2048]; buffer[0] = 0;
  GetModuleFileName(HINSTANCE(hinst),buffer, sizeof(buffer));

  tstring drive, dir, name, ext;
  split_path(buffer, drive, dir, name, ext);

  if( relpath )
    return tstring::format(TEXT("%s%s%s"), drive.c_str(), dir.c_str(), relpath);
  return drive + dir;
#else
#pragma TODO("Not supported yet!")
  return "";
#endif
}

tstring get_app_pathname(void* hinst)
{
#ifdef WIN32
  TCHAR buffer[2048]; buffer[0] = 0;
  GetModuleFileName(HINSTANCE(hinst),buffer, sizeof(buffer));
  return buffer;
#else
#pragma TODO("Not supported yet!")
  return "";
#endif
}

unsigned int get_ticks()
{
#ifdef WIN32
  return GetTickCount();
#else
  #pragma TODO("Not supported yet!")
  return 0;
#endif
}

bool get_lang_country(string& lang,string& country, bool for_user)
{
#ifdef WINDOWS
   long langId = for_user? GetUserDefaultLCID() : GetSystemDefaultLCID();
   TCHAR buf[256]; buf[0] = 0;
   GetLocaleInfo(MAKELCID(langId, SORT_DEFAULT), LOCALE_SISO639LANGNAME, buf, sizeof(buf));
   lang = buf;
   GetLocaleInfo(MAKELCID(langId, SORT_DEFAULT), LOCALE_SISO3166CTRYNAME, buf, sizeof(buf));
   country = buf;
   return true;
#else
   #pragma TODO("get lang id on this OS!")
   return false;
#endif
}

tstring  get_standard_dir(STANDARD_DIR sd)
{
#ifdef WINDOWS
  static int sysids[] =
  {
#if defined(UNDER_CE)
    CSIDL_WINDOWS,
    CSIDL_WINDOWS,
    CSIDL_PROGRAM_FILES,
    CSIDL_APPDATA,
    CSIDL_APPDATA,
    CSIDL_PERSONAL,
    CSIDL_PERSONAL,
#else
    CSIDL_WINDOWS,
    CSIDL_SYSTEM,
    CSIDL_PROGRAM_FILES,
    CSIDL_APPDATA,
    CSIDL_COMMON_APPDATA,
    CSIDL_PERSONAL,
    CSIDL_COMMON_DOCUMENTS,
#endif
  };

  TCHAR path[MAX_PATH]; path[0] = 0;

  HRESULT hr = SHGetSpecialFolderPath(
      NULL,path,
      sysids[sd],
      FALSE
      );
  return tstring(path);
#else
  #pragma TODO("get folder path on this OS!")
#endif
}

#ifdef _WINDOWS
  // http://blogs.msdn.com/oldnewthing/archive/2004/10/25/247180.aspx
  EXTERN_C IMAGE_DOS_HEADER __ImageBase;
  #define HINSTANCE_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

/*static char* ext2mimes[] = {
  "323","text/h323",
  "3dm","x-world/x-3dmf",
  "3dmf","x-world/x-3dmf",
  "a","application/octet-stream",
  "aab","application/x-authorware-bin",
  "aam","application/x-authorware-map",
  "aas","application/x-authorware-seg",
  "abc","text/vndabc",
  "acgi","text/html",
  "acx","application/internet-property-stream",
  "afl","video/animaflex",
  "ai","application/postscript",
  "aif","audio/x-aiff,audio/aiff",
  "aifc","audio/x-aiff,audio/aiff",
  "aiff","audio/x-aiff,audio/aiff",
  "aim","application/x-aim",
  "aip","text/x-audiosoft-intra",
  "ani","application/x-navi-animation",
  "aos","application/x-nokia-9000-communicator-add-on-software",
  "aps","application/mime",
  "arc","application/octet-stream",
  "arj","application/arj,application/octet-stream",
  "art","image/x-jg",
  "asc","text/plain",
  "asf","video/x-ms-asf",
  "asm","text/x-asm",
  "asp","text/asp",
  "asr","video/x-ms-asf",
  "asx","application/x-mplayer2,video/x-ms-asf,video/x-ms-asf-plugin",
  "au","audio/basic,audio/x-au",
  "avi","video/x-msvideo,application/x-troff-msvideo,video/avi,video/msvideo",
  "avs","video/avs-video",
  "axs","application/olescript",
  "bas","text/plain",
  "bat","application/bat,application/x-bat,application/x-msdos-program,application/textedit,application/octet-stream",
  "bcpio","application/x-bcpio",
  "bin","application/octet-stream,application/mac-binary,application/macbinary,application/x-binary,application/x-macbinary",
  "bm","image/bmp",
  "bmp","image/bmp,image/x-windows-bmp",
  "boo","application/book",
  "book","application/book",
  "boz","application/x-bzip2",
  "bsh","application/x-bsh",
  "bz","application/x-bzip",
  "bz2","application/x-bzip2",
  "c","text/plain,text/x-c",
  "C","application/x-cplusplus,text/x-c++src,text/plain",
  "c++","application/x-cplusplus,text/x-c++src,text/plain",
  "cab","application/vnd.ms-cab-compressed,application/cab,application/x-compress,application/x-compressed,zz-application/zz-winassoc-cab",
  "cat","application/vndms-pkiseccat",
  "cc","text/plain,text/x-c",
  "ccad","application/clariscad",
  "cco","application/x-cocoa",
  "cdf","application/x-netcdf,application/cdf,application/x-cdf",
  "cer","application/pkix-cert,application/x-x509-ca-cert",
  "cha","application/x-chat",
  "chat","application/x-chat",
  "class","application/octet-stream,application/java,application/java-byte-code,application/x-java-class",
  "clp","application/x-msclip",
  "cmx","image/x-cmx",
  "cod","image/cis-cod",
  "com","application/octet-stream,text/plain",
  "conf","text/plain",
  "cpio","application/x-cpio",
  "cpp","text/x-c",
  "cpt","application/mac-compactpro,application/x-compactpro,application/x-cpt",
  "crd","application/x-mscardfile",
  "crl","application/pkcs-crl,application/pkix-crl",
  "crt","application/pkix-cert,application/x-x509-ca-cert,application/x-x509-user-cert",
  "csh","application/x-csh,text/x-scriptcsh",
  "css","text/css,application/x-pointplus",
  "cxx","text/plain",
  "dcr","application/x-director",
  "deepv","application/x-deepv",
  "def","text/plain",
  "der","application/x-x509-ca-cert",
  "dif","video/x-dv",
  "dir","application/x-director",
  "dl","video/dl,video/x-dl",
  "dll","application/x-msdownload",
  "dms","application/octet-stream",
  "doc","application/msword",
  "docx","application/vnd.openxmlformats-officedocument.wordprocessingml.document",
  "dot","application/msword",
  "dp","application/commonground",
  "drw","application/drafting",
  "dump","application/octet-stream",
  "dv","video/x-dv",
  "dvi","application/x-dvi",
  "dwf","drawing/x-dwf,model/vnddwf",
  "dwg","application/acad,image/vnddwg,image/x-dwg",
  "dxf","application/dxf,image/vnddwg,image/x-dwg",
  "dxr","application/x-director",
  "el","text/x-scriptelisp",
  "elc","application/x-bytecodeelisp,application/x-elc",
  "env","application/x-envoy",
  "eps","application/postscript",
  "es","application/x-esrehber",
  "etx","text/x-setext",
  "evy","application/envoy,application/x-envoy",
  "exe","application/octet-stream",
  "ez","application/andrew-inset",
  "f","text/plain,text/x-fortran",
  "f77","text/x-fortran",
  "f90","text/plain,text/x-fortran",
  "fdf","application/vndfdf",
  "fif","application/fractals,image/fif",
  "fli","video/x-fli,video/fli",
  "flo","image/florian",
  "flr","x-world/x-vrml",
  "flx","text/vndfmiflexstor",
  "fmf","video/x-atomic3d-feature",
  "for","text/plain,text/x-fortran",
  "fpx","image/vndfpx,image/vndnet-fpx",
  "frl","application/freeloader",
  "funk","audio/make",
  "g","text/plain",
  "g3","image/g3fax",
  "gif","image/gif",
  "gl","video/gl,video/x-gl",
  "gsd","audio/x-gsm",
  "gsm","audio/x-gsm",
  "gsp","application/x-gsp",
  "gss","application/x-gss",
  "gtar","application/x-gtar",
  "gz","application/x-gzip,application/x-compressed",
  "gzip","application/x-gzip,multipart/x-gzip",
  "h","text/plain,text/x-h",
  "h++","application/x-cplusplus,text/x-c++src,text/plain",
  "H","application/x-cplusplus,text/x-c++src,text/plain",
  "hdf","application/x-hdf",
  "help","application/x-helpfile",
  "hgl","application/vndhp-hpgl",
  "hh","text/plain,text/x-h",
  "hlb","text/x-script",
  "hlp","application/hlp,application/x-helpfile,application/x-winhelp,application/winhlp",
  "hpg","application/vndhp-hpgl",
  "hpgl","application/vndhp-hpgl",
  "hpp","application/x-cplusplus,text/x-c++src,text/plain",
  "hqx","application/mac-binhex40,application/binhex,application/binhex4,application/mac-binhex,application/x-binhex40,application/x-mac-binhex40",
  "hta","application/hta",
  "htc","text/x-component",
  "htm","text/html",
  "html","text/html",
  "htmls","text/html",
  "htt","text/webviewhtml",
  "htx","text/html",
  "ice","x-conference/x-cooltalk",
  "ico","image/x-icon",
  "idc","text/plain",
  "ief","image/ief",
  "iefs","image/ief",
  "iges","model/iges,application/iges",
  "igs","model/iges,application/iges",
  "iii","application/x-iphone",
  "ima","application/x-ima",
  "imap","application/x-httpd-imap",
  "ini","text/plain,application/textedit,zz-application/zz-winassoc-ini",
  "inf","application/inf",
  "ins","application/x-internett-signup,application/x-internet-signup",
  "ip","application/x-ip2",
  "ips","application/x-ipscript",
  "ipx","application/x-ipix",
  "isp","application/x-internet-signup",
  "isu","video/x-isvideo",
  "it","audio/it",
  "iv","application/x-inventor",
  "ivr","i-world/i-vrml",
  "ivy","application/x-livescreen",
  "jam","audio/x-jam",
  "jar","application/java-archive",
  "jav","text/plain,text/x-java-source",
  "java","text/plain,text/x-java-source",
  "jcm","application/x-java-commerce",
  "jfif","image/jpeg,image/pjpeg,image/pipeg",
  "jfif-tbnl","image/jpeg",
  "jpe","image/jpeg,image/pjpeg",
  "jpeg","image/jpeg,image/pjpeg",
  "jpg","image/jpeg,image/pjpeg",
  "jps","image/x-jps",
  "js","application/x-javascript",
  "jsp","text/html",
  "jut","image/jutvision",
  "kar","audio/midi,music/x-karaoke",
  "ksh","application/x-ksh,text/x-scriptksh",
  "la","audio/nspaudio,audio/x-nspaudio",
  "lam","audio/x-liveaudio",
  "latex","application/x-latex",
  "lha","application/octet-stream,application/lha,application/x-lha",
  "lhx","application/octet-stream",
  "list","text/plain",
  "lma","audio/nspaudio,audio/x-nspaudio",
  "log","text/plain",
  "lnk","application/x-ms-shortcut",
  "lsf","video/x-la-asf",
  "lsp","application/x-lisp,text/x-scriptlisp",
  "lst","text/plain",
  "lsx","text/x-la-asf,video/x-la-asf",
  "ltx","application/x-latex",
  "lzh","application/octet-stream,application/x-lzh",
  "lzx","application/lzx,application/octet-stream,application/x-lzx",
  "m","text/plain,text/x-m",
  "m13","application/x-msmediaview",
  "m14","application/x-msmediaview",
  "m1v","video/mpeg",
  "m2a","audio/mpeg",
  "m2v","video/mpeg",
  "m3u","audio/x-mpequrl,audio/x-mpegurl",
  "man","application/x-troff-man",
  "map","application/x-navimap",
  "mar","text/plain",
  "mbd","application/mbedlet",
  "mc$","application/x-magic-cap-package-10",
  "mcd","application/mcad,application/x-mathcad",
  "mcf","image/vasa,text/mcf",
  "mcp","application/netmc",
  "mdb","application/x-msaccess",
  "me","application/x-troff-me",
  "mesh","model/mesh",
  "mht","message/rfc822",
  "mhtml","message/rfc822",
  "mid","audio/midi,application/x-midi,audio/x-mid,audio/x-midi,music/crescendo,x-music/x-midi,audio/mid",
  "midi","audio/midi,application/x-midi,audio/x-mid,audio/x-midi,music/crescendo,x-music/x-midi",
  "mif","application/vndmif,application/x-frame,application/x-mif",
  "mime","www/mime,message/rfc822",
  "mjf","audio/x-vndaudioexplosionmjuicemediafile",
  "mjpg","video/x-motion-jpeg",
  "mm","application/base64,application/x-meme",
  "mme","application/base64",
  "mny","application/x-msmoney",
  "mod","audio/mod,audio/x-mod",
  "moov","video/quicktime",
  "mov","video/quicktime",
  "movie","video/x-sgi-movie",
  "mp2","audio/mpeg,audio/x-mpeg,video/mpeg,video/x-mpeg,video/x-mpeq2a",
  "mp3","audio/mpeg,audio/mpeg3,audio/x-mpeg-3,video/mpeg,video/x-mpeg",
  "mpa","audio/mpeg,video/mpeg",
  "mpc","application/x-project",
  "mpe","video/mpeg",
  "mpeg","video/mpeg",
  "mpg","video/mpeg,audio/mpeg",
  "mpga","audio/mpeg",
  "mpp","application/vndms-project",
  "mpt","application/x-project",
  "mpv","application/x-project",
  "mpv2","video/mpeg",
  "mpx","application/x-project",
  "mrc","application/marc",
  "ms","application/x-troff-ms",
  "msh","model/mesh",
  "mv","video/x-sgi-movie",
  "mvb","application/x-msmediaview",
  "my","audio/make",
  "mzz","application/x-vndaudioexplosionmzz",
  "nap","image/naplps",
  "naplps","image/naplps",
  "nc","application/x-netcdf",
  "ncm","application/vndnokiaconfiguration-message",
  "nif","image/x-niff",
  "niff","image/x-niff",
  "nix","application/x-mix-transfer",
  "nsc","application/x-conference",
  "nvd","application/x-navidoc",
  "nws","message/rfc822",
  "o","application/octet-stream",
  "oda","application/oda",
  "odb","application/vnd.oasis.opendocument.database",
  "odc","application/vnd.oasis.opendocument.chart",
  "odf","application/vnd.oasis.opendocument.formula",
  "odg","application/vnd.oasis.opendocument.graphics",
  "odi","application/vnd.oasis.opendocument.image",
  "odm","application/vnd.oasis.opendocument.text-master",
  "odp","application/vnd.oasis.opendocument.presentation",
  "ods","application/vnd.oasis.opendocument.spreadsheet",
  "odt","application/vnd.oasis.opendocument.text",
  "omc","application/x-omc",
  "omcd","application/x-omcdatamaker",
  "omcr","application/x-omcregerator",
  "otg","application/vnd.oasis.opendocument.graphics-template",
  "oth","application/vnd.oasis.opendocument.text-web",
  "otp","application/vnd.oasis.opendocument.presentation-template",
  "ots","application/vnd.oasis.opendocument.spreadsheet-template",
  "ott","application/vnd.oasis.opendocument.text-template",
  "oxt","application/vnd.openofficeorg.extension",
  "p","text/x-pascal",
  "p10","application/pkcs10,application/x-pkcs10",
  "p12","application/pkcs-12,application/x-pkcs12",
  "p7a","application/x-pkcs7-signature",
  "p7b","application/x-pkcs7-certificates",
  "p7c","application/pkcs7-mime,application/x-pkcs7-mime",
  "p7m","application/pkcs7-mime,application/x-pkcs7-mime",
  "p7r","application/x-pkcs7-certreqresp",
  "p7s","application/pkcs7-signature,application/x-pkcs7-signature",
  "part","application/pro_eng",
  "pas","text/pascal",
  "pbm","image/x-portable-bitmap",
  "pcl","application/vndhp-pcl,application/x-pcl",
  "pct","image/x-pict",
  "pcx","image/x-pcx",
  "pdb","chemical/x-pdb",
  "pdf","application/pdf",
  "pfunk","audio/make,audio/makemyfunk",
  "pfx","application/x-pkcs12",
  "pgm","image/x-portable-graymap,image/x-portable-greymap",
  "pgn","application/x-chess-pgn",
  "pic","image/pict",
  "pict","image/pict",
  "pkg","application/x-newton-compatible-pkg",
  "pko","application/vndms-pkipko,application/yndms-pkipko",
  "pl","text/plain,text/x-scriptperl",
  "plx","application/x-pixclscript",
  "pm","image/x-xpixmap,text/x-scriptperl-module",
  "pm4","application/x-pagemaker",
  "pm5","application/x-pagemaker",
  "pma","application/x-perfmon",
  "pmc","application/x-perfmon",
  "pml","application/x-perfmon",
  "pmr","application/x-perfmon",
  "pmw","application/x-perfmon",
  "png","image/png",
  "pnm","image/x-portable-anymap,application/x-portable-anymap",
  "pot","application/mspowerpoint,application/vndms-powerpoint",
  "pov","model/x-pov",
  "ppa","application/vndms-powerpoint",
  "ppm","image/x-portable-pixmap",
  "pps","application/mspowerpoint,application/vndms-powerpoint",
  "ppt","application/mspowerpoint,application/powerpoint,application/vndms-powerpoint,application/x-mspowerpoint",
  "ppz","application/mspowerpoint",
  "pre","application/x-freelance",
  "prf","application/pics-rules",
  "properties","text/plain",
  "prt","application/pro_eng",
  "ps","application/postscript",
  "psd","application/octet-stream",
  "pub","application/x-mspublisher",
  "pvu","paleovu/x-pv",
  "pwz","application/vndms-powerpoint",
  "py","text/x-scriptphyton",
  "pyc","applicaiton/x-bytecodepython",
  "qcp","audio/vndqcelp",
  "qd3","x-world/x-3dmf",
  "qd3d","x-world/x-3dmf",
  "qif","image/x-quicktime",
  "qt","video/quicktime",
  "qtc","video/x-qtc",
  "qti","image/x-quicktime",
  "qtif","image/x-quicktime",
  "ra","audio/x-realaudio,audio/x-pn-realaudio,audio/x-pn-realaudio-plugin",
  "ram","audio/x-pn-realaudio",
  "ras","image/cmu-raster,application/x-cmu-raster,image/x-cmu-raster",
  "rast","image/cmu-raster",
  "rexx","text/x-scriptrexx",
  "rf","image/vndrn-realflash",
  "rgb","image/x-rgb",
  "rm","audio/x-pn-realaudio,application/vndrn-realmedia",
  "rmi","audio/mid",
  "rmm","audio/x-pn-realaudio",
  "rmp","audio/x-pn-realaudio,audio/x-pn-realaudio-plugin",
  "rng","application/ringing-tones,application/vndnokiaringing-tone",
  "rnx","application/vndrn-realplayer",
  "roff","application/x-troff",
  "rp","image/vndrn-realpix",
  "rpm","audio/x-pn-realaudio-plugin",
  "rt","text/richtext,text/vndrn-realtext",
  "rtf","text/rtf,application/rtf,application/x-rtf,text/richtext",
  "rtx","text/richtext,application/rtf",
  "rv","video/vndrn-realvideo",
  "s","text/x-asm",
  "s3m","audio/s3m",
  "saveme","application/octet-stream",
  "sbk","application/x-tbook",
  "scd","application/x-msschedule",
  "scm","application/x-lotusscreencam,text/x-scriptguile,text/x-scriptscheme,video/x-scm",
  "sct","text/scriptlet",
  "sda","application/vnd.stardivision.draw,application/x-stardraw",
  "sdc","application/vnd.stardivision.calc,application/x-starcalc",
  "sdd","application/vnd.stardivision.impress,application/x-starimpress",
  "sdm","application/vnd.stardivision.mail",
  "sdml","text/plain",
  "sdp","application/vnd.stardivision.impress-packed,application/sdp,application/x-sdp",
  "sdr","application/sounder",
  "sds","application/vnd.stardivision.chart,application/x-starchart",
  "sdw","application/vnd.stardivision.writer,application/x-starwriter",
  "sea","application/sea,application/x-sea",
  "set","application/set",
  "setpay","application/set-payment-initiation",
  "setreg","application/set-registration-initiation",
  "sgl","application/vnd.stardivision.writer-global",
  "sgm","text/sgml,text/x-sgml",
  "sgml","text/sgml,text/x-sgml",
  "sh","application/x-sh,application/x-bsh,application/x-shar,text/x-scriptsh",
  "shar","application/x-shar,application/x-bsh",
  "shtml","text/html,text/x-server-parsed-html",
  "sid","audio/x-psid",
  "silo","model/mesh",
  "sit","application/x-stuffit,application/x-sit",
  "skd","application/x-koan",
  "skm","application/x-koan",
  "skp","application/x-koan",
  "skt","application/x-koan",
  "sl","application/x-seelogo",
  "smf","application/vnd.stardivision.math,application/x-starmath",
  "smi","application/smil",
  "smil","application/smil",
  "snd","audio/basic,audio/x-adpcm",
  "sol","application/solids",
  "spc","application/x-pkcs7-certificates,text/x-speech",
  "spl","application/x-futuresplash,application/futuresplash",
  "spr","application/x-sprite",
  "sprite","application/x-sprite",
  "src","application/x-wais-source",
  "ssi","text/x-server-parsed-html",
  "ssm","application/streamingmedia",
  "sst","application/vndms-pkicertstore",
  "stc","application/vnd.sun.xml.calc.template",
  "std","application/vnd.sun.xml.draw.template",
  "step","application/STEP,application/step",
  "sti","application/vnd.sun.xml.impress.template",
  "stl","application/SLA,application/sla,application/vndms-pkistl,application/x-navistyle",
  "stm","text/html",
  "stp","application/STEP,application/step",
  "stw","application/vnd.sun.xml.writer.template",
  "sv4cpio","application/x-sv4cpio",
  "sv4crc","application/x-sv4crc",
  "svf","image/vnddwg,image/x-dwg",
  "svg","image/svg+xml",
  "svr","application/x-world,x-world/x-svr",
  "swf","application/x-shockwave-flash",
  "sxc","application/vnd.sun.xml.calc",
  "sxd","application/vnd.sun.xml.draw",
  "sxg","application/vnd.sun.xml.writer.global",
  "sxi","application/vnd.sun.xml.impress",
  "sxm","application/vnd.sun.xml.math",
  "sxw","application/vnd.sun.xml.writer",
  "sys","video/x-mpeg-system",
  "t","application/x-troff",
  "talk","text/x-speech",
  "tar","application/x-tar",
  "tar.gz","application/x-gzip",
  "tbk","application/toolbook,application/x-tbook",
  "tcl","application/x-tcl,text/x-scripttcl",
  "tcsh","text/x-scripttcsh",
  "tex","application/x-tex",
  "texi","application/x-texinfo",
  "texinfo","application/x-texinfo",
  "text","application/plain,text/plain",
  "tgz","application/gnutar,application/x-compressed",
  "tif","image/tiff,image/x-tiff",
  "tiff","image/tiff,image/x-tiff",
  "tr","application/x-troff",
  "trm","application/x-msterminal",
  "tsi","audio/TSP-audio,audio/tsp-audio",
  "tsp","application/dsptype,audio/tsplayer",
  "tsv","text/tab-separated-values",
  "turbot","image/florian",
  "txt","text/plain",
  "uil","text/x-uil",
  "uls","text/iuls",
  "uni","text/uri-list",
  "unis","text/uri-list",
  "unv","application/i-deas",
  "uri","text/uri-list",
  "url","wwwserver/redirection,application/internet-shortcut,application/x-url,message/external-body,text/url,text/x-url",
  "uris","text/uri-list",
  "ustar","application/x-ustar,multipart/x-ustar",
  "uu","application/octet-stream,text/x-uuencode",
  "uue","text/x-uuencode",
  "vcd","application/x-cdlink",
  "vcf","text/x-vcard",
  "vcs","text/x-vcalendar",
  "vda","application/vda",
  "vdo","video/vdo",
  "vew","application/groupwise",
  "viv","video/vndvivo,video/vivo",
  "vivo","video/vndvivo,video/vivo",
  "vmd","application/vocaltec-media-desc",
  "vmf","application/vocaltec-media-file",
  "voc","audio/voc,audio/x-voc",
  "vos","video/vosaic",
  "vox","audio/voxware",
  "vqe","audio/x-twinvq-plugin",
  "vqf","audio/x-twinvq",
  "vql","audio/x-twinvq-plugin",
  "vrml","model/vrml,application/x-vrml,x-world/x-vrml",
  "vrt","x-world/x-vrt",
  "vsd","application/x-visio",
  "vst","application/x-visio",
  "vsw","application/x-visio",
  "w60","application/wordperfect60",
  "w61","application/wordperfect61",
  "w6w","application/msword",
  "wav","audio/x-wav,audio/wav",
  "wax","audio/x-ms-wax",
  "wb1","application/x-qpro",
  "wbmp","image/vndwapwbmp",
  "wcm","application/vndms-works",
  "wdb","application/vndms-works",
  "web","application/vndxara",
  "wiz","application/msword",
  "wk1","application/x-123",
  "wks","application/vndms-works",
  "wm","video/x-ms-wm",
  "wmf","windows/metafile,application/x-msmetafile",
  "wma","audio/x-ms-wma",
  "wmd","application/x-ms-wmd",
  "wml","text/vndwapwml",
  "wmlc","application/vndwapwmlc",
  "wmls","text/vndwapwmlscript",
  "wmlsc","application/vndwapwmlscriptc",
  "wmv","audio/x-ms-wmv",
  "wmx","video/x-ms-wmx",
  "wmz","application/x-ms-wmz",
  "word","application/msword",
  "wp","application/wordperfect",
  "wp5","application/wordperfect,application/wordperfect60",
  "wp6","application/wordperfect",
  "wpd","application/wordperfect,application/x-wpwin",
  "wps","application/vndms-works",
  "wq1","application/x-lotus",
  "wri","application/mswrite,application/x-wri,application/x-mswrite",
  "wrl","model/vrml,application/x-world,x-world/x-vrml",
  "wrz","model/vrml,x-world/x-vrml",
  "wsc","text/scriplet",
  "wsrc","application/x-wais-source",
  "wtk","application/x-wintalk",
  "wvx","video/x-ms-wvx",
  "x-png","image/png",
  "xaf","x-world/x-vrml",
  "xbm","image/x-xbitmap,image/x-xbm,image/xbm",
  "xdr","video/x-amt-demorun",
  "xgz","xgl/drawing",
  "xhtml","application/xhtml+xml,text/html",
  "xif","image/vndxiff",
  "xl","application/excel",
  "xla","application/excel,application/x-excel,application/x-msexcel,application/vndms-excel",
  "xlb","application/excel,application/vndms-excel,application/x-excel",
  "xlc","application/vndms-excel,application/excel,application/x-excel",
  "xld","application/excel,application/x-excel",
  "xlk","application/excel,application/x-excel",
  "xll","application/vndms-excel,application/excel,application/x-excel",
  "xlm","application/vndms-excel,application/excel,application/x-excel",
  "xls","application/vndms-excel,application/excel,application/x-excel,application/x-msexcel",
  "xlsx","application/vnd.openxmlformats-officedocument.spreadsheetml.sheet",
  "xlt","application/excel,application/x-excel,application/vndms-excel",
  "xlv","application/excel,application/x-excel",
  "xlw","application/vndms-excel,application/excel,application/x-excel,application/x-msexcel",
  "xm","audio/xm",
  "xml","text/xml,application/xml",
  "xmz","xgl/movie",
  "xof","x-world/x-vrml",
  "xpix","application/x-vndls-xpix",
  "xpm","image/x-xpixmap,image/xpm",
  "xsd","text/xml",
  "xsl","text/xml",
  "xsr","video/x-amt-showrun",
  "xwd","image/x-xwindowdump,image/x-xwd",
  "xyz","chemical/x-pdb",
  "z","application/x-compress,application/x-compressed",
  "zip","application/zip,application/x-compressed,application/x-zip-compressed,multipart/x-zip",
  "zoo","application/octet-stream",
  "zsh","text/x-scriptzsh",
 }; 

 void get_ext_for_mime(chars mimetype, array<chars>& exts)
 {
   string mt = mimetype;
   for( int n = 0; n < items_in(ext2mimes); n += 2 )
   {
     const char* ext = ext2mimes[n];
     chars mimes = chars_of(ext2mimes[n+1]);
     chars mime;
     atokens toks(mimes,",");
     while(toks.next(mime))
       if( mime.like(mt) )
         exts.push( chars_of(ext) );
   }
 }
 */

#if defined(WINDOWS)
#pragma comment(lib,"Urlmon.lib")
#endif

 string guess_mime_type(const ustring& filename, bytes data)
 {
#if defined(WINDOWS)
    if(data.length > 256) data.length = 256;
    array<byte> test = data;
    wchar *mime = 0;
    HRESULT hr = FindMimeFromData(
      0,
      filename,
      test.head(),
      test.size(),
      0 /*pwzMimeProposed*/,
      0 /*dwMimeFlags*/,
      &mime /*ppwzMimeOut*/,
      0);
    return (SUCCEEDED(hr) && mime)? string(mime):string();
#else
    return string();
#endif
 }

#include "html_encodings_ph.h"

int get_lang_id(const string& name)
{
  string lcasename = name;
  lcasename.to_lower();
  int id = 0;

  html_encoding_def *hed = html_encodings::find_def(lcasename,lcasename.length());
  if(hed != 0)
    id = hed->value;
  else
    assert(false);

  return id;
}

bool decode_bytes(bytes bs, ustring& str, const string& encoding)
{
    int cp = get_lang_id(encoding);
#ifdef WINDOWS
    if(cp == 0) return false;
    if(cp == 65001)
      str = ustring::utf8(bs);
    else
    {
      int nu = MultiByteToWideChar(cp,0,(LPCSTR)bs.start,bs.length,0,0);
      ustring r(' ',nu);
      MultiByteToWideChar(cp,0,(LPCSTR)bs.start,bs.length,r.buffer(),r.length());
      str = r;
    }
    return true;
#else
    return ustring::utf8(bs);
#endif
}





}

